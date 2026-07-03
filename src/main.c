/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 12:34:33 by mbatty            #+#    #+#             */
/*   Updated: 2026/02/23 13:22:00 by mbatty           ###   ########.fr       */
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
#include <math.h>
#include <errno.h>

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
	while (g_running)
	{
		int	data_received = recvfrom(ctx->sock_fd, ctx->buffer, sizeof(ctx->buffer), 0, NULL, NULL);
		if (data_received == -1)
		{
			if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR)
				perror("ft_ping: recvfrom");
			return (-1);
		}

		clock_gettime(CLOCK_MONOTONIC, &ctx->time_end);
		double	elapsed_time = ((double)(ctx->time_end.tv_nsec - ctx->time_start.tv_nsec)) / 1000000.0;
		double	rtt_msec = (ctx->time_end.tv_sec - ctx->time_start.tv_sec) * 1000.0 + elapsed_time;

		if (data_received < (int)(sizeof(struct iphdr) + sizeof(struct icmphdr)))
			continue ;

		struct iphdr	*ip = (struct iphdr *)ctx->buffer;
		int				ip_header_length = ip->ihl * 4;

		t_pckt	*pckt_recv = (t_pckt *)(ctx->buffer + ip_header_length);

		if (pckt_check(&ctx->pckt, pckt_recv, ctx->pid) == 0)
		{
			ctx->packets_received++;
			if (!ctx->quiet._bool && !ctx->flood._bool)
				printf("%d bytes from %s: icmp_seq=%u ttl=%d time=%.3f ms\n", data_received - ip_header_length, ctx->ip_str, ntohs(pckt_recv->hdr.un.echo.sequence), ip->ttl, rtt_msec);
			if (ctx->audible._bool)
				printf("\a");
			if (rtt_msec < ctx->min_time)
				ctx->min_time = rtt_msec;
			if (rtt_msec > ctx->max_time)
				ctx->max_time = rtt_msec;
			double	*times = realloc(ctx->times, (ctx->times_count + 1) * sizeof(double));
			if (times)
			{
				ctx->times = times;
				ctx->times[ctx->times_count++] = rtt_msec;
			}
			return (0);
		}
		if (pckt_is_error(pckt_recv, data_received - ip_header_length, ctx->pid))
			return (dprintf(2, "ft_ping: invalid icmp packet"), -1);
	}
	return (-1);
}

static double	times_avg(t_ctx *ctx)
{
	double	accu = 0;

	for (int i = 0; i < ctx->times_count; i++)
		accu += ctx->times[i];
	return (ctx->times_count == 0 ? 0 : accu / (double)ctx->times_count);
}

static double	times_mdev(t_ctx *ctx)
{
	double	avg = times_avg(ctx);
	double	accu = 0;

	for (int i = 0; i < ctx->times_count; i++)
		accu += pow(ctx->times[i] - avg, 2);
	accu = ctx->times_count == 0 ? 0 : accu / (double)ctx->times_count;
	return (sqrt(accu));
}

static int	ft_ping(t_ctx *ctx)
{
	signal(SIGINT, handle_sigint);

	struct timeval	tv_out;
	tv_out.tv_sec = ctx->timeout._int;
	tv_out.tv_usec = 0;

	setsockopt(ctx->sock_fd, SOL_IP, IP_TTL, &ctx->ttl._int, sizeof(ctx->ttl._int));
	setsockopt(ctx->sock_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof tv_out);

	while (g_running)
	{
		pckt_init(&ctx->pckt, ctx->pid, ctx->seq++);

		send_packet(ctx);

		recv_packet(ctx);

		if (ctx->count._int != -1 && ctx->packets_sent >= ctx->count._int)
			break ;

		if (!ctx->flood._bool && g_running)
			sleep(1);
	}

	printf("--- %s ping statistics ---\n", ctx->hostname_str);
	printf("%d packets transmitted, %d packets received, ", ctx->packets_sent, ctx->packets_received);
	if (ctx->packets_sent)
		printf("%d%% packet loss", ((ctx->packets_sent - ctx->packets_received) * 100) / ctx->packets_sent);
	printf("\n");
	if (ctx->packets_received)
		printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n", ctx->min_time, times_avg(ctx), ctx->max_time, times_mdev(ctx));
	return (ctx->packets_received == 0);
}

int	main(int UNUSED(ac), char **av)
{
	t_ctx	ctx;

	if (ctx_init(&ctx, &av) == -1)
		return (1);

	printf("PING %s (%s): %zu data bytes", ctx.hostname_str, ctx.ip_str, ICMP_PAYLOAD_LENGTH);
	if (ctx.verbose._bool)
		printf(", id 0x%04x = %u", ctx.pid, ctx.pid);
	printf("\n");

	int	ret = ft_ping(&ctx);

	ctx_delete(&ctx);
	return (ret);
}
