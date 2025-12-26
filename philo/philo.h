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
	long	p_number;
	long	t_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	meal_max;
	long six_args;
}	t_rules;

typedef struct s_table
{
	pthread_mutex_t *fork_lock;
	pthread_mutex_t	meal;
	int				dead;
	int				go;
	pthread_mutex_t go_m;
	t_rules 		rules;
	long long		start;
	pthread_t		update_time;
	pthread_t		death;
	pthread_t		create_philo;
	pthread_mutex_t check_death;
}		t_table;

typedef struct s_philo
{
	pthread_t		thread;

	long long		last_meal;
	int				meal_nbr;
	pthread_mutex_t	*fork1_lock;
	pthread_mutex_t	*fork2_lock;
	int				i;
	t_table			*table;
	struct s_philo *next;
}	t_philo;

void precise_usleep(long usec);
t_philo *init_data(int argc, char **argv);
int	ft_atoi(char *str);
void *update_time(void *tabl);
void *take_forks(void *philos);
void *start_routine(void *philos);
void *check_death(void *philos);
long long g_time(void);
#endif
