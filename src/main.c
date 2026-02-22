/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 12:34:33 by mbatty            #+#    #+#             */
/*   Updated: 2026/02/22 21:47:15 by mbatty           ###   ########.fr       */
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
#include <time.h>

static bool	g_running = true;

static void	handle_sigint(int UNUSED(sig))
{
	g_running = false;
}

static int	ft_ping(t_ctx *ctx)
{
	signal(SIGINT, handle_sigint);

	pid_t	pid = getpid();
	int		seq = 0;

	int		packets_sent = 0;
	int		packets_received = 0;

	int ip_header_length = 20;
	int raw_icmp_response_length = ip_header_length + sizeof(t_pckt);

	double	rtt_msec = 0, total_msec = 0;
	struct timespec	time_start, time_end, tfs, tfe;

	clock_gettime(CLOCK_MONOTONIC, &tfs);

	t_pckt	pckt;

	while (g_running)
	{
		pckt_init(&pckt, pid, ++seq);

		clock_gettime(CLOCK_MONOTONIC, &time_start);
		if (sendto(ctx->sock_fd, &pckt, sizeof(t_pckt), 0, (struct sockaddr *)&ctx->addr, sizeof(ctx->addr)) == -1)
			perror("ft_ping: sendto");
		packets_sent++;

		char	buffer[1024];
		int		data_received = recvfrom(ctx->sock_fd, buffer, sizeof(buffer), 0, NULL, NULL); // TODO check if recv from right host
		if (data_received == -1)
			perror("ft_ping: recvfrom");

		clock_gettime(CLOCK_MONOTONIC, &time_end);
		double	timeElapsed = ((double)(time_end.tv_nsec - time_start.tv_nsec)) / 1000000.0;
		rtt_msec = (time_end.tv_sec - time_start.tv_sec) * 1000.0 + timeElapsed;

		if (data_received >= (int)(sizeof(struct iphdr) + sizeof(struct icmphdr)))
		{
			struct iphdr	*ip = (struct iphdr *)buffer;
			int				ip_header_length = ip->ihl * 4;
			int				ttl = ip->ttl;

			t_pckt *pckt_recv = (t_pckt *)(buffer + ip_header_length);

			if (pckt_check(&pckt, pckt_recv, pid) == 0)
			{
				packets_received++;
				printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%f ms\n", data_received, ctx->hostname_str, pckt_recv->hdr.un.echo.sequence, ttl, rtt_msec); // TODO add time
			}
			else
				printf("Wrong packet received (%d, %d)\n", pckt_recv->hdr.code, pckt_recv->hdr.type);
		}
		else
			printf("Failed to receive packet, received %d/%d bytes\n", data_received, raw_icmp_response_length);

		sleep(1);
	}
	clock_gettime(CLOCK_MONOTONIC, &tfe);
	double	timeElapsed = ((double)(tfe.tv_nsec - tfs.tv_nsec)) / 1000000.0;
	total_msec = (tfe.tv_sec - tfs.tv_sec) * 1000.0 + timeElapsed;

	double	ratio = ((packets_sent - packets_received) / (double)packets_sent) * 100.0;

	printf("--- %s ping statistics ---\n", ctx->ip_str); // TODO statistics
	printf("%d packets transmitted, %d received, %f%% packet loss, time %fms\n", packets_sent, packets_received, ratio, total_msec);
	printf("rtt min/avg/max/mdev = %f/%f/%f/%f ms\n", 67.0f, 67.0f, 67.0f, 67.0f);
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
