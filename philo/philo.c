#include "philo.h"

int check_args(char **argv)
{
	int i;
	int y;

	i = 1;
	while (argv[i])
	{
		y = 0;
		if (!argv[i][y])
			return 1;
		while (argv[i][y])
		{
			if (argv[i][y] >= '0' && argv[i][y] <= '9')
				y++;
			else
				return 1;

		}
		i++;
	}
	return 0;
}

int main(int argc, char **argv)
{
	t_philo *philosophers;
	pthread_t set_death;
	pthread_t print_time;

	if (argc < 5 || argc > 6)
		return 1;
	if (check_args(argv))
		return (printf("error"), 1);
	philosophers = init_data(argc, argv);
	return 0;
}
