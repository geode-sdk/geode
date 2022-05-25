#pragma once

#include <stdio.h>
/*template <typename ...Args>
void codegenerr(Args... stuff) {
	printf(stuff...); // to be changed later
}*/

#define codegenerr(...) { fprintf(stderr, __VA_ARGS__); abort(); }
// change later pls
