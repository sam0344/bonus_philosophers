

#include "philosophers.h"

void print_text(t_philosopher *philosopher, char *str)
{
	ssize_t time;

	time = get_current_time() - philosopher->start_time;
	printf("%zd %d %s\n", time, philosopher->id, str);
}

void *check_end_philo(void *arg)
{
	t_philosopher *philosopher;

	philosopher = (t_philosopher *)arg;
	while (1)
	{
		printf("tested\n");
		if (get_current_time() - philosopher->last_meal_time > (ssize_t)philosopher->program->time_to_die)
		{
			printf("died\n");
			sem_post(philosopher->program->stop_program_sem);
			sem_wait(philosopher->program->stop_print_sem);
			philosopher->program->stop_print = true;
			sem_post(philosopher->program->stop_print_sem);
			exit(1);
		}
		if (philosopher->program->time_to_eat != -1 && philosopher->meals_eaten > philosopher->program->num_times_to_eat)
		{
			printf("enough eaten\n");
			sem_post(philosopher->program->stop_program_sem);
			sem_wait(philosopher->program->stop_print_sem);
			philosopher->program->stop_print = true;
			sem_post(philosopher->program->stop_print_sem);
			exit(1);
		}
		usleep(100);
	}
	return (arg);
}

static void	even(t_philosopher *philosopher)
{
	sem_wait(philosopher->program->forks_sem);
	print_text(philosopher, "has taken a fork");
	// add_to_print(philosopher, "has taken a fork");
	sem_wait(philosopher->program->forks_sem);
	print_text(philosopher, "has taken a fork");
	// add_to_print(philosopher, "has taken a fork");
	// add_to_print(philosopher, "is eating");
	print_text(philosopher, "is eating");
	philosopher->meals_eaten++;
	philosopher->last_meal_time = get_current_time();
	ft_usleep(philosopher->program->time_to_eat);
	sem_post(philosopher->program->forks_sem);
	sem_post(philosopher->program->forks_sem);
}

static void	think(t_philosopher *philosopher)
{
	t_program	*program;

	program = philosopher->program;
	print_text(philosopher, "is thinking");
	// add_to_print(philosopher, "is thinking");
	if (program->num_philosophers % 2 == 1)
		ft_usleep(program->time_to_eat * 2 - program->time_to_sleep);
	while (get_current_time() - (ssize_t)philosopher->last_meal_time \
	< program->time_to_eat * 2)
		usleep(100);
}

// static bool	check_for_dead_philo(t_program *program)
// {
// 	pthread_mutex_lock(program->dead_lock);
// 	if (program->dead_philo)
// 		return (pthread_mutex_unlock(program->dead_lock), true);
// 	return (pthread_mutex_unlock(program->dead_lock), false);
// }

void	routine(t_philosopher	*philosopher)
{
	print_text(philosopher, "here");
	pthread_create(&philosopher->death_thread, NULL, check_end_philo, philosopher);
	pthread_detach(philosopher->death_thread);
	// if (philosopher->id % 2 == 0)
	// 	ft_usleep(philosopher->program->time_to_eat);
	// else if (philosopher->id == philosopher->program->num_philosophers)
	// 	ft_usleep(philosopher->program->time_to_eat * 2);
	while (1)
	{
		even(philosopher);
		// add_to_print(philosopher, "is sleeping");
		print_text(philosopher, "is sleeping");
		ft_usleep(philosopher->program->time_to_sleep);
		// if (check_for_dead_philo(philosopher->program))
		// 	break ;
		think(philosopher);
	}
	// return (arg);
}
