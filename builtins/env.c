/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melmarti <melmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 13:17:00 by melmarti          #+#    #+#             */
/*   Updated: 2024/08/02 18:04:00 by melmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_env(t_envp **envp, t_cmd *node, int flag)
{
	if (!*envp || !envp)
	{
		ft_putstr_fd("minishell , environment is empty or null", 2);
		ft_return_code(ft_strdup("127"), envp);
	}
	ft_print_envp(envp, node, flag);
	return (0);
}
