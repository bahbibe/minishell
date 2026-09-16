/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahbibe <bahbibe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 16:31:03 by ybel-hac          #+#    #+#             */
/*   Updated: 2023/03/06 00:46:21 by bahbibe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	init_lexer_node(t_lexer_node **head, char *line, char **env)
{
	int				i;
	int				len;
	char			*start;
	t_lexer			*lexer;
	int				j;
	t_lexer_node	*node;

	lexer = ft_calloc(sizeof(t_lexer), (get_token_len(line) + 1));
	i = 0;
	j = 0;
	while (line[i])
	{
		start = line + i;
		len = get_token_size(line + i, &i);
		lexer[j].content = get_substring(start, len);
		lexer[j].type = get_type(lexer[j].content);
		j++;
	}
	lexer[j].content = 0;
	node = lexer_create_node(head);
	node->lexer = lexer;
	node->lexer_size = get_token_len(line);
	node->env = env;
	free(line);
}
