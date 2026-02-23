/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 12:34:33 by mbatty            #+#    #+#             */
/*   Updated: 2026/02/23 11:21:41 by mbatty           ###   ########.fr       */
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

static int	send_packet(t_ctx *ctx)
{
	clock_gettime(CLOCK_MONOTONIC, &ctx->time_start);
	if (sendto(ctx->sock_fd, &ctx->pckt, sizeof(t_pckt), 0, (struct sockaddr *)&ctx->addr, sizeof(ctx->addr)) == -1)
		return (perror("ft_ping: sendto"), -1);
	ctx->packets_sent++;
	return (0);
}

static int	recv_packet(t_ctx *ctx)
{
	double	rtt_msec = 0;

	int		data_received = recvfrom(ctx->sock_fd, ctx->buffer, sizeof(ctx->buffer), 0, NULL, NULL);
	if (data_received == -1)
		perror("ft_ping: recvfrom");

	clock_gettime(CLOCK_MONOTONIC, &ctx->time_end);
	double	timeElapsed = ((double)(ctx->time_end.tv_nsec - ctx->time_start.tv_nsec)) / 1000000.0;
	rtt_msec = (ctx->time_end.tv_sec - ctx->time_start.tv_sec) * 1000.0 + timeElapsed;

	if (data_received >= (int)(sizeof(struct iphdr) + sizeof(struct icmphdr)))
	{
		struct iphdr	*ip = (struct iphdr *)ctx->buffer;

		int				ip_header_length = ip->ihl * 4;
		int				ttl = ip->ttl;

		t_pckt *pckt_recv = (t_pckt *)(ctx->buffer + ip_header_length);

		if (pckt_check(&ctx->pckt, pckt_recv, ctx->pid) == 0)
		{
			ctx->packets_received++;
			if (!ctx->quiet._bool)
				printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%f ms\n",
						data_received, ctx->hostname_str, pckt_recv->hdr.un.echo.sequence, ttl, rtt_msec);
			if (ctx->audible._bool)
				printf("\a");
		}
	}
	return (0);
}

static int	ft_ping(t_ctx *ctx)
{
	signal(SIGINT, handle_sigint);

	ctx->pid = getpid();

	double	total_msec = 0;
	struct timespec	tfs, tfe;
	clock_gettime(CLOCK_MONOTONIC, &tfs);

	while (g_running)
	{
		pckt_init(&ctx->pckt, ctx->pid, ++ctx->seq);

		send_packet(ctx);

		recv_packet(ctx);

		if (ctx->count._int != -1 && ctx->packets_received >= ctx->count._int)
			break ;

		if (!ctx->flood._bool)
			sleep(1);
	}
	clock_gettime(CLOCK_MONOTONIC, &tfe);
	double	timeElapsed = ((double)(tfe.tv_nsec - tfs.tv_nsec)) / 1000000.0;
	total_msec = (tfe.tv_sec - tfs.tv_sec) * 1000.0 + timeElapsed;

	double	ratio = ((ctx->packets_sent - ctx->packets_received) / (double)ctx->packets_sent) * 100.0;

	printf("--- %s ping statistics ---\n", ctx->ip_str);
	printf("%d packets transmitted, %d received, %f%% packet loss, time %fms\n", ctx->packets_sent, ctx->packets_received, ratio, total_msec);
	printf("rtt min/avg/max/mdev = %f/%f/%f/%f ms\n", 67.0f, 67.0f, 67.0f, 67.0f); // TODO statistics
	return (0);
}

int	main(int UNUSED(ac), char **av)
{
	t_ctx	ctx;

	if (ctx_init(&ctx, &av) == -1)
		return (1);

	printf("PING %s (%s) %ld(%ld) bytes of data.\n", ctx.ip_str, ctx.hostname_str, ICMP_PAYLOAD_LENGTH, sizeof(t_pckt)); // TODO fix size

	int	ret = ft_ping(&ctx);

	ctx_delete(&ctx);
	return (ret);
}
