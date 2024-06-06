

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
	int	i;

	philosopher = (t_philosopher *)arg;
	while (!philosopher->stop)
	{
		if (get_current_time() - philosopher->last_meal_time > (ssize_t)philosopher->time_to_die)
		{
			sem_wait(philosopher->stop_print_sem);
			// printf("start time:%ld, last meal time:%ld\n", get_current_time() - philosopher->start_time, get_current_time() - philosopher->last_meal_time);
			printf("%ld %d %s\n", \
					get_current_time() - philosopher->start_time, philosopher->id, "died");
			i = -1;
			while (++i < philosopher->num_philosophers)
				sem_post(philosopher->amt_philos_eat_enough_sem);
			break ;
		}
		if (philosopher->num_times_to_eat != -1 && philosopher->meals_eaten >= philosopher->num_times_to_eat)
		{
			sem_post(philosopher->amt_philos_eat_enough_sem);
			break ;
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
	philosopher->meals_eaten++;
	print_text(philosopher, "is eating");
	philosopher->last_meal_time = get_current_time();
	ft_usleep(philosopher->time_to_eat);
	sem_post(philosopher->forks_sem);
	sem_post(philosopher->forks_sem);
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

void *stop_philo_died(void *arg)
{
	t_philosopher *philosopher;

	philosopher = (t_philosopher *)arg;
	sem_wait(philosopher->stop_program_sem);
	philosopher->stop = true;
	sem_post(philosopher->stop_program_sem);
	return (arg);
}

void	routine(t_philosopher	*philosopher)
{
	pthread_t check_philo_died;

	philosopher->last_meal_time = philosopher->start_time;
	pthread_create(&philosopher->death_thread, NULL, check_end_philo, philosopher);
	pthread_detach(philosopher->death_thread);
	pthread_create(&check_philo_died, NULL, stop_philo_died, philosopher);
	pthread_detach(check_philo_died);
	if (philosopher->id % 2 == 0)
		ft_usleep(philosopher->time_to_sleep);
	while (!philosopher->stop)
	{
		eat(philosopher);
		print_text(philosopher, "is sleeping");
		ft_usleep(philosopher->time_to_sleep);
		think(philosopher);
	}
	// ft_usleep(philosopher->time_to_eat);
	// sem_post(philosopher->stop_program_sem);
	// return (arg);
}

