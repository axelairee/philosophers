/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abolea <abolea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:26:32 by abolea            #+#    #+#             */
/*   Updated: 2024/06/17 16:25:40 by abolea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_struct(t_init *init, char **argv, int argc)
{
	int	i;

	i = 0;
	init->nb_philo = ft_atoi(argv[1]);
	init->forks = malloc(init->nb_philo * sizeof(pthread_mutex_t));
	if (!init->forks)
		return (-1);
	init->time_to_die = ft_atoi(argv[2]);
	init->time_to_eat = ft_atoi(argv[3]);
	init->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		init->nb_philo_must_eat = ft_atoi(argv[5]);
	init->stop = 0;
	init->start_simu = false;
	pthread_mutex_init(&init->print_lock, NULL);
	pthread_mutex_init(&init->simulation_lock, NULL);
	pthread_mutex_init(&init->if_meals_eaten, NULL);
	while (i < init->nb_philo)
	{
		pthread_mutex_init(&init->forks[i], NULL);
		i++;
	}
	return (0);
}

void	init_philo_struct(t_philo **philo, t_init *init)
{
	int	i;

	i = 0;
	*philo = malloc(init->nb_philo * sizeof(t_philo));
	while (i < init->nb_philo)
	{
		(*philo)[i].id = i + 1;
		(*philo)[i].left_fork = &init->forks[i];
		(*philo)[i].right_fork = &init->forks[(i + 1) % init->nb_philo];
		(*philo)[i].last_meal_time = current_timestamp();
		(*philo)[i].meals_eaten = 0;
		(*philo)[i].init = init;
		(*philo)[i].start_time = current_timestamp();
		(*philo)[i].l_fork = false;
		(*philo)[i].r_fork = false;
		pthread_create(&(*philo)[i].thread, NULL, philosophers_routine, &(*philo)[i]);
		i++;
	}
	init->start_simu = true;
}

