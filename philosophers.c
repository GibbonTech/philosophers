/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhomsi <ykhomsi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 23:29:25 by ykhomsi           #+#    #+#             */
/*   Updated: 2025/03/18 03:03:08 by ykhomsi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	run_simulation(t_env *env)
{
	if (env->eat_count_max == 0)
		env->eat_count_max = -1;
	while (!env->stop_condition)
	{
		philo_dead(env, env->philos);
		pthread_mutex_lock(&env->meal);
		if (env->eat_count_max > 0 && env->max_ate >= env->count)
		{
			env->stop_condition = 1;
		}
		pthread_mutex_unlock(&env->meal);
		usleep(500);
	}
}

void	join_threads(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->count)
	{
		pthread_join(env->philos[i].thread_id, NULL);
		i++;
	}
}

void	destroy_mutexes(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->count)
	{
		pthread_mutex_destroy(&env->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&env->meal);
	pthread_mutex_destroy(&env->writing);
}

int	main(int argc, char *argv[])
{
	t_env	env;

	if (!validate_params(&env, argc, argv))
		return (1);
	if (!setup_environment(&env))
		return (1);
	if (!launch_philosophers(&env))
		return (1);
	run_simulation(&env);
	join_threads(&env);
	destroy_mutexes(&env);
	cleanup_environment(&env);
	return (0);
}
