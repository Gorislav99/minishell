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

static unsigned int	count_arg(char *line)
{
	unsigned int	i;
	unsigned int	count;
	int fl;

	i = 0;
	count = 0;
	fl = 1;
	while (line[i] != '\n')
	{
		if (line[i] == '\"')
		{
			++i;
			while (line[i] != '\n' && line[i] != '\"')
				++i;
			if (line[i++] == '\n' && fl)
				return (++count);
			if ((line[i] == '\n' || line[i] == ' ' || line[i] == '\t') && fl)
				++count;
			if (line[i] == '\n')
				return (count);
		}
		if (line[i] == '\'')
		{
			++i;
			while (line[i] != '\n' && line[i] != '\'')
				++i;
			if (line[i++] == '\n' && fl)
				return (++count);
			if ((line[i] == '\n' || line[i] == ' ' || line[i] == '\t') && fl)
				++count;
			if (line[i] == '\n')
				return (count);
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

static char			*get_env(char *varible, char **environ)
{
	int	i;
	int	j;

	i = 0;
	while (environ[i])
	{
		j = 0;
		while (environ[i][j] && environ[i][j] != '=' && varible[j] && environ[i][j] == varible[j])
			++j;
		if (environ[i][j] == '=')
			return (&environ[i][ft_strlen(varible) + 1]);
		i++;
	}
	return (NULL);
}

static		char 	*environ_varible(char *line, int *i, int *lenght, char **environ)
{
	int len;
	char		*temp;
	int		j;


	++(*i);
	j = 0;
	len = *i;
	if (!ft_isalpha(line[*i]) && !ft_isdigit(line[*i]))
	{
		*lenght += 1;
		return (ft_strdup("$"));
	}
	while (ft_isalpha(line[len]) || ft_isdigit(line[len]))
		++len;
	if ((temp = ft_strnew(len)) == NULL)
		exit(1);
	// ft_printf("%i %i\n", len, *i);
	while (*i < len)
		temp[j++] = line[(*i)++];
	// ft_printf("%s\n", temp);
	if (get_env(temp, environ))
	{
		*lenght += ft_strlen(get_env(temp, environ));
		return (get_env(temp, environ));
	}
	else
	{
		// ft_printf("!!!!!!!\n");
		*lenght += 1;
		return (" ");
	}

}

static unsigned int lenght_word(char *line, char **environ)
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

static int			dangare_tilda(char *line, int j, int temp)
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

static void			write_word(char **argv, char *line, unsigned int count,  char **environ, int *fd)
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
		temp = lenght_word(&line[j], environ);
		if ((fl = dangare_tilda(line, j, temp)))
		{
			if (ft_isalpha(line[fl + 1]) || (((line[fl + 1] == '\'' || line[fl + 1] == '\"') && fl + 2 < temp) && (ft_isalpha(line[fl + 2]))))
				temp += (ft_strlen("/Users/") - 1);
			else
			{
				if (get_env("HOME", environ))
					temp += (ft_strlen(get_env("HOME", environ)) - 1);
			}
		}
		if ((argv[i] = ft_strnew(temp)) == NULL)
			exit(1);
		while (j2 < temp)
		{
			if (line[j] == '$')
			{
				ft_strcpy(&argv[i][j2], environ_varible(line, &j, &j2, environ));
			}
			if (fl && j == fl)
			{
				if (ft_isalpha(line[j + 1]) || ((line[fl + 1] == '\'' || line[fl + 1] == '\"') && (j + 2 < temp) && (ft_isalpha(line[j + 2]))))
				{
					ft_strcpy(&argv[i][j2], "/Users/");
					j2 += ft_strlen("/Users/");
					fl = -1 * j2;
				}
				else
				{
					if (get_env("HOME", environ) != NULL)
					{
						ft_strcpy(&argv[i][j2], get_env("HOME", environ));
						j2 += ft_strlen(get_env("HOME", environ));
					}
					else
					{
						argv[i][j2] = '~';
						j2++;
					}
				}
				j++;
			}
			else if (line[j] == '\'')
			{
				j++;
				while (j2 < temp && line[j] != '\'')
					argv[i][j2++] = line[j++];
				if (line[j] == '\'')
					++j;
			}
			else if (line[j] == '\"')
			{
				j++;
				while (j2 < temp && line[j] != '\"')
				{
					if (line[j] == '$')
						ft_strcpy(&argv[i][j2], environ_varible(line, &j, &j2, environ));
					else
						argv[i][j2++] = line[j++];
				}
				if (line[j] == '\"')
					++j;
			}
			else if (j2 < temp)
				argv[i][j2++] = line[j++];
		}
		argv[i][j2] = '\0';
		if (fl < 0  && access(argv[i], 0) == -1)
		{
			write(2, "zsh: no such user or named directory: ", ft_strlen("zsh: no such user or named directory: "));
			write(2, &argv[i][-1 * fl], ft_strlen( &argv[i][-1 * fl]));
			write(2, "\n", 1);
			*fd = 1;
		}
		i++;
	}
	argv[i] = NULL;
}

char				**split_line(char *line, unsigned int *count,  char **environ, int *fd)
{
	char			**argv;
	int i = 0;
	int j;

	if (line == NULL)
		return (NULL);
	*count = count_arg(line);
	*fd = 0;
	if ((argv = (char**)malloc(sizeof(char*) * (*count + 1))) == NULL)
		exit(1);
	write_word(argv, line, *count, environ, fd);
	return (argv);
	// return (NULL);
}
