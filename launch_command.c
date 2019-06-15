/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgorczan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/09 17:01:45 by mgorczan          #+#    #+#             */
/*   Updated: 2019/06/09 17:01:46 by mgorczan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	brute_force_exec(t_shell *shell_struct)
{
	char	*temp;
	char	*path;
	char	*full_path;
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	if (execve(shell_struct->list->command, shell_struct->list->arg, shell_struct->environ) != -1)
			return (1);
	while (shell_struct->environ[i])
	{
		if (ft_strstr(shell_struct->environ[i], "PATH") != NULL)
			break;
		++i;
	}
	if (!shell_struct->environ[i])
	{
		if (execve(full_path, shell_struct->list->arg, shell_struct->environ) != -1)
			return (1);
		else
			return (0);
	}
	while (shell_struct->environ[i][j] && shell_struct->environ[i][j] != '/')
		++j;
	while (shell_struct->environ[i][j])
	{
		k = j;
		while (shell_struct->environ[i][k] != ':' && shell_struct->environ[i][k])
			++k;
		if (shell_struct->environ[i][k] == '\0')
			break;
		temp = ft_strsub(shell_struct->environ[i], j, k - j);
		path = ft_strjoin(temp, "/");
		full_path = ft_strjoin(path, shell_struct->list->command);
		if (execve(full_path, shell_struct->list->arg, shell_struct->environ) != -1)
			break;
		free(path);
		free(full_path);
		free(temp);
		j = k + 1;
	}
	if (!shell_struct->environ[i][j] || !shell_struct->environ[i][k])
		return (0);
	return (1);
}

void		launch_command(t_shell *shell_struct)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		if (!brute_force_exec(shell_struct))
		{
			write(2, "zsh: command not found: ", ft_strlen("zsh: command not found: "));
			write(2, shell_struct->list->command, ft_strlen(shell_struct->list->command));
			write(2, "\n", 1);
		}
	}
	else if (pid > 0)
	{
		wait(NULL);
	}
	else
	{
		write(2, "Error: the process is not created\n", ft_strlen("Error: the process is not created\n"));
	}
	
}