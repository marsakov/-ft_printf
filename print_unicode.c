/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_unicode.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakovyc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/04 17:53:30 by msakovyc          #+#    #+#             */
/*   Updated: 2018/06/04 17:54:04 by msakovyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int		fmask1(unsigned int v, unsigned int mask)
{
	unsigned char	o2;
	unsigned char	o1;
	unsigned char	octet;

	o2 = (v << 26) >> 26;
	o1 = ((v >> 6) << 27) >> 27;
	octet = (mask >> 8) | o1;
	write(1, &octet, 1);
	octet = ((mask << 24) >> 24) | o2;
	write(1, &octet, 1);
	return (2);
}

static int		fmask2(unsigned int v, unsigned int mask)
{
	unsigned char	o3;
	unsigned char	o2;
	unsigned char	o1;
	unsigned char	octet;

	o3 = (v << 26) >> 26;
	o2 = ((v >> 6) << 26) >> 26;
	o1 = ((v >> 12) << 28) >> 28;
	octet = (mask >> 16) | o1;
	write(1, &octet, 1);
	octet = ((mask << 16) >> 24) | o2;
	write(1, &octet, 1);
	octet = ((mask << 24) >> 24) | o3;
	write(1, &octet, 1);
	return (3);
}

static int		fmask3(unsigned int v, unsigned int mask)
{
	unsigned char	o4;
	unsigned char	o3;
	unsigned char	o2;
	unsigned char	o1;
	unsigned char	octet;

	o4 = (v << 26) >> 26;
	o3 = ((v >> 6) << 26) >> 26;
	o2 = ((v >> 12) << 26) >> 26;
	o1 = ((v >> 18) << 29) >> 29;
	octet = (mask >> 24) | o1;
	write(1, &octet, 1);
	octet = ((mask << 8) >> 24) | o2;
	write(1, &octet, 1);
	octet = ((mask << 16) >> 24) | o3;
	write(1, &octet, 1);
	octet = ((mask << 24) >> 24) | o4;
	write(1, &octet, 1);
	return (4);
}

int				print_unicode(wchar_t value)
{
	int				size;
	unsigned char	octet;

	size = count_base((unsigned int)value, 2);
	if (size <= 7)
	{
		octet = value;
		write(1, &octet, 1);
		return (1);
	}
	else if (size <= 11)
		return (fmask1((unsigned int)value, 49280));
	else if (size <= 16)
		return (fmask2((unsigned int)value, 14712960));
	else
		return (fmask3((unsigned int)value, 4034953344));
	return (0);
}

int				count_unicode(wchar_t *s)
{
	int bytes;
	int	size;
	int i;

	i = 0;
	bytes = 0;
	while (s[i])
	{
		size = count_base(s[i], 2);
		if (size <= 7)
			bytes += 1;
		else if (size <= 11)
			bytes += 2;
		else if (size <= 16)
			bytes += 3;
		else
			bytes += 4;
		i++;
	}
	return (bytes);
}
