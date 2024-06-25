/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abolea <abolea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:24:56 by abolea            #+#    #+#             */
/*   Updated: 2024/06/25 18:07:29 by abolea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_fork(t_init *init, t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	if (!philo->l_fork)
	{
		philo->l_fork = true;
		pthread_mutex_unlock(philo->left_fork);
		print_status(init, philo, philo->id, "has taken a fork");
		if (init->nb_philo == 1)
			return (0);
		while (1)
		{
			pthread_mutex_lock(philo->right_fork);
			if (!*philo->r_fork)
			{
				*philo->r_fork = true;
				pthread_mutex_unlock(philo->right_fork);
				print_status(init, philo, philo->id, "has taken a fork");
				return (1);
			}
			else 
				pthread_mutex_unlock(philo->right_fork);
			usleep(50);
		}
	}
	else
		pthread_mutex_unlock(philo->left_fork);
	return (0);
}

void	philo_takefork(t_init *init, t_philo *philo)
{
	while (1)
	{
		if (check_fork(init, philo) == 1)
			break;
		usleep(50);
	}
}

void	philo_eating(t_init *init, t_philo *philo)
{
	print_status(init, philo, philo->id, "is eating");
	pthread_mutex_lock(&init->meals_time);
	philo->last_meal_time = current_timestamp();
	pthread_mutex_unlock(&init->meals_time);
	ft_usleep(init->time_to_eat, init);
	pthread_mutex_lock(philo->left_fork);
	philo->l_fork = false;
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_lock(philo->right_fork);
	*philo->r_fork = false;
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_lock(&philo->meals_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meals_mutex);
}

void	philo_sleeps(t_init *init, t_philo *philo)
{
	int	time_to_think;
	
	time_to_think = (philo->init->time_to_die - (current_timestamp() - philo->last_meal_time) - philo->init->time_to_eat) / 2;
	if (time_to_think < 0)
		time_to_think = 0;
	print_status(init, philo, philo->id, "is sleeping");
	ft_usleep(init->time_to_sleep, init);
	print_status(init, philo, philo->id, "is thinking");
	ft_usleep(time_to_think, init);
}

void	*philosophers_routine(void *arg)
{
	t_philo *philo;
	t_init	*init;

	philo = (t_philo *)arg;
	init = philo->init;
	while (1)
	{
		pthread_mutex_lock(&init->s_simu);
		if (init->start_simu)
		{
			pthread_mutex_unlock(&init->s_simu);
			break;
		}
		pthread_mutex_unlock(&init->s_simu);
		usleep(10);
	}
	if (philo->id % 2 == 0)
		ft_usleep(init->time_to_eat / 2, init);
	while (1)
	{
		if (if_stop(init))
			break;
		philo_takefork(init, philo);
		philo_eating(init, philo);
		philo_sleeps(init, philo);
	}
	return (NULL);
}
