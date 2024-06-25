/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abolea <abolea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:50:14 by abolea            #+#    #+#             */
/*   Updated: 2024/06/25 18:02:42 by abolea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	current_timestamp()
{
	struct	timeval tv;
	
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	print_status(t_init *init, t_philo *philo, int id, const char *status)
{
	pthread_mutex_lock(&init->simulation_lock);
	pthread_mutex_lock(&init->print_lock);
	if (!init->stop)
		printf("%ld %d %s\n",current_timestamp() - philo->start_time, id, status);
	pthread_mutex_unlock(&init->print_lock);
	pthread_mutex_unlock(&init->simulation_lock);
}

int	is_dead(t_init *init, t_philo *philo)
{
	int j;
	int	i;

	j = 0;
	i = 0;
	while (i < init->nb_philo)
	{
		pthread_mutex_lock(&init->meals_time);
		if (current_timestamp() - philo[i].last_meal_time > init->time_to_die)
		{
			pthread_mutex_unlock(&init->meals_time);
			print_status(init, philo, philo[i].id, "died");
			pthread_mutex_lock(&init->simulation_lock);
			init->stop = 1;
		}
		else
			pthread_mutex_unlock(&init->meals_time);
		if (init->stop == 1)
		{
			pthread_mutex_unlock(&init->simulation_lock);
			while (j < init->nb_philo)
			{
				pthread_join(philo[j].thread, NULL);
				j++;
			}
			free(philo);
			free(init->forks);
			pthread_mutex_destroy(&init->print_lock);
			pthread_mutex_destroy(&init->simulation_lock);
			return (-1);
		}
		i++;
	}
	return (0);
}

int	check_philo_eat(t_init *init, t_philo *philo)
{
	int all_philos_ate_enough;
	int	i;

	i = 0;
	all_philos_ate_enough = 1;
	while (i < init->nb_philo)
	{
		pthread_mutex_lock(&philo[i].meals_mutex);
		if (philo[i].meals_eaten < init->nb_philo_must_eat)
		{
			all_philos_ate_enough = 0;
			pthread_mutex_unlock(&philo[i].meals_mutex);
			break;
		}
		else
			pthread_mutex_unlock(&philo[i].meals_mutex);
		i++;
	}
	if (all_philos_ate_enough)
		return (1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_init	init;
	t_philo	*philo = NULL;
	int i;

	i = 0;
	if (argc > 6 || argc < 5)
	{
		write(2, "Usage : ./philosophers number_of_philosophers time_to_die time_to_eat time_to_sleep number_of_times_each_philosopher_must_eat", 126);
		return (0);
	}
	if (init_struct(&init, argv, argc) == -1)
		return (-1);
	if (init_philo_struct(&philo, &init) == -1)
		return (-1);
	while (1)
	{
		if (argc == 6)
		{
			if (check_philo_eat(&init, philo) == 1)
				return (0);
		}
		if (is_dead(&init, philo) == -1)
			return (0);
		usleep(1000);
	}
	return (0);
}
