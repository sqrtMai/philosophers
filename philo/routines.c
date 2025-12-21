#include "philo.h"

// void *update_time(void *tabl)
// {
// 	int i;
// 	t_table *table;

// 	i = 0;
// 	table = (t_table *)tabl;
// 	while (table->has_someone_died == 0)
// 	{
// 		pthread_mutex_lock(&table->time_lock);
// 		gettimeofday(&table->current_time, NULL);
// 		table->time = table->current_time.tv_sec * 1000000LL + table->current_time.tv_usec;
// 		if (i == 0)
// 		{
// 			table->start_time = table->time;
// 			i++;
// 		}
// 		pthread_mutex_unlock(&table->time_lock);
// 	}
// 	return (void *)1;
// }


void *check_death(void *philos)
{
	t_philo *philo;
	long long current_time;

	philo = (t_philo *)philos;
	philo->table->start_time = get_current_time();
	while (1)
	{
		current_time = get_current_time();
		if (philo->last_meal == 0)
			{
				if ((current_time - philo->table->start_time) >= philo->table->rules.time_to_die)
				{
					return (philo->dead = 1, philo->table->has_someone_died = 1, printf("%lld %d has died\n", get_current_time() - philo->table->start_time,
						 philo->index + 1), (void *)1);
				}
			}
		else
			{
				if (current_time - philo->last_meal >= philo->table->rules.time_to_die)
					return (philo->dead = 1, philo->table->has_someone_died = 1, printf("%lld %d has died\n", get_current_time() - philo->table->start_time,
						philo->index + 1), (void *) 1);
			}
		philo = philo->next;
	}
	return (void *)1;
}


void sleep_philo(t_philo *philo)
{
	printf("%lld %d is sleeping\n", get_current_time() - philo->table->start_time,
			philo->index + 1);
	usleep(philo->table->rules.time_to_sleep * 1000);
	take_forks((void *)philo);
}


void *eat(t_philo *philo)
{
	while (*philo->fork1 && *philo->fork2 && !philo->table->has_someone_died)
	{
		philo->last_meal = get_current_time();
		printf("%lld %d is eating\n", get_current_time() - philo->table->start_time,
			philo->index + 1);
		usleep(philo->table->rules.time_to_eat * 1000);
		*philo->fork1 = 0;
		*philo->fork2 = 0;
		pthread_mutex_unlock(&philo->table->fork_lock);
	}
	if (!philo->table->has_someone_died)
		sleep_philo(philo);
	return (void *)1;
}

//(philo->fork1[0] == 0 || philo->fork2[0] == 0) && !

void *take_forks(void *philos)
{
	t_philo *philo;

	philo = (t_philo *)philos;
	if (*philo->fork1 == 1 || *philo->fork2 == 1)
		printf("%lld %d is thinking\n", get_current_time() - philo->table->start_time,
			philo->index + 1);

	pthread_mutex_lock(&philo->table->fork_lock);
	*philo->fork1 = 1;
	printf("%lld %d has taken a fork\n", get_current_time() - philo->table->start_time,
		philo->index + 1);
	*philo->fork2 = 1;
	printf("%lld %d has taken a fork\n", get_current_time() - philo->table->start_time,
		philo->index + 1);
	if (!philo->table->has_someone_died)
		return eat(philo);
	return (void *)1;
}
