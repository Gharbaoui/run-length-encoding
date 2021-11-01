#include "main.h"

void	launch_worker_threads(pthread_t *ths, supply_data *data, int n)
{
	for (int i = 0; i < n; ++i)
		pthread_create(ths + i, NULL, worker_func, (void *)data);
}


void*	worker_func(void *args)
{
	int found;
	Queue *task;
	
	supply_data *data = (supply_data *)args;
	while (1)
	{
		pthread_mutex_lock(&data->pu_pop_lock);
		while (empty(data->tasks_head))
			pthread_cond_wait(&data->wait_task_cv, &data->pu_pop_lock);
		task = pop(&data->tasks_head);
		pthread_mutex_unlock(&data->pu_pop_lock);
		excute_task(task->task, data);
		// here i need to free task (Queue)
	}
	return NULL;
}
// working in this section
void	excute_task(Task *task, supply_data *data)
{
	long long mem; // memory needed to hold the result
	TaskAfterProccess *comp_task = malloc(sizeof(TaskAfterProccess));

	mem = get_memory_needed(task);

	comp_task->start = mmap(NULL, mem, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	fill_memory_region_by_result(comp_task, task);
	// here task is no longer needed
	free_mmap(task, data);
	data->results_arr[task->task_index] = comp_task;
	// here i sould add signal other condtion variable
	// so main can try to see if resTask is ready in array so it can start printing
	pthread_cond_signal(&data->res_add_cv);
}

void	fill_memory_region_by_result(TaskAfterProccess *resTask, Task *task)
{
	unsigned char counter;
	char *ptr;
	char *reader;
	char cmp_char;
	char cur_char;

	ptr = resTask->start; // ptr will be used to write to memory
	reader = task->first;
	while (reader != task->last)
	{
		counter = 0;
		cmp_char = *reader;
		while (1)
		{
			cur_char = *reader;
			if (cur_char != cmp_char)
				break ;
			++counter;
			if (cmp_char == '-')
				printf("break here\n");
			++reader;
			if (reader == task->last){
				*ptr = cmp_char;
				*(ptr + 1) = counter;
				ptr += 2;
				resTask->end = ptr; // end will point to one charcter after last one
				return ;
			}
		}
		*ptr = cmp_char;
		*(ptr + 1) = counter;
		ptr += 2;
	}
}