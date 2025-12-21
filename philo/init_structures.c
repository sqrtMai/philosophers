#include "philo.h"

void init_forks_to_zero(t_table *table)
{
	int i;
	int len;
	i = 0;
	len = table->rules.number_of_philosophers;
	while (i < len)
		table->fork[i++] = 0;
}

t_table *init_table(int argc, char **argv)
{
	t_table *table;

	table = malloc(sizeof(t_table));
	table->rules.number_of_philosophers = ft_atoi(argv[1]);
	table->rules.time_to_die = ft_atoi(argv[2]);
	table->rules.time_to_eat = ft_atoi(argv[3]);
	table->rules.time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
	{
		table->rules.number_of_each_time = ft_atoi(argv[5]);
		table->rules.six_args = 1;
	}
	table->has_someone_died = 0;
	table->update_time = 0;
	table->time = 0;
	table->start_time = 0;
	if (table->rules.number_of_philosophers == 1)
		table->fork = malloc(sizeof(int) * (table->rules.number_of_philosophers + 1));
	else
		table->fork = malloc(sizeof(int) * table->rules.number_of_philosophers);
	if (!table->fork)
		return NULL;
	//memset(table->fork, 0, sizeof(int) * table->rules.number_of_philosophers);
	init_forks_to_zero(table);
	return table;
}

void init_philo_to_null(t_philo *philo)
{
	philo->next = NULL;
	philo->table = NULL;
	philo->last_meal = 0;
}

t_philo *init_philosophers(t_table *table)
{
	int i;
	t_philo *philo;
	t_philo *head;

	i = 0;
	philo = malloc(sizeof(t_philo));
	while (i < table->rules.number_of_philosophers)
	{
		init_philo_to_null(philo);
		philo->table = table;
		if (i == 0)
		{
			head = philo;
			philo->fork1 = &philo->table->fork[philo->table->rules.number_of_philosophers];
			philo->fork2 = &philo->table->fork[0];
		}
		else
		{
			philo->fork1 = &philo->table->fork[i - 1];
			philo->fork2 = &philo->table->fork[i];
		}
		// pthread_mutex_init(philo->fork1, NULL);
		// pthread_mutex_init(philo->fork2, NULL);
		philo->index = i++;
		philo->next = malloc(sizeof(t_philo));
		philo = philo->next;
	}
	return head;
}

t_philo *init_data(int argc, char **argv)
{
	t_table *table;

	table = init_table(argc, argv);
	return init_philosophers(table);
}
