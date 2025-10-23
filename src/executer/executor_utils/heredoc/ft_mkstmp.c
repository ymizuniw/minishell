#include "../../../../includes/minishell.h"

int ft_rand(void)
{
    int a = 1003;
    int m = 2147483647;
    int seed = (a * seed) % m;
    int random = seed / m;
    return (random);
}

int ft_mkstmpfd(char *template, int pid)
{
    size_t try_limit = 1000;
    if (template==NULL)
        return (-1);
    while (1)
    {
        char *name = ft_mkstmp(template, pid);
        int fd = open(name, O_CREAT|O_WRONLY|O_EXCL);
        if (fd>=0)
            return (fd);
        free(name);
        try_limit--;
        if (try_limit==0)
            return -1;
    }
    return (-1);
}

//template = "/tmp/heredoc_tmp_XXXXX".
char *ft_mkstmp(char *template, unsigned int num)
{
    static const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    char *p;
    size_t len = strlen(template);
    unsigned int num_keep = num;

    char *name  = strdup(template);
    if (!name)
        return (NULL);
    
    p = name + len - 1;
    while (strchr(name, 'X')!=NULL)
    {
        int random = ft_rand();
        int idx = num / random % (int)(sizeof(charset)-1);
        *p = charset[idx];
        num/=10;
        if (num==0)
            num = num_keep;
    }
    return (name);
}
