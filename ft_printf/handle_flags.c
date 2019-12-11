/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_flags.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 14:26:55 by sadawi            #+#    #+#             */
/*   Updated: 2019/12/10 17:33:31 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft/libft.h"

int		handle_flags(const char **format, va_list *args)
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

int		check_flag(char *flag)
{
	int i;

	i = 0;
	while (ft_strchr("#0-+ ", flag[i]))
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

int		print_flag(char *flag, va_list *args)
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

void	handle_flag(char **output, char *flag)
{
	int i;

	i = 0;
	if (ft_strchr(flag, '#'))
		handle_hashtag(output, flag);
	if (ft_strchr(flag, '+'))
		handle_plus(output, flag);
	else if (ft_strchr(flag, ' '))
		handle_space(output, flag);
	while (flag[i])
		if (ft_isdigit(flag[i++]))
		{
			if (flag[i - 1] == '0')
			{
				if (!ft_strchr(flag, '-'))
					handle_zero(output, flag);
			}
			else
				handle_width(output, flag);
		}
}
