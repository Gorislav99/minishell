/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgorczan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/13 19:42:44 by mgorczan          #+#    #+#             */
/*   Updated: 2019/06/13 19:42:45 by mgorczan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_(t_shell *shell_struct)
{
	int i;

	i = 0;
	while (shell_struct->environ[i])
		ft_printf("%s\n", shell_struct->environ[i++]);
}

void	setenv_(t_shell *shell_struct)
{
	int		i;
	int		j;
	char	**temp;
	char 	*tm;
	char 	*tm2;

	i = 0;
	j = 0;
	if (shell_struct->list->arg[1] == NULL)
	{
		write(2, "setenv: not enough arguments\n", ft_strlen("setenv: not enough arguments\n"));
		return ;
	}
	if (!ft_strcmp(shell_struct->list->arg[1], "="))
	{
		write(2, "zsh: bad assignment\n", ft_strlen("zsh: bad assignment\n"));
		return ;
	}
	if (shell_struct->list->arg[2] == NULL)
	{
		if (ft_strstr(shell_struct->list->arg[1], "="))
			tm = ft_strdup(shell_struct->list->arg[1]);
		else
			tm = ft_strjoin(shell_struct->list->arg[1], "=");
	}
	else if (shell_struct->list->arg[3] == NULL)
	{
		tm2 = ft_strjoin(shell_struct->list->arg[1], "=");
		tm = ft_strjoin(tm2, shell_struct->list->arg[2]);
		free(tm2);
	}
	else
		return ;
	while (shell_struct->environ[i])
	{
		while (shell_struct->environ[i][j] == tm[j] && tm[j] != '=')
			++j;
		if (tm[j] == '=')
		{
			free(shell_struct->environ[i]);
			shell_struct->environ[i] = tm;
			return ;
		}
		++i;
	}
	j = -1;
	if ((temp = (char**)malloc(sizeof(char*) * (i + 2))) == NULL)
		exit(1);
	temp[i + 1] = NULL;
	while (++j < i)
		temp[j] = shell_struct->environ[j];
	temp[j] = tm;
	free(shell_struct->environ);
	shell_struct->environ = temp;
}

void	unset_(t_shell *shell_struct)
{
	int i;
	int	j;
	int k;
	char	**temp;
	int		count;

	i = 0;
	count = 0;
	if (!shell_struct->list->arg[1])
	{
		write(2, "unset: not enough arguments\n", ft_strlen("unset: not enough arguments\n"));
		return ;
	}
	while (shell_struct->environ[i])
	{
		j = 1;		
		while (shell_struct->list->arg[j])
		{
			k = 0;
			while (shell_struct->environ[i][k] != '=' && shell_struct->list->arg[j][k] == shell_struct->environ[i][k])
				++k;
			if (shell_struct->environ[i][k] == '=')
			{
				free(shell_struct->environ[i]);
				shell_struct->environ[i] = ft_strdup("\0");
				++count;
			}
			++j;
		}
		++i;
	}
	if ((temp = (char**)malloc(sizeof(char*) * (i + 1 - count))) == NULL)
		exit(1);
	j = 0;
	k = 0;
	while (k < i)
	{
		if (shell_struct->environ[k][0] != '\0')
			temp[j++] = shell_struct->environ[k];
		else
			free(shell_struct->environ[k]);
		++k;
	}
	temp[j] = NULL;
	free(shell_struct->environ);
	 shell_struct->environ = temp;
}
