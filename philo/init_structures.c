#include "philo.h"

void init_forks_to_zero(t_table *table)
{
	int i;

	i = 0;
	while (i < (sizeof(table->fork) / sizeof(int)))
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
	table->fork = malloc(sizeof(int) * table->rules.number_of_philosophers);
	if (!table->fork)
		return NULL;
	init_forks_to_zero(table);
	return table;
}

void init_philo_to_null(t_philo *philo)
{
	philo->next = NULL;
	philo->table = NULL;
	philo->last_meal = 0;
	philo->thread = 0;
}

t_philo *init_philosophers(t_table *table)
{
	int i;
	t_philo *philo;
	t_philo *head;

	i = 0;
	while (i < table->rules.number_of_philosophers)
	{
		philo = malloc(sizeof(t_philo));
		init_philo_to_null(philo);
		if (i == 0)
		{
			head = philo;
			philo->fork1 = philo->table->fork[philo->table->rules.number_of_philosophers - 1];
			philo->fork2 = philo->table->fork[0];
		}
		else
		{
			philo->fork1 = philo->table->fork[i - 1];
			philo->fork2 = philo->table->fork[i];
		}
		philo->table = table;
		philo = philo->next;
		philo->index = i++;
	}
	return head;
}

t_philo *init_data(int argc, char **argv)
{
	t_table *table;

	table = init_table(argc, argv);
	return init_philosophers(table);
}
