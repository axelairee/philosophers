/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abolea <abolea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:50:14 by abolea            #+#    #+#             */
/*   Updated: 2024/06/12 14:47:42 by abolea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	current_timestamp()
{
	struct	timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	print_status(t_init *init, int id, const char *status)
{
	pthread_mutex_lock(&init->print_lock);
	printf("philo %d %s\n", id, status);
	pthread_mutex_unlock(&init->print_lock);
}

int	main(int argc, char **argv)
{
	t_init	init;
	t_philo	*philo;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (argc > 6 || argc < 5)
	{
		write(2, "Usage : ./philosophers number_of_philosophers time_to_die time_to_eat time_to_sleep number_of_times_each_philosopher_must_eat", 126);
		return (0);
	}
	init_struct(&init, argv, argc);
	philo = malloc(init.nb_philo * sizeof(t_philo));
	init_philo_struct(philo, init);
	while (1)
	{
		i = 0;
		while (i < init.nb_philo)
		{
			pthread_mutex_lock(&init.simulation_lock);
			if (current_timestamp() - philo[i].last_meal_time > init.time_to_die)
			{
				print_status(&init, philo[i].id, "died");
				init.stop = 1;
			}
			pthread_mutex_unlock(&init.simulation_lock);
			if (init.stop)
			{
				while (j < init.nb_philo)
					pthread_join(philo[j].thread, NULL);
				free(philo);
				free(init.forks);
				pthread_mutex_destroy(&init.print_lock);
				pthread_mutex_destroy(&init.simulation_lock);
				return (0);
			}
		}
		usleep(1000);
	}
	return (0);
}
