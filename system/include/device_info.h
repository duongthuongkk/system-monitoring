#ifndef __CPU_INFO__
#define __CPU_INFO__

int get_device_info(char *path,char *keyword,int keylength, char *value);
int create_file(char *filename, const char * cmd);

#endif
