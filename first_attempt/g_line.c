/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_line.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgorczan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/23 14:46:05 by mgorczan          #+#    #+#             */
/*   Updated: 2019/06/23 14:46:06 by mgorczan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct termios stored_settings;

static void set_input_mode(void)
{
   struct termios new_settings;

  tcgetattr(STDIN_FILENO, &stored_settings);
  tcgetattr(STDIN_FILENO, &new_settings);
  new_settings.c_lflag &= ~(ICANON);
  tcsetattr (STDIN_FILENO, TCSAFLUSH, &new_settings);
}

static void reset_input_mode(void)
{
	tcsetattr(0,TCSANOW,&stored_settings);
	return;
}

void    delete_char(char *line)
{
    char *temp;

    ft_printf("\b\b\b   \b\b\b");
    if (line != NULL)
    {
        temp = line;
        if (ft_strlen(temp) - 1 != 0)
        {
            line = ft_strsub(temp, 0, ft_strlen(temp) - 1);
        }
        else
            line = NULL;
        free(temp);
    }
}

char	*g_line(void)
{
	char	str[1];
	char	*line;
	char	*temp;
	int		fd;

	line = NULL;
    set_input_mode();
	while ((fd = read(1, str, 1)))
	{
        if (str[0] == 127)
        {
            delete_char(line);
            continue ;
        }
        ft_printf("\n c= %i\n", str[0]);
		if (!line)
			line = ft_strdup(str);
		else
		{
			temp = line;
			line = ft_strjoin(line, str);
			free(temp);
		}
		if (ft_strstr(line, "\n"))
        {
            reset_input_mode();
			return (line);
        }
	}
    reset_input_mode();
	return (line);
}





/*
char	*g_line(void)
{
	char	*str;
	char	*line;
	char	*temp;
	int		fd;

	str = ft_strnew(20);
	line = NULL;
    set_input_mode();
	while ((fd = read(1, str, 20)))
	{
		str[fd] = '\0';
        if (str[0] == 127)
        {
            delete_char();
            continue ;
        }
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
        {
            reset_input_mode();
			return (line);
        }
		str = ft_strnew(20);
	}
    reset_input_mode();
	free(str);
	return (line);
}*/