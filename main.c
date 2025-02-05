#include "libasm.h"
#include <criterion/criterion.h>
#include <criterion/internal/new_asserts.h>
#include <criterion/internal/test.h>
#include <criterion/new/assert.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

Test(strlen, should_be_five) {
	cr_assert(asm_strlen("cinco") == 5, "should be 5");
}

Test(strlen, should_be_0) {
	cr_assert(asm_strlen("") == 0, "should be 0");
}

Test(strlen, should_segfault, .signal = SIGSEGV) {
	asm_strlen(NULL);
}

Test(strlen, should_be_10) {
	cr_assert(asm_strlen("cincocinco") == 10, "should_be_10");
}

Test(write, primary_test) {
	int	pipefd [2];
	char	message[] = "essas duas strings tem que ser iguais";
	char	asm_readbuff[sizeof(message)] = {0};
	char	or_readbuff[sizeof(message)] = {0};
	int	asm_func_return_value;
	int	or_func_return_value;
	int	asm_func_errno;
	int	or_func_errno;

	// create a pipe
	pipe(pipefd);

	// test the asm version and record the values
	asm_func_return_value = asm_write(pipefd[1], message, sizeof(message));
	asm_func_errno = errno;
	read(pipefd[0], asm_readbuff, sizeof(message));

	// test the original version and record the values
	or_func_return_value = write(pipefd[1], message, sizeof(message));
	or_func_errno = errno;
	read(pipefd[0], or_readbuff, sizeof(message));

	// pipe teardown
	close(pipefd[0]);
	close(pipefd[1]);

	// compare original version results against asm version
	cr_assert(eq(int, strcmp(asm_readbuff, or_readbuff), 0), "the messages received should be equal, instead they're:\nasm_readbuff [%s]\nor_readbuff [%s]", asm_readbuff, or_readbuff);
	cr_assert(eq(int, asm_func_return_value, or_func_return_value), "the return values should be equal");
	cr_assert(eq(int, asm_func_errno, or_func_errno), "The errno values should be the same");
}

Test(write, errno_is_set_correctly) {
	int	asm_func_return_value;
	int	or_func_return_value;
	int	asm_func_errno;
	int	or_func_errno;
	char	*ptr;

	ptr = NULL;
	// test the asm version and record the values
	asm_func_return_value = asm_write(1, ptr, 1);
	asm_func_errno = errno;

	// test the original version and record the values
	or_func_return_value = write(1, ptr, 1);
	or_func_errno = errno;

	// compare original version results against asm version
	cr_assert(eq(int, asm_func_return_value, or_func_return_value), "the error return values should be equal");
	cr_assert(eq(int, asm_func_errno, or_func_errno), "The error errno values should be the same");
}

Test(read, primary_test) {
	int	pipefd [2];
	char	message[] = "essas duas strings tem que ser iguais";
	char	asm_readbuff[sizeof(message)] = {0};
	char	or_readbuff[sizeof(message)] = {0};
	int	asm_func_return_value;
	int	or_func_return_value;
	int	asm_func_errno;
	int	or_func_errno;

	// create a pipe
	pipe(pipefd);

	// test the asm version and record the values
	write(pipefd[1], message, sizeof(message));
	asm_func_return_value = asm_read(pipefd[0], asm_readbuff, sizeof(message));
	asm_func_errno = errno;

	// test the original version and record the values
	write(pipefd[1], message, sizeof(message));
	or_func_return_value = asm_read(pipefd[0], or_readbuff, sizeof(message));
	or_func_errno = errno;

	// pipe teardown
	close(pipefd[0]);
	close(pipefd[1]);

	// compare original version results against asm version
	cr_assert(eq(int, strcmp(asm_readbuff, or_readbuff), 0), "the messages received should be equal");
	cr_assert(eq(int, asm_func_return_value, or_func_return_value), "the return values should be equal");
	cr_assert(eq(int, asm_func_errno, or_func_errno), "The errno values should be the same");
}

Test(read, errno_is_set_correctly) {
	int	asm_func_return_value;
	int	or_func_return_value;
	int	asm_func_errno;
	int	or_func_errno;
	char	*ptr;

	ptr = NULL;
	// test the asm version and record the values
	asm_func_return_value = asm_read(-1, ptr, 1);
	asm_func_errno = errno;

	// test the original version and record the values
	or_func_return_value = asm_read(-1, ptr, 1);
	or_func_errno = errno;

	// compare original version results against asm version
	cr_assert(eq(int, asm_func_return_value, or_func_return_value), "the error return values should be equal");
	cr_assert(eq(int, asm_func_errno, or_func_errno), "The error errno values should be the same");
}

