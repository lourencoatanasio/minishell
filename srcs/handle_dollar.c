#include "../minishell.h"

char** cutString(char *str, int position)
{
    char** result = (char**)malloc(sizeof(char*) * 2);
    result[0] = (char*)malloc(sizeof(char) * (position + 1));
    result[1] = (char*)malloc(sizeof(char) * (ft_strlen(str) - position + 1));
    ft_strlcpy(result[0], str, position + 1);
    ft_strlcpy(result[1], str + position + 1, ft_strlen(str) - position + 1);
    return result;
}

char *get_name_env(int i, char **envcpy)
{
    int n;
    char *env;

    if(i == -1)
        return NULL;
    n = 0;
    while(envcpy[i][n] != '=')
        n++;
    env = (char *)malloc(sizeof(char) * (n + 1));
    n = 0;
    while(envcpy[i][n] != '=')
    {
        env[n] = envcpy[i][n];
        n++;
    }
    env[n] = '\0';
    return env;
}

char *cut_name(char *str, char *name)
{
    char *result;
    int i;
    int n;

    i = 0;
    n = 0;
    if(!name)
    {
        while(str[i] != '$' && str[i] != '\0')
            i++;
        if(str[i] == '\0')
        {
            return NULL;
        }
        else
        {
            result = (char *)malloc(sizeof(char) * (ft_strlen(str) - i + 1));
            result = ft_strlcpy(result, str + i, ft_strlen(str) - i + 1);
            return (str + i);
        }
    }
    while(str[i] == name[n] && str[i] != '\0' && name[n] != '\0')
    {
        i++;
        n++;
    }
    result = (char *)malloc(sizeof(char) * (ft_strlen(str) - i + 1));
    result = ft_strlcpy(result, str + i, ft_strlen(str) - i + 1);
    free(str);
    return (result);
}

char *get_env_val(int i, char **envcpy)
{
    int n;
    int j;
    char *env;

    if(i == -1)
        return NULL;
    n = 0;
    j = 0;
    while(envcpy[i][n] != '=')
        n++;
    env = (char *)malloc(sizeof(char) * (ft_strlen(envcpy[i]) - n + 1));
    n++;
    printf("n = %d\n", n);
    while(envcpy[i][n] != '\0')
    {
        env[j] = envcpy[i][n];
        printf("env[%d] = %c\n", n - 1, env[j]);
        n++;
        j++;
    }
    env[j] = '\0';
    printf("env = %s\n", env);
    return env;
}

char *ft_getenv(char *name, char **envcpy)
{
    int i;
    char *env;

    printf("ft_getenv\n");
    i = find_env_line(name, envcpy);
    env = get_env_val(i, envcpy);
    return env;
}

void handle_dollar(t_node **head, char **envcpy)
{
    int i;
    int n;
    t_node *tmp;
    char **halves;
    char *env;
    char *str;

    tmp = *head;
    printf("handle_dollar\n");
    while(tmp)
    {
        i = 0;
        while(tmp->args[i])
        {
            n = 0;
            while(tmp->args[i][n])
            {
                if(tmp->args[i][n] == '$')
                {
                    halves = cutString(tmp->args[i], n);
                    env = ft_getenv(halves[1], envcpy);
                }
                n++;
            }
            i++;
        }
        tmp = tmp->next;
    }
    printf("handle_dollar end\n");
}

int ft_strfcmp(char *s1, char *s2)
{
    int i;

    i = 0;
    while(s1[i] && s2[i] && s1[i] != '=' && s2[i] != '$')
    {
        if(s1[i] != s2[i])
            return 1;
        i++;
    }
    printf("exit\n");
    return 0;
}

int find_env_line(char *env, char **envcpy)
{
    int i;

    i = 0;
    while(envcpy[i] != NULL && i <= sizeof_array(envcpy))
    {
        if(ft_strfcmp(envcpy[i], env) == 0)
            return i;
        i++;
    }
    return -1;
}