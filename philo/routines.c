#include "philo.h"

long get_elapsed_time_microseconds(struct timeval start, struct timeval end)
{
    return (end.tv_sec - start.tv_sec) * 1000000L + (end.tv_usec - start.tv_usec);
}

void precise_usleep(long usec) {
	struct timeval start, current;
	long elapsed;
	long rem;

	elapsed = 0;
	gettimeofday(&start, NULL);
	while (elapsed < usec)
	{
		gettimeofday(&current, NULL);
		elapsed = get_elapsed_time_microseconds(start, current);
		rem = usec - elapsed;
		if (rem > 1000)
			usleep(rem / 2);
	}
}

int is_dead(t_table *table)
{
	int dead;

	pthread_mutex_lock(&table->check_death);
	dead = table->dead;
	pthread_mutex_unlock(&table->check_death);
	return dead;
}

int check_meals(void *philos)
{
	t_philo *philo;
	int i;

	philo = (t_philo *)philos;
	i = 0;
	while (i < philo->table->rules.p_number)
	{
		//printf("----------------------------------------------------> %d\n", philo->meal_nbr);
		if (philo->meal_nbr < philo->table->rules.meal_max)
			return 0;
		i++;
	}
	return 1;
}

void	m_lock(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->check_death);
	pthread_mutex_lock(&philo->table->meal);
}

void	*m_unlock(t_philo *philo)
{
	pthread_mutex_unlock(&philo->table->meal);
	pthread_mutex_unlock(&philo->table->check_death);
	precise_usleep(1000);
	return ((void *) 1);
}

void *check_death(void *philos)
{
	t_philo	*philo;

	philo = (t_philo *)philos;
	while (philo->table->dead == 0)
	{
		m_lock(philo);
		if (philo->last_meal == 0)
		{
			if (g_time() - philo->table->start >= philo->table->rules.t_die)
				return (printf("%lld %d died\n", g_time() - philo->table->start,
						philo->i + 1), philo->table->dead = 1, m_unlock(philo));
		}
		else
		{
			if (g_time() - philo->last_meal >= philo->table->rules.t_die)
				return (printf("%lld %d died\n", g_time() - philo->table->start,
						philo->i + 1), philo->table->dead++, m_unlock(philo));
		}
		if (philo->table->rules.six_args && check_meals(philo))
			return philo->table->dead++, m_unlock(philo);
		m_unlock(philo);
		philo = philo->next;
	}
	return ((void *)1);
}

void *sleep_philo(t_philo *philo)
{
	printf("%lld %d is sleeping\n",
			g_time() - philo->table->start,
			philo->i + 1);
	precise_usleep(philo->table->rules.time_to_sleep * 1000);

	if (!is_dead(philo->table))
		return (printf("%lld %d is thinking\n",
			g_time() - philo->table->start,
			philo->i + 1),
		take_forks((void *)philo));
	precise_usleep(500);
	return (void *)1;
}

void *eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->meal);
	philo->last_meal = g_time();
	printf("%lld %d is eating\n", philo->last_meal - philo->table->start,
		philo->i + 1);
	philo->meal_nbr++;
	pthread_mutex_unlock(&philo->table->meal);
	if (is_dead(philo->table))
		return pthread_mutex_unlock(philo->fork1_lock),
			pthread_mutex_unlock(philo->fork2_lock),(void *)1;
	precise_usleep(philo->table->rules.time_to_eat * 1000);
	pthread_mutex_unlock(philo->fork1_lock);
	precise_usleep(150);
	pthread_mutex_unlock(philo->fork2_lock);

	precise_usleep(500);
	if (!is_dead(philo->table))
		return (sleep_philo(philo), (void *)1);
	return (void *)1;
}

void *take_forks_reverse(t_philo *philo)
{
	if (is_dead(philo->table))
		return (void *)1;
	pthread_mutex_lock(philo->fork2_lock);
	if (is_dead(philo->table))
		return pthread_mutex_unlock(philo->fork2_lock), (void *)1;
	printf("%lld %d has taken a fork\n", g_time() - philo->table->start,
		philo->i + 1);
	pthread_mutex_lock(philo->fork1_lock);
	if (is_dead(philo->table))
		return pthread_mutex_unlock(philo->fork1_lock),
		pthread_mutex_unlock(philo->fork2_lock), (void *)1;
	printf("%lld %d has taken a fork\n", g_time() - philo->table->start,
		philo->i + 1);
	precise_usleep(500);
	return eat(philo);
}
void *take_forks(void *philos)
{
	t_philo *philo;

	philo = (t_philo *)philos;
	// if (philo->i + 1 == 0)
	// 	take_forks_reverse(philo);
	// else
	// {
		if (is_dead(philo->table))
			return (void *)1;
		pthread_mutex_lock(philo->fork1_lock);
		if (is_dead(philo->table))
			return pthread_mutex_unlock(philo->fork1_lock), (void *)1;
		printf("%lld %d has taken a fork\n", g_time() - philo->table->start,
			philo->i + 1);
		pthread_mutex_lock(philo->fork2_lock);
		if (is_dead(philo->table))
			return pthread_mutex_unlock(philo->fork1_lock),
				pthread_mutex_unlock(philo->fork2_lock), (void *)1;
		printf("%lld %d has taken a fork\n", g_time() - philo->table->start,
			philo->i + 1);
		precise_usleep(500);
		return eat(philo);
	// }
	// return (void *)1;
}
