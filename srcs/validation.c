/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhomsi <ykhomsi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 23:35:00 by ykhomsi           #+#    #+#             */
/*   Updated: 2025/03/18 03:01:22 by ykhomsi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	validate_params(t_env *env, int argc, char *argv[])
{
	if (argc < 5 || argc > 6)
		return (report_error("Invalid number of arguments"));
	if (!is_valid_int(argv[1]) || !is_valid_int(argv[2])
		|| !is_valid_int(argv[3]) || !is_valid_int(argv[4])
		|| (argc == 6 && !is_valid_int(argv[5])))
		return (report_error("Invalid argument type"));
	env->count = ft_atoi(argv[1]);
	env->time_to_die = ft_atoi(argv[2]);
	env->time_to_eat = ft_atoi(argv[3]);
	env->time_to_sleep = ft_atoi(argv[4]);
	env->eat_count_max = -1;
	if (argc == 6)
		env->eat_count_max = ft_atoi(argv[5]);
	if (env->count <= 0 || env->time_to_die <= 0 || env->time_to_eat <= 0
		|| env->time_to_sleep <= 0)
		return (report_error("Invalid argument value"));
	return (1);
}

int	setup_environment(t_env *env)
{
	if (!initialize_environment(env))
		return (0);
	return (1);
}

int	launch_philosophers(t_env *env)
{
	if (!start_philosopher_threads(env))
		return (0);
	return (1);
}

int	report_error(const char *msg)
{
	write(2, msg, ft_strlen(msg));
	return (0);
}

int	is_valid_int(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}
