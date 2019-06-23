/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgorczan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 17:20:13 by mgorczan          #+#    #+#             */
/*   Updated: 2019/06/08 17:20:15 by mgorczan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_arg(char *line)
{
	int		i;
	int		count;
	int		fl;
	char	temp;

	i = 0;
	count = 0;
	fl = 1;
	while (line[i] != '\n')
	{
		if (line[i] == '\"')
			temp = '\"';
		else if (line[i] == '\'')
			temp = '\'';
		if (line[i] == '\"' || line[i] == '\'')
		{
			while (line[i] != '\n' && line[i] != temp)
				++i;
			if (line[i++] == '\n' && fl)
				return (++count);
			if ((line[i] == '\n' || line[i] == ' ' || line[i] == '\t') && fl)
				return (++count);
		}
		if (line[i] != ' ' && line[i] != '\t' && fl)
		{
			count++;
			fl = 0;
		}
		if (line[i] == ' ' || line[i] == '\t')
			fl = 1;
		i++;
	}
	return (count);
}

static char	*environ_varible(char *line, int *i, int *lenght, char **environ)
{
	int		len;
	char	*temp;
	int		j;

	j = 0;
	len = (++(*i));
	if (!ft_isalpha(line[*i]) && !ft_isdigit(line[*i]))
	{
		*lenght += 1;
		return (ft_strdup("$"));
	}
	while (ft_isalpha(line[len]) || ft_isdigit(line[len]))
		++len;
	if ((temp = ft_strnew(len)) == NULL)
		exit(1);
	while (*i < len)
		temp[j++] = line[(*i)++];
	if (get_env(temp, environ))
	{
		*lenght += ft_strlen(get_env(temp, environ));
		return (get_env(temp, environ));
	}
	*lenght += 1;
	return (" ");
}

static int	lenght_word(char *line, char **environ)
{
	int i;
	int lenght;

	i = 0;
	lenght = 0;
	while (line[i] != '\n' && line[i] != ' ' && line[i] != '\t')
	{
		if (line[i] == '$')
			environ_varible(line, &i, &lenght, environ);
		else if (line[i] == '\"')
		{
			++i;
			while (line[i] != '\n' && line[i] != '\"')
			{
				if (line[i] == '$')
				{
					environ_varible(line, &i, &lenght, environ);
				}
				else
				{
					i++;
					lenght++;
				}
			}
			if (line[i] == '\n')
				return (lenght);
			++i;
		}
		else if (line[i] == '\'')
		{
			++i;
			while (line[i] != '\n' && line[i] != '\'')
			{
				i++;
				lenght++;
			}
			if (line[i] == '\n')
				return (lenght);
			++i;
		}
		else if (line[i] != ' ' && line[i] != '\t')
		{
			++lenght;
			++i;
		}
		else
			++i;
	}
	return (lenght);
}

static int	dangare_tilda(char *line, int j, int temp)
{
	int flag1;
	int flag2;
	int flag3;
	int	i;

	flag1 = 0;
	flag2 = 1;
	i = j + temp;
	while (j < i && line[j] != '~')
	{
		if (line[j] == '\"' && !flag1)
			flag1 = 1;
		if (line[j] == '\'' && !flag1)
			flag1 = 2;
		if (line[j] == '\"' && flag1 == 1)
		{
			i++;
			flag2++;
		}
		if (line[j] == '\'' && flag1 == 2)
		{
			i++;
			flag2++;
		}
		if (line[j] != '\"' && line[j] != '\'')
			break ;
		j++;
	}
	if (flag2 % 2 == 0 || line[j] != '~')
		return (0);
	++j;
	if (j >= i || line[j] == '/' || (line[j + 1] != '\n' && line[j + 1] == '/'))
		return (j - 1);
	if (ft_isalpha(line[j]) || (line[j + 1] != '\n' && ft_isalpha(line[j + 1])))
		return (j - 1);
	return (0);
}

static void	write_word(t_shell *shell_struct, char *line, int count, int *fd)
{
	int	i;
	int	j;
	int	j2;
	int	temp;
	int	fl;

	i = 0;
	j = 0;
	fl = 0;
	while (i < count)
	{
		j2 = 0;
		while (line[j] == ' ' || line[j] == '\t')
			j++;
		temp = lenght_word(&line[j], ENV);
		if ((fl = dangare_tilda(line, j, temp)))
		{
			if (ft_isalpha(line[fl + 1]) || (((line[fl + 1] == '\'' ||
			line[fl + 1] == '\"') && fl + 2 < temp) && (ft_isalpha(line[fl + 2]))))
				temp += (ft_strlen("/Users/") - 1);
			else if (get_env("HOME", ENV))
				temp += (ft_strlen(get_env("HOME", ENV)) - 1);
		}
		//---------------------------------------------------------
		if ((ARG[i] = ft_strnew(temp)) == NULL)
			exit(1);
		while (j2 < temp)
		{
			if (line[j] == '$')
				ft_strcpy(&(ARG)[i][j2], environ_varible(line, &j, &j2, ENV));
			if (fl && j == fl)
			{
				if (ft_isalpha(line[j + 1]) || ((line[fl + 1] == '\'' ||
				line[fl + 1] == '\"') && (j + 2 < temp) && (ft_isalpha(line[j + 2]))))
				{
					ft_strcpy(&(ARG)[i][j2], "/Users/");
					j2 += ft_strlen("/Users/");
					fl = -1 * j2;
				}
				else
				{
					if (get_env("HOME", ENV) != NULL)
					{
						ft_strcpy(&(ARG)[i][j2], get_env("HOME", ENV));
						j2 += ft_strlen(get_env("HOME", ENV));
					}
					else
						ARG[i][j2++] = '~';
				}
				j++;
			}
			else if (line[j] == '\'')
			{
				j++;
				while (j2 < temp && line[j] != '\'')
					ARG[i][j2++] = line[j++];
				if (line[j] == '\'')
					++j;
			}
			else if (line[j] == '\"')
			{
				j++;
				while (j2 < temp && line[j] != '\"')
				{
					if (line[j] == '$')
						ft_strcpy(&(ARG)[i][j2],
						environ_varible(line, &j, &j2, ENV));
					else
						ARG[i][j2++] = line[j++];
				}
				if (line[j] == '\"')
					++j;
			}
			else if (j2 < temp)
				ARG[i][j2++] = line[j++];
		}
		ARG[i][j2] = '\0';
		//---------------------------------------------------------
		if (fl < 0 && access(ARG[i], 0) == -1) // yharwyn/Desktop
		{
			print_error("zsh: no such user or named directory: ",
											&(ARG)[i][-1 * fl]);
			*fd = 1;
		}
		i++;
	}
	ARG[i] = NULL;
}

void		split_line(t_shell *shell_struct, char *comm, int *fd)
{
	int		count;

	if (comm == NULL)
		return ;
	count = count_arg(comm);
	if (count == 0)
	{
		*fd = 1;
		return ;
	}
	*fd = 0;
	if ((ARG = (char**)malloc(sizeof(char*) * (count + 1))) == NULL)
		exit(1);
	write_word(shell_struct, comm, count, fd);
}
