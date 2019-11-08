/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbelouni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/02 21:05:52 by nbelouni          #+#    #+#             */
/*   Updated: 2019/11/08 15:10:25 by nbelouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <stdio.h>

void	init_pages(size_t type)
{
	N("init_pages()");

	t_memory_chunk	*tmp;
	void			*tmp2;
	size_t			*pages;

	if (type == TINY)
	{
		tmp = g_allowed.tiny;
		pages = &g_allowed.tiny_pages;
	}
	else if (type == SMALL)
	{
		tmp = g_allowed.small;
		pages = &g_allowed.small_pages;
	}
	else
		return;

	N("________1");	
	while (tmp && tmp->next)
	{
		tmp = tmp->next;
	}
	
//	N("________2");	
	tmp2 = mmap(0, GETPAGESIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
//	N("________3");	
	((t_memory_chunk *)tmp2)->size = GETPAGESIZE - (sizeof(t_memory_chunk *) +  sizeof(t_memory_chunk));
//	N("________4");	
	((t_memory_chunk *)tmp2)->state = NOT_ALLOWED;
//	N("________5");	
	((t_memory_chunk *)tmp2)->next = NULL;
//	N("________6");	
	if (*pages == 0)
	{
		if (type == TINY)
		{
			g_allowed.tiny = tmp2;
		}
		else if (type == SMALL)
		{
			g_allowed.small = tmp2;
		}
	}	
	else
	{
//	N("________8");	
		tmp->next = tmp2;
	}
//	N("________9");	
	*pages += 1;
}

t_bool	first_init()
{
	static int	start;

	if (start)
		return (TRUE);
	else
	{
		g_allowed.large = NULL;
		g_allowed.large_pages = 0;

		g_allowed.small = NULL;
		g_allowed.small_pages = 0;
		init_pages(SMALL);

		g_allowed.tiny = NULL;
		g_allowed.tiny_pages = 0;
		init_pages(TINY);
		start = 1;
	}
	return (FALSE);
}

void		*alloc_chunk(size_t size, size_t type)
{
	N("alloc_chunk()");	
	void			*tmp;
	size_t			pages;
	t_memory_chunk	*tmp2;

	static size_t			x = 0;

	V("total allocated : ", x);
	(void)type;
//	if (type == TINY)
		tmp = g_allowed.tiny;
		pages = g_allowed.tiny_pages;
//	else
//		tmp = NULL;
//	N("_____________1");
	V("pages : ", pages);	
	V("total  size : ", GETPAGESIZE * pages);	
	while (((t_memory_chunk *)tmp)->next)
	{
	//	ft_putstr("tmp->size : ");ft_putnbr(((t_memory_chunk *)tmp)->size);N("");
//		N("_____________1.2");
		if (((t_memory_chunk *)tmp)->state == FREED && size <= ((t_memory_chunk *)tmp)->size)
		{
			N("_____________2");
			((t_memory_chunk *)tmp)->state = ALLOWED;
	x += size + sizeof(t_memory_chunk);
			return (tmp + sizeof(t_memory_chunk));
		}
		else if (((t_memory_chunk *)tmp)->state == NOT_ALLOWED)
		{
			ft_putstr("tmp->size : ");ft_putnbr(((t_memory_chunk *)tmp)->size);N("");
			if (((t_memory_chunk *)tmp)->size < size)
			{
				init_pages(type);
				tmp = ((t_memory_chunk *)tmp)->next;
			}
			N("_____________3");

			tmp2 = tmp + sizeof(t_memory_chunk) + size;
			tmp2->size = ((t_memory_chunk *)tmp)->size - (size + sizeof(t_memory_chunk));
			tmp2->state = NOT_ALLOWED;
			tmp2->next = NULL;

			((t_memory_chunk *)tmp)->size = size;
			((t_memory_chunk *)tmp)->state = ALLOWED;
			((t_memory_chunk *)tmp)->next = tmp2;

	x += size + sizeof(t_memory_chunk);
			return (tmp + sizeof(t_memory_chunk));
		}
		tmp = ((t_memory_chunk *)tmp)->next;
	}
	N("_____________4");

	ft_putstr("tmp->size before allocate chunk : ");ft_putnbr(((t_memory_chunk *)tmp)->size);N("");
	if (((t_memory_chunk *)tmp)->size < size)
	{
		init_pages(type);
		tmp = ((t_memory_chunk *)tmp)->next;
	}
	tmp2 = tmp + sizeof(t_memory_chunk) + size;
	tmp2->size = ((t_memory_chunk *)tmp)->size - (size + sizeof(t_memory_chunk));
	tmp2->state = NOT_ALLOWED;
	tmp2->next = NULL;

	((t_memory_chunk *)tmp)->size = size;
	((t_memory_chunk *)tmp)->state = ALLOWED;
	((t_memory_chunk *)tmp)->next = tmp2;

	ft_putstr("tmp->size : ");ft_putnbr(((t_memory_chunk *)tmp)->size);N("");
	ft_putstr("tmp2->size : ");ft_putnbr(((t_memory_chunk *)tmp2)->size);N("");
	x += size + sizeof(t_memory_chunk);
	return (tmp + sizeof(t_memory_chunk));

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
		return alloc_chunk(size, LARGE);
	}

	/*
	 * SMALL
	 */
	else if (size > TINY_ALLOC)
	{
		ft_putendl("SMALL");
		return alloc_chunk(size, SMALL);
	}
	/*
	 * TINY
	 */
	else
	{
		ft_putendl("TINY");
		return alloc_chunk(size, TINY);
	}


	return (NULL);
}
