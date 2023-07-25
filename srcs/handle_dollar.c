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
            return (str + i);
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

char ***handle_dollar(char ***cmds, char **envcpy)
{
    int i;
    int n;
    int c;
    char **halves;
    char *name;
    char *env;
    char *tmp;

    i = 0;
    c = 0;
    while(cmds[i])
    {
        n = 0;
        while(cmds[i][n])
        {
            while(cmds[i][n][c])
            {
                if(cmds[i][n][c] == '$')
                {
                    halves = cutString(cmds[i][n], c);
                    printf("check env\n");
                    if(find_env_line(halves[1], envcpy) != -1)
                    {
                        env = get_env(find_env_line(halves[1], envcpy), envcpy);
                        tmp = ft_strjoin(halves[0], env);
                    }
                    else
                        cmds[i][n] = ft_strjoin(halves[0], "");
                    printf("halves [0] = %s\n", halves[0]);
                    name = get_name_env(find_env_line(halves[1], envcpy), envcpy);
                    halves[1] = cut_name(halves[1], name);
                    free(cmds[i][n]);
                    printf("before strjoin\n");
                    cmds[i][n] = ft_strjoin(tmp, halves[1]);
                    printf("after strjoin\n");
                    free(tmp);
                    free(name);
                    free(env);
                    if(halves[0])
                        free(halves[0]);
                    if(halves[1])
                        free(halves[1]);
                    free(halves);
                }
                c++;
            }
            c = 0;
            n++;
        }
        i++;
    }
    printf("print triple\n");
    print_triple(cmds);
    return cmds;
}

char *get_env(int i, char **envcpy)
{
    int n;
    int c;
    int tmp;
    char *env;

    n = 0;
    c = 0;
    if(i == -1)
        return (" ");
    while(envcpy[i][n] != '=')
        n++;
    tmp = n + 1;
    while(envcpy[i][n])
    {
        c++;
        n++;
    }
    env = (char *)malloc(sizeof(char) * (c + 1));
    c = 0;
    while(envcpy[i][tmp] != '\0')
    {
        env[c] = envcpy[i][tmp];
        tmp++;
        c++;
    }
    env[c] = '\0';
    return env;
}

int ft_strfcmp(char *s1, char *s2)
{
    int i;

    i = 0;
    while(s1[i] && s2[i])
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