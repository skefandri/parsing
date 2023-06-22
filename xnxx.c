#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *get_env_val(const char *key, char **env);
char *replace_variables(const char *str, char **env);
char *get_double_quoted_variable(const char *str, char **env);
char *get_single_quoted_variable(const char *str);
char *ft_strjoin(const char *s1, const char *s2);

char *get_env_val(const char *key, char **env)
{
    size_t len = strlen(key);
    printf("key : %s\n", key);
    printf("Len : %zu\n", len);
    for (int i = 0; env[i] != 0; i++)
    {
        if (strncmp(key, env[i], len) == 0 && env[i][len] == '=')
        {
            return &env[i][len + 1];
        }
    }
    return NULL;
}

char *replace_variables(const char *str, char **env)
{
    size_t len = strlen(str);
    char *newStr = malloc((2 * len + 1) * sizeof(char));
    if (newStr == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

    int inside_double_quote = 0;
    int inside_single_quote = 0;

    int i = 0, j = 0;
    while (str[i])
    {
        if (str[i] == '"')
        {
            inside_double_quote = !inside_double_quote;
            i++;
        }
        else if (str[i] == '\'')
        {
            inside_single_quote = !inside_single_quote;
            i++;
        }
        else if (str[i] == '$' && inside_single_quote)
            newStr[j++] = str[i++];
        else if (str[i] == '$' && !inside_single_quote)
        {
            printf("hello\n");
            if (str[i+1] == '$')
            {
                newStr[j++] = str[i++];
                newStr[j++] = str[i++];
            }
            else if (str[i+1] == '/' || str[i+1] == '\\')
            {
                newStr[j++] = str[i++];
                newStr[j++] = str[i++];
                while (str[i] != '.' && str[i] != '$' && str[i] != '\'' && str[i] != '"' && str[i])
                    newStr[j++] = str[i++];
            }
            else if (str[i+1] == '?')
            {
                newStr[j++] = '0';
                i += 2;
                while (str[i] != '.' && str[i] != '$' && str[i] != '\'' && str[i] != '"' && str[i])
                    newStr[j++] = str[i++];
            }
            else
            {
                i++;
                printf("-------.......???.-------======>>>>>%s\n",str);
                while (str[i] != '.' && str[i] != '$' && str[i] != '\'' && str[i] != '"' && str[i])
                    newStr[j++] = str[i++];
                newStr[j] = '\0';
                printf("-------.==============-------======>>>>>%s\n",newStr);
                printf("------->%s\n", newStr);
                char *envVal = get_env_val(newStr, env);
                printf("======>>>>>%s\n", envVal);
                printf("8888---=====>>>>%s\n", envVal);
                if (envVal != NULL && (inside_double_quote || !inside_double_quote))
                {
                    char *tmpStr = strdup(envVal);
                    if (tmpStr == NULL)
                    {
                        fprintf(stderr, "Memory allocation failed.\n");
                        free(newStr);
                        return NULL;
                    }
                    free(newStr);
                    newStr = tmpStr;
                    printf("------>>>>>%s\n", newStr);
                    j = strlen(newStr);
                    free(newStr);
                }
                else if (envVal != NULL && (!inside_double_quote || inside_double_quote))
                {
                    newStr[j] = '\0';
                }
            }
        }
        else
            newStr[j++] = str[i++];
    }
    newStr[j] = '\0';

    return newStr;
}


char *ft_strjoin(const char *s1, const char *s2)
{
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    char *result = malloc((len1 + len2 + 1) * sizeof(char));
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

    strcpy(result, s1);
    strcat(result, s2);

    return result;
}

int main(int argc, char **argv, char **env)
{
    (void)argc;
    (void)argv;

    char *str = "$USER $PWD";
    char *newStr = replace_variables(str, env);
    if (newStr == NULL) {
        fprintf(stderr, "Error occurred during variable replacement.\n");
        return 1;
    }

    printf("%s\n", newStr);

    free(newStr);
    return 0;
}
