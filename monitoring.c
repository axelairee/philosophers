/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abolea <abolea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 16:31:57 by abolea            #+#    #+#             */
/*   Updated: 2024/07/15 14:51:35 by abolea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_dead(t_init *init, t_philo *philo, int i)
{
	pthread_mutex_lock(&init->print_lock);
	pthread_mutex_lock(&init->simulation_lock);
	if (init->stop != 1)
	{
		printf("%ld %d %s\n", current_timestamp() - \
		philo->start_time, philo[i].id, "died");
		init->stop = 1;
	}
	pthread_mutex_unlock(&init->simulation_lock);
	pthread_mutex_unlock(&init->print_lock);
}

int	is_dead(t_init *init, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < init->nb_philo)
	{
		pthread_mutex_lock(&init->meals_time);
		if (current_timestamp() - philo[i].last_meal_time >= init->time_to_die)
			print_dead(init, philo, i);
		pthread_mutex_unlock(&init->meals_time);
		pthread_mutex_lock(&init->simulation_lock);
		if (init->stop == 1)
		{
			pthread_mutex_unlock(&init->simulation_lock);
			return (-1);
		}
		pthread_mutex_unlock(&init->simulation_lock);
		i++;
	}
	return (0);
}

int	check_philo_eat(t_init *init, t_philo *philo)
{
	int	all_philos_ate_enough;
	int	i;

	i = -1;
	all_philos_ate_enough = 1;
	while (++i < init->nb_philo)
	{
		pthread_mutex_lock(&philo[i].meals_mutex);
		if (philo[i].meals_eaten < init->nb_philo_must_eat)
		{
			all_philos_ate_enough = 0;
			pthread_mutex_unlock(&philo[i].meals_mutex);
			break ;
		}
		else
			pthread_mutex_unlock(&philo[i].meals_mutex);
	}
	if (all_philos_ate_enough)
	{
		pthread_mutex_lock(&init->simulation_lock);
		init->stop = 1;
		pthread_mutex_unlock(&init->simulation_lock);
		return (1);
	}
	return (0);
}
