/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_actions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhomsi <ykhomsi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 23:29:03 by ykhomsi           #+#    #+#             */
/*   Updated: 2025/03/18 03:00:37 by ykhomsi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	single_philo_case(t_philo *philo)
{
	pthread_mutex_lock(&philo->env->forks[0]);
	print_message("has taken a fork", philo);
	while (!philo->env->stop_condition)
	{
		pthread_mutex_lock(&philo->env->meal);
		philo->last_ate = get_time();
		pthread_mutex_unlock(&philo->env->meal);
		usleep(100);
	}
	pthread_mutex_unlock(&philo->env->forks[0]);
}

void	philo_eat(t_philo *philo)
{
	if (!philo || !philo->env || philo->env->stop_condition)
		return ;
	if (philo->env->count == 1)
	{
		single_philo_case(philo);
		return ;
	}
	pthread_mutex_lock(&philo->env->forks[philo->ffork]);
	print_message("has taken a fork", philo);
	pthread_mutex_lock(&philo->env->forks[philo->sfork]);
	print_message("has taken a fork", philo);
	pthread_mutex_lock(&philo->env->meal);
	philo->last_ate = get_time();
	pthread_mutex_unlock(&philo->env->meal);
	print_message("is eating", philo);
	new_sleep(philo->env->time_to_eat, philo->env);
	pthread_mutex_lock(&philo->env->meal);
	philo->ate_times++;
	if (philo->env->eat_count_max > 0
		&& philo->ate_times >= philo->env->eat_count_max)
		philo->env->max_ate++;
	pthread_mutex_unlock(&philo->env->meal);
	pthread_mutex_unlock(&philo->env->forks[philo->sfork]);
	pthread_mutex_unlock(&philo->env->forks[philo->ffork]);
}

void	philo_print(const char *msg, t_philo *philo)
{
	unsigned long	current_time;
	char			*time_str;

	current_time = get_time();
	time_str = ft_itoa(current_time - philo->env->start_time);
	pthread_mutex_lock(&philo->env->writing);
	printf("%s %s %s\n", time_str, philo->pos_str, msg);
	pthread_mutex_unlock(&philo->env->writing);
	free(time_str);
}

void	philo_dead(t_env *env, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < env->count)
	{
		pthread_mutex_lock(&env->meal);
		if ((unsigned long)(get_time() - philos[i].last_ate) >
			(unsigned long)env->time_to_die)
		{
			env->stop_condition = 1;
			pthread_mutex_unlock(&env->meal);
			print_message("died", &philos[i]);
			return ;
		}
		pthread_mutex_unlock(&env->meal);
		i++;
	}
}

void	print_message(const char *msg, t_philo *philo)
{
	unsigned long	current_time;
	char			*time_str;

	if (!philo || !philo->env || !msg || philo->env->stop_condition)
		return ;
	current_time = get_time();
	time_str = ft_itoa(current_time - philo->env->start_time);
	if (!time_str)
		return ;
	pthread_mutex_lock(&philo->env->writing);
	if (!philo->env->stop_condition || strcmp(msg, "died") == 0)
		printf("%s %s %s\n", time_str, philo->pos_str, msg);
	pthread_mutex_unlock(&philo->env->writing);
	free(time_str);
}
