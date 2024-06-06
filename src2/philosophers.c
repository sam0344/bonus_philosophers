/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleunin <saleunin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 13:52:17 by saleunin          #+#    #+#             */
/*   Updated: 2024/06/06 15:53:56 by saleunin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers2.h"

void create_processes(t_philosopher *philosophers)
{
	int	i;
	i = 0;
	philosophers->start_time = get_current_time();
	while (i < philosophers->num_philosophers)
	{
		philosophers->id = i + 1;
		philosophers->pid[i] = fork();
		if (philosophers->pid[i] == 0)
			routine(philosophers);
		i++;
	}
}

void kill_processes(t_philosopher *philosopher)
{
	int i;

	i = -1;
	while (++i < philosopher->num_philosophers)
	{
		kill(philosopher->pid[i], SIGTERM);
	}
}

int main(int argc, char *argv[])
{
	t_philosopher *philosophers;
	int	i;

	philosophers = malloc(sizeof(t_philosopher));
	if (!philosophers)
		return (1);
	parse_input(argc, argv, philosophers);
	init_semaphores(philosophers);
	philosophers->pid = malloc(sizeof(pid_t) * philosophers->num_philosophers);
	if (!philosophers->pid)
		ft_error("failed to allocate for pid", 1, philosophers);
	create_processes(philosophers);
	i = -1;
	while (++i < philosophers->num_philosophers)
		sem_wait(philosophers->amt_philos_eat_enough_sem);
	sem_post(philosophers->stop_program_sem);
	usleep(5000);
	kill_processes(philosophers);
	free_philosophers(philosophers);
	return (0);
}
