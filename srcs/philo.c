/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jriga <jriga@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:39:26 by jriga             #+#    #+#             */
/*   Updated: 2026/03/23 14:36:22 by jriga            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void	threads_create(t_philo *philos)
{
	size_t	i;
	size_t	max;

	if (!philos)
		return ;
	i = 0;
	max = philos->ctx->number_of_philos;
	while (i < max)
	{
		pthread_create(&philos[i].thread, NULL, start_routine, philos + i);
		i++;
	}
}

void	threads_join(t_philo *philos)
{
	size_t	i;
	size_t	max;

	i = 0;
	max = philos->ctx->number_of_philos;
	while (i < max)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
}

int	main(int ac, char **av)
{
	t_context	*ctx;
	t_philo		*philos;

	(void)av;
	(void)ac;
	(void)ctx;
	if (ac < 5)
		return (1);
	ctx = get_context(av + 1, ac - 1);
	philos = init_philos(ctx);
	threads_create(philos);
	monitor(ctx, philos);
	threads_join(philos);
	destroy_philos(philos, ctx);
	destroy_forks_mtx(ctx->forks_mtx, ctx->number_of_philos);
	free(ctx);
	return (0);
}
