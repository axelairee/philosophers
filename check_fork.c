/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_fork.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abolea <abolea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 16:29:49 by abolea            #+#    #+#             */
/*   Updated: 2024/06/26 16:43:23 by abolea           ###   ########.fr       */
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
		if (init->nb_philo == 1)
		{
			print_status(init, philo, philo->id, "has taken a fork");
			break ;
		}
		if (check_fork(init, philo) == 1)
			break ;
		usleep(50);
	}
}
