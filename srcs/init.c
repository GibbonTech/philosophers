/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhomsi <ykhomsi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 23:28:59 by ykhomsi           #+#    #+#             */
/*   Updated: 2025/03/18 03:00:31 by ykhomsi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	init_mutexes(t_env *env)
{
	int	i;

	if (pthread_mutex_init(&env->meal, NULL) != 0
		|| pthread_mutex_init(&env->writing, NULL) != 0)
		return (0);
	i = 0;
	while (i < env->count)
	{
		if (pthread_mutex_init(&env->forks[i], NULL) != 0)
			return (0);
		i++;
	}
	return (1);
}

static int	allocate_resources(t_env *env)
{
	env->philos = malloc(sizeof(t_philo) * env->count);
	if (!env->philos)
		return (0);
	env->forks = malloc(sizeof(pthread_mutex_t) * env->count);
	if (!env->forks)
	{
		free(env->philos);
		return (0);
	}
	memset(env->philos, 0, sizeof(t_philo) * env->count);
	env->start_time = get_time();
	env->stop_condition = 0;
	env->max_ate = 0;
	return (1);
}

static int	init_philos(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->count)
	{
		init_philosopher(&env->philos[i], i, env);
		if (!env->philos[i].pos_str)
			return (0);
		i++;
	}
	return (1);
}

int	initialize_environment(t_env *env)
{
	if (env->count <= 0 || env->time_to_die <= 0 || env->time_to_eat <= 0
		|| env->time_to_sleep <= 0)
		return (0);
	if (!allocate_resources(env))
		return (0);
	if (!init_mutexes(env))
		return (0);
	if (!init_philos(env))
		return (0);
	return (1);
}

void	init_philosopher(t_philo *philo, int i, t_env *env)
{
	philo->pos = i + 1;
	philo->ate_times = 0;
	philo->last_ate = get_time();
	if (env->count == 1)
	{
		philo->ffork = 0;
		philo->sfork = 0;
	}
	else
	{
		philo->ffork = i;
		philo->sfork = (i + 1) % env->count;
	}
	philo->env = env;
	philo->pos_str = ft_itoa(philo->pos);
}
