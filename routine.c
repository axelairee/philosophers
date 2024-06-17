/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abolea <abolea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:24:56 by abolea            #+#    #+#             */
/*   Updated: 2024/06/17 16:36:29 by abolea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_left_fork(t_init *init, t_philo *philo)
{
	while (1)
	{
		pthread_mutex_lock(philo->left_fork);
		if (!philo->l_fork)
		{
			philo->l_fork = true;
			print_status(init, philo, philo->id, "has taken a fork");
			pthread_mutex_unlock(philo->left_fork);
			break;
		}
		pthread_mutex_unlock(philo->left_fork);
		usleep(10);
	}
	return;
}

void	check_right_fork(t_init *init, t_philo *philo)
{
	while (1)
	{
		pthread_mutex_lock(philo->right_fork);
		if (!philo->r_fork && philo->l_fork)
		{
			philo->r_fork = true;
			print_status(init, philo, philo->id, "has taken a fork");
			pthread_mutex_unlock(philo->right_fork);
			break;
		}
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_lock(philo->left_fork);
		philo->l_fork = false;
		pthread_mutex_unlock(philo->left_fork);
		usleep(10);
    }
	return;
}

void philo_takefork(t_init *init, t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		check_left_fork(init, philo);
		check_right_fork(init, philo);
	}
	else
	{
		check_left_fork(init, philo);
		check_right_fork(init, philo);
	}
}


void	philo_eating(t_init *init, t_philo *philo)
{
	philo->last_meal_time = current_timestamp();
	print_status(init, philo, philo->id, "is eating");
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
	print_status(init, philo, philo->id, "is thinking");
}

void	*philosophers_routine(void *arg)
{
	t_philo *philo;
	t_init	*init;

	philo = (t_philo *)arg;
	init = philo->init;
	while (init->start_simu != true)
		usleep(10);
	while (1)
	{
		pthread_mutex_lock(&init->simulation_lock);
		if (init->stop)
		{
			pthread_mutex_unlock(&init->simulation_lock);
			philo_setdown_forks(philo);
			break;
		}
		pthread_mutex_unlock(&init->simulation_lock);
		philo_takefork(init, philo);
		philo_eating(init, philo);
		philo_setdown_forks(philo);
		philo_sleeps(init, philo);
	}
	return (NULL);
}
