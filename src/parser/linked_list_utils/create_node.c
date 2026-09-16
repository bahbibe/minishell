/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 14:23:20 by ybel-hac          #+#    #+#             */
/*   Updated: 2023/03/06 22:11:29 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_lexer_node	*lexer_create_node(t_lexer_node **head)
{
	t_lexer_node	*node;

	node = malloc(sizeof(t_lexer_node));
	add_back_lst(head, node);
	return (node);
}

t_files	*files_create_node(t_lexer_node *current,
		t_files **head, char *file, char type)
{
	t_files	*node;

	node = malloc(sizeof(t_files));
	if (!*head)
		*head = node;
	else
		files_add_back_lst(*head, node);
	node->file = file;
	node->type = type;
	node->fd = -1;
	if (type == 'H')
	{
		if (current->temp_fd != -1)
			close(current->temp_fd);
		if (current->expand_here_doc)
			node->fd = open_herdoc(file, 1);
		else
			node->fd = open_herdoc(file, 0);
		current->temp_fd = node->fd;
	}
	node->next = 0;
	return (node);
}

t_mini_env	*env_create_node(char *name, char *value)
{
	t_mini_env	*node;

	node = malloc(sizeof(t_mini_env));
	if (!g_global.env_head)
	{
		g_global.env_head = node;
		node->previous = 0;
	}
	else
		env_add_back_lst(g_global.env_head, node);
	node->name = name;
	node->value = value;
	node->next = 0;
	return (node);
}
