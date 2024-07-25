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

void create_processes(t_philosopher *philosophers)
{
	int	i;
	// char *pos_str;
	// char *sem_name;

	i = 0;
	philosophers->start_time = get_current_time();
	while (i < philosophers->num_philosophers)
	{
		// pos_str = ft_itoa(i + 1);
		// if (!pos_str)
		// 	ft_error("failed itoa pos str\n", 1, philosophers);
		// sem_name = ft_strjoin("philo_sem", pos_str);
		// free(pos_str);
		// if (!sem_name)
		// 	ft_error("failed strjoin sem_name\n", 1, philosophers);
		// printf("sem name:%s\n", sem_name);
		// sem_unlink(sem_name);
		// if (sem_unlink("philo_sem") < 0)
		// 	printf("fail unlink\n");
		philosophers->philo_sem = sem_open("philo_sem", O_CREAT | O_EXCL, 0644, 1);
		if (sem_unlink("philo_sem") < 0)
			printf("fail unlink \n");
		// free(sem_name);
		if (philosophers->philo_sem == SEM_FAILED)
			ft_error("failed to open philo_sem\n", 1, philosophers);
		philosophers->id = i + 1;
		philosophers->pid[i] = fork();
		if (philosophers->pid[i] == 0)
			routine(philosophers);
		sem_close(philosophers->philo_sem);
		i++;
	}
	// printf("finished creation\n");
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
	t_philosopher	*philosophers;
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
	// i = -1;
	// while (++i < philosophers->num_philosophers)
	// 	sem_wait(philosophers->amt_threads_finished_sem);
	// kill_processes(philosophers);
	free_philosophers(philosophers);
	return (0);
}
