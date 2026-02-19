/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jriga <jriga@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 19:44:42 by jriga             #+#    #+#             */
/*   Updated: 2026/02/16 23:22:41 by jriga            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_context(t_context *ctx)
{
	printf("CONTEXT:\n");
	printf("number_of_philos: %lu\n", ctx->number_of_philos);
	printf("time_to_die: %lu\n", ctx->time_to_die);
	printf("time_to_eat: %lu\n", ctx->time_to_eat);
	printf("time_to_sleep: %lu\n", ctx->time_to_sleep);
	if (ctx->simulation_mod)
		printf("must_eat_quantity: %lu\n", ctx->must_eat_quantity);
	else
		printf("must_eat_quantity: N/A\n");
	printf("END CONTEXT\n\n\n");
}
