/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 20:14:04 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/03 15:36:18 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

static bool	g_running = true;

void	handle_sigint(int sig)
{
	(void)sig;
	g_running = false;
}

int	open_socket(t_ctx *ctx)
{
	ctx->sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (ctx->sock_fd < 0)
	{
		printf("ft_ping: Failed to open raw socket\n");
		return (0);
	}
	return (1);
}

int	ft_ping(t_ctx *ctx)
{
	signal(SIGINT, handle_sigint);
	if (!open_socket(ctx))
		return (0);

	while (g_running)
	{

	}
	return (1);
}

int	main(int ac, char **av)
{
	t_ctx	ctx;

	(void)ac;
	if (getuid() != 0)
	{
		printf("ft_ping: You are not root!\n");
		return (1);
	}
	if (!ctx_init(&ctx, av))
		return (1);
	ft_ping(&ctx);
	return (ctx_free(&ctx));
}
