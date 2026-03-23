/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jriga <jriga@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 12:50:15 by jriga             #+#    #+#             */
/*   Updated: 2026/03/23 12:58:45 by jriga            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_forks_mtx(pthread_mutex_t *forks_mtx, size_t number_of_forks)
{
	size_t	i;

	if (!forks_mtx)
		return ;
	i = 0;
	while (i < number_of_forks)
		pthread_mutex_destroy(forks_mtx + i++);
	free(forks_mtx);
}

void	destroy_philos(t_philo *philos, t_context *ctx)
{
	size_t	i;
	size_t	max;

	if (!ctx || !philos)
		return ;
	i = 0;
	max = ctx->number_of_philos;
	while (i < max)
	{
		pthread_mutex_destroy(&philos[i].is_dead_mtx);
		pthread_mutex_destroy(&philos[i].last_eat_mtx);
		pthread_mutex_destroy(&philos[i].thread_ended_mtx);
		i++;
	}
	free(philos);
}
