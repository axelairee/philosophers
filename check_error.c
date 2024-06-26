/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abolea <abolea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 16:35:32 by abolea            #+#    #+#             */
/*   Updated: 2024/06/26 16:43:01 by abolea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	if_error(int argc)
{
	if (argc > 6 || argc < 5)
	{
		write(2, "Usage : ./philo nb_of_philo time_to_die\
time_to_eat time_to_sleep nb_of_times_each_philo_must_eat", 99);
		return (-1);
	}
	return (0);
}
