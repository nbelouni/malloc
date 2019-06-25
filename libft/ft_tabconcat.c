/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tabconcat.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbelouni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/26 18:13:41 by nbelouni          #+#    #+#             */
/*   Updated: 2017/04/26 18:14:32 by nbelouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_tabconcat(char **tab)
{
	char	*concat;
	size_t	len;
	int		i;

	len = 0;
	i = -1;
	while (tab[++i])
	{
		len += ft_strlen(tab[i]);
	}
	if ((concat = ft_strnew(len + ft_tablen(tab))) == NULL)
		return (NULL);
	i = -1;
	while (tab[++i])
	{
		concat = ft_strcat(concat, tab[i]);
		concat[ft_strlen(concat)] = (tab[i + 1]) ? ' ' : '\0';
	}
	return (concat);
}
