# include <stdlib.h>
# include <stdio.h>
# include <limits.h>
# include <errno.h>
# include <unistd.h>
# include <stdbool.h>
//# include <strings.h>
# include <fcntl.h>
# include <string.h>

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	srclen;
	size_t	dstlen;
	size_t	i;
	size_t	res;

	srclen = strlen(src);
	dstlen = strlen(dst);
	i = 0;
	if (dstsize <= dstlen)
		res = srclen + dstsize;
	else
		res = srclen + dstlen;
	if (dstsize == 0)
		return (res);
	while (src[i] && dstlen < dstsize - 1)
	{
		dst[dstlen++] = src[i++];
	}
	dst[dstlen] = '\0';
	return (res);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	srclen;
	size_t	i;

	srclen = strlen(src);
	i = 0;
	if (size == 0)
		return (srclen);
	while (*src && (i < size - 1))
	{
		*dst++ = *src++;
		i++;
	}
	*dst = '\0';
	return (srclen);
}

char	*ft_strjoin(char *s1, const char *s2)
{
	char	*str;
	size_t	size;

	size = strlen(s1) + strlen(s2) + 1;
	str = (char *)calloc(sizeof(char), size);
	if (str == NULL)
		return (0);
	ft_strlcpy(str, s1, strlen(s1) + 1);
	ft_strlcat(str, s2, size);
	return (str);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	size;

	size = strlen(s);
	if (!*s || size < start)
		return (calloc(sizeof(char), 1));
	if ((len + start) > size)
		len = size - start;
	str = malloc(sizeof(char) * (len + 1));
	if ((!str) || (!s && len))
		return (0);
	ft_strlcpy(str, s + start, (len + 1));
	return (str);
}

int main (int argc, char **argv)
{
	char 	*promt;
	int		fd_in;
	int		file_in;
	char 	*line;
	char 	buffer[2];
	int		num_byte;
	char	*tmp;
	char	*clean;
  	FILE 	*file_out;

	if (argc != 2)
		return (EXIT_FAILURE);

	file_out =	fopen(".tmp/exec_outp_clean.txt", "w" );
	file_in	=	open(".tmp/start.txt", O_RDONLY );
	fd_in	=	open (argv[1], O_RDONLY);

	line = strdup("");
	num_byte = read(file_in, &buffer, 1);
	line = ft_strjoin(line, strdup(buffer));
	while (!(buffer[0] == '\n' || num_byte == 0))
	{
		num_byte = read(file_in, &buffer, 1);
		if (num_byte == 0)
			break;
		buffer[1] = '\0';
		tmp = line;
		line = ft_strjoin(line, strdup(buffer));
		free(tmp);
	}
	promt = ft_substr(line, 0, strlen(line) - 5);

	line = strdup("");
	num_byte = read(fd_in, &buffer, 1);
	line = ft_strjoin(line, strdup(buffer));
	while (num_byte > 0)
	{
		while (!(buffer[0] == '\n' || num_byte == 0))
		{
			num_byte = read(fd_in, &buffer, 1);
			if (num_byte == 0)
				break;
			buffer[1] = '\0';
			tmp = line;
			line = ft_strjoin(line, strdup(buffer));
			free(tmp);
		}
		tmp = strstr(line, promt);
		if (tmp == NULL)
			fprintf(file_out, "%s", line);
		if (strstr(line, promt))
		{
			if (tmp != line)
			{
				clean = ft_substr(line, 0, tmp - line);
				fputs(clean, file_out);
			}
		}
		free (line);
		line = strdup("");
		num_byte = read(fd_in, &buffer, 1);
		line = ft_strjoin(line, strdup(buffer));	
	}

	close(fd_in);
   	fclose(file_out);
}
