/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleunin <saleunin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 14:25:07 by saleunin          #+#    #+#             */
/*   Updated: 2024/06/07 14:30:36 by saleunin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers2.h"

static void	add_to_head(t_philosopher *philosopher, t_printable *printable)
{
	t_list	*printable_list;

	printable_list = ft_lstnew(printable);
	sem_wait(philosopher->philo_sem);
	if (!printable_list)
	{
		sem_post(philosopher->philo_sem);
		free(printable->str);
		free(printable);
		return ;
	}
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
	printable->id = philosopher->id;
	if (!printable->str)
	{
		free(printable);
		printf("%ld %d %s\n", get_current_time() - philosopher->start_time, philosopher->id, str);
	}
	add_to_head(philosopher, printable);
}