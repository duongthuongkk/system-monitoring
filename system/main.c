#include "include/cpu_info.h"
#include <stdio.h>
#include <string.h>
int count;
char value[256] = ""; //lưu nội dung của thuộc tính

int main() {
    // Processor
    count = count_info("count","/proc/cpuinfo","processor",9, value);
    printf("Processor: %d\n",count);

    // Model name
    count_info("value","/proc/cpuinfo","model name",10, value);
    printf("Model name: %s\n",value);

    // CPU MHz
    memset(value,0,sizeof(value));
    count_info("value","/proc/cpuinfo","cpu MHz",7, value);
    printf("CPU MHz: %s\n",value);
    return 0;
}
