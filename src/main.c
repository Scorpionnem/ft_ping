/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 12:34:33 by mbatty            #+#    #+#             */
/*   Updated: 2026/02/21 11:10:18 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

#include <stdio.h>

typedef struct	s_ctx
{
	t_opt	help;
	t_opt	verbose;
	t_opt	test;
}	t_ctx;

void	print_help()
{
	printf("usage: ./ft_ping [-h, -?]\n\n");
	printf("options:\n");
	printf("-? -h --help\tshow help message and exit\n");
}

int	main(int ac, char **av)
{	(void)ac;
	t_ctx	ctx;

	t_opt_ctx	opt_ctx;
	ctx.help = opt_new(OPT_BOOL);
	ctx.verbose = opt_new(OPT_BOOL);
	ctx.test = opt_new(OPT_STR);

	opt_ctx_init(&opt_ctx);

	opt_ctx_add_opt(&opt_ctx, "-h", &ctx.help);
	opt_ctx_add_opt(&opt_ctx, "-?", &ctx.help);
	opt_ctx_add_opt(&opt_ctx, "--help", &ctx.help);

	opt_ctx_add_opt(&opt_ctx, "-v", &ctx.verbose);
	opt_ctx_add_opt(&opt_ctx, "--verbose", &ctx.verbose);

	opt_ctx_add_opt(&opt_ctx, "--test", &ctx.test);

	if (opt_ctx_parse(&opt_ctx, &av) == -1)
	{
		opt_ctx_delete(&opt_ctx);
		return (1);
	}

	if (ctx.help._bool)
	{
		print_help();
		opt_ctx_delete(&opt_ctx);
		return (0);
	}

	while (*av)
	{
		printf("REMAINING %s\n", *av);
		av++;
	}

	opt_ctx_delete(&opt_ctx);
	return (0);
}
