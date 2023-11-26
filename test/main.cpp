#include <stdio.h>

#define F(...) printf("%s%s%s\n" __VA_ARGS__)


int main()
{
    F();
    return 0;
}