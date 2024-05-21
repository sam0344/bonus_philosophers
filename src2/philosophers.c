
#include "philosophers2.h"

// void create_threads(t_philosopher *philosophers, t_program *program)
// {
// 	// pthread_t printing_thread;
// 	ssize_t i;

// 	// pthread_create(&printing_thread, NULL, printable_thread, philosophers);
// 	i = -1;
// 	while (++i < program->num_philosophers)
// 	{
// 		philosophers[i].pid = fork();
// 		if (philosophers[i].pid == 0)
// 		{
// 			routine(&philosophers[i]);
// 		}
// 	}
// }

// void kill_processes(t_philosopher *philosophers)
// {
// 	ssize_t i;

// 	i = -1;
// 	while (++i < philosophers->program->num_philosophers)
// 	{
// 		kill(philosophers[i].pid, SIGKILL);
// 	}
// }

void check_end(t_philosopher *philosopher)
{
	ssize_t i;
	int status;
	i = 0;
	while (i < philosopher->num_philosophers)
	{
		waitpid(-1, &status, 0);
		if (status != 0)
		{
			i = -1;
			while (++i < philosopher->num_philosophers)
				kill(philosopher->pid[i], SIGKILL);
			break ;
		}
		i++;
	}
	free(philosopher->pid);
	sem_close(philosopher->forks_sem);
	sem_close(philosopher->stop_print_sem);
	sem_unlink("forks_sem");
	sem_unlink("stop_print_sem");
	free(philosopher);
}

int main(int argc, char *argv[])
{
	t_philosopher *philosophers;
	int i;

	philosophers = malloc(sizeof(t_philosopher));
	if (!philosophers)
		return (1);
	parse_input(argc, argv, philosophers);
	init_semaphores(philosophers);
	philosophers->pid = malloc(sizeof(pid_t) * philosophers->num_philosophers);
	i = 0;
	while (i < philosophers->num_philosophers)
	{

		philosophers->start_time = get_current_time();
		philosophers->id = i + 1;
		philosophers->pid[i] = fork();
		if (philosophers->pid[i] == 0)
			routine(philosophers);
		i++;
	}
	check_end(philosophers);
	return (0);
}
