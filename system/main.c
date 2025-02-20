#include "include/cpu_info.h"
#include <stdio.h>
#include <string.h>
int count;
char value[1024] = ""; //lưu nội dung của thuộc tính

int main() {
    // Processor
    count = count_info("count","/proc/cpuinfo","processor",9, value);
    printf("- Processor: %d\n",count);

    //Vendor ID
    count_info("value","/proc/cpuinfo","vendor_id",9, value);
    printf("- Vendor ID: %s\n",value);

    // Model
    memset(value,0,sizeof(value));
    count_info("value","/proc/cpuinfo","model",5, value);
    printf("- Model: %s\n",value);

    // Model name
    memset(value,0,sizeof(value));
    count_info("value","/proc/cpuinfo","model name",10, value);
    printf("- Model name: %s\n",value);

    // CPU MHz
    memset(value,0,sizeof(value));
    count_info("value","/proc/cpuinfo","cpu MHz",7, value);
    printf("- CPU MHz: %s\n",value);

     // Cache size
     memset(value,0,sizeof(value));
     count_info("value","/proc/cpuinfo","cache size",10, value);
     printf("- Cache size: %s\n",value);

    // CPU core 
    memset(value,0,sizeof(value));
    count_info("value","/proc/cpuinfo","cpu cores",9, value);
    printf("- CPU cores: %s\n",value);
    return 0;
}
