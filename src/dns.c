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
		dprintf(2, "ft_ping: %s: Name or service not known\n", host);
		return (NULL);
	}
	(*addr).sin_family = hostent->h_addrtype;
	(*addr).sin_port = htons(0);
	(*addr).sin_addr.s_addr = *(long *)hostent->h_addr;
	return (strdup(inet_ntoa(*(struct in_addr *)hostent->h_addr)));
}

char	*dns_reverse_lookup(const char *addr)
{
	struct sockaddr_in	temp_addr;
	char				buf[NI_MAXHOST];

	if (!addr)
		return (NULL);
	temp_addr.sin_family = AF_INET;
	temp_addr.sin_addr.s_addr = inet_addr(addr);
	if (getnameinfo((struct sockaddr *)&temp_addr, sizeof(struct sockaddr_in), buf, sizeof(buf), NULL, 0, NI_NAMEREQD))
		return (NULL);
	return (strdup(buf));
}
