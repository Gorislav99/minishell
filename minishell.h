#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft/libft.h"
# include "libft/ft_printf/ft_printf.h"
# include "stdlib.h"
# include "sys/types.h"
# include "unistd.h"

typedef struct		s_command
{
	char				*command;
	char				**arg;
	unsigned int		count_arg;
	struct s_command	*up_command;
	struct s_command	*down_command;
}						t_command;


typedef struct	s_shell
{
	t_command	*list;
	char		**built_in;	
	char		**environ;
}				t_shell;


char				**split_line(char *line, unsigned int *count,  char **environ, int *fd);


void    launch_command(t_shell *shell_struct);
void	echo_(t_shell *shell_struct);
void	cd_(t_shell *shell_struct);
char			*get_path(char **environ);
void	env_(t_shell *shell_struct);
void	setenv_(t_shell *shell_struct);
void	unset_(t_shell *shell_struct);
void	exit_(t_shell *shell_struct);
char    *g_line();


#endif
