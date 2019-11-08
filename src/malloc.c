/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbelouni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/02 21:05:52 by nbelouni          #+#    #+#             */
/*   Updated: 2019/11/08 16:40:35 by nbelouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	init_pages(size_t type)
{
//"init_pages()");

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

//"________1");	
	while (tmp && tmp->next)
	{
		tmp = tmp->next;
	}
	
//"________2");	
	tmp2 = mmap(0, GETPAGESIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
//"________3");	
	((t_memory_chunk *)tmp2)->size = GETPAGESIZE - (sizeof(t_memory_chunk *) +  sizeof(t_memory_chunk));
//"________4");	
	((t_memory_chunk *)tmp2)->state = NOT_ALLOWED;
//"________5");	
	((t_memory_chunk *)tmp2)->next = NULL;
//"________6");	
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
//"________8");	
		tmp->next = tmp2;
	}
//"________9");	
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

void		*add_large_chunk(void *last_chunk, int size)
{
	void	*new_chunk;
	size_t	chunk_size;

	chunk_size = GETPAGESIZE * (size / GETPAGESIZE);
	if (size < GETPAGESIZE || size % GETPAGESIZE > 0)
		chunk_size += 1;
	new_chunk = mmap(0, chunk_size, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);

	((t_memory_chunk *)new_chunk)->state = ALLOWED;
	((t_memory_chunk *)new_chunk)->size = chunk_size;
	((t_memory_chunk *)new_chunk)->next = NULL;
	if (last_chunk)
		((t_memory_chunk *)last_chunk)->next = new_chunk;
	else
		g_allowed.large = new_chunk;
	g_allowed.large_pages += 1;
	return (new_chunk);
}

void		*alloc_chunk(size_t size, size_t type)
{
//"alloc_chunk()");	
	void			*tmp;
	size_t			pages;
	t_memory_chunk	*tmp2;


	if (type == TINY)
	{
		tmp = g_allowed.tiny;
		pages = g_allowed.tiny_pages;
	}
	else if (type == SMALL)
	{
		tmp = g_allowed.small;
		pages = g_allowed.small_pages;
	}
	else /* if (type == LARGE) */
	{
		tmp = g_allowed.large;
		pages = g_allowed.large_pages;
	}
//"_____________1");
//	V("pages : ", pages);	
//	V("total  size : ", GETPAGESIZE * pages);	
	while (tmp && ((t_memory_chunk *)tmp)->next)
	{
		if (((t_memory_chunk *)tmp)->state == FREED && size <= ((t_memory_chunk *)tmp)->size)
		{
//"_____________2");
			((t_memory_chunk *)tmp)->state = ALLOWED;
			return (tmp + sizeof(t_memory_chunk));
		}
		else if (type != LARGE && ((t_memory_chunk *)tmp)->state == NOT_ALLOWED)
		{
//"");
			if (((t_memory_chunk *)tmp)->size < size)
			{
				init_pages(type);
				tmp = ((t_memory_chunk *)tmp)->next;
			}
//"_____________3");

			tmp2 = tmp + sizeof(t_memory_chunk) + size;
			tmp2->size = ((t_memory_chunk *)tmp)->size - (size + sizeof(t_memory_chunk));
			tmp2->state = NOT_ALLOWED;
			tmp2->next = NULL;

			((t_memory_chunk *)tmp)->size = size;
			((t_memory_chunk *)tmp)->state = ALLOWED;
			((t_memory_chunk *)tmp)->next = tmp2;

			return (tmp + sizeof(t_memory_chunk));
		}
		tmp = ((t_memory_chunk *)tmp)->next;
	}
	if (type == LARGE)
	{
		return add_large_chunk(tmp, size);
	}
//"_____________4");

//"");
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

//"");
//"");
	return (tmp + sizeof(t_memory_chunk));

}

void		*malloc(size_t size)
{
	ft_putendl("\n");
	ft_putnbr((int)size);
	ft_putendl(" , MALLOC");
	first_init();

	 void	*tmp;	
	/*
	 * LARGE
	 */
	if (size >= SMALL_ALLOC)
	{
		ft_putendl("LARGE");
		tmp = alloc_chunk(size, LARGE);
		V("new pointer : ", (int)tmp);
		return tmp;
	}

	/*
	 * SMALL
	 */
	else if (size >= TINY_ALLOC)
	{
		ft_putendl("SMALL");
		tmp = alloc_chunk(size, SMALL);
		V("new pointer : ", (int)tmp);
		return tmp;
	}
	/*
	 * TINY
	 */
	else
	{
		ft_putendl("TINY");
		tmp = alloc_chunk(size, TINY);
		V("new pointer : ", (int)tmp);
		return tmp;
	}


	return (NULL);
}
