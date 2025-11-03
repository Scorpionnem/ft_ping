/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 20:13:42 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/03 15:36:22 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CTX_H
# define CTX_H

# include "../libftgetopt/includes/libftgetopt.h"
# include <string.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
# include <stdlib.h>
# include <netinet/ip_icmp.h>
# include <time.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/time.h>

/*
	-v			verbose
	-? && -h	help
*/
#define BOOLEAN_FLAGS "?hv" 


#define ARG_FLAGS ""

#define PING_PKT_S 64 // packet size
#define PORT_NO 0 // port number
#define PING_SLEEP_RATE 1000000 // ping sleep rate (in microseconds)
#define RECV_TIMEOUT 1 // timeout for receiving packets (in seconds)

typedef struct s_packet
{
	struct icmphdr	hdr;
	char			msg[PING_PKT_S - sizeof(struct icmphdr)];
}	t_packet;

typedef struct s_ctx
{
	char				*ip;
	char				*host;
	char				*rev_host;

	int					sent_packets_count;
	int					received_packets_count;
	bool				sent_packet;
	int					ttl;
	unsigned long int	interval;

	t_opts				*opts;
	struct sockaddr_in	addr;
	int					sock_fd;
}	t_ctx;

int	ctx_init(t_ctx *ctx, char **av);
int	ctx_free(t_ctx *ctx);

unsigned int	checksum(void *b, int len);

char	*dns_lookup(const char *host, struct sockaddr_in *addr);
char	*dns_reverse_lookup(const char *addr);

#endif
