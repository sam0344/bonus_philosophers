
#include "philosophers.h"

void create_threads(t_philosopher *philosophers, t_program *program)
{
	// pthread_t printing_thread;
	ssize_t i;

	// pthread_create(&printing_thread, NULL, printable_thread, philosophers);
	i = -1;
	while (++i < program->num_philosophers)
	{
		philosophers[i].pid = fork();
		if (philosophers[i].pid == 0)
		{
			routine(&philosophers[i]);
		}
	}
}

void kill_processes(t_philosopher *philosophers)
{
	ssize_t i;

	i = -1;
	while (++i < philosophers->program->num_philosophers)
	{
		kill(philosophers[i].pid, SIGKILL);
	}
}

int main(int argc, char *argv[])
{
	t_program *program;
	t_philosopher *philosophers;

	program = malloc(sizeof(t_program));
	if (!program)
		return (1);
	if (parse_input(argc, argv, program))
		return (free(program), 2);
	philosophers = malloc(sizeof(t_philosopher) * 2);
	if (!philosophers)
		return (free(program), 1);
	init_philosophers(philosophers, program);
	if (init_semaphores(program))
		return (2);
	create_threads(philosophers, program);
	sem_wait(program->stop_program_sem);
	kill_processes(philosophers);
}