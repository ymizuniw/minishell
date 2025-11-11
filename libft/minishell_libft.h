
#ifndef MINISHELL_LIBFT_H
# define MINISHELL_LIBFT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

size_t	ft_strlen(const char *s);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
char	*ft_strchr(const char *s, int c);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strdup(const char *s);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	**ft_split(char const *s, char c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strncpy(char *dst, const char *src, size_t n);
int		ft_isspace(int c);
void	*ft_memset(void *s, int c, size_t n);
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
void	*ft_memcpy(void *dest, const void *src, size_t n);
int		ft_isalnum(int c);
void	*ft_calloc(size_t nmemb, size_t size);
void	ft_bzero(void *s, size_t n);
char	*ft_strjoin(char const *s1, char const *s2);
void	*ft_lst_last(void *head, size_t next_offset);
void	ft_lst_add_back(void **head, void *new, size_t next_offset);

#endif
