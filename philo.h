/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abolea <abolea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:04:15 by abolea            #+#    #+#             */
/*   Updated: 2024/07/15 13:45:24 by abolea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <unistd.h>
# include <limits.h>

typedef struct s_init
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_philo_must_eat;
	int				stop;
	bool			start_simu;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	simulation_lock;
	pthread_mutex_t	if_meals_eaten;
	pthread_mutex_t	meals_time;
	pthread_mutex_t	s_simu;
}	t_init;

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	meals_mutex;
	bool			l_fork;
	bool			*r_fork;
	long			start_time;
	long			last_meal_time;
	int				meals_eaten;
	t_init			*init;
}	t_philo;

void	*philosophers_routine(void *arg);
int		init_struct(t_init *init, char **argv, int argc);
int		init_philo_struct(t_philo **philo, t_init *init);
void	print_status(t_init *init, t_philo *philo, int id, const char *status);
long	current_timestamp(void);
int		ft_atoi(const char *nptr);
void	ft_usleep(int time, t_init *init);
int		if_stop(t_init *init);
int		check_fork(t_init *init, t_philo *philo);
void	philo_takefork(t_init *init, t_philo *philo);
void	if_dead(t_init *init, t_philo *philo, int i);
int		is_dead(t_init *init, t_philo *philo);
int		check_philo_eat(t_init *init, t_philo *philo);
int		if_error(int argc);
void	cleanup(t_init *init, t_philo *philo);
void	free_error_mutex(t_init *init);
int		init_args(t_init *init, char **argv, int argc);
int		init_mutex(t_init *init);
int		init_for_philo(t_philo **philo, t_init *init);
void	setdown_fork(t_philo *philo);

#endif
