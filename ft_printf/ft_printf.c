/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 14:26:55 by sadawi            #+#    #+#             */
/*   Updated: 2019/12/09 16:08:22 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include "libft/libft.h"
#include "ft_printf.h"
#include <stdio.h> //tmp for printf

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		amount;

	amount = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == '%' && format[1] == '%')
			ft_putchar(*format++);
		else if (*format == '%')
			amount += handle_flags(&format, &args);
		else
			ft_putchar(*format);
		format++;
		amount++;
	}
	return (amount);
}

int	handle_flags(const char **format, va_list *args)
{
	char	*flag;
	int		i;
	int		amount;

	i = 0;
	amount = 0;
	if ((*format)[1] == '%')
	{
		format++;
		return (0);
	}
	while (!ft_strchr("cspdiouxXf", (*format)[i]) && (*format)[i] != '\0')
		i++;
	flag = ft_strsub(*format, 1, i);
	if (check_flag(flag))
	{
		*format += i;
		amount = print_flag(flag, args);
	}
	free(flag);
	return (amount - 1);
}

int	print_flag(char *flag, va_list *args)
{
	int len;

	if (!(len = handle_signed(flag, args)))
		if (!(len = handle_unsigned(flag, args)))
			if (!(len = handle_pointer(flag, args)))
				if (!(len = handle_base(flag, args)))
					if (!(len = handle_float(flag, args)))
						return (0);
	return (len);
}

int	handle_signed(char *flag, va_list *args)
{
	char	*output;
	long	tmp;

	if (ft_strchr(flag, 'c'))
		output = ft_chartostr(va_arg(*args, int));
	else if (ft_strstr(flag, "hhi") || ft_strstr(flag, "hhd"))
		tmp = (signed char)va_arg(*args, int);
	else if (ft_strstr(flag, "hi") || ft_strstr(flag, "hd"))
		tmp = (short)va_arg(*args, int);
	else if (ft_strstr(flag, "lli") || ft_strstr(flag, "lld"))
		tmp = (long)va_arg(*args, long);
	else if (ft_strstr(flag, "li") || ft_strstr(flag, "ld"))
		tmp = (long)va_arg(*args, long);
	else if (ft_strchr(flag, 'd') || ft_strchr(flag, 'i'))
		tmp = va_arg(*args, int);
	else
		return (0);
	if (!ft_strchr(flag, 'c'))
		output = ft_itoa_base(tmp, 10);
	ft_putstr(output);
	return (ft_strlen(output));
}

int	handle_unsigned(char *flag, va_list *args)
{
	char			*output;
	unsigned long	tmp;

	if (ft_strstr(flag, "hhu"))
		tmp = (unsigned char)va_arg(*args, unsigned int);
	else if (ft_strstr(flag, "hu"))
		tmp = (unsigned short)va_arg(*args, unsigned int);
	else if (ft_strstr(flag, "llu") || ft_strstr(flag, "lu"))
		tmp = (unsigned long)va_arg(*args, unsigned long);
	else if (ft_strchr(flag, 'u'))
		tmp = (unsigned int)va_arg(*args, unsigned int);
	else
		return (0);
	output = ft_itoa_base_ul(tmp, 10);
	ft_putstr(output);
	return (ft_strlen(output));
}

int	handle_base(char *flag, va_list *args)
{
	char			*output;
	unsigned long	tmp;

	if (ft_strstr(flag, "hhx") || ft_strstr(flag, "hhX")
		|| ft_strstr(flag, "hho"))
		tmp = (signed char)va_arg(*args, unsigned int);
	else if (ft_strstr(flag, "hx") || ft_strstr(flag, "hX")
			|| ft_strstr(flag, "ho"))
		tmp = (short)va_arg(*args, unsigned int);
	else if (ft_strstr(flag, "llx") || ft_strstr(flag, "llX")
			|| ft_strstr(flag, "llo"))
		tmp = (long)va_arg(*args, unsigned long);
	else if (ft_strstr(flag, "lx") || ft_strstr(flag, "lX")
			|| ft_strstr(flag, "lo"))
		tmp = (long)va_arg(*args, unsigned long);
	else if (ft_strchr(flag, 'x') || ft_strchr(flag, 'o'))
		tmp = va_arg(*args, unsigned int);
	else
		return (0);
	if (ft_strchr(flag, 'x'))
		output = ft_itoa_base_ul_low(tmp, 16);
	else if (ft_strchr(flag, 'X'))
		output = ft_itoa_base_ul(tmp, 16);
	else
		output = ft_itoa_base_ul_low(tmp, 8);
	ft_putstr(output);
	return (ft_strlen(output));
}

int	handle_float(char *flag, va_list *args)
{
	char		*output;
	long double tmp;
	int			precision;

	if (ft_strchr(flag, '.'))
		precision = ft_atoi(ft_strchr(flag, '.') + 1);
	else
		precision = 6;
	if (ft_strstr(flag, "Lf"))
		tmp = va_arg(*args, long double);
	else if (ft_strchr(flag, 'f'))
		tmp = va_arg(*args, double);
	else
		return (0);
	output = ft_itoa_double(tmp, precision);
	ft_putstr(output);
	return ((ft_strlen(output)));
}

int	handle_pointer(char *flag, va_list *args)
{
	char			*output;
	unsigned long	tmp;
	int				len;

	if (ft_strchr(flag, 's'))
		output = ft_strdup(va_arg(*args, char*));
	else if (ft_strchr(flag, 'p'))
	{
		tmp = va_arg(*args, unsigned long);
		output = toaddress(tmp);
	}
	else
		return (0);
	if (ft_strchr(flag, '.'))
		if ((int)ft_strlen(output) > ft_atoi(ft_strchr(flag, '.') + 1))
			output[ft_atoi(ft_strchr(flag, '.') + 1)] = '\0';
	ft_putstr(output);
	len = ft_strlen(output);
	return (len);
}

char	*toaddress(unsigned long n)
{
	char *address;
	char *tmp;

	address = (char*)malloc(15);
	ft_strcpy(address, "0x");
	tmp = ft_itoa_base_ul(n, 16);
	ft_strcat(address, tmp);
	free(tmp);
	return (address);
}

int	check_flag(char *flag)
{
	int i;

	i = 0;
	while (strchr("#0-+ ", flag[i]))
		i++;
	while (ft_isdigit(flag[i]))
		i++;
	if (flag[i] == '.')
		i++;
	while (ft_isdigit(flag[i]))
		i++;
	if (flag[i] == 'h')
	{
		i++;
		if (flag[i] == 'h')
			i++;
	}
	else if (flag[i] == 'l')
	{
		i++;
		if (flag[i] == 'l')
			i++;
	}
	else if (flag[i] == 'L')
		i++;
	if (ft_strchr("cspdiouxXf", flag[i]))
		return (1);
	return (0);
}

int	main(int argc, char **argv)
{
	int *ptr;
	int a;
	long double d;

	d = 123456789;
	a = 4;
	ptr = &a;
	(void)argc;
	(void)argv;
	ft_putnbr(ft_printf("%.5s %s\n", "hello", "test") - 1);
	ft_putendl("\n");
	ft_putnbr(printf("%.5s %s\n", "hello", "test") - 1);
	ft_putendl("");
	return (0);
}

//change output in all handle functions to string, then handle precision first,
// then width, then flags.
//implement flags, width and precision.

//currently prints flag if flag is invalid, not sure if needs to be fixed
//ulonglen and other similar functions may be unnecessary, delete once project is done
// if true
