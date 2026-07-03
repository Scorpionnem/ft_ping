/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pckt.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 13:02:26 by mbatty            #+#    #+#             */
/*   Updated: 2026/02/22 13:03:01 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pckt.h"
#include <string.h>
#include <arpa/inet.h>
#include <netinet/ip.h>

void	pckt_init(t_pckt *pckt, int pid, int seq)
{
	memset(pckt, 0, sizeof(t_pckt));
	pckt->hdr.type = ICMP_ECHO;
	pckt->hdr.un.echo.id = htons(pid);
	pckt->hdr.un.echo.sequence = htons(seq);
	memset(pckt->payload, 42, sizeof(pckt->payload));
	pckt->hdr.checksum = checksum(pckt, sizeof(t_pckt));
}

int	pckt_check(t_pckt *pckt_sent, t_pckt *pckt_recv, int pid)
{
	if (pckt_recv->hdr.type != ICMP_ECHOREPLY)
		return (-1);
	if (pckt_recv->hdr.code != 0)
		return (-1);
	if (pckt_recv->hdr.un.echo.id != htons(pid))
		return (-1);
	if (memcmp(&pckt_sent->payload[0], &pckt_recv->payload[0], ICMP_PAYLOAD_LENGTH) != 0)
		return (-1);
	return 0;
}

int	pckt_is_error(t_pckt *pckt, int len, int pid)
{
	if (pckt->hdr.type != ICMP_DEST_UNREACH && pckt->hdr.type != ICMP_SOURCE_QUENCH
		&& pckt->hdr.type != ICMP_REDIRECT && pckt->hdr.type != ICMP_TIME_EXCEEDED
		&& pckt->hdr.type != ICMP_PARAMETERPROB)
		return (0);
	if (len < (int)(sizeof(struct icmphdr) + sizeof(struct iphdr)))
		return (0);

	struct iphdr	*orig_ip = (struct iphdr *)((char *)pckt + sizeof(struct icmphdr));
	if (len < (int)(sizeof(struct icmphdr) + orig_ip->ihl * 4 + sizeof(struct icmphdr)))
		return (0);
	struct icmphdr	*orig_icmp = (struct icmphdr *)((char *)orig_ip + orig_ip->ihl * 4);

	if (orig_ip->protocol != IPPROTO_ICMP || orig_icmp->type != ICMP_ECHO)
		return (0);
	if (orig_icmp->un.echo.id != htons(pid))
		return (0);
	return (1);
}
