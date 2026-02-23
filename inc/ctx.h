/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 12:34:07 by mbatty            #+#    #+#             */
/*   Updated: 2026/02/23 11:12:40 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define UNUSED(x)	_unused_##x __attribute__((unused))

#include "opt.h"
#include "dns.h"
#include "pckt.h"

#include <netinet/ip_icmp.h>
#include <time.h>

typedef struct	s_ctx
{
	t_opt_ctx			opt_ctx;

	t_opt				help;
	t_opt				verbose;
	t_opt				count;
	t_opt				flood;
	t_opt				quiet;
	t_opt				ttl;
	t_opt				timeout;
	t_opt				audible;

	struct sockaddr_in	addr;
	int					sock_fd;

	struct timespec		time_start;
	struct timespec		time_end;

	char				buffer[1024];

	t_pckt				pckt;
	int					packets_sent;
	int					packets_received;
	int					seq;

	char				*ip_str;
	char				*hostname_str;

	pid_t				pid;
}	t_ctx;

int		ctx_init(t_ctx *ctx, char ***av);
void	ctx_delete(t_ctx *ctx);
