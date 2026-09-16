/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 21:08:32 by bahbibe           #+#    #+#             */
/*   Updated: 2023/03/06 14:24:28 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	reset_io(int *save)
{
	fflush(stdout);
	dup2(save[0], 0);
	dup2(save[1], 1);
}

void	init_var(int ac, char **av, char **env)
{
	(void)av;
	(void)ac;
	g_global.env_head = 0;
	g_global.error = 0;
	create_env(env);
}

int	*save_(void)
{
	int	*save;

	save = malloc(sizeof(int) * 2);
	save[0] = dup(0);
	save[1] = dup(1);
	return (save);
}

void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_global.error = 1;
	}
}

void	sig_heredoc(int sig)
{
	if (sig == SIGINT)
		exit(EXIT_FAILURE);
}
