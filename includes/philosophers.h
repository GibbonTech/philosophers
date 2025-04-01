/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhomsi <ykhomsi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:47:20 by ykhomsi           #+#    #+#             */
/*   Updated: 2025/03/22 05:59:54 by ykhomsi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>
# include <string.h>

typedef enum e_status
{
	EATING,
	SLEEPING,
	THINKING,
	DIED
}	t_philo_status;

typedef struct s_philo
{
	int				pos;
	int				ate_times;
	long long		last_meal;
	struct s_philo	*left;
	struct s_philo	*right;
	struct s_table	*table;
	pthread_t		thread_id;
	t_philo_status	status;
	pthread_mutex_t	fork_mutex;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	status_mutex;
}	t_philo;

typedef struct s_table
{
	int				philo_count;
	long long		start_time;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	int				must_eat_count;
	struct s_philo	*first;
	pthread_mutex_t	table_mutex;
	pthread_mutex_t	print_mutex;
	int				is_dead;
	int				all_ate;
}	t_table;

// Core functions
int					init_table(char **av, t_table *table);
void				*philo_routine(void *arg);
void				cleanup_table(t_table *table);

// Action functions
void				take_forks(t_philo *philo);
void				eating(t_philo *philo);
void				sleeping(t_philo *philo);
void				thinking(t_philo *philo);

// Utility functions
int					is_dead(t_philo *philo);
int					check_death(t_table *table);
void				print_status(t_philo *philo, char *msg);
long long			get_time(void);
void				precise_sleep(long long ms, t_philo *philo);
int					ft_atoi(const char *str);
void				ft_bzero(void *s, size_t n);

#endif