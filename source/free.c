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


void close_semaphores(t_philosopher *philosopher)
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

void free_philosophers(t_philosopher *philosopher)
{
	t_printable *printable;
	// free_philo_sems()
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

// void free_philo_sems(sem_t **philo_sems, int num_philosophers)
// {
// 	char *pos_str;
// 	char *sem_name;
// 	int	i;
// 	i = 0;
// 	while (i < num_philosophers)
// 	{
// 		pos_str = ft_itoa(i + 1);
// 		if (!pos_str)
// 			exit(1);
// 		sem_name = ft_strjoin("philo_sem:", pos_str);
// 		free(pos_str);
// 		if (!sem_name)
// 			exit(1);
// 		unlink(sem_name);
// 		free(sem_name);
// 		sem_close(philo_sems[i]);
// 		i++;
// 	}
// }