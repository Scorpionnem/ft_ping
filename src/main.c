/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 12:34:33 by mbatty            #+#    #+#             */
/*   Updated: 2026/02/22 13:43:17 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"
#include "dns.h"

#include "pckt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>

static bool	g_running = true;

static void	handle_sigint(int UNUSED(sig))
{
	g_running = false;
}

static int	ft_ping(t_ctx *ctx)
{
	(void)ctx;

	signal(SIGINT, handle_sigint);

	pid_t	pid = getpid();
	int		seq = 0;

	int ip_header_length = 20;
	int raw_icmp_response_length = ip_header_length + sizeof(t_pckt);

	t_pckt	pckt;

	while (g_running)
	{
		pckt_init(&pckt, pid, ++seq);

		sendto(ctx->sock_fd, &pckt, sizeof(t_pckt), 0, (struct sockaddr *)&ctx->addr, sizeof(ctx->addr));

		bool done = false;
		while (!done)
		{
			char	buffer[1024];
			int		data_received = recvfrom(ctx->sock_fd, buffer, sizeof(buffer), 0, NULL, NULL);;

			if (data_received == raw_icmp_response_length)
			{
				t_pckt	*pckt_recv = (t_pckt *)&buffer[ip_header_length];

				if (pckt_check(&pckt, pckt_recv, pid) == 0)
				{
					printf("%d bytes from %s: icmp_seq=%d\n", data_received, ctx->hostname_str, pckt.hdr.un.echo.sequence); // TODO add time
					break ;
				}
				else // TODO fix localhost
					continue ;
			}
		}
	}
	printf("--- %s ping statistics ---\n", ctx->ip_str); // TODO statistics
	return (0);
}

int	main(int UNUSED(ac), char **av)
{
	t_ctx	ctx;

	if (ctx_init(&ctx, &av) == -1)
		return (1);

	printf("PING %s (%s) %d(%d) bytes of data.\n", ctx.ip_str, ctx.hostname_str, 67, 69); // TODO fix size

	int	ret = ft_ping(&ctx);

	ctx_delete(&ctx);
	return (ret);
}
