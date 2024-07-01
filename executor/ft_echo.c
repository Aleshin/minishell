#include "minishell.h"

//detect -n or -nnnnnnnnnnnnnnnnnnnn  0 no, 1 yes
int echo_n(char *first_arg)
{
    if (first_arg == NULL || *first_arg != '-')
        return 0;

    first_arg++; // Skip the initial '-'

    while (*first_arg)
    {
        if (*first_arg != 'n')
            return 0;
        first_arg++;
    }
    return 1;
}

void ft_echo(t_ast_node *command) {
    t_ast_node *exec;
    t_ast_node *num_args;
    t_ast_node *args;
    char *first_arg;
    int no_newline = 0; // Flag for the -n option

    if (command == NULL || command->first_child->next_sibling == NULL) {
        return ; // Error code indicating no command or no exec
    }

    exec = command->first_child->next_sibling;
    num_args = exec->next_sibling;

    // Check if the first argument is "-n"
    if (num_args != NULL && num_args->param > 0) {
        args = num_args->first_child;
        first_arg = args->value;
        if (echo_n(first_arg)) {
            no_newline = 1; // Set the flag to not print a newline
            args = args->next_sibling; // Move to the next argument
        }
    }
    // Print the command and its arguments
    if (num_args != NULL && num_args->param > 0) {
        while (args != NULL) {
            ft_putstr_fd(args->value, STDOUT_FILENO);
            if (args->next_sibling != NULL) {
                ft_putstr_fd(" ", STDOUT_FILENO); // Print space between arguments
            }
            args = args->next_sibling;
        }
    }
    // Print newline if -n flag is not set
    if (!no_newline) 
        ft_putendl_fd("", STDOUT_FILENO);
    exit(EXIT_SUCCESS); // Return success code or 1???
}

/*TESTS
echo
echo -n
echo -nnnnnnnnnnn
echo -nr -n
echo good morning
echo -n good m"orn"ing
*/