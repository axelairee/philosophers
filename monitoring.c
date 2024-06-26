/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abolea <abolea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 16:31:57 by abolea            #+#    #+#             */
/*   Updated: 2024/06/26 16:47:30 by abolea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	if_dead(t_init *init, t_philo *philo, int i)
{
	int	j;

	j = 0;
	pthread_mutex_unlock(&init->meals_time);
	print_status(init, philo, philo[i].id, "died");
	pthread_mutex_lock(&init->simulation_lock);
	init->stop = 1;
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
}

int	is_dead(t_init *init, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < init->nb_philo)
	{
		pthread_mutex_lock(&init->meals_time);
		if (current_timestamp() - philo[i].last_meal_time > init->time_to_die)
		{
			if_dead(init, philo, i);
			return (-1);
		}
		else
			pthread_mutex_unlock(&init->meals_time);
		i++;
	}
	return (0);
}

int	check_philo_eat(t_init *init, t_philo *philo)
{
	int	all_philos_ate_enough;
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
			break ;
		}
		else
			pthread_mutex_unlock(&philo[i].meals_mutex);
		i++;
	}
	if (all_philos_ate_enough)
		return (1);
	return (0);
}
