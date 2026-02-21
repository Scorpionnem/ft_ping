/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dns.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 17:35:51 by mbatty            #+#    #+#             */
/*   Updated: 2026/02/21 17:37:00 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>

char	*dns_reverse_lookup(const char *addr);
char	*dns_lookup(const char *host, struct sockaddr_in *addr);
