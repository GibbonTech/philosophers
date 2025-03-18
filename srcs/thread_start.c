/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_start.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhomsi <ykhomsi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 23:29:09 by ykhomsi           #+#    #+#             */
/*   Updated: 2025/03/18 03:01:08 by ykhomsi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	start_philosopher_threads(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->count)
	{
		if (i > 0)
			usleep(100000);
		if (pthread_create(&env->philos[i].thread_id, NULL,
				(void *)philosopher_routine, &env->philos[i]))
			return (0);
		i++;
	}
	return (1);
}
