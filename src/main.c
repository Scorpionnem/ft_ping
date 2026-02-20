/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 12:34:33 by mbatty            #+#    #+#             */
/*   Updated: 2026/02/20 13:58:46 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

#include <stdio.h>

int	main(int ac, char **av)
{
	(void)ac;(void)av;
	t_opt_ctx	opt_ctx;
	t_opt	help = opt_new(OPT_BOOL);
	
	opt_ctx_init(&opt_ctx);
	opt_ctx_add_opt(&opt_ctx, "-h", &help);
	opt_ctx_add_opt(&opt_ctx, "--help", &help);

	opt_ctx_parse(&opt_ctx, &av);
	while (*av)
	{
		printf("REMAINING %s\n", *av);
		av++;
	}

	opt_ctx_delete(&opt_ctx);
}
