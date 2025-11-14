# libft - Custom C Standard Library

A comprehensive, standalone C library implementing standard C library functions and additional utilities.

## Features

This library includes implementations of:

### String Functions
- `ft_strlen`, `ft_strchr`, `ft_strrchr`, `ft_strcmp`, `ft_strncmp`
- `ft_strdup`, `ft_strndup`, `ft_strncpy`
- `ft_strlcpy`, `ft_strlcat` (BSD-style safe string functions)
- `ft_strstr`, `ft_strnstr`
- `ft_substr`, `ft_strjoin`, `ft_strtrim`, `ft_split`
- `ft_strmapi`, `ft_striteri`

### Character Type Checks
- `ft_isalpha`, `ft_isdigit`, `ft_isalnum`
- `ft_isascii`, `ft_isprint`, `ft_isspace`
- `ft_tolower`, `ft_toupper`

### Memory Functions
- `ft_memset`, `ft_bzero`
- `ft_memcpy`, `ft_memmove`
- `ft_calloc`, `ft_realloc`

### Custom Memory Management
- `xmalloc` - malloc with error checking
- `xcalloc` - calloc with error checking
- `xfree` - NULL-safe free

### Conversion Functions
- `ft_atoi` - string to integer
- `ft_itoa` - integer to string

### Output Functions
- `ft_putchar_fd`, `ft_putstr_fd`
- `ft_putendl_fd`, `ft_putnbr_fd`

### Generic List Functions
- `ft_lst_last` - get last element using offset
- `ft_lst_add_back` - add element to end using offset

### Bonus: get_next_line
- `get_next_line` - read line from file descriptor

## Building

```bash
make        # Build libft.a
make clean  # Remove object files
make fclean # Remove object files and libft.a
make re     # Rebuild from scratch
```

## Usage

```c
#include "libft.h"

int main(void)
{
    char *str = ft_strdup("Hello, World!");
    ft_putendl_fd(str, 1);
    xfree(str);
    return (0);
}
```

Compile with:
```bash
cc your_program.c -L. -lft -o your_program
```

## Requirements

- C compiler (gcc or clang)
- make

## Notes

- This library is self-contained and does not depend on external libraries (except standard C headers)
- All functions follow 42 School coding standards
- Memory management functions (xmalloc, xcalloc, xfree) provide safer alternatives to standard malloc/calloc/free
