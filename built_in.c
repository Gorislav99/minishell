/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgorczan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/11 15:13:51 by mgorczan          #+#    #+#             */
/*   Updated: 2019/06/11 15:13:52 by mgorczan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo_(t_shell *shell_struct)
{
	int i;
	int	n;

	i = 1;
	n = 0;
	if (shell_struct->list->arg[1] == NULL)
	{
		write(1, "\n", 1);
	}
	else
	{
		while (shell_struct->list->arg[i] && (!ft_strcmp(shell_struct->list->arg[i], "-n") || !ft_strcmp(shell_struct->list->arg[i], "-e")))
			n = !ft_strcmp(shell_struct->list->arg[i++], "-n") ? 1 : 0;
		if (shell_struct->list->arg[i] && !ft_strcmp(shell_struct->list->arg[i], "-"))
			++i;
		if (!shell_struct->list->arg[i] && n)
			return ;
		while (shell_struct->list->arg[i])
		{
			ft_printf("%s", shell_struct->list->arg[i++]);
			if (shell_struct->list->arg[i])
				write(1, " ", 1);
		}
		if (!n)
			write(1, "\n", 1);
		else
			write(1, "%\n", 2);
	}
}

static void	set_pwd(t_shell *shell_struct)
{
	char new_dir[100];
	int i;
	int j;

	i = 0;
	j = 0;
	while (shell_struct->environ[i])
	{
		if (ft_strstr(shell_struct->environ[i], "PWD"))
			break;
		++i;
	}
	while (shell_struct->environ[j])
	{
		if (ft_strstr(shell_struct->environ[j], "OLDPWD"))
			break;
		++j;
	}
	free(shell_struct->environ[j]);
	shell_struct->environ[j] = ft_strjoin("OLDPWD=", &shell_struct->environ[i][4]);
	getcwd(new_dir, 100);
	free(shell_struct->environ[i]);
	shell_struct->environ[i] = ft_strjoin("PWD=", new_dir);
}

static void	back_to_oldpwd(t_shell *shell_struct)
{
	int	j;

	j = 0;
	while (shell_struct->environ[j])
	{
		if (ft_strstr(shell_struct->environ[j], "OLDPWD"))
			break;
		++j;
	}
	chdir(&shell_struct->environ[j][7]);
	if (ft_strstr(&shell_struct->environ[j][7], "/Users/mgorczan"))
		ft_printf("~%s\n", &shell_struct->environ[j][7 + ft_strlen("/Users/mgorczan")]);
	else
		ft_printf("%s\n", &shell_struct->environ[j][7]);
	set_pwd(shell_struct);
}

void	cd_(t_shell *shell_struct)
{
	int k;
	int i;
	int j;

	i = 1;

	if (shell_struct->list->arg[i] && !ft_strcmp(shell_struct->list->arg[i], "--"))
		++i;
	if (shell_struct->list->arg[i] == NULL)
		chdir(get_path(shell_struct->environ)) != -1 ? set_pwd(shell_struct) : 0; // обработать ошибки
	else if (!ft_strcmp(shell_struct->list->arg[i], "-") && shell_struct->list->arg[i + 1] == NULL)
		back_to_oldpwd(shell_struct);
	else if (shell_struct->list->arg[i + 1] != NULL)
	{
		write(2, "cd: string not in pwd: ", ft_strlen("cd: string not in pwd: "));
		write(2, shell_struct->list->arg[i], ft_strlen(shell_struct->list->arg[i]));
		write(2, "\n", 1);
	}
	else
	{
		k = chdir(shell_struct->list->arg[i]);
		k != -1 ? set_pwd(shell_struct) : 0;
		if (k == -1)
		{
			write(2, "cd: no such file or directory: ", ft_strlen("cd: no such file or directory: "));
			write(2, shell_struct->list->arg[i], ft_strlen(shell_struct->list->arg[i]));
			write(2 , "\n", 1);
		}
	}
}

void	exit_(t_shell *shell_struct)
{
	t_command *temp1;
	int			i;

	i = 0;
	while (shell_struct->list)
	{
		temp1 = shell_struct->list;
		shell_struct->list = shell_struct->list->up_command;
		free(temp1->command);
		ft_printf("%i\n", temp1->count_arg);
		while(i < temp1->count_arg)
			free(temp1->arg[i++]);
		free(temp1->arg[i]);
		free(temp1->arg);
		free(temp1);
	}
	i = 0;
	while (i < 7)
		free(shell_struct->built_in[i++]);
	free(shell_struct->built_in);
	i = 0;
	while (shell_struct->environ[i])
		free(shell_struct->environ[i++]);
	free(shell_struct->environ);
	free(shell_struct);
	exit(0);
}