
#include "philosophers.h"

void create_threads(t_philosopher *philosophers, t_program *program)
{
	pthread_t printing_thread;
	ssize_t i;

	pthread_create(&printing_thread, NULL, printable_thread, philosophers);
	i = -1;
	while (++i < program->num_philosophers)
	{
		philosophers[i].pid = fork();
		if (philosophers[i].pid == 0)
		{
			routine()
		}
	}
}

int main(int argc, char *argv[])
{
	t_program *program;
	t_philosopher *philosophers;

	program = malloc(sizeof(program));
	if (!program)
		return (1);
	if (parse_input(argc, argv, program))
		return (free(program), 2);
	philosophers = malloc(sizeof(t_philosopher) * program->num_philosophers);
	if (!philosophers)
		return (1);
	init_philosophers(philosophers, program);
	if (init_semaphores(program))
		return (free_philosophers(philosophers), 2);

}