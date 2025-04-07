/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhomsi <ykhomsi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:48:02 by ykhomsi           #+#    #+#             */
/*   Updated: 2025/04/02 17:29:03 by ykhomsi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	precise_sleep(long ms, t_philo *philo)
{
	long	start;

	start = get_time();
	while (!philo->table->is_dead && !philo->table->all_ate)
	{
		if (get_time() - start >= ms)
			break ;
		usleep(ms / 10);
	}
}

void	print_status(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->table->print_mutex);
	if (!philo->table->all_ate && (!philo->table->is_dead || msg[0] == 'd'))
		printf("%ld %d %s\n", get_time() - philo->table->start_time,
			philo->pos, msg);
	pthread_mutex_unlock(&philo->table->print_mutex);
}

int	is_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	if (get_time() - philo->last_meal > philo->table->time_to_die)
	{
		pthread_mutex_unlock(&philo->meal_mutex);
		pthread_mutex_lock(&philo->table->table_mutex);
		if (!philo->table->is_dead)
		{
			philo->table->is_dead = 1;
			pthread_mutex_unlock(&philo->table->table_mutex);
			print_status(philo, "died");
			return (1);
		}
		pthread_mutex_unlock(&philo->table->table_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->meal_mutex);
	pthread_mutex_lock(&philo->table->table_mutex);
	if (philo->table->is_dead)
	{
		pthread_mutex_unlock(&philo->table->table_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->table->table_mutex);
	return (0);
}

void	ft_bzero(void *s, size_t n)
{
	memset(s, 0, n);
}
