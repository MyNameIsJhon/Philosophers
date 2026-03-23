/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jriga <jriga@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 14:21:57 by jriga             #+#    #+#             */
/*   Updated: 2026/03/23 13:19:13 by jriga            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	fill_context_args(t_context *ctx, char **args, char ac)
{
	ctx->number_of_philos = (size_t)ft_atol(args[0]);
	ctx->time_to_die = (size_t)ft_atol(args[1]);
	ctx->time_to_eat = (size_t)ft_atol(args[2]);
	ctx->time_to_sleep = (size_t)ft_atol(args[3]);
	ctx->ended_simulation = FALSE;
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
}

static void	init_context_mtx(t_context *ctx)
{
	pthread_mutex_init(&ctx->output_mtx, NULL);
	pthread_mutex_init(&ctx->ended_threads_mtx, NULL);
	pthread_mutex_init(&ctx->dead_philosophers_mtx, NULL);
}

t_context	*get_context(char **args, char ac)
{
	t_context	*ctx;

	ctx = (t_context *)malloc(sizeof(t_context));
	if (!ctx)
		return (NULL);
	fill_context_args(ctx, args, ac);
	ctx->number_of_deaths = 0;
	ctx->number_of_ended_threads = 0;
	ctx->forks_mtx = init_forks_mtx(ctx->number_of_philos);
	ctx->init_time_ms = get_time_ms();
	if (!ctx->forks_mtx)
	{
		free(ctx);
		return (NULL);
	}
	init_context_mtx(ctx);
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
		philos[i].is_dead = FALSE;
		philos[i].thread_ended = FALSE;
		philos[i].last_eat_ms = ctx->init_time_ms;
		pthread_mutex_init(&philos[i].is_dead_mtx, NULL);
		pthread_mutex_init(&philos[i].last_eat_mtx, NULL);
		pthread_mutex_init(&philos[i].thread_ended_mtx, NULL);
		i++;
	}
	return (philos);
}
