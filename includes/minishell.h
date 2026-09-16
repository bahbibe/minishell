/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-hac <ybel-hac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 18:46:57 by ybel-hac          #+#    #+#             */
/*   Updated: 2023/03/08 21:53:20 by ybel-hac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../src/parser/get_next_line/get_next_line.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <dirent.h>
# include <errno.h>
# include <string.h>
# include <stdint.h>

# define LEFT_REDIRECT ">"
# define RIGHT_REDIRECT "<"
# define DOUBLE_QUOTES "\""
# define SINGLE_QUOTES "'"
# define SPACE_STR " "
# define TAB_STR "	"
# define DOLAR "$"
# define FIRST 1
# define INPIPE 2
# define LAST 3
# define SINGLE 4

typedef struct s_get_variable_struct
{
	char	*variable;
	char	*temp;
	char	*final;
	int		i;
	char	*string;
}	t_get_variable_struct;

typedef struct lexer_s
{
	char	*content;
	char	type;
}	t_lexer;

typedef struct s_files
{
	char			*file;
	char			type;
	int				fd;
	struct s_files	*next;
}	t_files;

typedef struct s_cmd
{
	char	**cmd;
	t_files	*files_head;
}	t_cmd;

typedef struct s_mini_env
{
	char				*name;
	char				*value;
	struct s_mini_env	*next;
	struct s_mini_env	*previous;
}	t_mini_env;

typedef struct s_lexer_node
{
	t_lexer				*lexer;
	int					lexer_size;
	int					temp_fd;
	int					expand_here_doc;
	t_cmd				cmd_struct;
	char				**env;
	struct s_lexer_node	*next;
}	t_lexer_node;

typedef t_lexer_node	t_executor;

typedef struct s_g_global
{
	t_mini_env		*env_head;
	t_lexer_node	*head;
	int				open_heredoc;
	int				error;
	int				*save;
	int				inparent;
}	t_global;

# ifndef G_GLOBAL

t_global				g_global;

# endif

void			*ft_memset(void *b, int c, size_t len);
void			*ft_calloc(size_t count, size_t size);
void			tab_free(char **tab);
char			**ft_split(char const *s, char *sep);
int				includes(char c, char *sep);
char			*ft_strdup(const char *s1);
char			*ft_substr(char const *s, unsigned int start, int len);
size_t			ft_strlen(const char *str);
void			add_back_lst(t_lexer_node **head, t_lexer_node *node);
void			add_front_lst(t_lexer_node **head, t_lexer_node *node);
void			lst_clear(t_lexer_node **head);
t_files			*files_create_node(t_lexer_node *current,
					t_files **head, char *file, char type);
void			files_add_back_lst(t_files *head, t_files *node);
t_lexer_node	*lexer_create_node(t_lexer_node **head);
int				ft_strcmp(char *s1, char *s2);
int				ft_strncmp(char *s1, char *s2, int n);
void			init_lexer_node(t_lexer_node **head, char *line, char **env);
void			init_lexer_node(t_lexer_node **head, char *line, char **env);
void			ft_error(char *str, int error);
int				check_quotes(char *str);
int				parser_utils(t_lexer_node **lexer_head, char *line);
void			files_add_back_lst(t_files *head, t_files *node);
t_lexer_node	*lexer_create_node(t_lexer_node **head);
void			env_add_back_lst(t_mini_env *head, t_mini_env *node);
t_mini_env		*env_create_node(char *name, char *value);
void			create_env(char **env);
char			*get_variable_cmd(char *variable);
void			env_del_node(char *variable);
void			echo_cmd(char **args);
char			*get_variable_cmd(char *variable);
void			export_cmd(char **args);
void			unset_cmd(char **args);
void			env_cmd(char c);
void			pwd_cmd(void);
void			exit_cmd(char **args);
void			ft_putstr(char *str, int fd);
int				ft_strchr(char *str, char c);
void			cd_cmd(char **args);
void			modifie_variable(char *variable, char *new_value);
void			free_parser(t_lexer_node *head);
int				get_last_of_var(char *str);
char			*join_string(t_lexer_node *node, int *nb, int flag);
char			*smart_get_variable(char *str, int flag);
int				check_export_syntax(char *str);
int				env_search(char *variable);
long long		ft_atoi(char *nb);
char			*ft_itoa(int n);
void			tab_free(char **tab);
int				get_last_of_var(char *str);
int				get_dolar(char *str);
char			*strjoin_small(char *s1, char c);
int				dolar_work(t_get_variable_struct *utils);
int				speciale_check(char c);
void			skip_files(t_lexer_node *node, int *i);
int				parser_get_size(t_lexer_node *node);
char			get_file_type(t_lexer_node *node, int *i);
int				get_after_file(t_lexer_node *node, int i);
int				lexer_check(char c);
int				get_next_word(char *str, int i);
int				get_token_len(char *str);
int				get_token_size(char *str, int *num);
char			get_type(char *content);
int				check_syntax(char *line);
void			parse_free(t_lexer_node *lexer_head);
int				is_builtin(char *str);
int				ft_strrchr(char *str, char c);
char			*search_in_path(char *cmd);
int				open_file(char *file, char flag);
void			get_builtin(char *cmd, char **args);
char			**ft_split_costom(char *str);
int				check_empty(char *line);
int				open_herdoc(char *limit, int flag);
int				tokens_check(char c, int *nb);
int				executor(t_lexer_node *head);
int				pipeline(t_lexer_node *head);
int				*open_files(t_lexer_node *head);
void			sig_handler(int sig);
void			sig_heredoc(int sig);
void			check_error(char *cmd);
int				executor(t_lexer_node *head);
void			reset_io(int *save);
int				exit_stat(int stat);
void			rl_replace_line(const char *str, int nb);
void			print_lex(t_lexer_node *head);
void			exec_builtin(t_lexer_node *head, int pip[2], int tmp, int flag);
int				cmd_exec(t_lexer_node *head, int pip[2], int tmp, int flag);
void			heredoc_prompt(char *limit, int fd[2], int flag);
int				*open_files(t_lexer_node *head);
int				*dup_files(t_lexer_node *head, int pip[2], int tmp, int flag);
void			get_file_fd(int *files, int *fd_io);
void			init_var(int ac, char **av, char **env);
int				*save_(void);
#endif