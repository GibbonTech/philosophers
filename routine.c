/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhomsi <ykhomsi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:47:36 by ykhomsi           #+#    #+#             */
/*   Updated: 2025/03/21 16:49:33 by ykhomsi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	take_forks(t_philo *philo)
{
	if (philo->table->philo_count == 1)
	{
		pthread_mutex_lock(&philo->fork_mutex);
		print_status(philo, "has taken a fork");
		precise_sleep(philo->table->time_to_die, philo);
		pthread_mutex_unlock(&philo->fork_mutex);
		return ;
	}
	if (philo->pos % 2 == 0)
	{
		pthread_mutex_lock(&philo->left->fork_mutex);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->fork_mutex);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->fork_mutex);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->left->fork_mutex);
		print_status(philo, "has taken a fork");
	}
}

void	eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_time();
	philo->ate_times++;
	pthread_mutex_unlock(&philo->meal_mutex);
	print_status(philo, "is eating");
	precise_sleep(philo->table->time_to_eat, philo);
	pthread_mutex_unlock(&philo->left->fork_mutex);
	pthread_mutex_unlock(&philo->fork_mutex);
}

void	sleeping(t_philo *philo)
{
	pthread_mutex_lock(&philo->status_mutex);
	philo->status = SLEEPING;
	pthread_mutex_unlock(&philo->status_mutex);
	print_status(philo, "is sleeping");
	precise_sleep(philo->table->time_to_sleep, philo);
}

void	thinking(t_philo *philo)
{
	pthread_mutex_lock(&philo->status_mutex);
	philo->status = THINKING;
	pthread_mutex_unlock(&philo->status_mutex);
	print_status(philo, "is thinking");
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->pos % 2 == 0)
		usleep(1000);
	while (!philo->table->is_dead)
	{
		take_forks(philo);
		if (philo->table->philo_count == 1)
			break;
		eating(philo);
		pthread_mutex_lock(&philo->table->table_mutex);
		if (philo->table->must_eat_count != -1 && 
			philo->ate_times >= philo->table->must_eat_count)
		{
			pthread_mutex_unlock(&philo->table->table_mutex);
			break;
		}
		pthread_mutex_unlock(&philo->table->table_mutex);
		if (philo->table->is_dead)
			break;
		sleeping(philo);
		if (philo->table->is_dead)
			break;
		thinking(philo);
	}
	return (NULL);
}
