/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abolea <abolea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:24:56 by abolea            #+#    #+#             */
/*   Updated: 2024/06/13 16:58:58 by abolea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_thinking(t_init *init, t_philo *philo)
{
	print_status(init, philo, philo->id, "is thinking");
}

void	philo_takefork(t_init *init, t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		philo->l_fork = true;
		print_status(init, philo, philo->id, "has taken a fork");

		pthread_mutex_lock(philo->right_fork);
		philo->r_fork = true;
		print_status(init, philo, philo->id, "has taken a fork");
	} 
	else 
	{
		pthread_mutex_lock(philo->right_fork);
		philo->r_fork = true;
		print_status(init, philo, philo->id, "has taken a fork");

		pthread_mutex_lock(philo->left_fork);
		philo->l_fork = true;
		print_status(init, philo, philo->id, "has taken a fork");
	}
}

void	philo_eating(t_init *init, t_philo *philo)
{
	pthread_mutex_lock(&init->simulation_lock);
	philo->last_meal_time = current_timestamp();
	print_status(init, philo, philo->id, "is eating");
	pthread_mutex_unlock(&init->simulation_lock);
	usleep(init->time_to_eat * 1000);
	philo->meals_eaten++;
}

void philo_setdown_forks(t_philo *philo)
{
	if (philo->r_fork)
	{
		pthread_mutex_unlock(philo->right_fork);
		philo->r_fork = false;
	}
	if (philo->l_fork)
	{
		pthread_mutex_unlock(philo->left_fork);
		philo->l_fork = false;
	}
}


void	philo_sleeps(t_init *init, t_philo *philo)
{
	print_status(init, philo, philo->id, "is sleeping");
	usleep(init->time_to_sleep * 1000);
}

void	*philosophers_routine(void *arg)
{
	t_philo *philo;
	t_init	*init;

	if (arg == NULL)
		printf("here");
	philo = (t_philo *)arg;
	init = philo->init;
	while (!init->stop)
	{
		philo_takefork(init, philo);
		philo_eating(init, philo);
		pthread_mutex_lock(&init->simulation_lock);
		if (init->stop)
		{
			pthread_mutex_unlock(&init->simulation_lock);
			philo_setdown_forks(philo);
			break;
		}
		pthread_mutex_unlock(&init->simulation_lock);
		philo_setdown_forks(philo);
		philo_sleeps(init, philo);
		philo_thinking(init, philo);
	}
	return (NULL);
}
