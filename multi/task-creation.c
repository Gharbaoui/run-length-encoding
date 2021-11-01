#include "main.h"

void*	create_tasks_func(void *args)
{
	struct stat f_info; // this will hold info about file
	int index; // will help in results of task not in this function
	int fd;
	int	index_based_on_file;
	long long size;
	Task *current_task;
	supply_data *data = (supply_data *)args;
	names *files = data->fnames;
	
	index = 0;
	while (files)
	{
		index_based_on_file = 0;
		fd = open (files->fname, O_RDONLY);
		if (fd != -1 && fstat(fd, &f_info) != -1) // else something is not right  about this file
		{
			size = f_info.st_size;
			while (size > 0)
			{
				char *map_ret = mmap(NULL, BLOCK_SIZE, PROT_READ, MAP_PRIVATE, fd, index_based_on_file * BLOCK_SIZE);
				// here i need to consider if map_ret is valid before adding to the queue
				Queue *task = getQueTask(map_ret, get_end_of_region(map_ret, size), index);
				size -= BLOCK_SIZE;
				pthread_mutex_lock(&data->pu_pop_lock);
				push(&data->tasks_head, &task);
				pthread_cond_signal(&data->wait_task_cv);
				pthread_mutex_unlock(&data->pu_pop_lock);
				++index; // 
				++index_based_on_file;
			}
		}
		close(fd);
		files = files->next;
	}
	return NULL;
}

char	*get_end_of_region(char *start, long long size)
{
	char *end;

	end = start;
	if (!(size - BLOCK_SIZE > 0))
	{
		return start + size;
	}
	return start + BLOCK_SIZE;
}

TaskAfterProccess	**get_init_arr_tasks(int size)
{
	TaskAfterProccess **tmp;

	tmp = malloc(sizeof(TaskAfterProccess *) * size);
	for (int i = 0; i < size; ++i)
		tmp[i] = NULL;
	return tmp;
}

void	free_mmap(Task *task, supply_data *data)
{
	// after i should signal to cond variable
	// so function that are trying to get memory can do so
	// later here
	munmap(task->first, task->last - task->first);
}

void	free_restask(TaskAfterProccess *task, supply_data *data)
{
	// after i should signal to cond variable
	// so function that are trying to get memory can do so
	// later here
	munmap(task->start, task->end - task->start);
}
