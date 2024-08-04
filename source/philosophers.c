/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleunin <saleunin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 13:52:17 by saleunin          #+#    #+#             */
/*   Updated: 2024/06/10 14:10:21 by saleunin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	create_processes(t_philosopher *philosophers)
{
	int		i;

	i = -1;
	philosophers->start_time = get_current_time();
	while (++i < philosophers->num_philosophers)
	{
		sem_unlink("philo_sem");
		if (errno != 0 && errno != ENOENT)
			ft_error_parent("failed unlink semaphore", errno, philosophers, 0);
		philosophers->philo_sem = sem_open(
				"philo_sem", O_CREAT | O_EXCL, 0644, 1);
		if (philosophers->philo_sem == SEM_FAILED)
		{
			philosophers->philo_sem = NULL;
			ft_error_parent(
				"failed to open philo_sem\n", errno, philosophers, i);
		}
		philosophers->id = i + 1;
		philosophers->pid = fork();
		if (philosophers->pid == 0)
			philo_start(philosophers);
		else if (philosophers->pid < 0)
			ft_error_parent("failed fork\n", errno, philosophers, i);
		sem_close(philosophers->philo_sem);
	}
}

int	main(int argc, char *argv[])
{
	t_philosopher	*philosophers;
	int				i;

	philosophers = malloc(sizeof(t_philosopher));
	if (!philosophers)
		return (1);
	parse_input(argc, argv, philosophers);
	init_semaphores(philosophers);
	create_processes(philosophers);
	i = -1;
	while (++i < philosophers->num_philosophers)
		sem_wait(philosophers->amt_philos_eat_enough_sem);
	sem_post(philosophers->stop_program_sem);
	i = -1;
	while (++i < philosophers->num_philosophers)
		waitpid(-1, NULL, 0);
	free_philosophers(philosophers);
	return (0);
}
