/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 09:51:21 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/03 09:58:47 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

int	ctx_get_options(t_ctx *ctx)
{
	t_arg	*remaining_args;

	remaining_args = ctx->opts->args;
	if (remaining_args)
		ctx->host = remaining_args->arg;
	else
	{
		printf("ft_ping: usage error: Destination address required\n");
		return (0);
	}
	return (1);
}

int	ctx_init_options(t_ctx *ctx, char **av)
{
	
	ctx->opts = ft_getopt(BOOLEAN_FLAGS, ARG_FLAGS, av);
	if (!ctx->opts)
		return (0);
	if (!ctx_get_options(ctx))
	{
		ft_free_opt(ctx->opts);
		return (0);
	}
	return (1);
}

int	ctx_init(t_ctx *ctx, char **av)
{
	memset(ctx, 0, sizeof(t_ctx));
	if (!ctx_init_options(ctx, av))
		return (0);
	return (1);
}

int	ctx_free(t_ctx *ctx)
{
	ft_free_opt(ctx->opts);
	return (0);
}
