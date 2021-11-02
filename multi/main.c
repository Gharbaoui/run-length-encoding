#include "main.h"

int main(int argc, char **argv)
{
	int nth;
	int found;
	pthread_t	*worker_threads;
	pthread_t	make_tasks;

	supply_data data;
	data.fnames = NULL;
	if (argc > 1)
	{
		nth = get_number_of_threads_and_files_name(argv, argc, &data.fnames, &found);
		if (nth > 0 && found)
		{
			init_data(&data);
			worker_threads = get_thread_pool(nth);
			if (!worker_threads){
				free_names(data.fnames);
				memory_error();
			}
			pthread_create(&make_tasks, NULL, create_tasks_func, (void *)(&data));
			pthread_detach(make_tasks);
			launch_worker_threads(worker_threads, &data, nth);
			proccess_resaults(&data);
			free_resources(&data, worker_threads);
		}
		else if (!found)
		{
			sq_rle(argc, argv);
		}
		else{
			printf("invalid number of threads\n");
			return 1;
		}
		free_names(data.fnames);
	}
}

void	init_data(supply_data *data)
{
	data->number_of_tasks = get_number_tasks(data->fnames);
	if (data->number_of_tasks == -1){
		free_names(data->fnames);
		exit(1);
	}
	data->tasks_head = NULL;
	pthread_mutex_init(&data->pu_pop_lock, NULL);
	pthread_cond_init(&data->wait_task_cv, NULL);
	pthread_cond_init(&data->res_add_cv, NULL);
	data->results_arr = get_init_arr_tasks(data->number_of_tasks);
	data->tasks_are_done = 0;
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
		if (fd < 0)
		{
			printf("bad file %s\n", files->fname);
			return -1;
		}
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
