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

static char	*get_full_path(char *temp, int i, int k, char *arg)
{
	char	*path;
	char	*full_path;

	path = ft_strsub(temp, i, k - i);
	full_path = ft_strnew(ft_strlen(path)
					+ ft_strlen(arg) + 1);
	full_path = ft_strcpy(full_path, path);
	ft_strcpy(full_path + ft_strlen(path), "/");
	ft_strcpy(full_path + ft_strlen(path) + 1, arg);
	free(path);
	return (full_path);
}

static char	*brute_force_exec(t_shell *shell_struct)
{
	char	*temp;
	char	*full_path;
	int		i;
	int		k;

	i = 0;
	if (access(shell_struct->list->arg[0], 1) != -1)
		return (ft_strdup(shell_struct->list->arg[0]));
	if ((temp = get_env("PATH", shell_struct->environ)) == NULL)
		return (NULL);
	while (temp[i])
	{
		k = i;
		while (temp[k] != ':' && temp[k])
			++k;
		full_path = get_full_path(temp, i, k, shell_struct->list->arg[0]);
		if (access(full_path, 1) != -1)
			return (full_path);
		free(full_path);
		i = temp[k] != '\0' ? k + 1 : k;
	}
	return (NULL);
}

static void	sigint_()
{
	exit(0);
}

void		launch_command(t_shell *shell_struct)
{
	pid_t	pid;
	char	*full_path;

	full_path = NULL;
	if (!(full_path = brute_force_exec(shell_struct)))
	{
		write(2, "zsh: command not found: ", 24);
		write(2, shell_struct->list->arg[0],
				ft_strlen(shell_struct->list->arg[0]));
		write(2, "\n", 1);
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, sigint_);
		if (execve(full_path, shell_struct->list->arg,
					shell_struct->environ) != -1)
			return ;
	}
	else if (pid > 0)
	{
		g_proc = 1;
		wait(NULL);
	}
	else
		write(2, "Error: the process is not created\n", 34);
	full_path != NULL ? free(full_path) : 0;
}
