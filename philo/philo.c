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

	pthread_create(&philo->table->update_time,
		NULL, update_time, philo->table);
	while (philo)
	{
		pthread_create(&philo->thread,
			NULL, take_forks, philo);
		pthread_create(&philo->death, NULL, check_death, philo);
		philo = philo->next;
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
	init_threads(philosophers);
	pthread_join(philosophers->table->update_time, NULL);
	return 0;
}
