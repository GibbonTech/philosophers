/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_conversion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhomsi <ykhomsi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 23:28:51 by ykhomsi           #+#    #+#             */
/*   Updated: 2025/03/18 03:01:02 by ykhomsi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static char	*allocate_itoa(int n, int *len, int *is_negative)
{
	char	*str;

	*is_negative = 0;
	if (n == 0)
		return (ft_strdup("0"));
	if (n < 0)
	{
		*is_negative = 1;
		n = -n;
	}
	*len = ft_intlen(n);
	if (*is_negative)
		(*len)++;
	str = (char *)malloc(sizeof(char) * ((*len) + 1));
	if (!str)
		return (NULL);
	str[*len] = '\0';
	if (*is_negative)
		str[0] = '-';
	return (str);
}

char	*ft_itoa(int n)
{
	int		len;
	int		is_negative;
	char	*str;

	str = allocate_itoa(n, &len, &is_negative);
	if (!str)
		return (NULL);
	if (n == 0)
		return (str);
	if (n < 0)
		n = -n;
	while (n > 0)
	{
		str[--len] = (n % 10) + '0';
		n /= 10;
	}
	return (str);
}

int	ft_intlen(int n)
{
	int	len;

	len = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		n = -n;
	while (n > 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_strdup(const char *s)
{
	char	*str;
	int		i;

	i = 0;
	str = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!str)
		return (NULL);
	while (s[i])
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

int	ft_atoi(const char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}
