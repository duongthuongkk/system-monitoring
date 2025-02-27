#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void signalHandler(int pid){
    printf("Process with pid = %d has been destroyed!",pid);
    exit(0);
}

int main(int argc, char *argv[]){
    // Kiểm tra số lượng tham số
    if (argc < 2) {
        printf("Usage: %s <PID>\n", argv[0]);
        return 1;
    }

    // Chuyển đổi argv[1] (chuỗi) thành số nguyên (PID)
    int pid = atoi(argv[1]);
    if (pid <= 0) {
        printf("Invalid PID!\n");
        return 1;
    }
    signal(SIGTERM,signalHandler);
    kill(pid, SIGTERM);
    sleep(0.5);

    // If process doesn't exit, use SIGKILL
    if (-1 == kill(pid, 0)) {
        printf("Process %d chưa thoát, gửi SIGKILL...\n", pid);
        kill(pid, SIGKILL);
    } else {
        printf("Process %d đã thoát\n", pid);
    }
    return 0;
}