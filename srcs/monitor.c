/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jriga <jriga@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 19:24:28 by jriga             #+#    #+#             */
/*   Updated: 2026/02/27 14:24:06 by jriga            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static t_bool	kill_all_philos(t_context *ctx, t_philo *philos)
{
	size_t	i;
	size_t	max;

	if (!ctx || !philos)
		return (FALSE);
	i = 0;
	max = ctx->number_of_philos;
	while (i < max)
	{
		pthread_mutex_lock(&philos[i].is_dead_mtx);
		philos[i].is_dead = TRUE;
		pthread_mutex_unlock(&philos[i].is_dead_mtx);
		i++;
	}
	return (TRUE);
}

static t_bool	judge_to_kill_philo(t_context *ctx, t_philo *philo)
{
	t_bool	killed;

	killed = FALSE;
	pthread_mutex_lock(&philo->is_dead_mtx);
	pthread_mutex_lock(&philo->thread_ended_mtx);
	pthread_mutex_lock(&philo->last_eat_mtx);
	if (!philo->is_dead && !philo->thread_ended
		&& get_time_since(philo->last_eat_ms) > ctx->time_to_die)
	{
		killed = TRUE;
		philo->is_dead = TRUE;
	}
	pthread_mutex_unlock(&philo->is_dead_mtx);
	pthread_mutex_unlock(&philo->thread_ended_mtx);
	pthread_mutex_unlock(&philo->last_eat_mtx);
	return (killed);
}

static void	monitor_check_philos(t_context *ctx, t_philo *philos, size_t max)
{
	size_t	i;
	t_bool	killed;

	i = 0;
	while (i < max)
	{
		killed = judge_to_kill_philo(ctx, philos + i);
		if (ctx->simulation_mod == FALSE && killed)
		{
			kill_all_philos(ctx, philos);
			philo_print_action("died", philos + i);
			ctx->ended_simulation = TRUE;
			return ;
		}
		else if (killed)
			philo_print_action("died", philos + i);
		killed = FALSE;
		i++;
	}
}

t_bool	monitor(t_context *ctx, t_philo *philos)
{
	size_t	max;

	if (!ctx || !philos)
		return (FALSE);
	max = ctx->number_of_philos;
	while (1)
	{
		usleep(500);
		if (ctx->ended_simulation)
			break ;
		monitor_check_philos(ctx, philos, max);
		pthread_mutex_lock(&ctx->ended_threads_mtx);
		if (ctx->number_of_ended_threads >= max)
		{
			pthread_mutex_unlock(&ctx->ended_threads_mtx);
			break ;
		}
		pthread_mutex_unlock(&ctx->ended_threads_mtx);
	}
	return (TRUE);
}
