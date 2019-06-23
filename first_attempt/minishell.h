#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft/libft.h"
# include "libft/ft_printf/ft_printf.h"
# include "stdlib.h"
# include "sys/types.h"
# include "sys/stat.h"
# include "signal.h"
# include "termios.h"
# include "unistd.h"
# include "termcap.h"
# define ENV shell_struct->environ
# define ARG shell_struct->list->arg
# define CHANGE_ENV { free(ENV[i]); ENV[i] = tm;return ;}

int					g_proc;

typedef struct		s_command
{
	char				*command;
	char				**arg;
	// unsigned int		count_arg;
	struct s_command	*up_command;
	struct s_command	*down_command;
}						t_command;


typedef struct	s_shell
{
	t_command	*list;
	char		**built_in;	
	char		**environ;
}				t_shell;


void				split_line(t_shell *shell_struct,char *comm, int *fd);



void    launch_command(t_shell *shell_struct);
void	echo_(t_shell *shell_struct);
void	cd_(t_shell *shell_struct);
char	*get_home(char **environ);
void	env_(t_shell *shell_struct);
void	setenv_(t_shell *shell_struct);
void	unset_(t_shell *shell_struct);
void	exit_(t_shell *shell_struct);
char    *g_line(void);
char			*get_env(char *varible, char **environ);
void				print_error(char *error, char *name);
void	free_strs(char **strs);
t_shell		*init_shell(char **environ);
t_command	*add_list(t_command *list, char *line);


#endif
