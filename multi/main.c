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
			proccess_resaults(&data);
		}
	}

	//char *p = malloc(10);
//	read(0, p, 1);
}

void	init_data(supply_data *data)
{
	data->tasks_head = NULL;
	pthread_mutex_init(&data->pu_pop_lock, NULL);
//	pthread_mutex_init(&data->take_task_lock, NULL);
	pthread_cond_init(&data->wait_task_cv, NULL);
	pthread_cond_init(&data->res_add_cv, NULL);
	data->number_of_tasks = get_number_tasks(data->fnames);
	data->results_arr = get_init_arr_tasks(data->number_of_tasks);
}

int	get_number_tasks(names *files)
{
	struct stat f_info;
	int nt;
	int fcount;
	long long fsize;
	int	fd;

	nt = 0;
	while (files)
	{
		fd = open(files->fname, O_RDONLY);
		fstat(fd, &f_info);
		fsize = f_info.st_size;
		fcount = fsize / BLOCK_SIZE;
		nt += fcount;
		if (fcount * BLOCK_SIZE < fsize)
			nt += 1;
		files = files->next;
		close(fd);
	}
	return nt;
}
