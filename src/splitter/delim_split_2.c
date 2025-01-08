/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delim_split_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:56:18 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/22 15:54:52 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_pipe(char c)
{
	return (c == '|');
}

t_token	get_token(char *c)
{
	int	i;

	i = 0;
	if (c[i] == '|' && c[i + 1] != '|')
		return (PIPE);
	else if (c[i] == '>' && c[i + 1] == '>')
		return (APPEND);
	else if (c[i] == '<' && c[i + 1] == '<')
		return (HDOC);
	else if (c[i] == '>' && c[i + 1] != '>')
		return (OUT);
	else if (c[i] == '<' && c[i + 1] != '<')
		return (IN);
	else if (is_builtin(c))
		return (BUILTIN);
	else
		return (WORD);
}

void	add_to_list(t_init_input **head, t_init_input **tail, char *substr,
		t_token token)
{
	t_init_input	*new_node;

	new_node = add_node(substr, token);
	if (!new_node)
		return ;
	if (!*head)
	{
		*head = new_node;
		*tail = new_node;
	}
	else
	{
		(*tail)->next = new_node;
		new_node->prev = *tail;
		*tail = new_node;
	}
}

void	extract_token_wrapper(t_wrapper_context *wrapper_ctx, int start,
								int end)
{
	t_token_context	ctx;

	ctx.head = NULL;
	ctx.tail = NULL;
	ctx.start = 0;
	ctx.end = 0;
	ctx.head = wrapper_ctx->head;
	ctx.tail = wrapper_ctx->tail;
	ctx.start = start;
	ctx.end = end;
	extract_token(&ctx, wrapper_ctx->s);
}

void	extract_token(t_token_context *ctx, char *s)
{
	char	*token;

	token = custom_dup(s, ctx->start, ctx->end);
	add_to_list(ctx->head, ctx->tail, token, get_token(token));
	free(token);
}
