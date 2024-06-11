/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abolea <abolea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:50:14 by abolea            #+#    #+#             */
/*   Updated: 2024/06/11 16:13:45 by abolea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	current_timestamp()
{
	struct	timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_sec / 1000);
}

void	print_status(t_init *init, int id, const char *status)
{
	pthread_mutex_lock(&init->print_lock);
	printf("%ld %d %s\n", current_timestamp(), id, status);
	pthread_mutex_unlock(&init->print_lock);
}

void	*philosophers_routine(void *arg)
{
	t_philo *philo;
	t_init	*init;

	philo = (t_philo *)arg;
	init = philo->init;
	while (1)
	{
		//philo pense
		print_status(init, philo->id, "is thinking");
		//philo prend les forks
		pthread_mutex_lock(philo->left_fork);
		print_status(init, philo->id, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(init, philo->id, "has taken a fork");
		//philo mange
		pthread_mutex_lock(&init->simulation_lock);
		philo->last_meal_time = current_timestamp();
		print_status(init, philo->id, "is eating");
		pthread_mutex_unlock(&init->simulation_lock);
		usleep(init->time_to_eat * 1000);
		philo->meals_eaten++;
		//relache les forks
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		//si repas max atteint
		if (init->nb_philo_must_eat > 0 && philo->meals_eaten >= init->nb_philo_must_eat)
			break;
		//philo sleep
		print_status(init, philo->id, "is sleeping");
		usleep(init->time_to_sleep * 1000);
	}
	return (NULL);
}

int	init_struct(t_init *init, char **argv)
{
	int	i;

	i = 0;
	init->forks = malloc(init->nb_philo * sizeof(pthread_mutex_t));
	if (!init->forks)
		return (-1);
	init->nb_philo = atoi(argv[0]);
	init->time_to_die = atoi(argv[1]);
	init->time_to_eat = atoi(argv[2]);
	init->time_to_sleep = atoi(argv[3]);
	init->nb_philo_must_eat = atoi(argv[4]);
	pthread_mutex_init(&init->print_lock, NULL);
	pthread_mutex_init(&init->simulation_lock, NULL);
	while (i < init->nb_philo)
	{
		pthread_mutex_init(init->forks, NULL);
		i++;
	}
	return (0);
}

void	init_philo_struct(t_philo *philo, t_init init)
{
	int	i;

	i = 0;
	philo = malloc(init.nb_philo * sizeof(t_philo));
	while (i < init.nb_philo)
	{
		philo[i].id = i + 1;
		philo[i].left_fork = &init.forks[i];
		philo[i].right_fork = &init.forks[(i + 1) % init.nb_philo];
		philo[i].last_meal_time = current_timestamp();
		philo[i].meals_eaten = 0;
		philo[i].init = &init;
		pthread_create(&philo[i].thread, NULL, philosophers_routine, &philo[i]);
	}
}


int	main(int argc, char **argv)
{
	t_init	init;
	t_philo	*philo;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (argc != 6)
	{
		write(2, "Usage : ./philosophers number_of_philosophers time_to_die time_to_eat time_to_sleep number_of_times_each_philosopher_must_eat", 126);
		return (0);
	}
	init_struct(&init, argv);
	init_philo_struct(philo, init);
	while (1)
	{
		while (i < init.nb_philo)
		{
			pthread_mutex_lock(&init.simulation_lock);
			if (current_timestamp() - philo[i].last_meal_time > init.time_to_die)
			{
				print_status(&init, philo[i].id, "died");
				init.stop = 1;
			}
			pthread_mutex_unlock(&init.simulation_lock);
			if (init.stop)
			{
				while (j < init.nb_philo)
					pthread_join(philo[j].thread, NULL);
				free(philo);
				free(init.forks);
				pthread_mutex_destroy(&init.print_lock);
				pthread_mutex_destroy(&init.simulation_lock);
				return (0);
			}
		}
		usleep(1000);
	}
	return (0);
}