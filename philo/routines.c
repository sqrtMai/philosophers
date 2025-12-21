#include "philo.h"

void *update_time(void *tabl)
{
	int i;
	t_table *table;

	i = 0;
	table = (t_table *)tabl;
	while (table->has_someone_died == 0)
	{
		gettimeofday(&table->current_time, NULL);
		if (i++ == 0)
			table->start_time = table->current_time.tv_usec;
	}
}

void *take_forks(void *philos)
{
	t_philo *philo;

	philo = (t_philo *)philos;
}

void *eat(void *philos)
{
	t_philo *philo;

	philo = (t_philo *)philos;
	while (philo->fork1 && philo->fork2)
	{
		philo->last_meal = philo->table->current_time.tv_usec;
		printf("%ld %d is eating", philo->last_meal - philo->table->start_time, philo->index + 1);
		usleep(philo->table->rules.time_to_sleep);
	}
}
