#include "philo.h"



void	ft_putnbr_fd(long long n, int fd)
{
	char	c;
	long long	nbr;

	nbr = n;
	if (n < 0)
	{
		write(fd, "-", 1);
		nbr *= -1;
	}
	c = nbr % 10 + '0';
	if (nbr / 10)
	{
		ft_putnbr_fd(nbr / 10, fd);
	}
	write(fd, &c, 1);
}

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
	int i;

	philo = (t_philo *)philos;
	i = 0;
	pthread_mutex_lock(&philo->table->go_m);
	philo->table->start = g_time() + (philo->table->rules.p_number * 2 * 10);
	pthread_mutex_unlock(&philo->table->go_m);
	while (i < philo->table->rules.p_number)
	{
		philo->last_meal = g_time() + (philo->table->rules.p_number * 2 * 10);
		philo = philo->next;
		i++;
	}

	while (philo->table->dead == 0)
	{
		m_lock(philo);
		if (g_time() - philo->last_meal >= philo->table->rules.t_die)
			return (printf("%lld %d died\n", g_time() - philo->table->start,
					philo->i + 1), philo->table->dead++, m_unlock(philo));
		if (philo->table->rules.six_args && check_meals(philo))
			return philo->table->dead++, m_unlock(philo);
		m_unlock(philo);
		philo = philo->next;
	}
	return ((void *)1);
}

void *sleep_philo(t_philo *philo)
{
	if (is_dead(philo->table))
		return (void *)1;
	printf("%lld %d is sleeping\n",
			g_time() - philo->table->start,
			philo->i + 1);
	precise_usleep(philo->table->rules.time_to_sleep * 1000);

	if (is_dead(philo->table))
		return (void *)1;

	printf("%lld %d is thinking\n", g_time() - philo->table->start,
			philo->i + 1);
	return (take_forks((void *)philo));
}

void *eat(t_philo *philo)
{
	// if (philo->last_meal && g_time() - philo->last_meal >= philo->table->rules.t_die - 15)

	// 	printf("%lld %d TIME LEFT -------------> %lld\n", g_time() - philo->last_meal, philo->i + 1, g_time() - philo->last_meal);
	if (is_dead(philo->table))
		return pthread_mutex_unlock(philo->fork1_lock),
			pthread_mutex_unlock(philo->fork2_lock),(void *)1;
	pthread_mutex_lock(&philo->table->meal);
	philo->last_meal = g_time();
	printf("%lld %d is eating\n", philo->last_meal - philo->table->start, philo->i + 1);
	philo->meal_nbr++;
	pthread_mutex_unlock(&philo->table->meal);
	if (is_dead(philo->table))
		return pthread_mutex_unlock(philo->fork1_lock),
			pthread_mutex_unlock(philo->fork2_lock),(void *)1;
	precise_usleep(philo->table->rules.time_to_eat * 1000);
	pthread_mutex_unlock(philo->fork1_lock);
	pthread_mutex_unlock(philo->fork2_lock);
	return (sleep_philo(philo));
}

void *take_forks(void *philos)
{
	t_philo *philo;

	philo = (t_philo *)philos;
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
	// precise_usleep(150);
	return eat(philo);
}

void *start_routine(void *philos)
{
	t_philo *philo;

	philo = (t_philo *)philos;
	precise_usleep(500);
	while (1)
	{
		pthread_mutex_lock(&philo->table->go_m);
		//printf("Time before start : %lld\n", philo->table->start - g_time());
		if (philo->table->start - g_time() == 0)
		{
			pthread_mutex_unlock(&philo->table->go_m);
			break;
		}
		pthread_mutex_unlock(&philo->table->go_m);
	}
	if (philo->i % 2 == 0)
		precise_usleep(250);
	return take_forks(philo);
}
