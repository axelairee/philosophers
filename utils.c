/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abolea <abolea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 13:23:34 by abolea            #+#    #+#             */
/*   Updated: 2024/06/26 16:46:28 by abolea           ###   ########.fr       */
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
	while ((*nptr >= 9 && *nptr <= 13))
		nptr++;
	if (*nptr == '-')
		sign *= -1;
	if (*nptr == '-' || *nptr == '+')
		nptr++;
	while (*nptr >= '0' && *nptr <= '9')
	{
		if (nb > (LONG_MAX - (*nptr - '0')) / 10)
		{
			if (sign == 1)
				return (-1);
			else
				return (0);
		}
		nb = nb * 10 + (*nptr - '0');
		nptr++;
	}
	return ((int)(nb * sign));
}

int	if_stop(t_init *init)
{
	pthread_mutex_lock(&init->simulation_lock);
	if (init->stop)
	{
		pthread_mutex_unlock(&init->simulation_lock);
		return (1);
	}
	else
		pthread_mutex_unlock(&init->simulation_lock);
	return (0);
}

void	ft_usleep(int time, t_init *init)
{
	long	start_time;
	long	time_to_wait;

	start_time = current_timestamp();
	while ((current_timestamp() - start_time) < time)
	{
		if (if_stop(init))
			break ;
		time_to_wait = ((time - (current_timestamp() - start_time)) * 1000) / 2;
		if (time_to_wait > 50)
			usleep(50);
		else
			usleep(time_to_wait);
	}
}

void	print_status(t_init *init, t_philo *philo, int id, const char *status)
{
	pthread_mutex_lock(&init->simulation_lock);
	pthread_mutex_lock(&init->print_lock);
	if (init->stop != 1)
		printf("%ld %d %s\n", current_timestamp() - \
		philo->start_time, id, status);
	pthread_mutex_unlock(&init->print_lock);
	pthread_mutex_unlock(&init->simulation_lock);
}
