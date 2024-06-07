#include "philosophers2.h"

static int	ft_num_len(int n)
{
	int	len;

	len = 0;
	if (n < 0)
	{
		if (n == -2147483648)
			return (11);
		len++;
		n *= -1;
	}
	if (n == 0)
		return (1);
	while (n > 0)
	{
		len++;
		n /= 10;
	}
	return (len);
}

static char	*ft_fill_string(int n, int isnegative, char *str_n, int i)
{
	str_n[i] = '\0';
	while (i > 0)
	{
		i--;
		if (isnegative == -1 && i == 0)
			str_n[i] = '-';
		else
			str_n[i] = (n % 10) + '0';
		n /= 10;
	}
	return (str_n);
}

char	*ft_itoa(int n)
{
	char	*str_n;
	int		isnegative;
	int		num_len;

	num_len = ft_num_len(n);
	str_n = (char *)malloc(sizeof(char) * (num_len + 1));
	if (str_n == NULL)
		return (NULL);
	isnegative = 1;
	if (n < 0)
	{
		isnegative = -1;
		n *= -1;
	}
	str_n = ft_fill_string(n, isnegative, str_n, num_len);
	return (str_n);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*newstring;
	int		i;
	int		j;

	newstring = (char *)malloc((ft_strlen(s1) + ft_strlen(s2) + 1));
	if (newstring == NULL)
		return (NULL);
	j = 0;
	i = 0;
	while (s1[i])
	{
		newstring[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		newstring[i + j] = s2[j];
		j++;
	}
	newstring[i + j] = '\0';
	return (newstring);
}
