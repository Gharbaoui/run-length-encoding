#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>

#define BLOCK_SIZE 4096 // 4k

typedef struct t_n{
	char *fname;
	struct t_n *next;
} names;

typedef struct {
	char 	*first;
	char 	*last;
	int		task_index;
} Task;

typedef struct {
	char	*start;
	char	*end;
}	TaskAfterProccess;

typedef	struct qu{
	Task *task;
	struct qu *next;
	struct qu *last;
}	Queue;

typedef	struct {
	int					tasks_are_done;
	Queue 				*tasks_head;
	names 				*fnames;
	pthread_mutex_t		pu_pop_lock;
	pthread_cond_t		wait_task_cv;
	pthread_cond_t		res_add_cv;
	int 				number_of_tasks;
	TaskAfterProccess	**results_arr;
}	supply_data;


void	push(Queue **head, Queue **new_node);
Queue	*pop(Queue **head);
Queue	*back(Queue *head);
int	empty(Queue *head);

int	get_number_of_threads_and_files_name(char **argv, int size, names **, int *);
void	add_to_end(names **head, names **node);
pthread_t	*get_thread_pool(int n);
Task	*getTask(char *first, char *last, int index);
Queue	*getQueTask(char *first, char *last, int index);
void*	create_tasks_func(void *args);
void	init_data(supply_data *data);
void	launch_worker_threads(pthread_t *ths, supply_data *data, int n);
void*	worker_func(void *args);
void	excute_task(Task *task, supply_data *data);
char	*get_end_of_region(char *start, long long size);
long long get_memory_needed(Task *task);
TaskAfterProccess	**get_init_arr_tasks(int size);
void	fill_memory_region_by_result(TaskAfterProccess *resTask, Task *task);
int	get_number_tasks(names *files);;
void	free_mmap(Task *task);
void	proccess_resaults(supply_data *data);
void	display_resault(TaskAfterProccess **res, int index, int is_last);
void	free_restask(TaskAfterProccess *task);
void	 memory_error();
void	free_resources(supply_data *data, pthread_t *ths);
void	free_queue(Queue *q);
void	free_names(names *fn);
char    excute(char *start_char, char counter, FILE *file_str);
void sq_rle(int argc, char **argv);
#endif
