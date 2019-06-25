/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_multi_join.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbelouni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/26 18:14:05 by nbelouni          #+#    #+#             */
/*   Updated: 2017/04/26 18:14:32 by nbelouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_multi_join(char *s1, char *s2, char *s3, char *s4)
{
	char	*tmp;
	size_t	len;

	tmp = NULL;
	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + ft_strlen(s4);
	if (s1)
	{
		tmp = ft_strdup(s1);
		ft_strdel(&s1);
	}
	s1 = ft_strnew(len);
	if (tmp)
		ft_strcpy(s1, tmp);
	s1 = ft_multi_concat(s1, s2, s3, s4);
	return (s1);
}
