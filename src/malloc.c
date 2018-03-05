/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbelouni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/02 21:05:52 by nbelouni          #+#    #+#             */
/*   Updated: 2017/01/02 19:46:08 by nbelouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <stdio.h>

t_bool	first_init()
{
	static int	start;

	if (start)
		return (TRUE);
	else
	{
		g_allowed.large = NULL;
		g_allowed.small = NULL;
		g_allowed.tiny = NULL;
		start = 1;
		ft_putendl("INIT");
	}
	return (FALSE);
}

void		*allow_large_bloc(int size)
{
	void	*tmp;
	t_bloc	*tmp2;
	int		final_size;
	int		static_size = size;

	ft_putendl("LARGE");
	final_size = 0;
	tmp = NULL;
	size += sizeof(t_bloc);
	if (GETPAGESIZE < size)
	{
		final_size = size - (size % GETPAGESIZE);
		size -= final_size;
	}
	while (GETPAGESIZE % size != 0)
	{
		size += 1;
	}
//	ft_putstr("size : ");
//	ft_putnbr(size);
//	ft_putendl("");
//	ft_putendl("___2");
	final_size += size;

//	ft_putnbr(final_size);
	tmp = mmap(0, final_size, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);

//	ft_putendl("___3");
	((t_bloc *)(tmp))->start = tmp;
	((t_bloc *)(tmp))->end = tmp + final_size;
	((t_bloc *)(tmp))->size = static_size;
	((t_bloc *)(tmp))->next = NULL;

//	ft_putendl("___4");
	if (g_allowed.large == NULL)
		g_allowed.large = tmp;
	else
	{
		tmp2 = (t_bloc *)(g_allowed.large);
		while (tmp2->next)
			tmp2 = tmp2->next;
		tmp2->next = tmp;
	}
	return (((t_bloc *)(tmp))->start + sizeof(t_bloc));
}

void		allow_small_bloc(size_t size)
{
	ft_putendl("SMALL");
	(void)size;
}

void		*allow_tiny_bloc(size_t size)
{
//	ft_putendl("TINY");
	void	*tmp;
	void	*tmp2;
	t_bloc	*new_bloc;

	tmp = NULL;
	ft_putendl("_________1.0");
	if (g_allowed.tiny == NULL)
	{
		tmp = mmap(0, GETPAGESIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);

		((t_page *)(tmp))->allowed = size + sizeof(t_page) + sizeof(t_bloc);
		((t_page *)(tmp))->next = NULL;
		new_bloc = (t_bloc *)(tmp + sizeof(t_page));
		new_bloc->start = new_bloc;
		new_bloc->end = new_bloc->start + size + sizeof(t_bloc);;
		new_bloc->size = size;
		new_bloc->next = NULL;
		g_allowed.tiny = tmp;

		show_alloc_mem();
		return (new_bloc + sizeof(t_bloc));
	}
	ft_putendl("_________2.0");
	tmp2 = g_allowed.tiny;
	new_bloc = NULL;
	ft_putstr("page->allowed : ");
	ft_putnbr(((t_page *)(tmp2))->allowed);
	write(1, "\n", 1);
	while (((t_page *)(tmp2)))
	{
		ft_putendl("_________2.1");
		if ((size_t)(GETPAGESIZE - ((t_page *)(tmp2))->allowed) > sizeof(t_bloc) + size)
		{
		ft_putendl("_________2.2");
			void *tmp3;

			tmp3 = tmp2 + sizeof(t_page);
			while (((t_bloc *)(tmp3))->next)
			{
//				print_bloc(tmp3, "tmp3", 0);
				tmp3 = ((t_bloc *)(tmp3))->next;
			}
//			print_bloc(tmp3, "tmp3", 0);
//			ft_putendl("##################");

			new_bloc = (t_bloc *)(tmp2 + ((t_page *)(tmp2))->allowed);
			((t_bloc *)(tmp3))->next = new_bloc;
			new_bloc->start = new_bloc;
			new_bloc->end = new_bloc + sizeof(t_bloc) + size;
			new_bloc->size = size;
			new_bloc->next = NULL;
//			print_bloc(((t_bloc *)(tmp3))->next, "tmp3->next", 0);
			((t_page *)(tmp2))->allowed += size + sizeof(t_bloc);
		ft_putendl("_________2.3");
			return (new_bloc + sizeof(t_bloc));
		}
		ft_putendl("_________2.4");
		if (!((t_page *)(tmp2))->next)
			break;
		tmp2 = ((t_page *)(tmp2))->next;
	}

	if (((t_page *)(tmp2))->next == NULL)
	{
	ft_putendl("_________2.5");
		tmp = mmap(0, GETPAGESIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
		((t_page *)(tmp))->allowed = size;
		((t_page *)(tmp))->next = NULL;
		((t_page *)(tmp2))->next = tmp;
		new_bloc = (t_bloc *)(tmp + sizeof(t_page));
	}
	ft_putendl("_________3");
	new_bloc->start = new_bloc;
	new_bloc->end = new_bloc + sizeof(t_bloc) + size;
	new_bloc->size = size;
	new_bloc->next = NULL;
	((t_page *)(tmp))->allowed += size + sizeof(t_page) + sizeof(t_bloc);
	ft_putendl("_________4");
//	show_alloc_mem();
	return (new_bloc->start);
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
		return (allow_large_bloc((int)size));

	/*
	 * SMALL
	 */
//	else if (size > TINY_ALLOC)
//		allow_small_bloc(size);

	/*
	 * TINY
	 */
	else
		return(allow_tiny_bloc(size));

	return (NULL);
}
