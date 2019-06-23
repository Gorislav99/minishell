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

static int	launch_built_in(t_shell *shell_struct)
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

static int	several_arg(char **line, char **comm)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while ((*line)[i] != '\n' && (*line)[i] != ';')
		++i;
	if ((*line)[i] == ';')
	{
		if ((*line)[i + 1] == ';' || (*line)[i + 1] == '&' || (*line)[i + 1] == '|')
		{
			temp = ft_strsub(*line, i, i + 2);
			temp[2] = '\0';
			print_error("zsh: parse error near ", temp);
			free(temp);
			return (0);
		}
		*comm = ft_strsub(*line, 0, i + 2);
		(*comm)[i] = '\n';
		(*comm)[i + 1] = '\0';
		j = i + 1;
		while ((*line)[j] != '\n')
			++j;
		temp = *line;
		*line = ft_strsub(temp, i + 1, j + 1);
		free(temp);
	}
	else
	{
		*comm = *line;
		*line = ft_strdup("\n");
	}
	return (1);
}

static void	sigint_()
{
	if (!g_proc)
	{
		ft_printf("\b\b  \n");
		ft_printf("$>");
	}
}




int	main(int argc, char **argv, char **environ)
{
	char	*line;
	char	*comm;
	int		fd;
	t_shell	*shell_struct;

	g_proc = 0;
	shell_struct = init_shell(environ);
	signal(SIGINT, sigint_);
	line = g_line();
	ft_printf("%s\n", line);
	// while (1)
	// {
	// 	g_proc = 0;
	// 	ft_printf("$>");
	// 	line = g_line();
	// 	if (ft_strcmp(line, "\n"))
	// 		shell_struct->list = add_list(shell_struct->list, line);
	// 	while (ft_strcmp(line, "\n"))
	// 	{
	// 		if (!several_arg(&line, &comm))
	// 			break ;
	// 		split_line(shell_struct, comm, &fd);
	// 		if (!fd && !launch_built_in(shell_struct))
	// 			launch_command(shell_struct);
	// 		free_strs(shell_struct->list->arg);
	// 		shell_struct->list->arg = NULL;
	// 		free(comm);
	// 	}
	// 	free(line);
	// }
	return (0);
}
