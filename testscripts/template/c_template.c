#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <inttypes.h>

int main()
{
    <SETUPCODE>
    <RETTYPE> (*<FNAME>) (<INTYPES>);
    void *handle = dlopen("<LIBRARY>",RTLD_LAZY);
    <FNAME> = dlsym(handle,"<FNAME>");
    printf("<OUTFORMAT>\n",(*<FNAME>)(<INPUT>)<ADDITIONAL_PRINT>);
    dlclose(handle);
    return 0;
}