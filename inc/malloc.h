/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbelouni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/02 14:25:56 by nbelouni          #+#    #+#             */
/*   Updated: 2017/01/02 16:54:57 by nbelouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <libft.h>

# define TINY			0
# define SMALL			1
# define LARGE			2

# define TINY_ALLOC		1//645
# define SMALL_ALLOC	512

# define GETPAGESIZE	getpagesize()

typedef enum		e_bool
{
	FALSE,
	TRUE
}					t_bool;

typedef struct		s_page
{
	int				allowed;
	struct s_page	*next;
}					t_page;

typedef struct		s_bloc
{
//	int				type;
	void			*start;
	void			*end;
	int				size;
	struct s_bloc	*next;
}					t_bloc;

typedef struct		s_alloc
{
	void			*tiny;
	void			*small;
	void			*large;
}					t_alloc;

t_alloc				g_allowed;

void		free(void *ptr);
void		*malloc(size_t size);
void		*realloc(void *ptr, size_t size);

void		print_bloc(void *bloc, const char *type_name, long int *n);
void		show_alloc_mem(void);

#endif
