/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers2.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleunin <saleunin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 12:51:13 by saleunin          #+#    #+#             */
/*   Updated: 2024/06/10 14:04:14 by saleunin         ###   ########.fr       */
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

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;
t_list	*ft_lstnew(void *content);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lst_remove_first_node(t_list **head);

typedef struct s_printable
{
	size_t		time_stamp;
	int			id;
	char		*str;
}	t_printable;

typedef struct s_philosopher
{
	int				num_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_times_to_eat;
	sem_t			*forks_sem;
	sem_t			*philo_sem;
	sem_t			*stop_print_sem;
	sem_t			*stop_program_sem;
	sem_t			*amt_philos_eat_enough_sem;
	pthread_t		death_thread;
	pid_t			*pid;
	ssize_t			last_meal_time;
	ssize_t			start_time;
	int				id;
	t_list			*printable_head;
	int				meals_eaten;
	bool			moniter_thread_stopped;
	bool			check_death_thread_stopped;
	bool			printing_thread_stopped;
	bool			stop;
	//				sem_t			*printable_head_sem;
}	t_philosopher;



int ft_strlen(const char *str);
int	parse_input(int argc, char *argv[], t_philosopher *philosopher);
int init_semaphores(t_philosopher *philosopher);
int	ft_atoi(char *num);
ssize_t	get_current_time(void);
void	routine(t_philosopher	*philosopher);
void ft_usleep(ssize_t time_to_sleep);
void free_philosophers(t_philosopher *philosopher);
void ft_error(char *str, int exit_code, t_philosopher *philosopher);
char	*ft_strdup(const char *s);
void add_to_printable(t_philosopher *philosopher, char *str);
char	*ft_itoa(int n);
char	*ft_strjoin(char const *s1, char const *s2);
void	*print_printable_thread(void *arg);
#endif