#include "philo.h"

int check_args(char **argv)
{
	int i;
	int y;

	i = 1;
	while (argv[i])
	{
		y = 0;
		if (!argv[i][y])
			return 1;
		while (argv[i][y])
		{
			if (argv[i][y] >= '0' && argv[i][y] <= '9')
				y++;
			else
				return 1;

		}
		i++;
	}
	return 0;
}

void init_threads(t_philo *philo)
{

	int i;

	i = 0;
	pthread_create(&philo->table->death, NULL, check_death, philo);
	pthread_mutex_init(&philo->table->fork_lock, NULL);
	while (i < philo->table->rules.number_of_philosophers)
	{
		if ((i + 1) % 2 == 1)
			usleep(5000);
		pthread_create(&philo->thread,
			NULL, take_forks, philo);
		philo = philo->next;
		i++;
	}
}

int main(int argc, char **argv)
{
	t_philo *philosophers;

	if (argc < 5 || argc > 6)
		return 1;
	// if (check_args(argv))
	// 	return (printf("error"), 1);
	philosophers = init_data(argc, argv);
	// while (1)
	// {
	// 	printf("%d\n", philosophers->index);
	// 	philosophers = philosophers->next;
	// }

	init_threads(philosophers);
	pthread_join(philosophers->table->death, NULL);
	return 0;
}
