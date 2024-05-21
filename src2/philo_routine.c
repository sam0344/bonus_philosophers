

#include "philosophers2.h"

void print_text(t_philosopher *philosopher, char *str)
{
	sem_wait(philosopher->stop_print_sem);
	if (!philosopher->stop)
		printf("%ld %d %s\n", get_current_time() - philosopher->start_time, philosopher->id, str);
	sem_post(philosopher->stop_print_sem);
}

void *check_end_philo(void *arg)
{
	t_philosopher *philosopher;

	philosopher = (t_philosopher *)arg;
	while (1)
	{
		if (get_current_time() - philosopher->last_meal_time > (ssize_t)philosopher->time_to_die)
		{
			sem_wait(philosopher->stop_print_sem);
			printf("%ld %d %s\n", \
					get_current_time() - philosopher->start_time, philosopher->id, "died");
			philosopher->stop = true;
			free(philosopher->pid);
			sem_close(philosopher->forks_sem);
			sem_close(philosopher->stop_print_sem);
			free(philosopher);
			exit(1);
		}
		if (philosopher->num_times_to_eat != -1 && philosopher->meals_eaten >= philosopher->num_times_to_eat)
		{
			philosopher->stop = true;
			/* printf("%ld %d %s\n", \
					get_current_time() - philosopher->start_time, philosopher->id, "eaten enough"); */
			free(philosopher->pid);
			sem_close(philosopher->forks_sem);
			sem_close(philosopher->stop_print_sem);
			exit(0);
		}
		usleep(100);
	}
	return (arg);
}

static void	eat(t_philosopher *philosopher)
{
	sem_wait(philosopher->forks_sem);
	print_text(philosopher, "has taken a fork");
	sem_wait(philosopher->forks_sem);
	print_text(philosopher, "has taken a fork");
	// if (philosopher->stop)
	// 		return ;
	print_text(philosopher, "is eating");
	philosopher->last_meal_time = get_current_time();
	ft_usleep(philosopher->time_to_eat);
	sem_post(philosopher->forks_sem);
	sem_post(philosopher->forks_sem);
	philosopher->meals_eaten++;
}

static void	think(t_philosopher *philosopher)
{
	print_text(philosopher, "is thinking");
	if (philosopher->stop)
		return ;
	if (philosopher->num_philosophers % 2 == 1)
		ft_usleep(philosopher->time_to_eat * 2 - philosopher->time_to_sleep);
	while (get_current_time() - (ssize_t)philosopher->last_meal_time \
	< philosopher->time_to_eat * 2)
		usleep(100);
}

void	routine(t_philosopher	*philosopher)
{
	philosopher->last_meal_time = philosopher->start_time;
	pthread_create(&philosopher->death_thread, NULL, check_end_philo, philosopher);
	pthread_detach(philosopher->death_thread);
	while (!philosopher->stop)
	{
		eat(philosopher);
		if (philosopher->stop)
			break ;
		print_text(philosopher, "is sleeping");
		if (philosopher->stop)
			break ;
		ft_usleep(philosopher->time_to_sleep);
		think(philosopher);

	}
	// pthread_join(philosopher->death_thread, NULL);
	// return (arg);
}
