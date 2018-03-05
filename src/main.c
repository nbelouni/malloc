/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbelouni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/02 14:25:03 by nbelouni          #+#    #+#             */
/*   Updated: 2017/01/02 19:47:07 by nbelouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>

void	free(void *ptr)
{
	(void)ptr;
}

int		main()
{
	char *s[2000];
	int i = -1;

	while (++i < 2000)
	{
		s[i] = malloc(10);
		ft_memset(s, '@', 10);
	}
	show_alloc_mem();
//	ft_putendl(s);
//	ft_putnbr(sizeof(t_bloc));
	return (0);
}
