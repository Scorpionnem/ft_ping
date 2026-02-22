/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 12:47:54 by mbatty            #+#    #+#             */
/*   Updated: 2026/02/22 13:42:22 by mbatty           ###   ########.fr       */
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

int	opt_ctx_add_opt(t_opt_ctx *ctx, char *id, t_opt *opt, t_opt_type type)
{
	*opt = opt_new(type);
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

static int	find_opt(t_opt_ctx *ctx, char *id)
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
	int	i = -1;
	int	dump = 0;

	(*av)++;
	while ((*av)[++i])
	{
		char	*arg = (*av)[i];
		int		find = find_opt(ctx, arg);
		if (find != -1)
		{
			t_opt	*op = ctx->options[find].opt;
			char	*id = ctx->options[find].id;

			switch (op->type)
			{
				case OPT_BOOL:
				{
					op->_bool = true;
					break ;
				}
				case OPT_STR:
				{
					if (!(*av)[i + 1])
						return (dprintf(2, "%s requires a string argument\n", id), -1);
					op->_str = (*av)[++i];
					break ;
				}
				case OPT_INT:
				{
					if (!(*av)[i + 1])
						return (dprintf(2, "%s requires an integer argument\n", id), -1);
					op->_int = atoi((*av)[++i]);
					break ;
				}
				case OPT_FLOAT:
				{
					if (!(*av)[i + 1])
						return (dprintf(2, "%s requires a float argument\n", id), -1);
					op->_float = atof((*av)[++i]);
					break ;
				}
				default:
					return (dprintf(2, "FATAL: %d invalid opt enum\n", op->type), -1);
			}
		}
		else
			(*av)[dump++] = (*av)[i];
	}
	(*av)[dump] = NULL;
	return (0);
}
