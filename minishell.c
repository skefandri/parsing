/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysabr <ysabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 11:17:37 by ysabr             #+#    #+#             */
/*   Updated: 2023/06/13 18:47:12 by ysabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

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

t_data    *get_cmd_file(t_command *lexer, t_data *data, t_file *file, t_cmd *cmd)
{
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
    return (data);
}

int main(int ac, char **av, char **env)
{
    (void)ac;
    (void)av;
    (void)env;
    while(1)
    {
        char *line;
        if (read_line(&line))
            break;
        t_command *lexer = NULL;
        process_tokens(line, &lexer);
        if (check_syntax_errors(lexer) || check_quotes(line))
            continue;
        t_data *data = NULL;
        t_file *file = NULL;
        t_cmd *cmd = NULL;
        data = get_cmd_file(lexer, data, file, cmd);
        t_command *lst = NULL;
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
