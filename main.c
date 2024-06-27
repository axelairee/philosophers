/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abolea <abolea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:50:14 by abolea            #+#    #+#             */
/*   Updated: 2024/06/27 11:36:24 by abolea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_init	init;
	t_philo	*philo;

	philo = NULL;
	if (if_error(argc) == -1)
		return (-1);
	if (init_struct(&init, argv, argc) == -1)
		return (-1);
	if (init_philo_struct(&philo, &init) == -1)
		return (-1);
	while (1)
	{
		if (argc == 6)
		{
			if (check_philo_eat(&init, philo) == 1)
				break ;
		}
		if (is_dead(&init, philo) == -1)
			break ;
		usleep(1000);
	}
	return (0);
}
