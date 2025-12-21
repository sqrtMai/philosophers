#ifndef PHILO_H
# define PHILO_H

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

typedef struct s_rules
{
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_of_each_time;
	int six_args;
}	t_rules;

typedef struct s_table
{
	int				*fork;
	int				has_someone_died;
	t_rules 		rules;
	long long		start_time;
	long long		time;
	pthread_t		update_time;
	struct timeval	current_time;
}		t_table;

typedef struct s_philo
{
	pthread_t		thread;
	pthread_t		death;
	long	long	last_meal;
	int				*fork1;
	int				*fork2;
	int				index;
	t_table			*table;
	struct s_philo *next;
}	t_philo;


t_philo *init_data(int argc, char **argv);
int	ft_atoi(char *str);
void *update_time(void *tabl);
void *take_forks(void *philos);
void *check_death(void *philos);
#endif
