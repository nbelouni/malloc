/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbelouni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/02 14:25:03 by nbelouni          #+#    #+#             */
/*   Updated: 2019/11/07 13:30:10 by nbelouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>

void	free(void *ptr)
{
	(void)ptr;
}

#define ARRAY_SIZE 500
int		main()
{
	char *s[ARRAY_SIZE];
	int i = -1;

	while (++i < ARRAY_SIZE)
	{
		s[i] = ft_strdup("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
	ft_putendl("la");
	}
//	show_alloc_mem();
	i = -1;

	while (++i < ARRAY_SIZE)
	{
		ft_putendl(s[i]);
	}
	return (0);
}
