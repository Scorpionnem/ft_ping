/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dns.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 14:16:31 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/03 14:16:36 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

char	*dns_lookup(const char *host, struct sockaddr_in *addr)
{
	struct hostent	*hostent;

	if (!addr || !host)
		return (NULL);
	hostent = gethostbyname(host);
	if (!hostent)
	{
		printf("ft_ping: %s: Name or service not known\n", host);
		return (NULL);
	}
	(*addr).sin_family = hostent->h_addrtype;
	(*addr).sin_port = htons(PORT_NO);
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
	{
		printf("ft_ping: %s: Failed to reverse lookup\n", addr);
		return (NULL);
	}
	return (strdup(buf));
}
