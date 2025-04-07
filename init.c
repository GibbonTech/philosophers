/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhomsi <ykhomsi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:47:25 by ykhomsi           #+#    #+#             */
/*   Updated: 2025/03/21 16:49:24 by ykhomsi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static t_philo	*create_philosopher(t_table *table, int pos)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	ft_bzero(philo, sizeof(t_philo));
	philo->pos = pos;
	philo->table = table;
	philo->last_meal = get_time();
	if (pthread_mutex_init(&philo->fork_mutex, NULL) != 0
		|| pthread_mutex_init(&philo->meal_mutex, NULL) != 0)
	{
		free(philo);
		return (NULL);
	}
	return (philo);
}

static int	init_philosophers(t_table *table)
{
	int		i;
	t_philo	*philo;
	t_philo	*prev;

	i = 1;
	table->first = create_philosopher(table, 1);
	if (!table->first)
		return (0);
	prev = table->first;
	while (i < table->philo_count)
	{
		philo = create_philosopher(table, i + 1);
		if (!philo)
			return (0);
		prev->right = philo;
		philo->left = prev;
		prev = philo;
		i++;
	}
	prev->right = table->first;
	table->first->left = prev;
	return (1);
}

static int	create_threads(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = 0;
	philo = table->first;
	table->start_time = get_time();
	while (i < table->philo_count)
	{
		if (pthread_create(&philo->thread_id, NULL, philo_routine, philo) != 0)
			return (0);
		philo = philo->right;
		i++;
	}
	return (1);
}

int	init_table(char **av, t_table *table)
{
	table->philo_count = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]);
	table->time_to_eat = ft_atoi(av[3]);
	table->time_to_sleep = ft_atoi(av[4]);
	if (av[5])
		table->must_eat_count = ft_atoi(av[5]);
	else
		table->must_eat_count = -1;
	table->is_dead = 0;
	table->all_ate = 0;
	table->meals_completed = 0;
	if (table->philo_count <= 0 || table->time_to_die <= 0
		|| table->time_to_eat <= 0 || table->time_to_sleep <= 0
		|| (av[5] && table->must_eat_count <= 0))
		return (0);
	if (pthread_mutex_init(&table->table_mutex, NULL) != 0
		|| pthread_mutex_init(&table->print_mutex, NULL) != 0)
		return (0);
	if (!init_philosophers(table))
		return (0);
	if (!create_threads(table))
		return (0);
	return (1);
}
