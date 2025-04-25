/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albbermu <albbermu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 15:45:04 by fefa              #+#    #+#             */
/*   Updated: 2025/04/25 17:22:45 by albbermu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	parse_word(char **word)
// {
// 	void(**word);
// }

void	double_linked_token(t_token **token)
{
	t_token	*prev;
	t_token	*tmp;

	prev = NULL;
	tmp = *token;
	while (tmp)
	{
		tmp->prev = prev;
		prev = tmp;
		tmp = tmp->next;
	}
}

void	add_token_end(t_token **first, t_token *new)
{
	t_token	*tmp;

	if (!(*first))
	{
		*first = new;
		return ;
	}
	tmp = *first;
	while (tmp ->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	create_node_token(t_token **token, char *str,  int single_quoted, int double_quoted)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	*token = new;
	new->str = str;
	new->is_single_quoted = single_quoted;
	new->is_double_quoted = double_quoted;
	new->next = NULL;
	new->prev = NULL;
}

// void	create_tokens(t_cmd *cmd, t_mini *shell)
// {
// 	t_token	*token;
// 	size_t	i;
// 	char	*cleaned_word;

// 	i = 0;
// 	while (cmd->words[i])
// 	{
// 		if (cmd->words[i][0] == '\'' || cmd->words[i][ft_strlen(cmd->words[i]) - 1] == '\'')
// 			cleaned_word = ft_strtrim(cmd->words[i], "\"");
// 		else
// 			cleaned_word = remove_quotes(cmd->words[i]);
// 		if (!cleaned_word)
// 			return ;
// 		create_node_token(&token, cleaned_word);
// 		add_token_end(&cmd->tokens, token);
// 		i++;
// 	}
// 	double_linked_token(&cmd->tokens);
// 	type_tokens(&cmd->tokens);
// 	(void)shell;
// }

// void	create_tokens(t_cmd *cmd, t_mini *shell)
// {
//     t_token	*token;
//     size_t	i;
//     char	*cleaned_word;

//     i = 0;
//     while (cmd->words[i])
//     {
//         // Check if the word contains quotes but is not fully enclosed by them
//         if (ft_strchr(cmd->words[i], '\'') || ft_strchr(cmd->words[i], '\"'))
//         {
//             // Remove quotes from the word
//             cleaned_word = remove_quotes(cmd->words[i]);
//         }
//         else
//         {
//             // Handle other cases
//             cleaned_word = ft_strdup(cmd->words[i]);
//         }
//         if (!cleaned_word)
//             return ;
//         create_node_token(&token, cleaned_word);
//         add_token_end(&cmd->tokens, token);
//         i++;
//     }
//     double_linked_token(&cmd->tokens);
//     type_tokens(&cmd->tokens);
//     (void)shell;
// }


// void	create_tokens(t_cmd *cmd, t_mini *shell)
// {
//     t_token	*token;
//     size_t	i;
//     char	*cleaned_word;

//     i = 0;
//     while (cmd->words[i])
//     {
//         // Handle fully single-quoted strings (e.g., '$PWD')
//         if (cmd->words[i][0] == '\'' && cmd->words[i][ft_strlen(cmd->words[i]) - 1] == '\'')
//         {
//             // Remove only the surrounding single quotes, keep the content literal
//             cleaned_word = ft_substr(cmd->words[i], 1, ft_strlen(cmd->words[i]) - 2);
//         }
//         // Handle words with mixed quotes (e.g., hello'world')
//         else if (ft_strchr(cmd->words[i], '\'') || ft_strchr(cmd->words[i], '\"'))
//         {
//             // Remove quotes from the word
//             cleaned_word = remove_quotes(cmd->words[i]);
//         }
//         else if (cmd->words[i][0] == '$')
//         {
//             // Expand variables for unquoted words starting with '$'
//             cleaned_word = expand_variable(cmd->words[i], shell);
//         }
//         else
//         {
//             // Handle other cases
//             cleaned_word = ft_strdup(cmd->words[i]);
//         }
//         if (!cleaned_word)
//             return ;
//         create_node_token(&token, cleaned_word);
//         add_token_end(&cmd->tokens, token);
//         i++;
//     }
//     double_linked_token(&cmd->tokens);
//     type_tokens(&cmd->tokens);
//     (void)shell;
// }

// void	create_tokens(t_cmd *cmd, t_mini *shell)
// {
//     t_token	*token;
//     size_t	i;
//     char	*cleaned_word;

//     i = 0;
//     while (cmd->words[i])
//     {
//         // Handle fully single-quoted strings (e.g., '$PWD')
//         if (cmd->words[i][0] == '\'' && cmd->words[i][ft_strlen(cmd->words[i]) - 1] == '\'')
//         {
//             // Remove only the surrounding single quotes, keep the content literal
//             cleaned_word = ft_substr(cmd->words[i], 1, ft_strlen(cmd->words[i]) - 2);
//         }
//         // Handle unquoted words starting with '$' (e.g., $PWD)
//         else if (cmd->words[i][0] == '$' && !ft_strchr(cmd->words[i], '\''))
//         {
//             // Expand variables only for unquoted words
//             cleaned_word = expand_variable(cmd->words[i], shell);
//         }
//         // Handle words with mixed quotes (e.g., hello'world')
//         else if (ft_strchr(cmd->words[i], '\'') || ft_strchr(cmd->words[i], '\"'))
//         {
//             // Remove quotes from the word
//             cleaned_word = remove_quotes(cmd->words[i]);
//         }
//         else
//         {
//             // Handle other cases (e.g., plain words)
//             cleaned_word = ft_strdup(cmd->words[i]);
//         }
//         if (!cleaned_word)
//             return ;
//         create_node_token(&token, cleaned_word);
//         add_token_end(&cmd->tokens, token);
//         i++;
//     }
//     double_linked_token(&cmd->tokens);
//     type_tokens(&cmd->tokens);
//     (void)shell;
// }


// void	create_tokens(t_cmd *cmd, t_mini *shell)
// {
//     t_token	*token;
//     size_t	i;
//     char	*cleaned_word;

//     i = 0;
//     while (cmd->words[i])
//     {
//         // Handle fully single-quoted strings (e.g., '$PWD')
//         if (cmd->words[i][0] == '\'' && cmd->words[i][ft_strlen(cmd->words[i]) - 1] == '\'')
//         {
//             // Remove only the surrounding single quotes, keep the content literal
//             cleaned_word = ft_substr(cmd->words[i], 1, ft_strlen(cmd->words[i]) - 2);
//         }
//         // Handle unquoted words starting with '$' (e.g., $PWD)
//         else if (cmd->words[i][0] == '$')
//         {
//             // Expand variables only for unquoted words
//             cleaned_word = expand_variable(cmd->words[i], shell);
//         }
//         // Handle words with mixed quotes (e.g., hello'world')
//         else if (ft_strchr(cmd->words[i], '\'') || ft_strchr(cmd->words[i], '\"'))
//         {
//             // Remove quotes from the word
//             cleaned_word = remove_quotes(cmd->words[i]);
//         }
//         else
//         {
//             // Handle other cases (e.g., plain words)
//             cleaned_word = ft_strdup(cmd->words[i]);
//         }
//         if (!cleaned_word)
//             return ;
//         create_node_token(&token, cleaned_word);
//         add_token_end(&cmd->tokens, token);
//         i++;
//     }
//     double_linked_token(&cmd->tokens);
//     type_tokens(&cmd->tokens);
//     (void)shell;
// }

// void create_tokens(t_cmd *cmd, t_mini *shell)
// {
// 	t_token *token;
// 	size_t i = 0;
// 	char *word;

// 	while (cmd->words[i])
// 	{
// 		int single_q = 0, double_q = 0;
// 		word = cmd->words[i];

// 		if (word[0] == '\'' && word[ft_strlen(word) - 1] == '\'')
// 		{
// 			word = ft_substr(word, 1, ft_strlen(word) - 2);
// 			single_q = 1;
// 		}
// 		else if (word[0] == '\"' && word[ft_strlen(word) - 1] == '\"')
// 		{
// 			word = ft_substr(word, 1, ft_strlen(word) - 2);
// 			double_q = 1;
// 		}
// 		else if (ft_strchr(word, '\'') || ft_strchr(word, '"'))
// 		{
// 			word = remove_quotes(word);
// 		}
// 		else
// 			word = ft_strdup(word);

// 		create_node_token(&token, word, single_q, double_q);
// 		add_token_end(&cmd->tokens, token);
// 		i++;
// 	}

// 	double_linked_token(&cmd->tokens);
// 	type_tokens(&cmd->tokens);

// 	// Clearly handle expansion
// 	token = cmd->tokens;
// 	while (token)
// 	{
// 		if (!token->is_single_quoted)
// 		{
// 			char *expanded = expand_variable(token->str, shell);
// 			free(token->str);
// 			token->str = expanded;
// 		}
// 		token = token->next;
// 	}
// }


void create_tokens(t_cmd *cmd, t_mini *shell)
{
    t_token *token;
    size_t i = 0;
    char *word;

    while (cmd->words[i])
    {
        int single_q = 0, double_q = 0;
        word = cmd->words[i];

        // Handle fully single-quoted strings
        if (word[0] == '\'' && word[ft_strlen(word) - 1] == '\'')
        {
            word = ft_substr(word, 1, ft_strlen(word) - 2);
            single_q = 1;
        }
        // Handle fully double-quoted strings
        else if (word[0] == '\"' && word[ft_strlen(word) - 1] == '\"')
        {
            word = ft_substr(word, 1, ft_strlen(word) - 2);
            double_q = 1;
        }
        // Handle words with mixed quotes
        else if (ft_strchr(word, '\'') || ft_strchr(word, '"'))
        {
            word = remove_quotes(word);
        }
        else
        {
            // Handle unquoted words
            word = ft_strdup(word);
        }

        create_node_token(&token, word, single_q, double_q);
        add_token_end(&cmd->tokens, token);
        i++;
    }

    double_linked_token(&cmd->tokens);
    type_tokens(&cmd->tokens);

    // Handle variable expansion
    token = cmd->tokens;
    while (token)
    {
        // Only expand variables for tokens that are NOT single-quoted
        if (!token->is_single_quoted)
        {
            char *expanded = expand_variable(token->str, shell);
            free(token->str);
            token->str = expanded;
        }
        token = token->next;
    }
}
