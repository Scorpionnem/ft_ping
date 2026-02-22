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

#define ICMP_PAYLOAD_LENGTH (64 - sizeof(struct icmphdr))
typedef struct	s_pckt
{
	struct icmphdr	hdr;
	char			payload[ICMP_PAYLOAD_LENGTH];
}	t_pckt;

void	pckt_init(t_pckt *pckt, int pid, int seq);
int		pckt_check(t_pckt *pckt_sent, t_pckt *pckt_recv, int pid);

unsigned int	checksum(void *b, int len);
