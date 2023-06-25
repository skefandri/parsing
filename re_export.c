/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   re_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysabr <ysabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 23:06:48 by ysabr             #+#    #+#             */
/*   Updated: 2023/06/23 23:58:41 by ysabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "./includes/minishell.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct s_env
{
    char    *str;
    char    *env;
    char    *value;
    struct s_env *next;
} t_env;

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


void	ft_lstadd_back_env(t_env **lst, t_env *new)
{
	t_env	*ptr;

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

char *get_env(char *str)
{
    char *new;
    int i;
    int j;

    i = 0;
    j = 0;
    while(str[i])
    {
        if (str[i] == '=')
            j = i;
        i++;
    }
    new = ft_substr(str, 0, j);
    if (!new)
        strdup("");
    return (new);
}

char *get_value(char *str)
{
    char *new;
    int i;
    int j;

    i = 0;
    j = 0;
    while(str[i])
    {
        if (str[i] == '=')
            j = i;
        i++;
    }
    new = ft_substr(str, j + 1, i);
    if (!new)
        strdup("");
    return (new);
}
t_env	*ft_lstnew_env(char *str)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->str = strdup(str);
    new->env = get_env(str);
    new->value = get_value(str);
	new->next = NULL;
	return (new);
}
t_env   *set_envs(char **av)
{
    int i;
    t_env   *head;

    i = 0;
    while(av[i])
        ft_lstadd_back_env(&head, ft_lstnew_env(av[i++]));
    return (head);
}

void    declare_envs(t_env *env)
{
    t_env   *tmp;

    tmp = env;
    while (tmp)
    {
        if (tmp->value)
        {
            printf("declare -x %s=\"", tmp->env);
            printf("%s\"\n", tmp->value);
        }
        else
        {
            printf("declare -x %s\n", tmp->env);
        }
        tmp = tmp->next;
    }
}

void    re_export(char **args)
{
    int i;

    i = 0;
    // env = malloc(sizeof(t_env));
    if (!args[i])
    {
        declare_envs(env);
        return ;
    }
    while(args[i])
    {
        if (is_valid_identifier())
    }
}

int main(int ac, char **av, char **env)
{
    t_env   *tmp;
    (void)ac;
    (void)av;

    tmp = set_envs(env);
    //int i = 0;
    // while (tmp)
    // {
    //     printf("str : %s\n", tmp->str);
    //     printf("env : %s\n", tmp->env);
    //     printf("value : %s\n", tmp->value);
    //     tmp = tmp->next;
    // }
    re_export(tmp);
}