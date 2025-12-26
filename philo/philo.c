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
	pthread_mutex_init(&philo->table->go_m, NULL);
	while (i < philo->table->rules.p_number)
		pthread_mutex_init(&philo->table->fork_lock[i++], NULL);
}

void destroy_mutex(t_philo *philo)
{
	int i;

	i = 0;
	pthread_mutex_destroy(&philo->table->check_death);
	pthread_mutex_destroy(&philo->table->meal);
	pthread_mutex_destroy(&philo->table->go_m);
	while (i < philo->table->rules.p_number)
		pthread_mutex_destroy(&philo->table->fork_lock[i++]);
}

void init_threads(t_philo *philo)
{

	int i;

	i = 0;
	init_mutex(philo);
	//pthread_mutex_lock(&philo->table->go_m);
	pthread_create(&philo->table->death, NULL, check_death, philo);
	while (i++ < philo->table->rules.p_number)
	{
		pthread_create(&philo->thread,
			NULL, start_routine, philo);
		philo = philo->next;
	}
	philo->table->go = 1;
	//pthread_mutex_unlock(&philo->table->go_m);
}

int main(int argc, char **argv)
{
	t_philo *philosophers;
	int i;

	if (argc < 5 || argc > 6)
		return 1;
	// if (check_args(argv))
	// 	return (printf("error"), 1);
	i = 0;
	philosophers = init_data(argc, argv);

	init_threads(philosophers);
	pthread_join(philosophers->table->death, NULL);
	while (i < philosophers->table->rules.p_number)
	{
		pthread_join(philosophers->thread, NULL);
		i++;
		philosophers = philosophers->next;
	}
	destroy_mutex(philosophers);
	return 0;
}
