/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 12:34:33 by mbatty            #+#    #+#             */
/*   Updated: 2026/02/20 15:07:30 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

#include <stdio.h>

typedef struct	s_ctx
{
	t_opt	help;
	t_opt	verbose;
}	t_ctx;

int	main(int ac, char **av)
{	(void)ac;
	t_ctx	ctx;

	t_opt_ctx	opt_ctx;
	ctx.help = opt_new(OPT_BOOL);
	ctx.verbose = opt_new(OPT_BOOL);

	opt_ctx_init(&opt_ctx);

	opt_ctx_add_opt(&opt_ctx, "-h", &ctx.help);
	opt_ctx_add_opt(&opt_ctx, "-?", &ctx.help);
	opt_ctx_add_opt(&opt_ctx, "--help", &ctx.help);

	opt_ctx_add_opt(&opt_ctx, "-v", &ctx.verbose);
	opt_ctx_add_opt(&opt_ctx, "--verbose", &ctx.verbose);

	opt_ctx_parse(&opt_ctx, &av);

	while (*av)
	{
		printf("REMAINING %s\n", *av);
		av++;
	}

	opt_ctx_delete(&opt_ctx);
}
