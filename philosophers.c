/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhomsi <ykhomsi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:47:20 by ykhomsi           #+#    #+#             */
/*   Updated: 2025/03/21 16:47:22 by ykhomsi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	monitor_philos(t_table *table)
{
	t_philo	*philo;
	int		i;
	int		all_ate_enough;

	while (!table->is_dead)
	{
		i = 0;
		all_ate_enough = 1;
		philo = table->first;
		while (i < table->philo_count)
		{
			if (is_dead(philo))
				return;
			if (table->must_eat_count != -1)
			{
				pthread_mutex_lock(&philo->meal_mutex);
				if (philo->ate_times < table->must_eat_count)
					all_ate_enough = 0;
				pthread_mutex_unlock(&philo->meal_mutex);
			}
			philo = philo->right;
			i++;
		}
		if (table->must_eat_count != -1 && all_ate_enough)
		{
			pthread_mutex_lock(&table->table_mutex);
			table->all_ate = 1;
			pthread_mutex_unlock(&table->table_mutex);
			return;
		}
		usleep(1000);
	}
}

int	main(int ac, char **av)
{
	t_table	table;

	if (ac != 5 && ac != 6)
	{
		printf("Error: wrong number of arguments\n");
		return (1);
	}
	ft_bzero(&table, sizeof(t_table));
	if (!init_table(av, &table))
		return (1);
	monitor_philos(&table);
	cleanup_table(&table);
	return (0);
}
