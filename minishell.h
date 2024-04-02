#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>

# include "libft/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>
# include <sys/types.h>
# include <dirent.h>
# include <signal.h>


typedef struct s_struct
{
	int		i;
	int		n;
	int		flag;
	int		start;
	int		in_fd;
	int		out_fd;
	char	**paths;
	char	*path1;
	char	*path2;
	char	**in_cmd;
	char	**out_cmd;
}	t_pipe;

// void	initialize_tpipe(t_pipe *info)
// {
// 	info -> i = -1;
// 	info -> n = -1;
// 	info -> flag = 0;
// 	info -> in_fd = -1;
// 	info -> out_fd = -1;
// 	info -> paths = NULL;
// 	info -> path1 = NULL;
// 	info -> path2 = NULL;
// 	info -> in_cmd = NULL;
// 	info -> out_cmd = NULL;
// }


#endif