#pragma once

#include <stdio.h>
/*template <typename ...Args>
void cacerr(Args... stuff) {
	printf(stuff...); // to be changed later
}*/

#define cacerr(...) { fprintf(stderr, __VA_ARGS__); abort(); }
// change later pls