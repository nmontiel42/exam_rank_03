#include "get_next_line.h"

size_t	ft_strlen(char *str) 
{
	size_t i = 0;
	while (str[i])
		i++;
	return(i);
}

//ft_strchr modificado para buscar '\n' (10) directamente
bool ft_nlsrch(char *str)
{
	if (!str)
		return (false);
	while (*str)
	{
		if (*str == 10)
			return (true);
		str++;
	}
	return (false);
}
//calloc modificado para ahorrar casteos y sizeof(char)
char	*ft_challoc(size_t count)
{
	size_t	i;
	char	*ptr;

	ptr = (char *)malloc(count);
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < count)
		ptr[i++] = 0;
	return (ptr);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*ptr;
	size_t	i = 0;
	size_t	j = 0;

	ptr = ft_challoc((ft_strlen(s1) + ft_strlen(s2)) + 1);
	while (s1[i])
	{
		ptr[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		ptr[i + j] = s2[j];
		j++;
	}
	ptr[i + j] = 0;
	return (free(s1), ptr);
}

static char	*get_piece(char *memo, int fd)
{
	char	*aux;
	int		ret;

	if (!memo) //si aún no hay nada en la estática 
		memo = ft_challoc(1); //se inicializa
	ret = 1;
	aux = ft_challoc(BUFFER_SIZE + 1); //el buffer de read
	while (ret > 0 && !ft_nlsrch(aux))
	{
		ret = read(fd, aux, BUFFER_SIZE);
		if (ret < 0) //si read falla libero todo
			return (free(aux), free(memo), NULL);
		aux[ret] = 0;
		memo = ft_strjoin(memo, aux);
		if (!memo)
			return (free(memo), free(aux), NULL);
	}
	return (free(aux), memo);
}

static char	*get_line(char	*memo)
{
	char	*line;
	int		i = 0;

	if (!memo[i]) //esta condición no se puede eliminar o peta jeje
		return (NULL);
	while (memo[i] && memo[i] != 10)
		i++;
	line = ft_challoc((i + 1 + (memo[i] == 10)));
//si hay salto de línea (no estamos al final del archivo), aloja uno de más para ponerlo
	i = 0;
	while (memo[i] && memo[i] != 10)
	{
		line[i] = memo[i];
		i++;
	}
	if (memo[i] == 10)
		line[i++] = 10;
	line[i] = 0;
	return (line);
}

static char	*ft_clean(char *memo)
{
	char	*new_line;
	int		i = 0;
	int		j = 0;

	while (memo[i] && memo[i] != 10)
		i++;
	if (!memo[i]) //si al final de memo hay nulo (hemos llegado al final del archivo)
		return (free(memo), NULL); //liberamos la estática porque no la vamos a necesitar más
	new_line = ft_challoc((ft_strlen(memo) - i)); //alojo para lo que queda tras 10
	i++; //paso el salto de línea
	while (memo[i])
		new_line[j++] = memo[i++];
	new_line[j] = 0;
	return (free(memo), new_line);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*memo;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	memo = get_piece(memo, fd);
	line = get_line(memo);
	memo = ft_clean(memo);
	return (line);
}