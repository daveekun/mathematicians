/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhorvath <dhorvath@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:47:57 by dhorvath          #+#    #+#             */
/*   Updated: 2024/02/20 11:48:02 by dhorvath         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(char *str)
{
	int		i;
	int		sign;
	long	res;
	long	prev;

	sign = 1;
	i = 0;
	res = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		sign = (str[i] == '-') * -1 * sign + (str[i] != '-') * sign;
		i++;
	}
	while ('0' <= str[i] && str[i] <= '9')
	{
		prev = res;
		res = res * 10 + (str[i++] - '0');
		if ((res > 0 && prev < 0) || (res < 0 && prev > 0))
		{
			if (sign == 1)
				return (-1);
			return (0);
		}
	}
	return ((int) sign * res);
}
