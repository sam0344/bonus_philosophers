/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleunin <saleunin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 13:24:07 by saleunin          #+#    #+#             */
/*   Updated: 2024/06/06 15:53:51 by saleunin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers2.h"


void close_semaphores(t_philosopher *philosopher)
{
	if (philosopher->forks_sem)
		sem_close(philosopher->forks_sem);
	if (philosopher->stop_print_sem)
		sem_close(philosopher->stop_print_sem);
	if (philosopher->amt_philos_eat_enough_sem)
		sem_close(philosopher->amt_philos_eat_enough_sem);
	if (philosopher->stop_program_sem)
		sem_close(philosopher->stop_program_sem);
}

void free_philosophers(t_philosopher *philosopher)
{
	close_semaphores(philosopher);
	if (philosopher->pid)
		free(philosopher->pid);
	free(philosopher);
}