/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jriga <jriga@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 19:24:28 by jriga             #+#    #+#             */
/*   Updated: 2026/02/19 19:32:26 by jriga            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_context	*get_context(char **args, char ac)
{
	t_context	*ctx;

	ctx = (t_context *)malloc(sizeof(t_context));
	if (!ctx)
		return (NULL);
	ctx->number_of_philos = (size_t)ft_atol(args[0]);
	ctx->time_to_die = (size_t)ft_atol(args[1]);
	ctx->time_to_eat = (size_t)ft_atol(args[2]);
	ctx->time_to_sleep = (size_t)ft_atol(args[3]);
	ctx->number_of_deaths = 0;
	ctx->number_of_ended_threads = 0;
	ctx->forks_mtx = init_forks_mtx(ctx->number_of_philos);
	ctx->init_time_ms = get_time_ms();
	if (!ctx->forks_mtx)
	{
		free(ctx);
		return (NULL);
	}
	if (ac >= 5)
	{
		ctx->must_eat_quantity = (size_t)ft_atol(args[4]);
		ctx->simulation_mod = TRUE;
	}
	else
	{
		ctx->must_eat_quantity = 0;
		ctx->simulation_mod = FALSE;
	}
	pthread_mutex_init(&ctx->output_mtx, NULL);
	pthread_mutex_init(&ctx->ended_threads_mtx, NULL);
	pthread_mutex_init(&ctx->dead_philosophers_mtx, NULL);
	return (ctx);
}

t_philo	*init_philos(t_context *ctx)
{
	t_philo	*philos;
	size_t	i;
	size_t	max;

	if (!ctx)
		return (NULL);
	philos = malloc(sizeof(t_philo) * (ctx->number_of_philos));
	i = 0;
	max = ctx->number_of_philos;
	if (!philos)
		return (NULL);
	assign_forks_to_philos_mtx(ctx->forks_mtx, philos, ctx);
	while (i < max)
	{
		philos[i].ctx = ctx;
		philos[i].id = i + 1;
		pthread_mutex_init(&philos[i].is_dead_mtx, NULL);
		pthread_mutex_init(&philos[i].last_eat_mtx, NULL);
		pthread_mutex_init(&philos[i].thread_ended_mtx, NULL);
		i++;
	}
	return (philos);
}

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

t_bool	monitor(t_context *ctx, t_philo *philos)
{
	size_t	i;
	size_t	max;
	t_bool	killed;

	if (!ctx || !philos)
		return (FALSE);
	max = ctx->number_of_philos;
	killed = FALSE;
	while (1)
	{
		i = 0;
		usleep(500);
		if (ctx->ended_simulation)
			break ;
		while (i < max)
		{
			killed = judge_to_kill_philo(ctx, philos + i);
			if (ctx->simulation_mod == FALSE && killed)
			{
				kill_all_philos(ctx, philos);
				philo_print_action("died", philos + i);
				ctx->ended_simulation = TRUE;
				break ;
			}
			else if (killed)
				philo_print_action("died", philos + i);
			killed = FALSE;
			i++;
		}
		if (ctx->number_of_ended_threads >= max)
			break ;
	}
	return (TRUE);
}
