/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhomsi <ykhomsi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:47:36 by ykhomsi           #+#    #+#             */
/*   Updated: 2025/04/03 16:28:28 by ykhomsi          ###   ########.fr       */
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
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_time();
	philo->ate_times++;
	pthread_mutex_unlock(&philo->meal_mutex);
	precise_sleep(philo->table->time_to_eat, philo);
	pthread_mutex_unlock(&philo->left->fork_mutex);
	pthread_mutex_unlock(&philo->fork_mutex);
}

void	sleeping(t_philo *philo)
{
	print_status(philo, "is sleeping");
	precise_sleep(philo->table->time_to_sleep, philo);
}

void	thinking(t_philo *philo)
{
	print_status(philo, "is thinking");
	if (philo->table->philo_count % 2 == 1)
		usleep(500);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->pos % 2 == 0)
		usleep(1000);
	while (1)
	{
		if (should_stop_routine(philo))
			break ;
		if (check_forks_and_eat(philo))
			break ;
		if (should_stop_routine(philo))
			break ;
		sleeping(philo);
		if (should_stop_routine(philo))
			break ;
		thinking(philo);
	}
	return (NULL);
}
