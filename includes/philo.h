/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jriga <jriga@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:40:49 by jriga             #+#    #+#             */
/*   Updated: 2026/02/27 14:29:04 by jriga            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

typedef enum e_bool
{
	FALSE,
	TRUE,
}					t_bool;

// INFO:
// simulation mod 0 correspand au premier mourant
// le 1 correspond a une limite de nourriture
typedef struct s_context
{
	size_t			init_time_ms;
	size_t			number_of_philos;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			must_eat_quantity;
	size_t			number_of_ended_threads;
	size_t			number_of_deaths;
	pthread_mutex_t	dead_philosophers_mtx;
	pthread_mutex_t	output_mtx;
	pthread_mutex_t	ended_threads_mtx;
	pthread_mutex_t	*forks_mtx;
	t_bool			simulation_mod;
	t_bool			ended_simulation;
}					t_context;

typedef struct s_philo
{
	pthread_mutex_t	*forks_mtx[2];
	pthread_mutex_t	last_eat_mtx;
	pthread_mutex_t	is_dead_mtx;
	pthread_mutex_t	thread_ended_mtx;
	pthread_t		thread;
	size_t			id;
	size_t			last_eat_ms;
	t_bool			thread_ended;
	t_bool			is_dead;
	t_context		*ctx;
}					t_philo;

long				ft_atol(char *str_nb);
void				ft_putstr_fd(const char *str, int fd);

pthread_mutex_t		*init_forks_mtx(size_t number_of_forks);
t_bool				assign_forks_to_philos_mtx(pthread_mutex_t *forks_mtx,
						t_philo *philo, t_context *ctx);

void				print_time_of_day(void);
void				philo_print_action(const char *action, t_philo *philo);
size_t				get_time_ms(void);
void				*start_routine(void *data);
size_t				get_time_since(size_t time_ms);
t_philo				*init_philos(t_context *ctx);
t_bool				monitor(t_context *ctx, t_philo *philos);
t_context			*get_context(char **args, char ac);
void				philo_end_thread(t_context *ctx, t_philo *philo);
t_bool				philo_take_first_fork(t_context *ctx, t_philo *philo);
t_bool				philo_take_second_fork(t_philo *philo);

// NOTE: DEBUG

void				print_context(t_context *ctx);
