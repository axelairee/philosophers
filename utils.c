/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abolea <abolea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 13:23:34 by abolea            #+#    #+#             */
/*   Updated: 2024/07/23 10:35:30 by abolea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	current_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	ft_atoi(const char *nptr)
{
	int		sign;
	long	nb;

	sign = 1;
	nb = 0;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == ' ')
		nptr++;
	if (*nptr == '-')
		return (-1);
	else if (*nptr == '+')
		nptr++;
	while (*nptr)
	{
		if (*nptr >= '0' && *nptr <= '9')
		{
			if (nb > (LONG_MAX - (*nptr - '0')) / 10)
				return (-1);
			nb = nb * 10 + (*nptr - '0');
		}
		else
			return (-1);
		nptr++;
	}
	return ((int)(nb * sign));
}

int	if_stop(t_init *init)
{
	int	stopped;

	pthread_mutex_lock(&init->simulation_lock);
	stopped = init->stop;
	pthread_mutex_unlock(&init->simulation_lock);
	return (stopped);
}

void	ft_usleep(int time, t_init *init)
{
	long	start_time;
	long	time_to_wait;

	start_time = current_timestamp();
	while ((current_timestamp() - start_time) < time)
	{
		time_to_wait = ((time - (current_timestamp() - start_time)) * 1000) / 2;
		if (time_to_wait > 150)
			usleep(150);
		else
			usleep(time_to_wait);
		if (if_stop(init))
			return ;
	}
}

void	print_status(t_init *init, t_philo *philo, int id, const char *status)
{
	pthread_mutex_lock(&init->print_lock);
	pthread_mutex_lock(&init->simulation_lock);
	if (init->stop != 1)
		printf("%ld %d %s\n", current_timestamp() - \
		philo->start_time, id, status);
	pthread_mutex_unlock(&init->simulation_lock);
	pthread_mutex_unlock(&init->print_lock);
}
