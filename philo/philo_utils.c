#include "philo.h"

static int	is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r')
		return (1);
	return (0);
}

int	ft_atoi(char *str)
{
	int		i;
	int		result;
	int		is_negative;

	i = 0;
	result = 0;
	is_negative = 1;
	while (is_space(str[i]) && str[i])
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			is_negative = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * is_negative);
}


long long get_current_time(void)
{
	struct timeval	current_time;
	gettimeofday(&current_time, NULL);
	return (current_time.tv_sec * 1000LL) + (current_time.tv_usec / 1000);
}
