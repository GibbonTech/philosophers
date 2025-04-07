/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhomsi <ykhomsi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:47:20 by ykhomsi           #+#    #+#             */
/*   Updated: 2025/04/03 16:26:58 by ykhomsi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	monitor_meals(t_table *table)
{
	t_philo	*philo;
	int		i;
	int		full_philos;

	if (table->must_eat_count == -1)
		return ;
	i = 0;
	full_philos = 0;
	philo = table->first;
	while (i < table->philo_count)
	{
		pthread_mutex_lock(&philo->meal_mutex);
		if (philo->ate_times >= table->must_eat_count)
			full_philos++;
		pthread_mutex_unlock(&philo->meal_mutex);
		philo = philo->right;
		i++;
	}
	if (full_philos == table->philo_count)
	{
		pthread_mutex_lock(&table->table_mutex);
		table->all_ate = 1;
		table->meals_completed = 1;
		pthread_mutex_unlock(&table->table_mutex);
	}
}

static int	check_simulation_end(t_table *table)
{
	pthread_mutex_lock(&table->table_mutex);
	if (table->all_ate)
	{
		if (table->meals_completed)
			printf("Each philosopher ate %d time(s)\n", table->must_eat_count);
		pthread_mutex_unlock(&table->table_mutex);
		return (1);
	}
	pthread_mutex_unlock(&table->table_mutex);
	return (0);
}

static void	monitor_philos(t_table *table)
{
	t_philo	*philo;
	int		i;

	while (1)
	{
		i = 0;
		philo = table->first;
		while (i < table->philo_count)
		{
			if (is_dead(philo))
				return ;
			philo = philo->right;
			i++;
		}
		monitor_meals(table);
		if (check_simulation_end(table))
			return ;
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
