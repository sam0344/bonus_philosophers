


#include "philosophers.h"

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
	{
		usleep(125);
	}
}

char	*ft_strdup(char *s)
{
	int		i;
	char	*dup_string;

	dup_string = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
	if (dup_string == NULL)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		dup_string[i] = s[i];
		i++;
	}
	dup_string[i] = '\0';
	return (dup_string);
}
