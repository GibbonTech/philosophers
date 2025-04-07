/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhomsi <ykhomsi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:47:16 by ykhomsi           #+#    #+#             */
/*   Updated: 2025/03/21 16:49:27 by ykhomsi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	cleanup_table(t_table *table)
{
	int		i;
	t_philo	*current;
	t_philo	*next;

	if (!table)
		return ;
	i = 0;
	current = table->first;
	while (i < table->philo_count)
	{
		next = current->right;
		pthread_join(current->thread_id, NULL);
		pthread_mutex_destroy(&current->fork_mutex);
		pthread_mutex_destroy(&current->meal_mutex);
		free(current);
		current = next;
		i++;
	}
	pthread_mutex_destroy(&table->table_mutex);
	pthread_mutex_destroy(&table->print_mutex);
}
