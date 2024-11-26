#include "minishell.h"

/*This function extracts the name of an environment variable from a string. It returns the name as a new string if found, or NULL if not found.*/
char	*extract_env_var_name(const char *str)
{
	int	i;

	if (!str)
		return (NULL);
	if (str[0] == '?')
		return (ft_strdup("?"));
	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (ft_substr(str, 0, i));
}

/*This function extracts the value of an assignment from a given string. \ 
It returns a new string containing the value, \ 
or an empty string if no assignment is found.*/
char	*extract_env_var_value(char *arg)
{
	char	*equals;

	equals = ft_strchr(arg, '=');
	if (!equals)
		return (ft_strdup(""));
	return (ft_strdup(equals + 1));
}

/*This function checks if a variable name is valid. \ 
It checks if the name is not empty | if the first character is a digit | \ 
if the name contains an equal sign.*/
int	validate_env_var(char *name)
{
	int	i;

	if (!name || !*name || ft_isdigit(name[0]))
		return (0);
	i = 0;
	while (name[i])
	{
		if (name[i] == '=')
		{
			break ;
		}
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	has_equals_sign(char *str)
{
	while (*str)
	{
		if (*str == '=')
			return (1);
		str++;
	}
	return (0);
}

/*This function checks the number of arguments passed to the env command. If there are more than one argument, it prints an error message and returns 0.*/
int	check_env_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	if (i > 1)
	{
		ft_putstr_fd("env: '", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putendl_fd("': too many arguments", STDERR_FILENO);
		return (0);
	}
	return (1);
}