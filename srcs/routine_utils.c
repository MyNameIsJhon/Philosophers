/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jriga <jriga@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 14:25:28 by jriga             #+#    #+#             */
/*   Updated: 2026/02/27 14:27:21 by jriga            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_end_thread(t_context *ctx, t_philo *philo)
{
	pthread_mutex_lock(&philo->thread_ended_mtx);
	philo->thread_ended = TRUE;
	pthread_mutex_unlock(&philo->thread_ended_mtx);
	pthread_mutex_lock(&ctx->ended_threads_mtx);
	ctx->number_of_ended_threads++;
	pthread_mutex_unlock(&ctx->ended_threads_mtx);
}

t_bool	philo_take_first_fork(t_context *ctx, t_philo *philo)
{
	pthread_mutex_lock(philo->forks_mtx[0]);
	pthread_mutex_lock(&philo->is_dead_mtx);
	if (philo->is_dead)
	{
		pthread_mutex_unlock(philo->forks_mtx[0]);
		pthread_mutex_unlock(&philo->is_dead_mtx);
		return (FALSE);
	}
	pthread_mutex_unlock(&philo->is_dead_mtx);
	philo_print_action("has taken a fork", philo);
	if (ctx->number_of_philos == 1)
	{
		usleep(ctx->time_to_die * 1000);
		pthread_mutex_unlock(philo->forks_mtx[0]);
		return (FALSE);
	}
	return (TRUE);
}

t_bool	philo_take_second_fork(t_philo *philo)
{
	pthread_mutex_lock(philo->forks_mtx[1]);
	pthread_mutex_lock(&philo->is_dead_mtx);
	if (philo->is_dead)
	{
		pthread_mutex_unlock(philo->forks_mtx[0]);
		pthread_mutex_unlock(philo->forks_mtx[1]);
		pthread_mutex_unlock(&philo->is_dead_mtx);
		return (FALSE);
	}
	pthread_mutex_unlock(&philo->is_dead_mtx);
	return (TRUE);
}
