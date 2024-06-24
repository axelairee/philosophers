/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abolea <abolea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:24:56 by abolea            #+#    #+#             */
/*   Updated: 2024/06/24 16:23:15 by abolea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_first(t_init *init, t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	if (!*philo->r_fork)
	{
		*philo->r_fork = true;
		print_status(init, philo, philo->id, "has taken a fork");
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_lock(philo->left_fork);
		if (!philo->l_fork)
		{
			philo->l_fork = true;
			print_status(init, philo, philo->id, "has taken a fork");
			pthread_mutex_unlock(philo->left_fork);
			return (1);
		}
		else
		{
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_lock(philo->right_fork);
			*philo->r_fork = false;
			pthread_mutex_unlock(philo->right_fork);
		}
	}
	else
		pthread_mutex_unlock(philo->right_fork);
	return (0);
}

int	philo_second(t_init *init, t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	if (!philo->l_fork)
	{
		philo->l_fork = true;
		print_status(init, philo, philo->id, "has taken a fork");
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_lock(philo->right_fork);
		if (!*philo->r_fork)
		{
			*philo->r_fork = true;
			print_status(init, philo, philo->id, "has taken a fork");
			pthread_mutex_unlock(philo->right_fork);
			return (1);
		}
		else
		{
			pthread_mutex_unlock(philo->right_fork);
			pthread_mutex_lock(philo->left_fork);
			philo->l_fork = false;
			pthread_mutex_unlock(philo->left_fork);
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
		if (philo->id % 2 == 0)
		{
			if (philo_first(init, philo) == 1)
				break;
		}
		else
		{
			if (philo_second(init, philo) == 1)
				break;
		}
		usleep(50);
	}
}

void	philo_eating(t_init *init, t_philo *philo)
{
	pthread_mutex_lock(&init->meals_time);
	philo->last_meal_time = current_timestamp();
	pthread_mutex_unlock(&init->meals_time);
	print_status(init, philo, philo->id, "is eating");
	ft_usleep(init->time_to_eat);
	pthread_mutex_lock(philo->left_fork);
	philo->l_fork = false;
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_lock(philo->right_fork);
	*philo->r_fork = false;
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_lock(&init->meals_eaten);
	philo->meals_eaten++;
	pthread_mutex_unlock(&init->meals_eaten);
}

void	philo_sleeps(t_init *init, t_philo *philo)
{
	int	time_to_think;
	
	time_to_think = (philo->init->time_to_die - (current_timestamp() - philo->last_meal_time) - philo->init->time_to_eat) / 2;
	if (time_to_think < 0)
		time_to_think = 0;
	print_status(init, philo, philo->id, "is sleeping");
	ft_usleep(init->time_to_sleep);
	print_status(init, philo, philo->id, "is thinking");
	ft_usleep(time_to_think);
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
		ft_usleep(init->time_to_eat / 2);
	while (1)
	{
		philo_takefork(init, philo);
		philo_eating(init, philo);
		philo_sleeps(init, philo);
	}
	return (NULL);
}
