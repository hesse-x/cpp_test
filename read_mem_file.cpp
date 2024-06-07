#include <unistd.h>
#include <sys/mman.h>
#include <string>
#include <iostream>
#include <fstream>

int main(int argc, char *argv[]) {
    int memfd = memfd_create("decrypted_shared_library", MFD_CLOEXEC);
    if (memfd == -1) {
        perror("memfd_create");
        return EXIT_FAILURE;
    }

    std::string val{"1\nhaha\nzhe shi yi ge nei cun wen jian\n"};
    write(memfd, val.data(), val.size());
    std::cout << memfd << "?????????\n";
    pid_t pid = getpid();
    std::string file_name{"/proc/"};
    file_name += std::to_string(pid);
    file_name += "/fd/";
    file_name += std::to_string(memfd);
    std::cout << file_name << "\n";
    std::ifstream fin(file_name);
    // read /proc/pid/fd/memfd
    std::string line;
    while (std::getline(fin, line)) {
      std::cout << line << std::endl;
    }
    getchar();
    return EXIT_SUCCESS;
}
