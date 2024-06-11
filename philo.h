/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abolea <abolea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:04:15 by abolea            #+#    #+#             */
/*   Updated: 2024/06/11 15:32:13 by abolea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_philo {
	int				id;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	long			last_meal_time;
	int				meals_eaten;
	struct s_init	*init;
}	t_philo;

typedef struct s_init {
    int				nb_philo;
    int				time_to_die;
    int				time_to_eat;
    int				time_to_sleep;
    int				nb_philo_must_eat;
    int				stop;
    pthread_mutex_t	*forks;
    pthread_mutex_t	print_lock;
    pthread_mutex_t	simulation_lock;	
}	t_init;



#endif