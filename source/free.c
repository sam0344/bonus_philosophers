/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleunin <saleunin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 13:24:07 by saleunin          #+#    #+#             */
/*   Updated: 2024/06/10 15:53:56 by saleunin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	close_semaphores(t_philosopher *philosopher)
{
	sem_unlink("forks_sem");
	sem_unlink("printf_sem");
	sem_unlink("stop_program_sem");
	sem_unlink("amt_philos_eat_enough_sem");
	if (philosopher->forks_sem)
		sem_close(philosopher->forks_sem);
	if (philosopher->printf_sem)
		sem_close(philosopher->printf_sem);
	if (philosopher->amt_philos_eat_enough_sem)
		sem_close(philosopher->amt_philos_eat_enough_sem);
	if (philosopher->stop_program_sem)
		sem_close(philosopher->stop_program_sem);
	if (philosopher->philo_sem)
		sem_close(philosopher->philo_sem);
}

void	free_philosophers(t_philosopher *philosopher)
{
	t_printable	*printable;

	if (philosopher->pid)
		free(philosopher->pid);
	while (philosopher->printable_head)
	{
		printable = philosopher->printable_head->content;
		ft_lst_remove_first_node(&philosopher->printable_head);
		free(printable->str);
		free(printable);
	}
	close_semaphores(philosopher);
	free(philosopher);
}

void	ft_error_parent(
	char *str, int exit_code, t_philosopher *philosopher, int started_philos)
{
	int	i;

	if (str)
		write(2, str, ft_strlen(str));
	if (philosopher->stop_program_sem)
		sem_post(philosopher->stop_program_sem);
	free_philosophers(philosopher);
	i = 0;
	while (i++ < started_philos)
	{
		waitpid(-1, NULL, 0);
	}
	exit(exit_code);
}

void	ft_error_child(
	char *str, int exit_code, t_philosopher *philosopher)
{
	if (str)
		write(2, str, ft_strlen(str));
	if (philosopher->stop_program_sem)
		sem_post(philosopher->stop_program_sem);
	sem_post(philosopher->philo_sem);
	while (1)
	{
		sem_wait(philosopher->philo_sem);
		if (philosopher->moniter_thread_stopped && \
		philosopher->printing_thread_stopped && \
		philosopher->check_death_thread_stopped)
		{
			free_philosophers(philosopher);
			exit(exit_code);
		}
		sem_post(philosopher->philo_sem);
		usleep(100);
	}
}
