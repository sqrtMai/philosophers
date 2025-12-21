#ifndef PHILO_H
# define PHILO_H

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


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
	pthread_t		update_time;
	t_rules 		rules;
	int				start_time;
	struct timeval	current_time;
}		t_table;

typedef struct s_philo
{
	pthread_t		thread;
	int				last_meal;
	int				*fork1;
	int				*fork2;
	t_table			*table;
	int				index;
	struct s_philo *next;
}	t_philo;


t_philo *init_data(int argc, char **argv);
int	ft_atoi(char *str);
void *update_time(void *args);
void *set_death_lol(void *args);
void *print_time_lol(void *tabl);
#endif
