/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 20:14:04 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/03 09:58:38 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

int	main(int ac, char **av)
{	
	t_ctx	ctx;

	ac = 42;
	if (getuid() != 0)
	{
		printf("ft_ping: you are not root!\n");
		return (1);
	}
	if (!ctx_init(&ctx, av))
		return (1);
	printf("%s\n", ctx.host);
	return (ctx_free(&ctx));
}
