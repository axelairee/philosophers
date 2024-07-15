/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   use_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abolea <abolea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 10:47:31 by abolea            #+#    #+#             */
/*   Updated: 2024/07/15 14:51:15 by abolea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_error_mutex(t_init *init)
{
	free(init->forks);
	pthread_mutex_destroy(&init->print_lock);
	pthread_mutex_destroy(&init->simulation_lock);
	pthread_mutex_destroy(&init->if_meals_eaten);
	pthread_mutex_destroy(&init->meals_time);
	pthread_mutex_destroy(&init->s_simu);
}

void	cleanup(t_init *init, t_philo *philo)
{
	int	i;
	int	k;

	k = 0;
	i = 0;
	while (i < init->nb_philo)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	while (k < init->nb_philo)
	{
		pthread_mutex_destroy(&init->forks[k]);
		pthread_mutex_destroy(&philo[k].meals_mutex);
		k++;
	}
	pthread_mutex_destroy(&init->print_lock);
	pthread_mutex_destroy(&init->simulation_lock);
	pthread_mutex_destroy(&init->meals_time);
	pthread_mutex_destroy(&init->s_simu);
	free(philo);
	free(init->forks);
}
