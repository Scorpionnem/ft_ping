/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 12:47:26 by mbatty            #+#    #+#             */
/*   Updated: 2026/02/21 13:03:33 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdbool.h>

typedef enum	e_opt_type
{
	OPT_BOOL,
	OPT_INT,
	OPT_FLOAT,
	OPT_STR,
}	t_opt_type;

typedef struct	s_opt
{
	t_opt_type	type;
	union
	{
		bool	_bool;
		int		_int;
		float	_float;
		char	*_str;
	};
}	t_opt;

t_opt	opt_new(t_opt_type type);

typedef struct	s_opt_pair
{
	char	*id;
	t_opt	*opt;
}	t_opt_pair;

typedef struct	s_opt_ctx
{
	t_opt_pair	*options;
	int			options_count;
}	t_opt_ctx;

/* Add option to be parsed in the opt_ctx */
int	opt_ctx_add_opt(t_opt_ctx *ctx, char *id, t_opt *opt, t_opt_type type);

/* Init all values inside ctx */
int	opt_ctx_init(t_opt_ctx *ctx);

/* Free memory used by ctx */
int	opt_ctx_delete(t_opt_ctx *ctx);

/* parse av using options set by opt_ctx_add_opt, sets all unused values in order in av */
int	opt_ctx_parse(t_opt_ctx *ctx, char ***av);
