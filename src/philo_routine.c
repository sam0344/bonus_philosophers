

#include "philosophers.h"

void check_end_philo(void *arg)
{
	t_philosopher *philosopher;

	philosopher = (t_philosopher *)arg;
	while (1)
	{
		if (get_current_time() - philosopher->last_meal_time > philosopher->program->time_to_die)
		{
			sem_post(philosopher->program->stop_program_sem);
			sem_wait(philosopher->program->stop_print_sem);
			philosopher->program->stop_print = true;
			sem_post(philosopher->program->stop_print_sem);
			break ;
		}
		if (philosopher->program->time_to_eat != -1 && philosopher->meals_eaten > philosopher->program->num_times_to_eat)
		{
			sem_post(philosopher->program->stop_program_sem);
			sem_wait(philosopher->program->stop_print_sem);
			philosopher->program->stop_print = true;
			sem_post(philosopher->program->stop_print_sem);
			break ;
		}
		usleep(10);
	}
}

static void	even(t_philosopher *philosopher)
{
	sem_wait(philosopher->program->forks_sem);
	add_to_print(philosopher, "has taken a fork");
	sem_wait(philosopher->program->forks_sem);
	add_to_print(philosopher, "has taken a fork");
	add_to_print(philosopher, "is eating");
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
	add_to_print(philosopher, "is thinking");
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

void	*routine(void *arg)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)arg;
	// if (philosopher->id % 2 == 0)
	// 	ft_usleep(philosopher->program->time_to_eat);
	// else if (philosopher->id == philosopher->program->num_philosophers)
	// 	ft_usleep(philosopher->program->time_to_eat * 2);
	while (1)
	{
		even(philosopher);
		add_to_print(philosopher, "is sleeping");
		ft_usleep(philosopher->program->time_to_sleep);
		// if (check_for_dead_philo(philosopher->program))
		// 	break ;
		think(philosopher);
	}
	return (arg);
}
