/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbelouni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/02 21:05:52 by nbelouni          #+#    #+#             */
/*   Updated: 2019/11/06 20:06:00 by nbelouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <stdio.h>

void	init_pages(size_t type)
{
	void	*tmp;
	void	*tmp2;
//	int i = 0;

	if (type == TINY)
	{
		tmp = g_allowed.tiny;
		tmp2 = g_allowed.tiny + GETPAGESIZE;
		ft_putnbr((int)tmp2 - (int)tmp);N("");
	}
	else if (type == SMALL)
	{
		tmp = g_allowed.small;
		tmp2 = tmp + GETPAGESIZE;
		ft_putnbr(GETPAGESIZE);N("");
		ft_putnbr((int)tmp);N("");
		ft_putnbr((int)g_allowed.small);N("");
		ft_putnbr((int)g_allowed.small + GETPAGESIZE);N("");
		ft_putnbr((int)tmp2);N("");
		ft_putnbr((int)tmp2 - (int)tmp);N("");
	}
	else
		return;
/*	while (i < 10)
	{
		tmp->next = tmp2;
		tmp = tmp2;
		ft_putnbr(tmp->allowed);N("");
		ft_putnbr((int)tmp);N("");
		tmp2 += GETPAGESIZE;
		ft_putnbr((int)tmp2);N("");
		i++;
	}
		tmp->next = NULL;
*/
}

t_bool	first_init()
{
	static int	start;

	if (start)
		return (TRUE);
	else
	{
		g_allowed.large = NULL;
		g_allowed.small = mmap(0, GETPAGESIZE * 10, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
		ft_bzero(g_allowed.small, GETPAGESIZE * 10);

		init_pages(SMALL);
		g_allowed.tiny = mmap(0, GETPAGESIZE * 10, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
		init_pages(TINY);
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

void		put_alloc_size(int type)
{
	t_page	*tmp = (t_page *)g_allowed.tiny;
	int		total_size = 0;

	(void)type;
	while (tmp != NULL)
	{
		t_bloc	*tmp2 = (t_bloc *)(tmp->blocs);
		total_size += sizeof(t_page);
		while (tmp2 != NULL)
		{
			total_size += (sizeof(t_bloc)) + tmp2->size;
			ft_putnbr(tmp2->size);
			ft_putendl("");
			tmp2 = tmp2->next;
		}
		tmp = tmp->next;
	}
	ft_putstr("tiny size : ");
	ft_putnbr(total_size);
	ft_putendl("");
	ft_putstr("page size : ");
	ft_putnbr(GETPAGESIZE);
	ft_putendl("");
}

void		*get_new_page_and_bloc(size_t size)
{
	void	*tmp;
	t_page	*tmp2;
	t_bloc	*new_bloc;

	tmp = mmap(0, GETPAGESIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);

	((t_page *)(tmp))->allowed = size + sizeof(t_page) + sizeof(t_bloc);
		((t_page *)(tmp))->next = NULL;
	if (g_allowed.tiny != NULL)
	{
		tmp2 = g_allowed.tiny;
		while (tmp2->next)
			tmp2 = tmp2->next;
		tmp2->next = tmp;
	}
	new_bloc = (t_bloc *)(tmp + sizeof(t_page));
	new_bloc->start = new_bloc;
	new_bloc->end = new_bloc->start + size + sizeof(t_bloc);;
	new_bloc->size = size;
	new_bloc->next = NULL;
	((t_page *)(tmp))->blocs = new_bloc;

	return (tmp);
}

void		*allow_tiny_bloc(size_t size)
{
//	ft_putendl("TINY");
	void	*tmp;
	t_page	*tmp2;
	t_bloc	*new_bloc;

	tmp = NULL;
	ft_putendl("_________1.0");
	put_alloc_size(TINY);
	if (g_allowed.tiny == NULL)
	{
	ft_putendl("_________1.0");
		/*
		tmp = mmap(0, GETPAGESIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);

		((t_page *)(tmp))->allowed = size + sizeof(t_page) + sizeof(t_bloc);
		((t_page *)(tmp))->next = NULL;
		get_new_page(size);
		new_bloc = (t_bloc *)(tmp + sizeof(t_page));
		new_bloc->start = new_bloc;
		new_bloc->end = new_bloc->start + size + sizeof(t_bloc);;
		new_bloc->size = size;
		new_bloc->next = NULL;
		((t_page *)(tmp))->blocs = new_bloc;
*/
		
		g_allowed.tiny = get_new_page_and_bloc(size);
		show_alloc_mem();
		return (g_allowed.tiny->blocs);
	}
	ft_putendl("_________2.0");
	tmp2 = g_allowed.tiny;
	new_bloc = NULL;
	ft_putstr("page->allowed : ");
	ft_putnbr(((t_page *)(tmp2))->allowed);
	write(1, "\n", 1);
	while (tmp2)
	{
		ft_putendl("_________2.1");
		if ((size_t)(GETPAGESIZE * 10 - ((t_page *)(tmp2))->allowed) > sizeof(t_bloc) + size)
		{
		ft_putendl("_________2.2");
			void *tmp3;

			tmp3 = tmp2->blocs;
			while (((t_bloc *)(tmp3))->next)
			{
//				print_bloc(tmp3, "tmp3", 0);
				tmp3 = ((t_bloc *)(tmp3))->next;
			}
			print_bloc(tmp3, "tmp3", 0);
			ft_putendl("##################");

			new_bloc = tmp3 + sizeof(t_bloc) + ((t_bloc *)tmp3)->size;
			((t_bloc *)(tmp3))->next = new_bloc;
			new_bloc->start = new_bloc;
			new_bloc->end = new_bloc + sizeof(t_bloc) + size;
			new_bloc->size = size;
			new_bloc->next = NULL;
			print_bloc(((t_bloc *)(tmp3))->next, "tmp3->next", 0);
			((t_page *)(tmp2))->allowed += size + sizeof(t_bloc);
		ft_putendl("_________2.3");
			
			return (new_bloc);// + sizeof(t_bloc));
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
		new_bloc = get_new_page_and_bloc(size) + sizeof(t_page);
	}
	else
	{
		ft_putendl("_________3");
		new_bloc->start = new_bloc;
		new_bloc->end = new_bloc + sizeof(t_bloc) + size;
		new_bloc->size = size;
		new_bloc->next = NULL;
		((t_page *)(tmp))->allowed += size + sizeof(t_page) + sizeof(t_bloc);
		ft_putendl("_________4");
	}
	show_alloc_mem();
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
	{
		ft_putendl("LARGE");
		void *tmp = allow_large_bloc(size);
		print_bloc(((t_bloc *)tmp), "new_bloc", 0);
		return(tmp + sizeof(t_bloc));
		return (allow_large_bloc((int)size));
	}

	/*
	 * SMALL
	 */
//	else if (size > TINY_ALLOC)
//		allow_small_bloc(size);

	/*
	 * TINY
	 */
	else
	{
		ft_putendl("TINY");
		void *tmp = allow_tiny_bloc(size);
		ft_putendl("ici ?");
		print_bloc(((t_bloc *)tmp), "new_bloc", 0);
		ft_putendl("ici ?");
		return(tmp + sizeof(t_bloc));
	}


	return (NULL);
}
