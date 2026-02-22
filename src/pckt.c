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

void	pckt_init(t_pckt *pckt, int pid, int seq)
{
	memset(pckt, 0, sizeof(t_pckt));
	pckt->hdr.type = ICMP_ECHO;
	pckt->hdr.un.echo.id = pid;
	pckt->hdr.un.echo.sequence = seq;
	memset(pckt->payload, 42, sizeof(pckt->payload));
	pckt->hdr.checksum = checksum(pckt, sizeof(t_pckt));
}

int	pckt_check(t_pckt *pckt_sent, t_pckt *pckt_recv, int pid)
{
	if (pckt_recv->hdr.type != ICMP_ECHOREPLY)
		return (-1);
	if (pckt_recv->hdr.code != 0)
		return (-1);
	if (pckt_recv->hdr.un.echo.id != pid)
		return (-1);
	if (memcmp(&pckt_sent->payload[0], &pckt_recv->payload[0], ICMP_PAYLOAD_LENGTH) != 0)
		return (-1);
	return 0;
}
