/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleunin <saleunin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 16:15:17 by saleunin          #+#    #+#             */
/*   Updated: 2024/06/07 13:57:02 by saleunin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	check_philo_died(t_philosopher *philosopher)
{
	int	i;

	if (get_current_time() - philosopher->last_meal_time \
	> (ssize_t)philosopher->time_to_die)
	{
		sem_post(philosopher->stop_program_sem);
		printf("%ld %d %s\n",
			get_current_time() - philosopher->start_time, \
			philosopher->id, "died");
		printf("\n\n\n\n\n\n\n\n\n\n");
		i = -1;
		while (++i < philosopher->num_philosophers)
			sem_post(philosopher->amt_philos_eat_enough_sem);
		return (true);
	}
	return (false);
}

void	*monitor_thread(void *arg)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)arg;
	while (1)
	{
		sem_wait(philosopher->philo_sem);
		if (philosopher->stop)
			break ;
		if (check_philo_died(philosopher))
			break ;
		if (philosopher->num_times_to_eat != -1 && \
		philosopher->meals_eaten >= philosopher->num_times_to_eat)
		{
			philosopher->stop = true;
			sem_post(philosopher->amt_philos_eat_enough_sem);
			break ;
		}
		sem_post(philosopher->philo_sem);
		usleep(250);
	}
	philosopher->moniter_thread_stopped = true;
	sem_post(philosopher->philo_sem);
	return (arg);
}
