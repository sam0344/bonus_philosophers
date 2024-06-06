/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleunin <saleunin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:03:29 by saleunin          #+#    #+#             */
/*   Updated: 2024/06/06 15:53:53 by saleunin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers2.h"

int	parse_input(int argc, char *argv[], t_philosopher *philosopher)
{
	memset(philosopher, 0, sizeof(t_philosopher));
	if (argc < 5 || argc > 6)
		ft_error("wrong number arguments\n", 1, philosopher);
	philosopher->num_philosophers = ft_atoi(argv[1]);
	if (philosopher->num_philosophers <= 0)
		ft_error("wrong input number philosophers\n", 1, philosopher);
	philosopher->time_to_die = ft_atoi(argv[2]);
	if (philosopher->time_to_die < 0)
		ft_error("wrong input time to die\n", 1, philosopher);
	philosopher->time_to_eat = ft_atoi(argv[3]);
	if (philosopher->time_to_eat < 0)
		ft_error("wrong input time to eat\n", 1, philosopher);
	philosopher->time_to_sleep = ft_atoi(argv[4]);
	if (philosopher->time_to_sleep < 0)
		ft_error("wrong input time to sleep\n", 1, philosopher);
	philosopher->num_times_to_eat = -1;
	if (argc == 6)
	{
		philosopher->num_times_to_eat = ft_atoi(argv[5]);
		if (philosopher->num_times_to_eat < 0)
			ft_error("wrong input times to eat\n", 1, philosopher);
	}
	return (0);
}

int init_semaphores(t_philosopher *philosopher)
{
	sem_unlink("forks_sem");
	philosopher->forks_sem = sem_open("forks_sem", O_CREAT, 0644, philosopher->num_philosophers);
	if (philosopher->forks_sem == SEM_FAILED)
		ft_error("failed init forks semaphore\n", 1, philosopher);
	sem_unlink("stop_print_sem");
	philosopher->stop_print_sem = sem_open("stop_print_sem", O_CREAT, 0644, 1);
	if (philosopher->stop_print_sem == SEM_FAILED)
		ft_error("failed init stop print semaphore\n", 1, philosopher);
	sem_unlink("stop_program_sem");
	philosopher->stop_program_sem = sem_open("stop_program_sem", O_CREAT, 0644, 0);
	if (philosopher->stop_program_sem == SEM_FAILED)
		ft_error("failed init stop program semaphore\n",1, philosopher);
	sem_unlink("amt_philos_eat_enough_sem");
	philosopher->amt_philos_eat_enough_sem = sem_open("amt_philos_eat_enough_sem", O_CREAT, 0644, 0);
	if (philosopher->amt_philos_eat_enough_sem == SEM_FAILED)
		ft_error("failed init stop program semaphore\n",1, philosopher);
	return (0);
}
