/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jriga <jriga@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 23:08:06 by jriga             #+#    #+#             */
/*   Updated: 2026/02/18 16:05:57 by jriga            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

pthread_mutex_t	*init_forks_mtx(size_t number_of_forks)
{
	pthread_mutex_t	*forks_mtx;
	size_t			i;

	forks_mtx = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* number_of_forks);
	if (!forks_mtx)
		return (NULL);
	i = 0;
	while (i < number_of_forks)
		pthread_mutex_init(forks_mtx + i++, NULL);
	return (forks_mtx);
}

t_bool	assign_forks_to_philos_mtx(pthread_mutex_t *forks_mtx, t_philo *philo,
		t_context *ctx)
{
	size_t	i;
	size_t	max;

	if (!forks_mtx || !philo)
		return (0);
	i = 0;
	max = ctx->number_of_philos;
	while (i < max)
	{
		philo[i].forks_mtx[i % 2] = &forks_mtx[i % (max)];
		philo[i].forks_mtx[(i + 1) % 2] = &forks_mtx[(i + 1) % (max)];
		i++;
	}
	return (1);
}
