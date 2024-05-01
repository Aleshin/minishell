
#include "libft/libft.h"
//bash: command_name: command not found
//bash: command_name: not a valid identifier
//bash: syntax error near unexpected token
//bash: permission denied
//bash: file_or_directory: No such file or directory
//bash: option: invalid option
//bash: command: command failed
//bash: cannot allocate memory
void	put_error_fd(char *bash, char *file, char *error)
{
	ft_putstr_fd(bash, 2);
	ft_putstr_fd(": ", 2);
	if (file)
	{	
		ft_putstr_fd(file, 2);
		ft_putstr_fd(": ", 2);
	}
	if (error)
		ft_putendl_fd(error, 2);
}