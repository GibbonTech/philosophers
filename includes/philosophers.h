/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhomsi <ykhomsi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 23:28:38 by ykhomsi           #+#    #+#             */
/*   Updated: 2025/03/18 03:03:26 by ykhomsi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define LOCK 1
# define UNLOCK 0

typedef struct s_env	t_env;

typedef struct s_philo
{
	int				pos;
	int				ate_times;
	unsigned long	last_ate;
	char			*pos_str;
	int				ffork;
	int				sfork;
	pthread_t		thread_id;
	t_env			*env;
}					t_philo;

typedef struct s_env
{
	int				count;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				eat_count_max;
	unsigned long	start_time;
	int				stop_condition;
	int				max_ate;
	pthread_mutex_t	*forks;
	pthread_mutex_t	meal;
	pthread_mutex_t	writing;
	t_philo			*philos;
}					t_env;

int					initialize_environment(t_env *env);
int					validate_params(t_env *env, int argc, char *argv[]);
int					setup_environment(t_env *env);
int					launch_philosophers(t_env *env);
int					report_error(const char *msg);
int					is_valid_int(const char *str);
int					initialize_environment(t_env *env);
int					start_philosopher_threads(t_env *env);
int					allocate_memory(t_env *env);
char				*ft_itoa(int n);

unsigned long		get_time(void);
void				new_sleep(unsigned long duration, t_env *env);
int					ft_strlen(const char *str);
int					ft_return_error(char *msg);
int					ft_isint(const char *nptr);
int					ft_atoi(const char *str);

void				philo_dead(t_env *env, t_philo *philos);
void				philo_eat(t_philo *philo);
void				philo_print(const char *msg, t_philo *philo);
// Updated declaration for philo_print
void				print_message(const char *msg, t_philo *philo);
// Updated declaration for print_message

char				*ft_strdup(const char *s);
int					ft_intlen(int n);

void				*philosopher_routine(void *arg);
void				cleanup_environment(t_env *env);
void				init_philosopher(t_philo *philo, int i, t_env *env);

#endif
