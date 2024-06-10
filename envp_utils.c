char	*ft_strdup(const char *s)
{
	char	*test;
	char	*ptr_keeper;

	test = (char *)malloc(sizeof(char) * ft_strlen(s) + 1);
	if (!test)
		return (NULL);
	ptr_keeper = test;
	while (*s)
	{
		*test = *s;
		test++;
		s++;
	}
	*test = '\0';
	return ((char *)ptr_keeper);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == c)
			return ((char *)s);
		s++;
	}
	if (c == '\0')
		return ((char *)s);
	return (NULL);
}

