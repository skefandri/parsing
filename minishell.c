/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysabr <ysabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 11:17:37 by ysabr             #+#    #+#             */
/*   Updated: 2023/06/11 16:59:33 by ysabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"


char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	char	*ptr;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	ptr = malloc (((strlen(s1) + strlen(s2)) + 1) * sizeof(char));
	if (!ptr)
		return (NULL);
	while (s1[i])
	{
		ptr[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		ptr[i + j] = s2[j];
		j++;
	}
	ptr[i + j] = '\0';
	return (ptr);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	char			*ptr;

	if (!s)
		return (0);
	i = strlen(s);
	if (len == 0 || start > i)
		return (strdup(""));
	if (len > i - start)
		len = i - start;
	ptr = malloc ((len + 1) * sizeof(char));
	if (!ptr)
		return (NULL);
	i = 0;
	while (s[i] && (len > 0))
	{
		ptr[i] = s[i + start];
		i++;
		len--;
	}
	ptr[i] = '\0';
	return (ptr);
}

int read_line(char **line)
{
    *line = readline("minishell> ");
    if (!(*line))
        return 1;
    add_history(*line);
    return 0;
}

void display_data(t_data *data)
{
    while (data)
    {
        while (data->cmd && data->cmd->cmd)
        {
            printf("cmd: %s\n", data->cmd->cmd);
            data->cmd = data->cmd->next;
        }
        while (data->file && data->file->filename)
        {
            printf("file: %s | type: %d\n", data->file->filename, data->file->type);
            data->file = data->file->next;
        }
        printf("<-------------------- next node ---------------->\n");
        data = data->next;
    }
}

int main()
{
    while(1)
    {
        char *line;
        if (read_line(&line))
            break;
        if (check_quotes(line))
            continue;
        t_command *lexer = NULL;
        process_tokens(line, &lexer);
        if (check_syntax_errors(lexer))
            continue;
        t_data *data = NULL;
        t_file *file = NULL;
        t_cmd *cmd = NULL;
        while(lexer)
        {
            if (lexer->content->type != 0 && lexer->content->type != 3 && lexer->content->type != 6)
            {
                ft_lstadd_back_file(&file, ft_lstnew_file(lexer->next->content->content, lexer->content->type));
                lexer = lexer->next;
            }
            else if (lexer->content->type == 3)
                 ft_lstadd_back_cmd(&cmd, ft_lstnew_cmd(lexer->content->content));
            if (lexer->content->type == 0 || lexer->content->type == 6)
            {
                ft_lstadd_back_data(&data, ft_lstnew_data(file, cmd));
                file = NULL;
                cmd = NULL;
            }
            lexer = lexer->next;
        }
        t_command *lst = NULL;
        ft_lstadd_back(&lst, ft_lstnew(create_token(' ', ' ')));
        lexer = lst;
        while(lexer)
        {
            t_command *next = lexer->next;
            free(lexer->content->content);
            free(lexer->content);
            free(lexer);
            lexer = next;
        }
        display_data(data);
    }
    return 0;
}


// int    main()
// {
//     while(1)
//     {
//         char *line = readline("minishell> ");
//         if (!line)
//             break;
//         if (line)
//             add_history(line);
//         int i = 0;
//         t_command *lexer = NULL;
//         if (check_quotes(line))
//             continue;
//         t_token *token = NULL;
//         while (line[i])
//         {
//             if (line[i] == '<' || line[i] == '>')
//             {
//                 token = create_token(line[i], line[i+1]);
//                 if (line[i] == line[i + 1])
//                     i+=2;
//                 else
//                     i++;
//             }
//             else if (line[i] == '|')
//                 token = create_token(line[i++], '\0');
//             else if (line[i] != ' ')
//             {
//                 while ((line[i]) && line[i] != '<' && line[i] != '>' && line[i] != '|' && line[i] != ' ')
//                 {
//                     if (!token)
//                         token = create_token(line[i], '\0');
//                     else
//                         token->content = add_char(token->content, line[i]);
//                     i++;
//                 }
//                 if (token)
//                     token->type = TOKEN_STRING;
//             }
//             else
//                 i++;
//             if (token)
//             {
//                 ft_lstadd_back(&lexer, ft_lstnew(token));
//                 token = NULL;
//             }
//         }
//         if (line[i] == '\0')
//         {
//             token = create_token(line[i], line[i]);
//             ft_lstadd_back(&lexer, ft_lstnew(token));
//         }
//         if (check_syntax_errors(lexer))
//             continue ;
//         t_data  *data = NULL;
//         t_file  *file = NULL;
//         t_cmd   *cmd = NULL;
//         while(lexer)
//         {
//             if (lexer->content->type != 0 && lexer->content->type != 3 && lexer->content->type != 6)
//             {
//                 ft_lstadd_back_file(&file, ft_lstnew_file(lexer->next->content->content, lexer->content->type));
//                 lexer = lexer->next;
//             }
//             else if (lexer->content->type == 3)
//                  ft_lstadd_back_cmd(&cmd, ft_lstnew_cmd(lexer->content->content));
//             if (lexer->content->type == 0 || lexer->content->type == 6)
//             {
//                 ft_lstadd_back_data(&data, ft_lstnew_data(file, cmd));
//                 file = NULL;
//                 cmd = NULL;
//             }
//             lexer = lexer->next;
//         }
//         while (data)
//         {
//             while (data->cmd && data->cmd->cmd)
//             {
//                 printf("cmd: %s\n", data->cmd->cmd);
//                 data->cmd = data->cmd->next;
//             }
//             while (data->file && data->file->filename)
//             {
//                 printf("file: %s | type: %d\n", data->file->filename, data->file->type);
//                 data->file = data->file->next;
//             }
//             printf("<-------------------- next node ---------------->\n");
//             data = data->next;
//         }
//     }
// }
