/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleunin <saleunin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 14:25:07 by saleunin          #+#    #+#             */
/*   Updated: 2024/06/10 16:23:04 by saleunin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	add_to_head(t_philosopher *philosopher, t_printable *printable)
{
	t_list	*printable_list;

	printable_list = ft_lstnew(printable);
	if (!printable_list)
	{
		free(printable);
		ft_error_child("failed to allocate memory", ENOMEM, philosopher);
	}
	sem_wait(philosopher->philo_sem);
	if (!philosopher->printable_head)
	{
		philosopher->printable_head = printable_list;
		sem_post(philosopher->philo_sem);
	}
	else
	{
		ft_lstadd_back(&philosopher->printable_head, printable_list);
		sem_post(philosopher->philo_sem);
	}
}

void	add_to_printable(t_philosopher *philosopher, char *str)
{
	t_printable	*printable;

	printable = (t_printable *)malloc(sizeof(t_printable));
	if (!printable)
		ft_error_child("failed to allocate memory", ENOMEM, philosopher);
	printable->str = ft_strdup(str);
	if (!printable->str)
		ft_error_child("failed to allocate memory", ENOMEM, philosopher);
	printable->id = philosopher->id;
	add_to_head(philosopher, printable);
}

bool	print_printable(t_philosopher *philosopher)
{
	t_printable	*printable;

	printable = philosopher->printable_head->content;
	ft_lst_remove_first_node(&philosopher->printable_head);
	if (philosopher->stop)
	{
		free(printable->str);
		free(printable);
		return (true);
	}
	sem_post(philosopher->philo_sem);
	sem_wait(philosopher->printf_sem);
	printf("%zd %d %s\n", get_current_time() - philosopher->start_time,
		printable->id, printable->str);
	sem_post(philosopher->printf_sem);
	free(printable->str);
	free(printable);
	return (false);
}

void	*print_printable_thread(void *arg)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)arg;
	while (1)
	{
		sem_wait(philosopher->philo_sem);
		if (philosopher->printable_head)
		{
			if (print_printable(philosopher))
				break ;
		}
		else
		{
			if (philosopher->stop)
				break ;
			sem_post(philosopher->philo_sem);
			usleep(100);
		}
	}
	philosopher->printing_thread_stopped = true;
	sem_post(philosopher->philo_sem);
	return (arg);
}
