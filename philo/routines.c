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
		table->time = table->current_time.tv_sec * 1000000LL + table->current_time.tv_usec;
		if (i == 0)
		{
			table->start_time = table->time;
			i++;
		}
	}
	return (void *)1;
}

void *check_death(void *philos)
{
	t_philo *philo;

	philo = (t_philo *)philos;
	while (philo->table->has_someone_died == 0)
	{
		if (philo->last_meal == 0)
			{
				if ((philo->table->time - philo->table->start_time) >= philo->table->rules.time_to_die)
				{
					return (philo->table->has_someone_died = 1, printf("%d has died\n", philo->index + 1), (void *)1);
				}
			}
		else
			{
				if (philo->table->time - philo->last_meal >= philo->table->rules.time_to_die)
					return (philo->table->has_someone_died = 1, printf("%d has died\n", philo->index + 1), (void *) 1);
			}
	}
	return (void *)1;
}


void sleep_philo(t_philo *philo)
{
	printf("%d is sleeping\n",
			philo->index + 1);
	usleep(philo->table->rules.time_to_sleep);
	take_forks((void *)philo);
}


void *eat(t_philo *philo)
{
	while (*philo->fork1 && *philo->fork2 && !philo->table->has_someone_died)
	{
		philo->last_meal = philo->table->time;
		printf("%d is eating\n",
			philo->index + 1);
		usleep(philo->table->rules.time_to_sleep);
		*philo->fork1 = 0;
		*philo->fork2 = 0;
		// pthread_mutex_unlock(&philo->fork1);
		// pthread_mutex_unlock(&philo->fork2);
	}
	if (!philo->table->has_someone_died)
		sleep_philo(philo);
	return (void *)1;
}

void *take_forks(void *philos)
{
	t_philo *philo;

	philo = (t_philo *)philos;
	if ((philo->index + 1) % 2 == 1)
			usleep(500);
	while ((*philo->fork1 == 0 || *philo->fork2 == 0) && !philo->table->has_someone_died)
	{
		*philo->fork1 = 1;
		printf("%d has taken a fork\n", philo->index + 1);
		*philo->fork2 = 1;
		printf("%d has taken a fork\n", philo->index + 1);
		if (*philo->fork1 == 0 || *philo->fork2 == 0)
			printf("%d is thinking\n", philo->index + 1);
		// if (philo->fork1)
		// 	pthread_mutex_lock(&philo->fork1);
		// if (philo->fork2)
		// 	pthread_mutex_lock(&philo->fork2);
	}
	if (!philo->table->has_someone_died)
		return eat(philo);
	return (void *)1;
}


