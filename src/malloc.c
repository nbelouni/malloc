/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbelouni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/02 21:05:52 by nbelouni          #+#    #+#             */
/*   Updated: 2019/11/07 16:28:31 by nbelouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <stdio.h>

void	init_pages(size_t type)
{
	N("init_pages()");
	t_memory_array	*tmp;
	t_memory_array	*tmp2;

	if (g_allowed.tiny == NULL)
		N("tiny null");
	if (g_allowed.small == NULL)
		N("small null");
	if (type == TINY)
	{
		tmp = g_allowed.tiny;
	}
	else if (type == SMALL)
	{
		tmp = g_allowed.small;
	}
	else
		return;

	if (INIT_MAX_PAGES > 1)
	{
		tmp2 = tmp;
		tmp2 += GETPAGESIZE / sizeof(t_memory_array);
	}
	else
		tmp2 = NULL;
	tmp->size = 0;
	ft_putstr("tmp->size : ");ft_putnbr((int)tmp->size);N("");

	int i = 0;
	while (i < INIT_MAX_PAGES)
	{
			ft_putstr("tmp : ");ft_putnbr((int)tmp);N("");
			ft_putstr("tmp2 : ");ft_putnbr((int)tmp2);N("");
			ft_putstr("space between tmp - tmp2 : ");ft_putnbr((int)tmp2 - (int)tmp);N("");
		tmp->size = 0;
		tmp->next = tmp2;
		if (i < INIT_MAX_PAGES - 1)
		{
			tmp = tmp2;
			tmp2 += GETPAGESIZE / sizeof(t_memory_array);
		}
		i++;
	}

	tmp->next = NULL;
}

t_bool	first_init()
{
	static int	start;

	if (start)
		return (TRUE);
	else
	{
		g_allowed.large = NULL;
		g_allowed.small = mmap(0, GETPAGESIZE * INIT_MAX_PAGES, PROT_READ | PROT_WRITE,
		MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
		ft_bzero(g_allowed.small, GETPAGESIZE * INIT_MAX_PAGES);

		init_pages(SMALL);

		g_allowed.tiny = mmap(0, GETPAGESIZE * INIT_MAX_PAGES, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
		ft_bzero(g_allowed.small, GETPAGESIZE * INIT_MAX_PAGES);
		init_pages(TINY);
		start = 1;
	}
	return (FALSE);
}

void		alloc_array(size_t size, size_t type)
{
	void	*tmp;

	if (type == TINY)
		tmp = g_allowed.tiny;

	while (tmp->size == GETPAGESIZE)
		tmp = tmp->next;

}

void		*malloc(size_t size)
{
	ft_putendl("\n\n");
	ft_putnbr((int)size);
	ft_putendl(" , MALLOC");
	first_init();

	/*
	 * LARGE
	 */
	if (size > SMALL_ALLOC)
	{
		ft_putendl("LARGE");
	}

	/*
	 * SMALL
	 */
	else if (size > TINY_ALLOC)
		ft_putendl("SMALL");

	/*
	 * TINY
	 */
	else
	{
//		alloc_array();
		ft_putendl("TINY");
	}


	return (NULL);
}
