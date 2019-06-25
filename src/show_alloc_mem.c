/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbelouni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/14 18:05:56 by nbelouni          #+#    #+#             */
/*   Updated: 2019/06/25 14:00:26 by nbelouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void		print_bloc(void *bloc, const char *type_name, long int *n)
{
	ft_putstr(type_name);
	ft_putstr(" : bloc size =  ");
	ft_putnbr(sizeof(t_bloc));
	ft_putstr(" , location = ");
	ft_putnbr((int)(((t_bloc *)(bloc))->start));//, 1);
	ft_putendl("");
	ft_putnbr((int)(((t_bloc *)(bloc))->start) + sizeof(t_bloc));//, 1);
	ft_putstr(" - ");
	ft_putnbr((int)(((t_bloc *)(bloc))->end));//, 1);
	ft_putstr(" : ");
	ft_putnbr((int)(((t_bloc *)(bloc))->size));
	ft_putendl(" octets");
	if (n)
		*n += (int)(((t_bloc *)(bloc))->size);
}

void		show_alloc_mem(void)
{
	t_bloc		*lbloc;
	t_page		*spage;
//	t_page		*tpage;
	void		*tpage;
	t_bloc		*sbloc;
	t_bloc		*tbloc;
	long int	n;

	n = 0;
	lbloc = (t_bloc *)g_allowed.large;
	spage = (t_page *)g_allowed.small;
//	tpage = (t_page *)g_allowed.tiny;
	tpage = g_allowed.tiny;
//	ft_putstr("tpage : size = ");
//	ft_putnbr(tpage->allowed);
//	ft_putstr("\nnext = ");
//	ft_putstr((char *)tpage->next);
//	write(1, "\n", 1);
	ft_putstr("tpage : size = ");
	ft_putnbr(((t_page *)(tpage))->allowed);
	ft_putstr("\nnext = ");
	ft_putnbr((size_t)((t_page *)(tpage))->next);
	write(1, "\n", 1);
	if (spage)
		sbloc = (t_bloc *)(spage + sizeof(t_page));
	else
		sbloc = NULL;
	if (tpage)
	{
		tbloc = (t_bloc *)(g_allowed.tiny + sizeof(t_page));
//		print_bloc(tbloc, "FIRST INIT TBLOC", 0);
//		print_bloc(tpage + sizeof(t_page), "FIRST INIT TPAGE", 0);
//		print_bloc(g_allowed.tiny + sizeof(t_page), "FIRST INIT ALLOWED", 0);
	}
	else
		tbloc = NULL;




	while (lbloc || sbloc|| tbloc)
	{
		if (lbloc)
		{
			if ((!sbloc ||
			lbloc->start < sbloc->start) &&
			(!tbloc ||
			lbloc->start < tbloc->start))
			{
				print_bloc(lbloc, "LARGE", &n);
				lbloc = lbloc->next;
			}
		}
		if (sbloc)
		{
			if ((!lbloc ||
			sbloc->start < lbloc->start) &&
			(!tbloc ||
			sbloc->start < tbloc->start))
			{
				print_bloc(sbloc, "SMALL", &n);
				if (sbloc)
					sbloc = sbloc->next;
				if (!sbloc)
				{
					spage = spage->next;
					if (spage)
						sbloc = (t_bloc *)(spage + sizeof(t_page));
				}
			}
		}
		if (tbloc)
		{
			if ((!lbloc ||
			tbloc->start < lbloc->start) &&
			(!sbloc ||
			tbloc->start < sbloc->start))
			{
				print_bloc(tbloc, "TINY", &n);
				if (tbloc)
					tbloc = tbloc->next;
				if (!tbloc)
				{
					tpage = ((t_page *)(tpage))->next;
					if (tpage)
						tbloc = (t_bloc *)(tpage + sizeof(t_page));
				}
			}
		}
	}
	ft_putstr("Total : ");
	ft_putnbr(n);
	write(1, "\n", 1);
}
