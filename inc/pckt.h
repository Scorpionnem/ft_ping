/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pckt.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 13:01:43 by mbatty            #+#    #+#             */
/*   Updated: 2026/02/22 13:03:31 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <netinet/ip_icmp.h>
#include <stdint.h>

#define ICMP_PAYLOAD_LENGTH (64 - sizeof(struct icmphdr))
typedef struct	s_pckt
{
	struct icmphdr	hdr;
	char			payload[ICMP_PAYLOAD_LENGTH];
}	t_pckt;

void	pckt_init(t_pckt *pckt, int pid, int seq);
int		pckt_check(t_pckt *pckt_sent, t_pckt *pckt_recv, int pid);

int		pckt_is_error(t_pckt *pckt, int len, int pid);

/* human readable diagnostic for an ICMP error type/code pair */
const char	*pckt_strerror(int type, int code);

uint16_t	checksum(void *data, int len);
