#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    size_t N = 100 * 1024 * 1024;

    char *buf = (char *)malloc(N);

    printf("\nANTES DEL MEMSET\n");
    system("grep -E \"VmRSS|VmPeak\" /proc/self/status");

    memset(buf, 0xAA, N);

    printf("\nDESPUES DEL MEMSET\n");
    system("grep -E \"VmRSS|VmPeak\" /proc/self/status");

    free(buf);

    return 0;
}