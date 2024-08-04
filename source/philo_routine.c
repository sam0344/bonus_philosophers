/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleunin <saleunin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 16:15:17 by saleunin          #+#    #+#             */
/*   Updated: 2024/06/07 13:57:02 by saleunin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	eat(t_philosopher *philosopher)
{
	sem_wait(philosopher->forks_sem);
	add_to_printable(philosopher, "has taken a fork");
	sem_wait(philosopher->forks_sem);
	add_to_printable(philosopher, "has taken a fork");
	add_to_printable(philosopher, "is eating");
	sem_wait(philosopher->philo_sem);
	philosopher->meals_eaten++;
	philosopher->last_meal_time = get_current_time();
	sem_post(philosopher->philo_sem);
	ft_usleep(philosopher->time_to_eat);
	sem_post(philosopher->forks_sem);
	sem_post(philosopher->forks_sem);
}

static void	think(t_philosopher *philosopher)
{
	add_to_printable(philosopher, "is thinking");
	if (philosopher->stop)
		return ;
	if (philosopher->num_philosophers % 2 == 1)
		ft_usleep(philosopher->time_to_eat * 2 - philosopher->time_to_sleep);
	while (get_current_time() - (ssize_t)philosopher->last_meal_time \
	< philosopher->time_to_eat * 2)
		usleep(100);
}

void	*stop_philo_died(void *arg)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)arg;
	sem_wait(philosopher->stop_program_sem);
	sem_post(philosopher->stop_program_sem);
	sem_wait(philosopher->philo_sem);
	philosopher->stop = true;
	philosopher->check_death_thread_stopped = true;
	sem_post(philosopher->philo_sem);
	return (arg);
}

void	philo_routine(t_philosopher *philo)
{
	while (1)
	{
		eat(philo);
		sem_wait(philo->philo_sem);
		if (philo->stop)
		{
			sem_post(philo->philo_sem);
			while (1)
			{
				sem_wait(philo->philo_sem);
				if (philo->moniter_thread_stopped && \
			philo->printing_thread_stopped && philo->check_death_thread_stopped)
				{
					free_philosophers(philo);
					exit(0);
				}
				sem_post(philo->philo_sem);
				usleep(100);
			}
		}
		sem_post(philo->philo_sem);
		add_to_printable(philo, "is sleeping");
		ft_usleep(philo->time_to_sleep);
		think(philo);
	}
}

void	philo_start(t_philosopher	*philosopher)
{
	pthread_t	check_philo_died;
	pthread_t	printing_thread;

	philosopher->last_meal_time = philosopher->start_time;
	pthread_create(
		&philosopher->death_thread, NULL, monitor_thread, philosopher);
	pthread_detach(philosopher->death_thread);
	pthread_create(&check_philo_died, NULL, stop_philo_died, philosopher);
	pthread_detach(check_philo_died);
	pthread_create(&printing_thread, NULL, print_printable_thread, philosopher);
	pthread_detach(printing_thread);
	philo_routine(philosopher);
}
