/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_line.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgorczan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/14 15:34:19 by mgorczan          #+#    #+#             */
/*   Updated: 2019/06/14 15:34:20 by mgorczan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char			*g_line()
{
	char    *str;
	char    *line;
	char	*temp;

	int     fd;
	
	str = ft_strnew(20);
	line = NULL;
	while ((fd = read(1, str, 20)))
	{
		str[fd] = '\0';
		if (!line)
			line = ft_strdup(str);
		else
		{
			temp = line;
			line = ft_strjoin(line, str);
			free(temp);
		}
		free(str);
		if (ft_strstr(line, "\n"))
			return (line);
		str = ft_strnew(20);

	}
	free(str);
	return (line);
}