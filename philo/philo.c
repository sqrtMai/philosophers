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


void init_mutex(t_philo *philo)
{
	int i;

	i = 0;
	pthread_mutex_init(&philo->table->check_death, NULL);
	pthread_mutex_init(&philo->table->meal, NULL);
	while (i < philo->table->rules.p_number)
		pthread_mutex_init(&philo->table->fork_lock[i++], NULL);
}

void destroy_mutex(t_philo *philo)
{
	int i;

	i = 0;
	pthread_mutex_destroy(&philo->table->check_death);
	while (i < philo->table->rules.p_number)
		pthread_mutex_destroy(&philo->table->fork_lock[i++]);
}

void init_threads(t_philo *philo)
{

	int i;

	i = 0;
	init_mutex(philo);
	philo->table->start = g_time();
	pthread_create(&philo->table->death, NULL, check_death, philo);
	while (i < philo->table->rules.p_number)
	{
		if (i + 1 % 2)
			usleep(500);
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
	int i = 0;
	while (i < philosophers->table->rules.p_number)
	{
		pthread_join(philosophers->thread, NULL);
		i++;
		philosophers = philosophers->next;
	}
	//destroy_mutex(philosophers);
	return 0;
}
