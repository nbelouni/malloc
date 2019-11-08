/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbelouni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/02 14:25:56 by nbelouni          #+#    #+#             */
/*   Updated: 2019/11/08 17:05:59 by nbelouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include "libft.h"

# define TINY			0
# define SMALL			1
# define LARGE			2

# define TINY_ALLOC		1024
# define SMALL_ALLOC	4096

# define INIT_MAX_PAGES	1

# define GETPAGESIZE	getpagesize()

# define N(x)			ft_putendl(x)
# define V(x, y)		ft_putstr(x);ft_putnbr(y);ft_putendl("")

typedef enum		e_bool
{
	FALSE,
	TRUE
}					t_bool;

typedef enum		e_memory_state
{
	NOT_ALLOWED,
	ALLOWED,
	FREED
}					t_memory_state;

//typedef struct		s_bloc
//{
////	int				type;
//	void			*start;
//	void			*end;
//	int				size;
//	struct s_bloc	*next;
//}					t_bloc;
//
//typedef struct		s_page
//{
//	int				allowed;
//	t_bloc			*blocs;
//	struct s_page	*next;
//}					t_page;
//
//typedef struct		s_alloc
//{
//	t_page			*tiny;
//	t_page			*small;
//	t_page			*large;
//}					t_alloc;
//
//t_alloc				g_allowed;

typedef struct				s_memory_chunk
{
	size_t					size;
	t_memory_state			state;
	struct s_memory_chunk	*next;
}							t_memory_chunk;


typedef struct				s_alloc
{
	void					*tiny;
	size_t					tiny_pages;
	void					*small;
	size_t					small_pages;
	void					*large;
	size_t					large_pages;
}							t_alloc;

t_alloc						g_allowed;

//void		free(void *ptr);
void		*malloc(size_t size);
//void		*realloc(void *ptr, size_t size);
//
//void		print_bloc(void *bloc, const char *type_name, long int *n);
//void		show_alloc_mem(void);

#endif
