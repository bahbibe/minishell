/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 09:59:14 by bahbibe           #+#    #+#             */
/*   Updated: 2023/03/07 19:12:47 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	*open_files(t_lexer_node *head)
{
	int		*fd;
	t_files	*current;

	current = head->cmd_struct.files_head;
	fd = malloc(sizeof(int) * 2);
	fd[0] = -1;
	fd[1] = -1;
	while (current)
	{
		if (current->type == 'H')
			fd[0] = current->fd;
		else if (current->type == 'A')
			fd[1] = open_file(current->file, 'A');
		else if (current->type == 'O')
			fd[1] = open_file(current->file, 'O');
		else if (current->type == 'I')
			fd[0] = open_file(current->file, 'I');
		if (fd[1] == -2)
			return (free(fd), NULL);
		current = current->next;
	}
	return (fd);
}

void	executor_builtin(t_lexer_node *head, int pip[2], int tmp, int flag)
{
	int	*files;

	files = dup_files(head, pip, tmp, flag);
	if (!files)
	{
		g_global.error = 1;
		return ;
	}
	get_builtin(*head->cmd_struct.cmd, head->cmd_struct.cmd);
	reset_io(g_global.save);
	if (files[0] != -1)
		close(files[0]);
	if (files[1] != -1)
		close(files[1]);
	free(files);
}

int	cmd_exec(t_lexer_node *head, int pip[2], int tmp, int flag)
{
	pid_t	pid;
	int		*files;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (is_builtin(*head->cmd_struct.cmd))
		{
			files = dup_files(head, pip, tmp, flag);
			get_builtin(*head->cmd_struct.cmd, head->cmd_struct.cmd);
			reset_io(g_global.save);
			free(files);
			exit(g_global.error);
		}
		if (flag != SINGLE)
			close(pip[0]);
		dup_files(head, pip, tmp, flag);
		if (!*head->cmd_struct.cmd)
			exit (0);
		check_error(*head->cmd_struct.cmd);
		execve(*head->cmd_struct.cmd, head->cmd_struct.cmd, head->env);
		perror("");
		exit(127);
	}
	return (pid);
}

int	pipeline(t_lexer_node *head)
{
	t_lexer_node	*cur;
	int				pip[2];
	int				tmp;
	int				pid;

	tmp = 0;
	cur = head;
	while (cur)
	{
		pipe(pip);
		if (cur == head)
			pid = cmd_exec(cur, pip, tmp, FIRST);
		else if (cur->next == NULL)
			pid = cmd_exec(cur, pip, tmp, LAST);
		else
			pid = cmd_exec(cur, pip, tmp, INPIPE);
		close(tmp);
		tmp = dup(pip[0]);
		close(pip[0]);
		close(pip[1]);
		cur = cur->next;
	}
	return (pid);
}

int	executor(t_lexer_node *head)
{
	int	pip[2];
	int	status;
	int	pid;

	if (head->next == NULL && is_builtin(*head->cmd_struct.cmd))
	{
		g_global.inparent = 0;
		executor_builtin(head, pip, 0, SINGLE);
	}
	else
	{	
		g_global.inparent = 1;
		if (head->next == NULL)
			pid = cmd_exec(head, pip, 0, SINGLE);
		else
			pid = pipeline(head);
		waitpid(pid, &status, 0);
		while (wait(NULL) != -1)
			;
		g_global.error = exit_stat(status);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
			printf("^\\Quit: %d\n", WTERMSIG(status));
	}
	return (0);
}
