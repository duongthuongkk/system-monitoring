#include "include/device_info.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Variable declaration
int count;

// Main function
int main() {

    // Dynamic allocation
    int n = 1024; //size of buffer
    char *value = (char *)malloc(n * sizeof(char));
    if (value == NULL){
        printf("Fail to allocate memory\n");
        return 1;
    }
    // Processor
    count = get_device_info("/proc/cpuinfo","processor",9, value);
    printf("- Processor: %d\n",count);

    //Vendor ID
    get_device_info("/proc/cpuinfo","vendor_id",9, value);
    printf("- Vendor ID: %s\n",value);

    // Model
    memset(value,0,n);
    get_device_info("/proc/cpuinfo","model",5, value);
    printf("- Model: %s\n",value);

    // Model name
    memset(value,0,n);
    get_device_info("/proc/cpuinfo","model name",10, value);
    printf("- Model name: %s\n",value);

    // CPU MHz
    memset(value,0,n);
    get_device_info("/proc/cpuinfo","cpu MHz",7, value);
    printf("- CPU MHz: %s\n",value);

     // Cache size
     memset(value,0,n);
     get_device_info("/proc/cpuinfo","cache size",10, value);
     printf("- Cache size: %s\n",value);

    // CPU core 
    memset(value,0,n);
    get_device_info("/proc/cpuinfo","cpu cores",9, value);
    printf("- CPU cores: %s\n",value);

    // RAM info
    memset(value,0,n);
    get_device_info("/proc/meminfo","MemTotal",8, value);
    printf("- Total RAM: %s\n",value);

    // Rootfile system info    
    memset(value,0,n);
    create_file("disk", "echo $(df -h / ) >");
    get_device_info("disk.txt","/dev/sda5",9, value);
    printf("- Root volume: %sB\n",value);

    free(value);
    return 0;
}
