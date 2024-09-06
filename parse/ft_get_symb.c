/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_symb.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: renard <renard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 00:30:08 by renard            #+#    #+#             */
/*   Updated: 2024/07/29 00:30:09 by renard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_check_double_symbols3(char *s, char **cmd)
{
	if (s[0] == '(')
		*cmd = ft_strdup("(");
	else if (s[0] == ')')
		*cmd = ft_strdup(")");
	else if (s[0] == '&' && s[1] != '&')
		*cmd = ft_strdup("&");
	return (1);
}

int	ft_check_double_symbols2(char *s, char **cmd)
{
	if (s[0] == '|' && s[1] == '|')
	{
		*cmd = ft_strdup("||");
		return (2);
	}
	else if (s[0] == '|' && s[1] != '|')
	{
		*cmd = ft_strdup("|");
		return (1);
	}
	else if (s[0] == '&' && s[1] == '&')
	{
		*cmd = ft_strdup("&&");
		return (2);
	}
	return (ft_check_double_symbols3(s, cmd));
}

int	ft_check_double_symbols(char *s, char **cmd)
{
	if (s[0] == '<' && s[1] == '<')
	{
		*cmd = ft_strdup("<<");
		return (2);
	}
	else if (s[0] == '<' && s[1] != '<')
	{
		*cmd = ft_strdup("<");
		return (1);
	}
	else if (s[0] == '>' && s[1] == '>')
	{
		*cmd = ft_strdup(">>");
		return (2);
	}
	else if (s[0] == '>' && s[1] != '>')
	{
		*cmd = ft_strdup(">");
		return (1);
	}
	return (ft_check_double_symbols2(s, cmd));
}
