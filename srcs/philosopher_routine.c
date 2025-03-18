/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_routine.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhomsi <ykhomsi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 23:29:09 by ykhomsi           #+#    #+#             */
/*   Updated: 2025/03/18 03:00:44 by ykhomsi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->pos % 2)
		usleep(5000);
	while (!philo->env->stop_condition)
	{
		philo_eat(philo);
		if (philo->env->stop_condition)
			break ;
		print_message("is sleeping", philo);
		new_sleep(philo->env->time_to_sleep, philo->env);
		if (philo->env->stop_condition)
			break ;
		print_message("is thinking", philo);
	}
	return (NULL);
}
