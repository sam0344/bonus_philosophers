

#include "philosophers2.h"

// void print_text(t_philosopher *philosopher, char *str)
// {
// 	sem_wait(philosopher->printf_sem);
// 	if (!philosopher->stop)
// 		printf("%ld %d %s\n", get_current_time() - philosopher->start_time, philosopher->id, str);
// 	sem_post(philosopher->printf_sem);
// }

void *monitor_philo(void *arg)
{
	t_philosopher *philosopher;
	int	i;

	philosopher = (t_philosopher *)arg;
	while (1)
	{
		sem_wait(philosopher->philo_sem);
		// printf("here2\n");
		if (philosopher->stop)
			break ;
		if (get_current_time() - philosopher->last_meal_time > (ssize_t)philosopher->time_to_die)
		{
			sem_post(philosopher->stop_program_sem);
			sem_wait(philosopher->printf_sem);
			printf("%ld %d %s\n", \
					get_current_time() - philosopher->start_time, philosopher->id, "died");
			i = -1;
			while (++i < philosopher->num_philosophers)
				sem_post(philosopher->amt_philos_eat_enough_sem);
			break ;
		}
		if (philosopher->num_times_to_eat != -1 && philosopher->meals_eaten >= philosopher->num_times_to_eat)
		{
			philosopher->stop = true;
			sem_post(philosopher->amt_philos_eat_enough_sem);
			break ;
		}
		sem_post(philosopher->philo_sem);
		usleep(500);
	}
	philosopher->moniter_thread_stopped = true;
	sem_post(philosopher->philo_sem);
	return (arg);
}
static void	eat(t_philosopher *philosopher)
{
	sem_wait(philosopher->forks_sem);
	add_to_printable(philosopher, "has taken a fork");
	sem_wait(philosopher->forks_sem);
	add_to_printable(philosopher, "has taken a fork");
	add_to_printable(philosopher, "is eating");
	sem_wait(philosopher->philo_sem);
	philosopher->meals_eaten++;
	philosopher->last_meal_time = get_current_time();
	sem_post(philosopher->philo_sem);
	ft_usleep(philosopher->time_to_eat);
	sem_post(philosopher->forks_sem);
	sem_post(philosopher->forks_sem);
}

static void	think(t_philosopher *philosopher)
{
	add_to_printable(philosopher, "is thinking");
	if (philosopher->stop)
		return ;
	if (philosopher->num_philosophers % 2 == 1)
		ft_usleep(philosopher->time_to_eat * 2 - philosopher->time_to_sleep);
	while (get_current_time() - (ssize_t)philosopher->last_meal_time \
	< philosopher->time_to_eat * 2)
		usleep(100);
}

void *stop_philo_died(void *arg)
{
	t_philosopher *philosopher;

	philosopher = (t_philosopher *)arg;
	sem_wait(philosopher->stop_program_sem);
	sem_post(philosopher->stop_program_sem);
	sem_wait(philosopher->philo_sem);
	philosopher->stop = true;
	philosopher->check_death_thread_stopped = true;
	sem_post(philosopher->philo_sem);
	return (arg);
}

void	routine(t_philosopher	*philosopher)
{
	pthread_t check_philo_died;
	pthread_t	printing_thread;

	philosopher->last_meal_time = philosopher->start_time;
	pthread_create(&philosopher->death_thread, NULL, monitor_philo, philosopher);
	pthread_detach(philosopher->death_thread);
	pthread_create(&check_philo_died, NULL, stop_philo_died, philosopher);
	pthread_detach(check_philo_died);
	pthread_create(&printing_thread, NULL, print_printable_thread, philosopher);
	pthread_detach(printing_thread);
	if (philosopher->id % 2 == 0)
		ft_usleep(philosopher->time_to_eat);
	while (1)
	{
		eat(philosopher);
		sem_wait(philosopher->philo_sem);
		if (philosopher->stop)
		{
			sem_post(philosopher->philo_sem);
			while (1)
			{
				sem_wait(philosopher->philo_sem);
				if (philosopher->moniter_thread_stopped && philosopher->printing_thread_stopped && philosopher->check_death_thread_stopped)
				{
					free_philosophers(philosopher);
					exit(0);
				}
				usleep(100);
			}
		}
		sem_post(philosopher->philo_sem);
		add_to_printable(philosopher, "is sleeping");
		ft_usleep(philosopher->time_to_sleep);
		think(philosopher);
	}
	// ft_usleep(philosopher->time_to_eat);
	// sem_post(philosopher->stop_program_sem);
	// return (arg);
}
