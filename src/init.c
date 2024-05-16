

#include "philosophers.h"

int	parse_input(int argc, char *argv[], t_program *program)
{
	memset(program, 0, sizeof(t_program));
	if (argc < 5 || argc > 6)
		return (write(2, "wrong number of arguments\n", 26));
	program->num_philosophers = ft_atoi(argv[1]);
	if (program->num_philosophers <= 0)
		return (write(2, "wrong input number philosophers\n", 32));
	program->time_to_die = ft_atoi(argv[2]);
	if (program->time_to_die < 0)
		return (write(2, "wrong input time to die\n", 25));
	program->time_to_eat = ft_atoi(argv[3]);
	if (program->time_to_eat < 0)
		return (write(2, "wrong input time to eat\n", 25));
	program->time_to_sleep = ft_atoi(argv[4]);
	if (program->time_to_sleep < 0)
		return (write(2, "wrong input time to sleep\n", 26));
	program->num_times_to_eat = -1;
	if (argc == 6)
	{
		program->num_times_to_eat = ft_atoi(argv[5]);
		if (program->num_times_to_eat < 0)
			return (write(2, "wrong input times to eat\n", 26));
	}
	return (0);
}

int init_semaphores(t_program *program)
{
	sem_unlink("forks_sem");
	program->forks_sem = sem_open("forks_sem", O_CREAT, 0644, program->num_philosophers);
	if (program->forks_sem == SEM_FAILED)
		return (write(2, "failed init forks semaphore\n", 29));
	sem_unlink("printable_head_sem");
	program->printable_head_sem = sem_open("printable_head_sem", O_CREAT, 0644, 1);
	if (program->printable_head_sem == SEM_FAILED)
		return (write(2, "failed init printable head semaphore\n", 38));
	sem_unlink("stop_print_sem");
	program->stop_print_sem = sem_open("stop_print_sem", O_CREAT, 0644, 1);
	if (program->stop_print_sem == SEM_FAILED)
		return (write(2, "failed init stop print semaphore\n", 34));
}

void init_philosophers(t_philosopher *philosophers, t_program *program)
{
	ssize_t i;
	ssize_t start_time;

	i = -1;
	start_time = get_current_time();
	while (++i < program->num_philosophers)
	{
		memset(&philosophers[i], 0, sizeof(t_philosopher));
		philosophers[i].id = i + 1;
		philosophers[i].program = program;
		philosophers[i].last_meal_time = start_time;
		philosophers[i].start_time = start_time;
	}
}