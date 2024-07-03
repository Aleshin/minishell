#include "./minishell.h"
//if use strerror(errno) error is "No such file or directory"
//so temporary it is custom
void ft_shell_error(char *cmd, char *error)
{
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    ft_putstr_fd(cmd, STDERR_FILENO);
    ft_putstr_fd(": ", STDERR_FILENO);
    ft_putendl_fd(error, STDERR_FILENO);
}

//bash: syntax error near unexpected token `|'
void ft_sintax_error(char *cmd)
{
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    
    ft_putstr_fd("syntax error near unexpected token ", STDERR_FILENO);
    ft_putstr_fd("`", STDERR_FILENO);
    ft_putstr_fd(cmd, STDERR_FILENO);
    ft_putendl_fd("'", STDERR_FILENO);
}

void ft_cmd_error(char *arg)
{
    ft_putstr_fd(arg, STDERR_FILENO);
    ft_putstr_fd(": ", STDERR_FILENO);
    ft_putstr_fd(arg, STDERR_FILENO);
    ft_putstr_fd(": ", STDERR_FILENO);
    ft_putendl_fd(strerror(errno), STDERR_FILENO);
}

void ft_perror(char *str)
{
	perror(str);
    exit(EXIT_FAILURE);
}