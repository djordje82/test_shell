#include "minishell.h"

char **expand_wildcards(char *pattern)
{
    glob_t  globbuf;
    char    **result;
    int     i;
    int     count;
    
    if (!ft_strchr(pattern, '*'))
        return (NULL);
        
    if (glob(pattern, GLOB_NOSORT, NULL, &globbuf) != 0)
        return (NULL);
        
    count = globbuf.gl_pathc;
    if (count == 0)
    {
        globfree(&globbuf);
        return (NULL);
    }
    
    result = malloc(sizeof(char *) * (count + 1));
    if (!result)
    {
        globfree(&globbuf);
        return (NULL);
    }
    
    i = 0;
    while (i < count)
    {
        result[i] = ft_strdup(globbuf.gl_pathv[i]);
        if (!result[i])
        {
            free_array((void **)result, i);
            globfree(&globbuf);
            return (NULL);
        }
        i++;
    }
    result[count] = NULL;
    
    globfree(&globbuf);
    return (result);
}