Test(strcmp, primary_test) {
	char	s1[] = "cinco";
	char	s2[] = "cinco";
	int	asm_func_return_value;
	int	or_func_return_value;

	asm_func_return_value = asm_strcmp(s1, s2);
	or_func_return_value = strcmp(s1, s2);

	cr_assert(eq(int, asm_func_return_value, or_func_return_value), "The return values should be 0");
}

Test(strcmp, less_than_0) {
	char	s1[] = "cinca";
	char	s2[] = "cinco";
	int	asm_func_return_value;
	int	or_func_return_value;

	asm_func_return_value = asm_strcmp(s1, s2);
	or_func_return_value = strcmp(s1, s2);

	cr_assert(eq(int, asm_func_return_value, or_func_return_value), "The return values should be <0");
}

Test(strcmp, more_than_0) {
	char	s1[] = "cinco";
	char	s2[] = "cinca";
	int	asm_func_return_value;
	int	or_func_return_value;

	asm_func_return_value = asm_strcmp(s1, s2);
	or_func_return_value = strcmp(s1, s2);

	cr_assert(eq(int, asm_func_return_value, or_func_return_value), "The return values should be >0");
}

Test(strmcp, shorter_but_equal) {
	char	s1[] = "cincao";
	char	s2[] = "cinc";
	int	asm_func_return_value;
	int	or_func_return_value;

	asm_func_return_value = asm_strcmp(s1, s2);
	or_func_return_value = strcmp(s1, s2);

	cr_assert(eq(int, asm_func_return_value, or_func_return_value), "The return values should be 0");
}

Test(strcpy, primary_test) {
	char	message1[] = "isso é um teste";
	char	message2[] = "Isso aqui é uma mensagem maior ainda";
	char	message3[] = "E essa é pequena";
	char	or_dest[sizeof(message2) + 1] = {0};
	char	asm_dest[sizeof(message2) + 1] = {0};

	char	*asm_func_return_value;
	char	*or_func_return_value;

	// write message1 into buffers
	asm_func_return_value = asm_strcpy(asm_dest, message1);
	or_func_return_value = strcpy(or_dest, message1);

	cr_assert(eq(ptr, asm_func_return_value, asm_dest), "return value should be dst");
	cr_assert(eq(int, strcmp(asm_dest, or_dest), 0), "the dest buffers should be the same, but are:\nasm_dest [%s]\nor_dest [%s]", asm_dest, or_dest);

	// write message2 into buffers
	asm_func_return_value = asm_strcpy(asm_dest, message2);
	or_func_return_value = strcpy(or_dest, message2);

	cr_assert(eq(ptr, asm_func_return_value, asm_dest), "return value should be dst");
	cr_assert(eq(int, strcmp(asm_dest, or_dest), 0), "the dest buffers should be the same, but are:\nasm_dest [%s]\nor_dest [%s]", asm_dest, or_dest);

	// write message3 into buffers
	asm_func_return_value = asm_strcpy(asm_dest, message3);
	or_func_return_value = strcpy(or_dest, message3);

	cr_assert(eq(ptr, asm_func_return_value, asm_dest), "return value should be dst");
	cr_assert(eq(int, strcmp(asm_dest, or_dest), 0), "the dest buffers should be the same, but are:\nasm_dest [%s]\nor_dest [%s]", asm_dest, or_dest);
}

Test(strcpy, should_segfault, .signal = SIGSEGV) {
	asm_strcpy(NULL, "oie");
}

Test(strdup, simple_dup) {
	char	str1[] = "String numero 1";
	char	str2[] = "Essa é a segunda mensagem em";
	char	*dup;

	dup = asm_strdup(str1);
	cr_assert(eq(int, strcmp(dup, str1), 0), "the buffers should be equal, but are:\noriginal [%s]\ndup [%s]", str1, dup);
	free(dup);

	dup = asm_strdup(str2);
	cr_assert(eq(int, strcmp(dup, str2), 0), "the buffers should be equal, but are:\noriginal [%s]\ndup [%s]", str2, dup);
	free(dup);
}
