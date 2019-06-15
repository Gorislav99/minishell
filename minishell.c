/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgorczan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 16:25:30 by mgorczan          #+#    #+#             */
/*   Updated: 2019/06/08 16:25:32 by mgorczan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char			**init_built_in()
{
	char **built_in;

	if ((built_in = (char**)malloc(sizeof(char*) * 7)) == NULL)
		exit(1);
	(built_in[0] = ft_strdup("echo")) == NULL ? exit(1) : 0;
	(built_in[1] = ft_strdup("cd")) == NULL ? exit(1) : 0;
	(built_in[2] = ft_strdup("setenv")) == NULL ? exit(1) : 0;
	(built_in[3] = ft_strdup("unsetenv")) == NULL ? exit(1) : 0;
	(built_in[4] = ft_strdup("env")) == NULL ? exit(1) : 0;
	(built_in[5] = ft_strdup("exit")) == NULL ? exit(1) : 0;
	built_in[6] = NULL;
	return (built_in);
}

static t_shell		*init_shell(char **environ)
{
	t_shell	*shell_struct;
	int		i;

	if ((shell_struct = (t_shell*)malloc(sizeof(t_shell))) == NULL)
		exit(1);
	i = 0;
	while (environ[i])
		++i;
	if ((shell_struct->environ = (char**)malloc(sizeof(char *) * (i + 1))) == NULL)
		exit(1);
	shell_struct->environ[i] = NULL;
	while (--i >= 0)
		shell_struct->environ[i] = ft_strdup(environ[i]);
	shell_struct->built_in = init_built_in();
	shell_struct->list = NULL;
	return (shell_struct);
}

static t_command	*add_list(t_command *list, char **arg, unsigned int count)
{
	if (list == NULL)
	{
		if ((list = (t_command*)malloc(sizeof(t_command))) == NULL)
			exit(1);
		list->command = ft_strdup(arg[0]);
		list->arg = arg;
		list->up_command = NULL;
		list->down_command = NULL;
		list->count_arg = count;
		return (list);
	}
	if ((list->down_command = (t_command*)malloc(sizeof(t_command))) == NULL)
		exit(1);
	
	list->down_command->command = ft_strdup(arg[0]);
	list->down_command->arg = arg;
	list->down_command->up_command = list;
	list->down_command->down_command = NULL;
	list->count_arg = count;
	return (list->down_command);
}

int					launch_built_in(t_shell *shell_struct)
{
	if (!ft_strcmp("echo", shell_struct->list->arg[0]))
		echo_(shell_struct);
	else if (!ft_strcmp("cd", shell_struct->list->arg[0]))
		cd_(shell_struct);
	else if (!ft_strcmp("env", shell_struct->list->arg[0]))
		env_(shell_struct);
	else if (!ft_strcmp("setenv", shell_struct->list->arg[0]))
		setenv_(shell_struct);
	else if (!ft_strcmp("unsetenv", shell_struct->list->arg[0]))
		unset_(shell_struct);
	else if (!ft_strcmp("exit", shell_struct->list->arg[0]))
		exit_(shell_struct);
	else
		return (0);
	return (1);
}

char			*get_path(char **environ) // доработать
{
	char	*path;
	int i;

	i = 0;
	path = NULL;
	while (environ[i] && !ft_strstr(environ[i], "HOME"))
		++i;
	if (environ[i])
		path = ft_strjoin("/", &environ[i][6]);
	return (path);
}

int					main(int argc, char **argv, char **environ)
{
	char			*line;
	char			**arg;
	int				fd;
	unsigned int	count;
	t_shell			*shell_struct;

	shell_struct = init_shell(environ);
	while (1)
	{
		count = 0;
		ft_printf("$>");
		line = g_line();
		if (ft_strcmp(line, "\0"))
		{
			arg = split_line(line, &count, shell_struct->environ, &fd);
			shell_struct->list = add_list(shell_struct->list, arg, count);
			if (!fd)
			{
				if (!launch_built_in(shell_struct))
					launch_command(shell_struct);
			}
		}
		free(line);
	}
	exit_(shell_struct);
	return (0);
}
