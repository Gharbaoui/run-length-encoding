#include "main.h"

void	proccess_resaults(supply_data *data)
{
	pthread_mutex_t support_cond_var;
	pthread_mutex_init(&support_cond_var, NULL);
	int	num_of_tasks = data->number_of_tasks;
	for (int i = 0; i < num_of_tasks; ++i)
	{
		pthread_mutex_lock(&support_cond_var);
		while (data->results_arr[i] == NULL) // this  means that task not complted by worker thread
			pthread_cond_wait(&data->res_add_cv, &support_cond_var);
		pthread_mutex_unlock(&support_cond_var);
		// above 4 lines just check if next job is ready
		display_resault(data->results_arr, i, i == num_of_tasks -1);
		if (i)
			free_restask(data->results_arr[i - 1], data);
	}
}

void	display_resault(TaskAfterProccess **res, int index, int is_last)
{
	TaskAfterProccess *tmp;

	if(index)
	{
	//	a3 a6 => a9
		tmp = res[index - 1];
		if (*(tmp->end - 2) == *(res[index]->start))
		{
			// check if first of current is same as last in prev txt
			*(res[index]->start + 1) += *(tmp->end - 1);
		}
	}
	tmp = res[index];
	write(1, tmp->start, tmp->end - tmp->start - 2);
	if (is_last)
		write(1, tmp->end - 2, 2); // print the last two chars
}
