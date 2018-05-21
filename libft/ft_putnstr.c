/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakovyc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/12 19:59:33 by msakovyc          #+#    #+#             */
/*   Updated: 2018/05/12 19:59:35 by msakovyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_putnstr(char const *s, int n)
{
	int		i;

	i = 0;
	while (s && s[i] && n--)
		ft_putchar(s[i++]);
	return (i);
}
