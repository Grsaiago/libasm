#include "my_asm.h"
#include <criterion/criterion.h>
#include <signal.h>

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
