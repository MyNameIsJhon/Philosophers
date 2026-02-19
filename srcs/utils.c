/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jriga <jriga@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 18:09:47 by jriga             #+#    #+#             */
/*   Updated: 2026/02/19 19:34:54 by jriga            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

long	ft_atol(char *str_nb)
{
	long long	nb;
	int			sgn;

	nb = 0;
	sgn = 1;
	if (!str_nb)
		return (0);
	if (*str_nb == '-')
	{
		sgn = (-1);
		str_nb++;
	}
	else if (*str_nb == '+')
		str_nb++;
	while (*str_nb >= '0' && *str_nb <= '9')
	{
		nb *= 10;
		nb = nb + *str_nb - '0';
		str_nb++;
	}
	return ((long)(nb * (long long)sgn));
}

size_t	get_time_ms()
{
	struct timeval time;

	gettimeofday(&time, NULL);
	return (size_t)((time.tv_sec * 1000000) + time.tv_usec) / 1000;
}

size_t	get_time_since(size_t time_ms)
{
	return (get_time_ms() - time_ms);
}

void	philo_print_action(const char *action, t_philo *philo)
{
	pthread_mutex_lock(&philo->ctx->output_mtx);
	printf("%lu %lu %s\n", get_time_since(philo->ctx->init_time_ms), philo->id,
		action);
	pthread_mutex_unlock(&philo->ctx->output_mtx);
}
