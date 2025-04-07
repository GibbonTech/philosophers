/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhomsi <ykhomsi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:34:00 by ykhomsi           #+#    #+#             */
/*   Updated: 2025/04/03 16:27:42 by ykhomsi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	should_stop_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->table_mutex);
	if (philo->table->is_dead || philo->table->all_ate)
	{
		pthread_mutex_unlock(&philo->table->table_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->table->table_mutex);
	return (0);
}

int	check_forks_and_eat(t_philo *philo)
{
	take_forks(philo);
	if (philo->table->philo_count == 1)
		return (1);
	pthread_mutex_lock(&philo->table->table_mutex);
	if (philo->table->is_dead)
	{
		pthread_mutex_unlock(&philo->table->table_mutex);
		pthread_mutex_unlock(&philo->fork_mutex);
		pthread_mutex_unlock(&philo->left->fork_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->table->table_mutex);
	eating(philo);
	return (0);
}
