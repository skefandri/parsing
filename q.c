

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_token
{
    enum
    {
        TOKEN_PIPE,
        TOKEN_INFILE,
        TOKEN_OUTFILE,
        TOKEN_STRING,
        TOKEN_HERE_DOC,
        TOKEN_APPEND,
        TOKEN_EOF
    } type;
    char *content;
} t_token;

typedef struct s_command
{
    t_token *content;
    struct s_command *next;
} t_command;


typedef struct s_cmd
{
    char    *cmd;
    struct s_cmd *next;
} t_cmd;

typedef struct s_file
{
    char    *filename;
    int     type;
    struct s_file *next;
} t_file;

typedef struct DATA_STRUCT
{
    t_cmd               *cmd;
    t_file              *file;
    struct DATA_STRUCT  *next;
} t_data;


typedef struct s_process
{
    char    **cmd;
    char    *content;
    char    *line;
    t_token *token;
    struct s_process *next;
} t_process;

t_data	*ft_lstnew_data(t_file *file_name, t_cmd *cmd)
{
	t_data	*new;

	new = malloc(sizeof(t_data));
	if (!new)
		return (NULL);
	new->cmd = cmd;
    new->file = file_name;
	new->next = NULL;
	return (new);
}
void	ft_lstadd_back_data(t_data **lst, t_data *new)
{
	t_data	*ptr;

	if (!lst)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	ptr = *lst;
	while (ptr->next)
	{
		ptr = ptr->next;
	}
	ptr->next = new;
}

t_file	*ft_lstnew_file(char *file_name, int type)
{
	t_file	*new;

	new = malloc(sizeof(t_file));
	if (!new)
		return (NULL);
    new->filename = file_name;
    new->type = type;
	new->next = NULL;
	return (new);
}
void	ft_lstadd_back_file(t_file **lst, t_file *new)
{
	t_file	*ptr;

	if (!lst)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	ptr = *lst;
	while (ptr->next)
	{
		ptr = ptr->next;
	}
	ptr->next = new;
}

t_cmd	*ft_lstnew_cmd(char *cmd)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
    new->cmd = cmd;
	new->next = NULL;
	return (new);
}
void	ft_lstadd_back_cmd(t_cmd **lst, t_cmd *new)
{
	t_cmd	*ptr;

	if (!lst)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	ptr = *lst;
	while (ptr->next)
	{
		ptr = ptr->next;
	}
	ptr->next = new;
}

/* this is the exp */
t_command	*ft_lstnew(t_token *content)
{
	t_command	*new;

	new = malloc(sizeof(t_command));
	if (!new)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}

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

t_process	*new_lstnew(void *content)
{
	t_process	*new;

	new = malloc(sizeof(t_process));
	if (!new)
		return (NULL);
	new->content = content;
    new->line = NULL;
    new->cmd = NULL;
	new->next = NULL;
	return (new);
}

void	ft_lstadd_back(t_command **lst, t_command *new)
{
	t_command	*ptr;

	if (!lst)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	ptr = *lst;
	while (ptr->next)
	{
		ptr = ptr->next;
	}
	ptr->next = new;
}


void	red_lstadd_back(t_process **lst, t_process *new)
{
	t_process	*ptr;

	if (!lst)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	ptr = *lst;
	while (ptr->next)
	{
		ptr = ptr->next;
	}
	ptr->next = new;
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

char *add_char(char *str, char c)
{
    int i = 0;
    char *new;
    if (!str)
    {
        new = malloc(2);
        new[0] = c;
        new[1] = '\0';
    }
    else
    {
        new = malloc(strlen(str) + 2);
        while (str[i])
        {
            new[i] = str[i];
            i++;
        }
        new[i] = c;
        new[i + 1] = '\0';
        free(str);
    }
    return (new);
}

t_token *create_token(char character, char next_char)
{
    t_token *token = malloc(sizeof(t_token));
    if (!token)
        return NULL;
    if (character == '<')
    {
        token->content = add_char(NULL, character);
        if (character == next_char)
        {
            token->content = add_char(token->content, character);
            token->type = TOKEN_HERE_DOC;
        }
        else
            token->type = TOKEN_INFILE;
    }
    else if (character == '>')
    {
        token->content = add_char(NULL, character);
        if (character == next_char)
        {
            token->content = add_char(token->content, character);
            token->type = TOKEN_APPEND;
        }
        else
            token->type = TOKEN_OUTFILE;
    }
    else if (character == '|')
    {
        token->content = add_char(NULL, character);
        token->type = TOKEN_PIPE;
    }
    else if (character == '\0')
    {
        token->content = add_char(NULL, character);
        token->type = TOKEN_EOF;
    }
    else
    {
        token->content = add_char(NULL, character);
        token->type = TOKEN_STRING;
    }
    return token;
}

int check_quotes(char *input)
{
    int i;
    char quote;
    
    i = 0;
    while(input[i])
    {
        if (input[i] == '\'' || input[i] == '"')
        {
            quote = input[i];
            i++;
            while(input[i] && input[i] != quote)
                i++;
            if (!input[i])
            {
                printf("syntax error near unexpected token quotes \n");
                return 1;
            }
        }
        i++;
    }
    return 0;
}

int check_syntax_errors(t_command *lexer)
{
    t_command *current = lexer;
    if (!lexer)
        return 0;
    if (current->content->type == TOKEN_PIPE)
    {
        printf("syntax error near unexpected token %s\n", current->content->content);
        return 1;
    }
    while (current)
    {
        if (current->content->type == TOKEN_INFILE || 
            current->content->type == TOKEN_OUTFILE || current->content->type == TOKEN_HERE_DOC || 
            current->content->type == TOKEN_APPEND)
        {
            if (!current->next || current->next->content->type != TOKEN_STRING)
            {
                printf("syntax error near unexpected token %s\n", current->content->content);
                return 1;
            }
        }
        else if (current->content->type == TOKEN_PIPE)
        {
            if (!current->next || current->next->content->type == TOKEN_PIPE)
            {
                printf("syntax error near unexpected token %s\n", current->content->content);
                return 1;
            }
        }
        current = current->next;
    }
    return (0);
}

int read_line(char **line)
{
    *line = readline("minishell> ");
    if (!(*line))
        return 1;
    add_history(*line);
    return 0;
}

void    process_tokens(char *line, t_command **lexer)
{
    int i = 0;
    t_token *token = NULL;
    while (line[i])
    {
        if (line[i] == '<' || line[i] == '>')
        {
            token = create_token(line[i], line[i+1]);
            if (line[i] == line[i + 1])
                i+=2;
            else
                i++;
        }
        else if (line[i] == '|')
            token = create_token(line[i++], '\0');
        else if (line[i] != ' ')
        {
            while ((line[i]) && line[i] != '<' && line[i] != '>' && line[i] != '|' && line[i] != ' ')
            {
                if (!token)
                    token = create_token(line[i], '\0');
                else
                    token->content = add_char(token->content, line[i]);
                i++;
            }
            if (token)
                token->type = TOKEN_STRING;
        }
        else
            i++;
        if (token)
        {
            ft_lstadd_back(lexer, ft_lstnew(token));
            token = NULL;
        }
    }
    if (line[i] == '\0')
    {
        token = create_token(line[i], line[i]);
        ft_lstadd_back(lexer, ft_lstnew(token));
    }
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
