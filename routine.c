/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abolea <abolea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:24:56 by abolea            #+#    #+#             */
/*   Updated: 2024/06/12 14:58:40 by abolea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_thinking(t_init *init, t_philo *philo)
{
	print_status(init, philo->id, "is thinking");
}

void	philo_takefork(t_init *init, t_philo *philo) //check si le philo a les deux forks
{
	pthread_mutex_lock(philo->left_fork);
	print_status(init, philo->id, "has taken left fork");
	pthread_mutex_lock(philo->right_fork);
	print_status(init, philo->id, "has taken right fork");
}

void	philo_eating(t_init *init, t_philo *philo)
{
	pthread_mutex_lock(&init->simulation_lock);
	philo->last_meal_time = current_timestamp();
	print_status(init, philo->id, "is eating");
	pthread_mutex_unlock(&init->simulation_lock);
	usleep(init->time_to_eat * 1000);
	philo->meals_eaten++;
}

void	philo_setdown_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

void	philo_sleeps(t_init *init, t_philo *philo)
{
	print_status(init, philo->id, "is sleeping");
	usleep(init->time_to_sleep * 1000);
}

void	*philosophers_routine(void *arg)
{
	t_philo *philo;
	t_init	*init;

	philo = (t_philo *)arg;
	init = philo->init;
	while (1)
	{
		philo_thinking(init, philo);
		philo_takefork(init, philo);
		philo_eating(init, philo);
		philo_setdown_forks(philo);
		if (init->nb_philo_must_eat > 0 && philo->meals_eaten >= init->nb_philo_must_eat)
			break;
		philo_sleeps(init, philo);
	}
	return (NULL);
}