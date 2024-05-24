/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleunin <saleunin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 12:55:30 by saleunin          #+#    #+#             */
/*   Updated: 2024/05/21 16:31:42 by saleunin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers2.h"

int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

ssize_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

int	ft_atoi(char *num)
{
	unsigned int	result;
	int				i;

	result = 0;
	i = -1;
	while (num[++i])
	{
		if (num[i] < '0' || num[i] > '9')
			return (-1);
		result = (result * 10) + num[i] - '0';
	}
	return (result);
}

void	ft_usleep(ssize_t wait_time)
{
	ssize_t	start_time;

	start_time = get_current_time();
	while (get_current_time() - start_time < wait_time)
		usleep(125);
}

void ft_error(char *str, int exit_code)
{
	if (str)
		write(2, str, ft_strlen(str));
	exit(exit_code);
}
