/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 12:47:54 by mbatty            #+#    #+#             */
/*   Updated: 2026/02/20 13:58:31 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "opt.h"
#include <stdlib.h>
#include <string.h>

t_opt	opt_new(t_opt_type type)
{
	t_opt	f =
	{
		.type = type
	};
	return (f);
}

int	opt_ctx_add_opt(t_opt_ctx *ctx, char *id, t_opt *opt)
{
	ctx->options = realloc(ctx->options, (ctx->options_count + 1) * sizeof(t_opt_pair));
	if (!ctx->options)
		return (-1);
	ctx->options[ctx->options_count].id = id;
	ctx->options[ctx->options_count].opt = opt;
	ctx->options_count++;
	return (0);
}

int	opt_ctx_init(t_opt_ctx *ctx)
{
	memset(ctx, 0, sizeof(t_opt_ctx));
	return (0);
}

int	opt_ctx_delete(t_opt_ctx *ctx)
{
	free(ctx->options);
	return (0);
}

static int	has_opt(t_opt_ctx *ctx, char *id)
{
	int	i = 0;
	while (i < ctx->options_count)
	{
		if (!strcmp(ctx->options[i].id, id))
			return (i);
		i++;
	}
	return (-1);
}

#include <stdio.h>

int	opt_ctx_parse(t_opt_ctx *ctx, char ***av)
{
	(void)ctx;
	(*av)++;
	int	i = -1;
	int	dump = 0;
	while ((*av)[++i])
	{
		char	*arg = (*av)[i];
		int		find = has_opt(ctx, arg);
		if (find != -1)
		{
			if (!(*av)[i + 1])
				return (printf("ERROR NO ARG\n"), -1);
			printf("%s ", ctx->options[find].id);
			printf("%s\n", (*av)[i + 1]);
			i++;
		}
		else
			(*av)[dump++] = (*av)[i];
	}
	(*av)[dump] = NULL;
	return (1);
}
