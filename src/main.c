/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 12:34:33 by mbatty            #+#    #+#             */
/*   Updated: 2026/02/21 18:15:04 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"
#include "dns.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	print_help()
{
	printf("usage: ./ft_ping [-h, -?]\n\n");
	printf("options:\n");
	printf("-? -h --help\tshow help message and exit\n");
}

#define UNUSED(x)	_unused_##x __attribute__((unused))

int	main(int UNUSED(ac), char **av)
{
	t_ctx	ctx;

	if (ctx_init(&ctx, &av) == -1)
		return (1);

	printf("Reverse lookup: %s\n", ctx.ip_str);
	printf("Lookup: %s\n", ctx.hostname_str);

	free(ctx.ip_str);
	free(ctx.hostname_str);
	opt_ctx_delete(&ctx.opt_ctx);
	return (0);
}
