/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abolea <abolea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:26:32 by abolea            #+#    #+#             */
/*   Updated: 2024/06/25 17:58:59 by abolea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_struct(t_init *init, char **argv, int argc)
{
	int	i;

	i = 0;
	init->nb_philo = ft_atoi(argv[1]);
	if (init->nb_philo <= 0 || init->nb_philo > 200)
		return (write(2, "Bad Arguments\n", 15), -1);
	init->forks = malloc(init->nb_philo * sizeof(pthread_mutex_t));
	if (!init->forks)
		return (-1);
	init->time_to_die = ft_atoi(argv[2]);
	if (init->time_to_die <= 0)
		return (write(2, "Bad Arguments\n", 15), -1);
	init->time_to_eat = ft_atoi(argv[3]);
	if (init->time_to_eat <= 0)
		return (write(2, "Bad Arguments\n", 15), -1);
	init->time_to_sleep = ft_atoi(argv[4]);
	if (init->time_to_sleep <= 0)
		return (write(2, "Bad Arguments\n", 15), -1);
	if (argc == 6)
	{
		init->nb_philo_must_eat = ft_atoi(argv[5]);
		if (init->nb_philo_must_eat <= 0)
			return (write(2, "Bad Arguments\n", 15), -1);
	}
	init->stop = 0;
	init->start_simu = false;
	pthread_mutex_init(&init->print_lock, NULL);
	pthread_mutex_init(&init->simulation_lock, NULL);
	pthread_mutex_init(&init->if_meals_eaten, NULL);
	pthread_mutex_init(&init->meals_time, NULL);
	pthread_mutex_init(&init->start_mutex, NULL);
	pthread_mutex_init(&init->s_simu, NULL);
	pthread_mutex_init(&init->s_fork, NULL);
	while (i < init->nb_philo)
	{
		pthread_mutex_init(&init->forks[i], NULL);
		i++;
	}
	return (0);
}

int	init_philo_struct(t_philo **philo, t_init *init)
{
	int	i;
	long first_time;

	i = 0;
	*philo = malloc(init->nb_philo * sizeof(t_philo));
	if (!philo)
		return (-1);
	while (i < init->nb_philo)
	{
		pthread_mutex_init(&(*philo)[i].meals_mutex, NULL);
		(*philo)[i].id = i + 1;
		(*philo)[i].left_fork = &init->forks[i];
		if (init->nb_philo > 1)
			(*philo)[i].right_fork = &init->forks[(i + 1) % init->nb_philo];
		(*philo)[i].meals_eaten = 0;
		(*philo)[i].init = init;
		(*philo)[i].l_fork = false;
		(*philo)[i].r_fork = &(*philo)[(i + 1) % init->nb_philo].l_fork;
		pthread_create(&(*philo)[i].thread, NULL, philosophers_routine, &(*philo)[i]);
		i++;
	}
	i = 0;
	first_time = current_timestamp();
	while (i < init->nb_philo)
	{
		(*philo)[i].last_meal_time = first_time;
		(*philo)[i].start_time = first_time;
		i++;
	}
	pthread_mutex_lock(&init->s_simu);
	init->start_simu = true;
	pthread_mutex_unlock(&init->s_simu);
	return (0);
}


