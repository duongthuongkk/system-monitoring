#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int count_info(char *typeofAttribute, char *path,char *keyword,int keylength, char *value){
    int fd;
    char buffer[1024];
    ssize_t byteRead;
    off_t offset = 0;
    int count = 0;  // Khởi tạo count
    char *colonPos; 
    char * endPos;
    for (int i = 0; i < sizeof(value); i++) {
        buffer[i] = '\0';
    }
    


    fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("Cannot open this file\n");
        return -1;
    }

    // Đọc file và tìm từ khóa "Processor"
    while (1) {
        // Di chuyển con trỏ file
        if (lseek(fd, offset, SEEK_SET) == -1) {
            perror("Move pointer error!\n");
            close(fd);
            return 1;
        }

        // Đọc dữ liệu vào bộ đệm
        byteRead = read(fd, buffer, sizeof(buffer));
        if (byteRead == -1) {
            perror("Error when reading this file!\n");
            close(fd);
            return 0;
        }

        if (byteRead == 0) {  // Kết thúc file
            break;
        }

        // Tìm kiếm từ khóa "processor" trong bộ đệm
        for (ssize_t i = 0; i < byteRead - keylength + 1; ++i) {
            if (strncmp(&buffer[i],keyword, keylength) == 0) {
                count++;
                if (count == 1 && strcmp(typeofAttribute,value)){       // find value of attributes
                    colonPos= strchr(&buffer[i], *keyword);
                    if (colonPos != NULL) {
                        // Bỏ qua tất cả các ký tự khoảng trắng sau ":"
                        colonPos+= keylength;  // Chuyển con trỏ đến ký tự sau ":"
                        colonPos += strspn(colonPos, "\t: ");  // Bỏ qua tất cả các ký tự " " (dấu cách)
                        if ((endPos = strchr(colonPos, '\n')) != NULL)
                            endPos--;
                            strncpy(value, colonPos, endPos - colonPos + 1);
                    }
                }
            }
        }

        // Cập nhật vị trí con trỏ file
        offset += byteRead;
    }

    // In kết quả
    close(fd);
    return count;       // Return number of object info
}