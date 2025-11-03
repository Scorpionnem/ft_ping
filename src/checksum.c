/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checksum.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 14:15:38 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/03 14:15:44 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

unsigned int	checksum(void *b, int len)
{
	unsigned int	*buf = b;
	unsigned int	sum = 0;
	unsigned int	result;

	for (sum = 0; len > 1; len -= 2)
		sum += *buf++;
	if (len == 1)
		sum += *(unsigned char *)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return (result);
}
