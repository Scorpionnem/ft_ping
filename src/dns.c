/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dns.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 17:35:03 by mbatty            #+#    #+#             */
/*   Updated: 2026/02/21 18:12:13 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dns.h"

char	*dns_lookup(const char *host, struct sockaddr_in *addr)
{
	struct hostent	*hostent;

	if (!addr || !host)
		return (NULL);
	hostent = gethostbyname(host);
	if (!hostent)
	{
		dprintf(2, "ft_ping: unknown host\n");
		return (NULL);
	}
	(*addr).sin_family = AF_INET;
	(*addr).sin_port = htons(0);
	(*addr).sin_addr = *(struct in_addr *)hostent->h_addr;
	return (strdup(inet_ntoa(*(struct in_addr *)hostent->h_addr)));
}
