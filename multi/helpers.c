#include "main.h"

int	get_number_of_threads_and_files_name(char **argv, int size, names **files, int *found)
{
	int i = 1;
	int nth = 0;
	*found = 0;
	while (i < size)
	{
		if (!strcmp(argv[i], "-j"))
		{
			*found = 1;
			nth = atoi(argv[i + 1]);
			++i;
		}
		else
		{
			names *tmp = malloc(sizeof(names));
			if (!tmp)
			{
				free_names(*files);
				memory_error();
			}
			tmp->fname = strdup(argv[i]);
			if (!tmp->fname)
			{
				free_names(*files);
				free(tmp);
				memory_error();
			}
			add_to_end(files, &tmp);
		}
		++i;
	}
	return nth;
}

void	add_to_end(names **head, names **node)
{
	(*node)->next = NULL;
	if (*head)
	{
		names *next = *head;
		while (next->next)
			next = next->next;
		next->next = *node;
	}
	else
		*head = *node;
}


pthread_t	*get_thread_pool(int n)
{
	return malloc(sizeof(pthread_t) * n);
}

Task	*getTask(char *first, char *last, int index)
{
	Task *task = malloc(sizeof(Task));
	task->first = first;
	task->last = last;
	task->task_index = index;
	return task;
}

Queue	*getQueTask(char *first, char *last, int index)
{
	Queue *tmp = malloc(sizeof(Queue));
	tmp->task = getTask(first, last, index);
	return tmp;
}

long long get_memory_needed(Task *task)
{
	long long diff;

	diff = task->last - task->first;
	if (diff < BLOCK_SIZE)
		if (diff * 2 <= BLOCK_SIZE)
			return BLOCK_SIZE;
	return BLOCK_SIZE * 2;

}


void	 memory_error()
{
	printf("could not get memory by mmap\n");
	exit(1);
}

void	free_resources(supply_data *data, pthread_t *ths)
{
	pthread_mutex_destroy(&data->pu_pop_lock);
	pthread_cond_destroy(&data->wait_task_cv);
	pthread_cond_destroy(&data->res_add_cv);
	free(ths);
	ths = NULL;
}


void	free_queue(Queue *q)
{
	munmap(q->task->first, q->task->last - q->task->first);
	free(q->task);
	free(q);
}

void	free_names(names *fn)
{
	names *next;

	while (fn)
	{
		next = fn->next;
		free(fn->fname);
		free(fn);
		fn = next;
	}
}
