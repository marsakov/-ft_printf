/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakovyc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/28 15:43:31 by msakovyc          #+#    #+#             */
/*   Updated: 2018/04/28 15:43:32 by msakovyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		check_number(char *str, int i)
{
	if (str[i + 1] && str[i + 1] >= '0' && str[i + 1] <= '9')
		return (ft_atoi(str + i + 1));
	else
		return (0);
}

int		check_conv3(char *str, int i, va_list ap)
{
	unsigned long int	lud;
	int					bytes;

	if (str[i] == 'U')
	{
		lud = va_arg(ap, unsigned long int);
		bytes = print_lu(lud, 0);
	}
	else if (str[i] == 'O')
	{
		lud = va_arg(ap, unsigned long int);
		bytes = print_lu(lud, 0);
	}
	else if (str[i] == '%')
	{
		ft_putchar(str[i]);
		bytes = 1;
	}
	return (bytes);
}

int		check_conv2(char *str, int i, va_list ap)
{
	char	*s;
	char	c;
	int		d;
	int		bytes;

	bytes = 0;
	if (str[i] == 's')
	{
		s = va_arg(ap, char *);
		bytes = ft_putstr(s);
	}
	else if (str[i] == 'c')
	{
		c = va_arg(ap, int);
		ft_putchar((char)c);
		bytes++;
	}
	else if (str[i] == 'd' || str[i] == 'i' || str[i] == 'u')
	{
		d = va_arg(ap, long int);
		bytes = print_base(d, 10, 0, 0);
	}
	else
		bytes = check_conv3(str, i, ap);
	return (bytes);
}

int		check_conv1(char *str, int i, va_list ap)
{
	long int			d;
	unsigned long int	ld;
	int					bytes;

	bytes = 0;
	if (str[i] == 'o')
	{
		d = va_arg(ap, long int);
		bytes = print_base(d, 8, 0, 0);
	}
	else if (str[i] == 'x')
	{
		d = va_arg(ap, long int);
		bytes = print_base(d, 16, 0, 0);
	}
	else if (str[i] == 'X')
	{
		d = va_arg(ap, long int);
		bytes = print_base(d, 16, 0, 1);
	}
	else
		bytes = check_conv2(str, i, ap);
	return (bytes);
}

t_frmt	check(char *str, int *i)
{
	t_frmt		frmt;
	char		*s;

	frmt.max_width = 0;
	frmt.min_width = 0;
	frmt.modifier = 0;
	frmt.spec = 0;
	frmt.flag = 0;
	frmt.prec = 0;
	if ((s = ft_strchr("#0-+ ", str[(*i)])) != NULL)
	{
		frmt.flag = *s;
		frmt.min_width = check_number(str, *i);
		*i += count_base(frmt.min_width, 10) + 1;
	}
	if (str[(*i)] == '.')
	{
		frmt.prec = 1;
		frmt.max_width = check_number(str, *i);
		*i += count_base(frmt.max_width, 10) + 1;
	}
	if ((s = ft_strchr("hljz", str[(*i)])) != NULL)
	{
		frmt.spec = *s;
		(*i)++;
	}
	if ((s = ft_strchr("sSpdDioOuUxXcC%", str[(*i)])) != NULL)
	{
		frmt.modifier = *s;
		(*i)++;
	}
	return (frmt);
}

int		print_s(t_frmt frmt, va_list ap)
{
	char	*s;
	int		bytes;

	s = va_arg(ap, char*);
	if (frmt.flag == '-')
	{
		bytes = (frmt.prec && frmt.max_width <= ft_strlen(s)) ? ft_putnstr(s, frmt.max_width) : ft_putstr(s);
		return (bytes + repeat_char(' ', frmt.min_width - bytes));
	}
	else if (frmt.flag == '0' || frmt.flag == '+' || frmt.flag == '#')
	{
		bytes = (frmt.prec && frmt.max_width <= ft_strlen(s)) ? frmt.max_width : ft_strlen(s);
		return (repeat_char((frmt.flag == '0') ? '0' : ' ', frmt.min_width - bytes) + ft_putnstr(s, bytes));
	}
	else if (frmt.prec && frmt.max_width <= ft_strlen(s))
		return (ft_putnstr(s, frmt.max_width));
	else
		return (ft_putstr(s));
}

int		print_d(t_frmt frmt, va_list ap)
{
	int		bytes;
	int		minus;
	int		d;

	d = va_arg(ap, int);
	minus = 0;
	if (d < 0)
		minus = 1;
	if (frmt.flag == '-')
	{
		if (minus)
			ft_putchar('-');
		bytes = (frmt.prec && frmt.max_width > count_base(d, 10)) ? repeat_char('0', frmt.max_width - count_base(d, 10)) : 0;
		print_base(d, 10, count_base(d, 10), 0);
		if ((frmt.prec && frmt.max_width < frmt.min_width) || !frmt.prec)
			bytes += repeat_char(' ', frmt.min_width - bytes - minus - count_base(d, 10));
		return (bytes + count_base(d, 10) + minus);
	}
	else if (frmt.flag == '0' || frmt.flag == '#')
	{
		if (minus && frmt.flag == '0' && !frmt.prec)
			ft_putchar('-');
		if (frmt.prec)
			bytes = repeat_char(' ', (frmt.max_width > count_base(d, 10) - minus) ? frmt.min_width - frmt.max_width - minus : frmt.min_width - count_base(d, 10) - minus);
		else
			bytes = repeat_char((frmt.flag == '0') ? '0' : ' ', frmt.min_width - count_base(d, 10) - minus);
		if ((minus && frmt.flag != '0') || frmt.prec)
			ft_putchar('-');
		bytes += (frmt.prec) ? repeat_char('0', frmt.max_width - count_base(d, 10)) : 0;
		print_base(d, 10, count_base(d, 10), 0);
		return (count_base(d, 10) + minus + bytes);
	}
	else if (frmt.flag == '+')
	{
		bytes = repeat_char(' ', (frmt.max_width > count_base(d, 10) - 1) ? frmt.min_width - frmt.max_width - 1 : frmt.min_width - count_base(d, 10) - 1);
		if (minus)
			ft_putchar('-');
		else
			ft_putchar('+');
		bytes += repeat_char('0', frmt.max_width - count_base(d, 10));
		print_base(d, 10, count_base(d, 10), 0);
		return (count_base(d, 10) + bytes + 1);
	}
	else
		return (print_base(d, 10, count_base(d, 10), 0) + count_base(d, 10));
}

int		print_uox(t_frmt frmt, va_list ap)
{
	int				bytes;
	unsigned int	u;
	int				base;

	u = va_arg(ap, int);
	if (frmt.modifier == 'u')
		base = 10;
	else
		base = (frmt.modifier == 'o') ? 8 : 16; 
	// if (frmt.flag == '-')
	// {
	// 	bytes = repeat_char('0', (count_unsign_base(u, base) < frmt.max_width) ? frmt.max_width - count_unsign_base(u, base) : 0);
	// 	return (bytes + count_unsign_base(u, base) + print_base(u, base, ))
	// }
	
}

int		print(t_frmt frmt, va_list ap)
{
	if (frmt.modifier == 's')
		return (print_s(frmt, ap));
	else if (frmt.modifier == 'd' || frmt.modifier == 'i')
		return (print_d(frmt, ap));
	else if (frmt.modifier == 'u' || frmt.modifier == 'o' || frmt.modifier == 'x' || frmt.modifier == 'X')
		return (print_uox(frmt, ap));
}

int		ft_printf(char *str, ...)
{
	int		start;
	int		i;
	int		result;
	t_frmt	frmt;
	va_list	ap;

	va_start(ap, str);
	start = 0;
	i = 0;
	result = 0;
	while (str[i])
	{
		start = i;
		if (str[i] == '%')
		{
			i++;
			frmt = check(str, &i);
			result += print(frmt, ap);
		}
		else
		{
			while (str[i] && str[i] != '%')
				i++;
			result += i - start;
			ft_putnstr(str + start, i - start);
		}
	}
	va_end(ap);
	return (result);
}

int main()
{
	// int bytes = ft_printf("%c\n", "ϗ");
	// setlocale(P_ALL, "");
	// int bytesp = printf("%.3ls\n", L"aa");

	//int ft = 0; orig : {    -01234}

	int ft = ft_printf("  my : {%-10.5d}\n", 1234);
	int orig = printf("orig : {%-10.5d}\n", 1234);
	printf("%d || %d\n", ft, orig);
	return 0;
}