/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhomsi <ykhomsi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 23:35:00 by ykhomsi           #+#    #+#             */
/*   Updated: 2025/03/18 03:00:20 by ykhomsi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	free_mutexes(t_env *env)
{
	int	i;

	if (env && env->forks)
	{
		pthread_mutex_destroy(&env->meal);
		pthread_mutex_destroy(&env->writing);
		i = 0;
		while (i < env->count)
		{
			pthread_mutex_destroy(&env->forks[i]);
			i++;
		}
	}
}

static void	free_memory(t_env *env)
{
	int	i;

	if (env && env->philos)
	{
		i = 0;
		while (i < env->count)
		{
			if (env->philos[i].pos_str)
				free(env->philos[i].pos_str);
			i++;
		}
		free(env->philos);
	}
	if (env && env->forks)
		free(env->forks);
}

void	cleanup_environment(t_env *env)
{
	free_mutexes(env);
	free_memory(env);
}
