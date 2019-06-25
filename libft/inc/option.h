/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   option.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbelouni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/26 18:15:23 by nbelouni          #+#    #+#             */
/*   Updated: 2017/04/26 18:15:35 by nbelouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPTION_H
# define OPTION_H

# include <ctype.h>

# define NOT_OPT 63
# define NO_ARG 58
# define ERR_ILLEGAL_OPT "illegal option -- "
# define NOT_ARG_ERRSTR ": option require an argument.\n"
# define RESET ""

typedef struct	s_opt
{
	int			opt_ind;
	int			opt_res;
	int			opt_char;
	char		*opt_arg;
}				t_opt;

int				ft_getopt(int argc, char **argv, char *optstr, t_opt *st_opt);
void			init_optstruct(t_opt *opt_struct);

#endif
