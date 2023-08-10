#include "../minishell.h"

void free_triple(char ***triple)
{
	int i;
	int j;

	i = 0;
	if(!triple)
		return ;
	while (triple[i])
	{
		j = 0;
		while (triple[i][j])
		{
			free(triple[i][j]);
			j++;
		}
		free(triple[i]);
		i++;
	}
	free(triple);
}

void free_list(t_node **head)
{
	t_node *tmp;

	while (*head)
	{
		tmp = *head;
		*head = (*head)->next;
        if(tmp->cmd)
            free(tmp->cmd);
        if(tmp->args)
            free_array(tmp->args);
		if(tmp->quotes)
			free_array(tmp->quotes);
		free(tmp);
	}
}

void	free_array(char **array)
{
	int i;

	i = 0;
	if(!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
