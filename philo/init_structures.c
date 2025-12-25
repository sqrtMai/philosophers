#include "philo.h"

// void init_forks_to_zero(t_table *table)
// {
// 	int i;
// 	int len;
// 	i = 0;
// 	len = table->rules.p_number;
// 	while (i < len)
// 		table->fork[i++] = 0;
// }

t_table *init_table(int argc, char **argv)
{
	t_table *table;

	table = malloc(sizeof(t_table));
	table->rules.p_number = ft_atoi(argv[1]);
	table->rules.t_die = ft_atoi(argv[2]);
	table->rules.time_to_eat = ft_atoi(argv[3]);
	table->rules.time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
	{
		table->rules.meal_max = ft_atoi(argv[5]);
		table->rules.six_args = 1;
	}
	table->update_time = 0;
	table->dead = 0;
	table->fork_lock = malloc(sizeof(pthread_mutex_t) * table->rules.p_number);
	if (!table->fork_lock)
		return (NULL);
	return (table);
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
	while (i < table->rules.p_number)
	{
		init_philo_to_null(philo);
		philo->table = table;
		if (i == 0)
		{
			head = philo;
			philo->fork1_lock = &philo->table->fork_lock[philo->table->rules.p_number - 1];
			philo->fork2_lock = &philo->table->fork_lock[0];
		}
		else
		{
			philo->fork1_lock = &philo->table->fork_lock[i - 1];
			philo->fork2_lock = &philo->table->fork_lock[i];
		}
		// if (i == table->rules.p_number - 1)
		// {
		// 	philo->fork1_lock = &philo->table->fork_lock[i];
		// 	philo->fork2_lock = &philo->table->fork_lock[i - 1];
		// }
		philo->i = i++;
		if (i < table->rules.p_number)
		{
			philo->next = malloc(sizeof(t_philo));
			philo = philo->next;
		}
	}
	philo->next = head;
	return head;
}

t_philo *init_data(int argc, char **argv)
{
	t_table *table;

	table = init_table(argc, argv);
	return init_philosophers(table);
}
