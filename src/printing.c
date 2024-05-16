

#include "philosophers.h"

static void	add_to_head(t_philosopher *philosopher, t_printable *printable)
{
	t_list	*printable_list_new;

	printable_list_new = ft_lstnew(printable);
	if (!printable_list_new)
	{
		free(printable->str);
		free(printable);
		return ;
	}
	sem_wait(philosopher->program->printable_head_sem);
	printable->time_stamp = get_current_time() - philosopher->start_time;
	if (!philosopher->program->printable_list)
	{
		philosopher->program->printable_list = printable_list_new;
		sem_post(philosopher->program->printable_head_sem);
	}
	else
	{
		ft_lstadd_back(&philosopher->program->printable_list, printable_list_new);
		sem_post(philosopher->program->printable_head_sem);
	}
}

void	add_to_print(t_philosopher *philosopher, char *str)
{
	t_printable	*printable;

	printable = malloc(sizeof(t_printable));
	if (!printable)
		printf("%zd %d %s", get_current_time() - philosopher->start_time, \
		philosopher->id, str);
	else
	{
		printable->id = philosopher->id;
		printable->str = ft_strdup(str);
		add_to_head(philosopher, printable);
	}
}

static void	print_printable(t_program *program)
{
	t_printable	*printable;

	printable = (t_printable *)program->printable_list->content;
	ft_lst_remove_first_node(&program->printable_list);
	sem_post(program->printable_head_sem);
	printf("%zd %d %s\n", printable->time_stamp, printable->id, printable->str);
	free(printable->str);
	free(printable);
}

void	*printable_thread(void *arg)
{
	t_philosopher	*philosophers;

	philosophers = (t_philosopher *)arg;
	while (1)
	{
		sem_wait(philosophers->program->printable_head_sem);
		if (philosophers->program->printable_list)
			print_printable(philosophers->program);
		else
		sem_post(philosophers->program->printable_head_sem);

		usleep(25);
		sem_wait(philosophers->program->stop_print_sem);
		if (philosophers->program->stop_print)
		{
			sem_post(philosophers->program->stop_print_sem);
			break ;
		}
		sem_post(philosophers->program->stop_print_sem);
	}
	return (arg);
}
