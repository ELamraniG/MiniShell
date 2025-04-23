#include <stdio.h>
#include <string.h>

int	calc_min(char *str)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (str[i])
	{
		if (str[i] == '(')
			res++;
		if (str[i] == ')')
			res--;
		i++;
	}
	if (res < 0)
		res = res * -1;
	return (res);
}

int	is_valid(char *str)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (str[i])
	{
		if (str[i] == '(')
			res++;
		if (str[i] == ')')
			res--;
		if (res < 0)
			return (0);
		i++;
	}
	if (res == 0)
		return (1);
	return (0);
}

void	rip(char *str, int min, int index, int change, int size)
{
	int		i;
	char	c;

		if (is_valid(str) && change == min)
			puts(str);
	i = index;
	while (str[i])
	{
		c = str[i];
		str[i] = '_';
		change++;
		rip(str, min, i + 1, change, size);
		str[i] = c;
		change--;
		i++;
	}
}

int	main(void)
{
	char str[9] = "(()(()(";
	int min = calc_min(str);
	rip(str, min, 0, 0, strlen(str));
}