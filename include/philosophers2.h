/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers2.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleunin <saleunin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 12:51:13 by saleunin          #+#    #+#             */
/*   Updated: 2024/05/24 15:36:32 by saleunin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS2_H
# define PHILOSOPHERS2_H
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <stdbool.h>
# include <semaphore.h>
# include <fcntl.h>
# include <signal.h>

typedef struct s_philosopher
{
	int				num_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_times_to_eat;
	sem_t			*forks_sem;
	sem_t			*stop_print_sem;
	sem_t			*stop_program_sem;
	sem_t			*meal_lock_sem;
	pthread_t		death_thread;
	pid_t			*pid;
	ssize_t			last_meal_time;
	ssize_t			start_time;
	int				id;
	int				meals_eaten;
	bool			stop;
	//				sem_t			*printable_head_sem;
}	t_philosopher;


void ft_error(char *str, int exit_code);
int	parse_input(int argc, char *argv[], t_philosopher *philosopher);
int init_semaphores(t_philosopher *philosopher);
int	ft_atoi(char *num);
ssize_t	get_current_time(void);
void	routine(t_philosopher	*philosopher);
void ft_usleep(ssize_t time_to_sleep);
char	*ft_itoa(int n);
char	*ft_strjoin(char const *s1, char const *s2);
#endif