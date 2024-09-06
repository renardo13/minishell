/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melmarti <melmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 01:43:57 by renard            #+#    #+#             */
/*   Updated: 2024/08/02 17:59:17 by melmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <errno.h>

static int	ft_redir_echo(t_redir *current, int fd)
{
	while (current)
	{
		if (current->type == R_OUT)
			fd = open(current->next->redir, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		else if (current->type == R_APPEND)
			fd = open(current->next->redir, O_WRONLY | O_APPEND | O_CREAT,
					0644);
		else
		{
			current = current->next;
			continue ;
		}
		if (fd == -1)
		{
			redir_error(current->next->redir);
			return (-1);
		}
		if (!current->next->next)
			break ;
		else
			close(fd);
		current = current->next->next;
	}
	return (fd);
}

int	redir_out(t_cmd *cmd)
{
	int		fd;
	t_redir	*current;

	current = cmd->redir;
	fd = -1;
	if (!cmd->redir)
		return (-1);
	return (ft_redir_echo(current, fd));
}

static int	ft_echo_str(t_cmd *cmd, int option, int i, int fd)
{
	while (cmd->cmd[i])
	{
		ft_putstr_fd(cmd->cmd[i], fd);
		if (cmd->cmd[i][0] && cmd->cmd[i + 1])
			ft_putchar_fd(' ', fd);
		i++;
	}
	if (!option)
		ft_putchar_fd('\n', fd);
	if (cmd->std_out != 1)
	{
		close(cmd->std_out);
		cmd->std_out = 1;
	}
	return (0);
}

static int	ft_handle_option(char **cmd, int *option)
{
	int	i;
	int	j;

	i = 1;
	if (cmd[i][0] == '-')
	{
		while (cmd[i] && cmd[i][0] == '-')
		{
			if (cmd[i][0] == '-' && cmd[i][1] == 'n')
			{
				j = 1;
				while (cmd[i][j] && cmd[i][j] == 'n')
					j++;
				if (!cmd[i][j])
					*option = 1;
				else
					break ;
			}
			else
				break ;
			i++;
		}
	}
	return (i);
}

int	ft_echo(t_cmd *cmd, t_envp **env, int flag)
{
	int	option;
	int	i;
	int	fd;

	option = 0;
	i = 0;
	fd = -1;
	ft_set_fd(flag, cmd, &fd);
	if (!cmd->cmd[1])
	{
		ft_putchar_fd('\n', fd);
		if (fd != STDOUT_FILENO && flag)
			close(fd);
		return (ft_return_code(ft_strdup("0"), env));
	}
	if ((!cmd->cmd[1] || !cmd->cmd || !*cmd->cmd))
		return (ft_echo_return(flag, &fd, env));
	if (cmd->cmd[1])
		i = ft_handle_option(cmd->cmd, &option);
	ft_echo_str(cmd, option, i, fd);
	if (fd != STDOUT_FILENO && flag)
		close(fd);
	return (ft_return_code(ft_strdup("0"), env));
}
