#include "main.h"

int	get_number_of_threads_and_files_name(char **argv, int size, names **files)
{
	int i = 1;
	int nth = 0;
	while (i < size)
	{
		if (!strcmp(argv[i], "-j")) // search for -j option
		{
			nth = atoi(argv[i + 1]); // if not number after -j atoi will return 0
			++i;
		}
		else
		{
			names *tmp = malloc(sizeof(tmp));
			tmp->fname = strdup(argv[i]);
			add_to_end(files, &tmp);
		}
		++i;
	}
	return nth; // means that theres no -j option
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
	// will give how much will be needed in worst case
	long long diff;

	diff = task->last - task->first;
	if (diff < BLOCK_SIZE)
		if (diff * 2 <= BLOCK_SIZE)
			return BLOCK_SIZE; // only need 4k to represent output at most
	return BLOCK_SIZE * 2; // return twice what we have if thers non repeating characterss

}
