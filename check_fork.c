/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_fork.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abolea <abolea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 16:29:49 by abolea            #+#    #+#             */
/*   Updated: 2024/07/15 15:22:00 by abolea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	left_fork(t_init *init, t_philo *philo)
{
	philo->l_fork = true;
	pthread_mutex_unlock(philo->left_fork);
	print_status(init, philo, philo->id, "has taken a fork");
}

int	check_fork(t_init *init, t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	if (!philo->l_fork)
	{
		left_fork(init, philo);
		while (1)
		{
			if (if_stop(init))
				break ;
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
			usleep(100);
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
		if (if_stop(init))
			break ;
		if (check_fork(init, philo) == 1)
			break ;
		usleep(100);
	}
}
