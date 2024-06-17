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

#include "philosophers2.h"

static void	add_to_head(t_philosopher *philosopher, t_printable *printable)
{
	t_list	*printable_list;

	printable_list = ft_lstnew(printable);
	if (!printable_list)
	{
		printf("%ld %d %s\n", get_current_time() - philosopher->start_time, philosopher->id, printable->str);
		free(printable->str);
		free(printable);
		return ;
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

void add_to_printable(t_philosopher *philosopher, char *str)
{
	t_printable *printable;

	printable = (t_printable *)malloc(sizeof(t_printable));
	if (!printable)
	{
		printf("%ld %d %s\n", get_current_time() - philosopher->start_time, philosopher->id, str);
	}
	printable->str = ft_strdup(str);
	if (!printable->str)
	{
		free(printable);
		printf("%ld %d %s\n", get_current_time() - philosopher->start_time, philosopher->id, str);
	}
	printable->id = philosopher->id;
	add_to_head(philosopher, printable);
}

void *print_printable_thread(void *arg)
{
	t_philosopher	*philosopher;
	t_printable		*printable;

	philosopher = (t_philosopher *)arg;
	while (1)
	{
		sem_wait(philosopher->philo_sem);
		if (philosopher->printable_head)
		{
			printable = philosopher->printable_head->content;
			ft_lst_remove_first_node(&philosopher->printable_head);
			if (philosopher->stop)
			{
				free(printable->str);
				free(printable);
				break;
			}
			sem_post(philosopher->philo_sem);
			sem_wait(philosopher->printf_sem);
			printable->time_stamp = get_current_time() - philosopher->start_time;
			printf("%zd %d %s\n", printable->time_stamp, printable->id, printable->str);\
			sem_post(philosopher->printf_sem);
			free(printable->str);
			free(printable);
		}
		else
		{
			sem_post(philosopher->philo_sem);
			usleep(100);
		}
	}
	philosopher->printing_thread_stopped = true;
	sem_post(philosopher->philo_sem);
	return (arg);
}
