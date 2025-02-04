#ifndef MYASM
# define MYASM
# include <stdio.h>
# include <stddef.h>
extern size_t	asm_strlen(const char *str);
extern ssize_t	asm_write(int fd, const void *buf, size_t count);
extern ssize_t	asm_read(int fd, void *buf, size_t count);
extern int	asm_strcmp(const char *s1, const char *s2);
extern char	*asm_strcpy(char *dest, const char *src);
#endif // !MYASM
