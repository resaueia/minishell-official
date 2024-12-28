/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delim_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thfranco <thfranco@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 16:40:07 by rsaueia           #+#    #+#             */
/*   Updated: 2024/12/28 14:54:52 by thfranco         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

void	process_pipe_token(t_init_input **head, t_init_input **tail,
			char *s, size_t i)
{
	t_token_context	ctx;

	ctx.head = head;
	ctx.tail = tail;
	ctx.start = i;
	ctx.end = i + 1;
	extract_token(&ctx, s);
}

/* Function: process_pipe_token
 * Handles the processing of pipe or operator tokens by extracting
 * and adding them to the linked list.
 */

static void	handle_pipe_or_end(t_process_context *ctx, char *s, size_t i)
{
	t_token_context	token_ctx;

	if (*(ctx->start) >= 0)
	{
		token_ctx.head = ctx->head;
		token_ctx.tail = ctx->tail;
		token_ctx.start = *(ctx->start);
		token_ctx.end = i + (s[i + 1] == '\0');
		extract_token(&token_ctx, s);
		*(ctx->start) = -1;
	}
	if (is_pipe(s[i]))
		process_pipe_token(ctx->head, ctx->tail, s, i);
}

void	process_current_character(t_process_context *ctx, char *s, size_t i)
{
	if (is_pipe(s[i]) || s[i + 1] == '\0')
		handle_pipe_or_end(ctx, s, i);
}

/* Function: process_current_character
 * Processes the current character in the input string `s`. If it is a
 * pipe or the end of the string, it handles token extraction and delimiter
 * processing. Resets the start index after processing a WORD token.
 */

t_init_input	*delim_split(char *s)
{
	t_init_input		*head;
	t_init_input		*tail;
	t_process_context	ctx;
	size_t				i;
	int					start;

	head = NULL;
	tail = NULL;
	ctx.head = &head;
	ctx.tail = &tail;
	i = 0;
	start = -1;
	ctx.start = &start;
	if (!s)
		return (NULL);
	while (s[i])
	{
		if (!is_pipe(s[i]) && start < 0)
			start = i;
		else
			process_current_character(&ctx, s, i);
		i++;
	}
	return (head);
}

/* Function: delim_split
 * Splits the input string `s` into tokens based on delimiters like `|`, `>`,
 * and `<`. Adds each token to a linked list and returns the head of the list.
 */
