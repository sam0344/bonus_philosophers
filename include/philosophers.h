#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <sys/types.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <stdbool.h>
# include <semaphore.h>
# include <fcntl.h>
# include <signal.h>
#
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
	ssize_t		time_stamp;
	int			id;
	char		*str;
}	t_printable;

typedef struct s_program
{
	int				num_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_times_to_eat;
	sem_t			*forks_sem;
	sem_t			*stop_program_sem;
	sem_t			*printable_head_sem;
	sem_t			*stop_print_sem;
	bool			stop_print;
	t_list			*printable_list;
}	t_program;

typedef struct s_philosopher
{
	pthread_t			death_thread;
	t_program			*program;
	pid_t				pid;
	ssize_t				last_meal_time;
	ssize_t				start_time;
	int					id;
	int					meals_eaten;
}	t_philosopher;

int	parse_input(int argc, char *argv[], t_program *program);
int init_semaphores(t_program *program);
void init_philosophers(t_philosopher *philosophers, t_program *program);
ssize_t	get_current_time(void);
void	*printable_thread(void *arg);
void	ft_usleep(ssize_t wait_time);
int	ft_atoi(char *num);
void	add_to_print(t_philosopher *philosopher, char *str);
void	routine(t_philosopher	*philosopher);
char	*ft_strdup(char *s);
#endif