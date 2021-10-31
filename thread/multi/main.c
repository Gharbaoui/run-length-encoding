#include "main.h"

// maybe results_arr in task-creation.c is null and i 'm trying to put stuff in it
// in wirker thread

int main(int argc, char **argv)
{
	int nth; // number of threads
	pthread_t	*worker_threads;
	pthread_t	make_tasks; // this thread will make task

	supply_data data;
	data.fnames = NULL;
	if (argc > 1)
	{
		nth = get_number_of_threads_and_files_name(argv, argc, &data.fnames);
		if (nth)
		{
			init_data(&data);
			worker_threads = get_thread_pool(nth);
			pthread_create(&make_tasks, NULL, create_tasks_func, (void *)(&data));
			launch_worker_threads(worker_threads, &data, nth);
		}
	}

	char *p = malloc(10);
	read(0, p, 1);
}

void	init_data(supply_data *data)
{
	data->tasks_head = NULL;
	pthread_mutex_init(&data->pu_pop_lock, NULL);
//	pthread_mutex_init(&data->take_task_lock, NULL);
	pthread_cond_init(&data->wait_task_cv, NULL);
}
