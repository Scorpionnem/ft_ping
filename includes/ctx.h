/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 20:13:42 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/03 09:53:19 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CTX_H
# define CTX_H

# include <libftgetopt/includes/libftgetopt.h>
# include <string.h>
# include <stdio.h>

/*
	open socket
	create packet
	send packet

	-v			verbose
	-? && -h	help
*/

#define BOOLEAN_FLAGS "?hv" 
#define ARG_FLAGS ""

typedef struct s_ctx
{
	t_opts	*opts;
	char	*host;
}	t_ctx;

int	ctx_init(t_ctx *ctx, char **av);
int	ctx_free(t_ctx *ctx);

#endif
