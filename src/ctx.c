/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 18:05:50 by mbatty            #+#    #+#             */
/*   Updated: 2026/02/22 13:43:09 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"
#include <stdlib.h>

int		ctx_init_opts(t_ctx *ctx, char ***av);
void	print_help();

int	ctx_init(t_ctx *ctx, char ***av)
{
	if (ctx_init_opts(ctx, av) == -1)
		return (-1);
	if (ctx->help._bool)
	{
		opt_ctx_delete(&ctx->opt_ctx);
		print_help();
		return (-1);
	}

	if (!*av[0])
	{
		opt_ctx_delete(&ctx->opt_ctx);
		dprintf(2, "ft_ping: usage error: Destination address required\n");
		return (-1);
	}

	ctx->ip_str = dns_reverse_lookup(*av[0]);
	if (!ctx->ip_str)
		ctx->ip_str = strdup(*av[0]);
	ctx->hostname_str = dns_lookup(ctx->ip_str, &ctx->addr);
	if (!ctx->hostname_str)
	{
		opt_ctx_delete(&ctx->opt_ctx);
		free(ctx->ip_str);
		return (-1);
	}
	ctx->sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (ctx->sock_fd == -1)
	{
		opt_ctx_delete(&ctx->opt_ctx);
		free(ctx->ip_str);
		free(ctx->hostname_str);
		perror("ft_ping: socket");
		return (-1);
	}
	return (0);
}

void	ctx_delete(t_ctx *ctx)
{
	close(ctx->sock_fd);
	free(ctx->ip_str);
	free(ctx->hostname_str);
	opt_ctx_delete(&ctx->opt_ctx);
}

void	print_help()
{
	printf("usage: ./ft_ping [-h, -?]\n\n");
	printf("options:\n");
	printf("-? -h --help\tshow help message and exit\n");
}

int	ctx_init_opts(t_ctx *ctx, char ***av)
{
	opt_ctx_init(&ctx->opt_ctx);

	opt_ctx_add_opt(&ctx->opt_ctx, "-h", &ctx->help, OPT_BOOL);
	opt_ctx_add_opt(&ctx->opt_ctx, "-?", &ctx->help, OPT_BOOL);
	opt_ctx_add_opt(&ctx->opt_ctx, "--help", &ctx->help, OPT_BOOL);

	opt_ctx_add_opt(&ctx->opt_ctx, "-v", &ctx->verbose, OPT_BOOL);
	opt_ctx_add_opt(&ctx->opt_ctx, "--verbose", &ctx->verbose, OPT_BOOL);

	opt_ctx_add_opt(&ctx->opt_ctx, "--test", &ctx->test, OPT_STR);

	if (opt_ctx_parse(&ctx->opt_ctx, av) == -1)
	{
		opt_ctx_delete(&ctx->opt_ctx);
		return (-1);
	}
	return (0);
}

