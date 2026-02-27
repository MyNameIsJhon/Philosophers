/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jriga <jriga@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 19:49:23 by jriga             #+#    #+#             */
/*   Updated: 2026/02/27 14:26:51 by jriga            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>

static t_bool	philo_is_eating(t_context *ctx, t_philo *philo)
{
	if (!philo_take_first_fork(ctx, philo))
		return (FALSE);
	if (!philo_take_second_fork(philo))
		return (FALSE);
	philo_print_action("is eating", philo);
	pthread_mutex_lock(&philo->last_eat_mtx);
	philo->last_eat_ms = get_time_ms();
	pthread_mutex_unlock(&philo->last_eat_mtx);
	usleep(ctx->time_to_eat * 1000);
	pthread_mutex_unlock(philo->forks_mtx[0]);
	pthread_mutex_unlock(philo->forks_mtx[1]);
	return (TRUE);
}

static t_bool	philo_is_sleeping(t_context *ctx, t_philo *philo)
{
	pthread_mutex_lock(&philo->is_dead_mtx);
	if (philo->is_dead)
	{
		pthread_mutex_unlock(&philo->is_dead_mtx);
		return (FALSE);
	}
	pthread_mutex_unlock(&philo->is_dead_mtx);
	philo_print_action("is sleeping", philo);
	usleep(ctx->time_to_sleep * 1000);
	return (TRUE);
}

static t_bool	philo_is_thinking(t_context *ctx, t_philo *philo)
{
	size_t	think_time;

	pthread_mutex_lock(&philo->is_dead_mtx);
	if (philo->is_dead)
	{
		pthread_mutex_unlock(&philo->is_dead_mtx);
		return (FALSE);
	}
	pthread_mutex_unlock(&philo->is_dead_mtx);
	philo_print_action("is thinking", philo);
	if (ctx->number_of_philos % 2 == 1)
	{
		think_time = ctx->time_to_eat * 2 - ctx->time_to_sleep;
		usleep(think_time * 1000);
	}
	return (TRUE);
}

static t_bool	philos_routine(t_context *ctx, t_philo *philo)
{
	size_t	i;

	i = 1;
	while (1)
	{
		if (!philo_is_thinking(ctx, philo))
			break ;
		if (!philo_is_eating(ctx, philo))
			break ;
		if (!philo_is_sleeping(ctx, philo))
			break ;
		if (ctx->simulation_mod != 0)
		{
			if (i >= ctx->must_eat_quantity)
				break ;
			i++;
		}
	}
	philo_end_thread(ctx, philo);
	return (TRUE);
}

void	*start_routine(void *data)
{
	t_philo		*philos;
	t_context	*ctx;

	philos = data;
	if (!philos || !philos->ctx)
		return (NULL);
	ctx = philos->ctx;
	philos->last_eat_ms = get_time_ms();
	if (philos->id % 2 == 0)
		usleep(ctx->time_to_eat * 1000);
	philos_routine(ctx, philos);
	return (NULL);
}
