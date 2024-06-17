#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <sys/mman.h>
#include <unistd.h>

TEST(TEST0, TEST0) {
  int memfd = memfd_create("decrypted_shared_library", MFD_CLOEXEC);
  if (memfd == -1) {
    perror("memfd_create");
    ASSERT_TRUE(false);
  }

  std::string val{"1\nhaha\nzhe shi yi ge nei cun wen jian\n"};
  write(memfd, val.data(), val.size());
  // std::cout << memfd << "?????????\n";
  pid_t pid = getpid();
  std::string file_name{"/proc/"};
  file_name += std::to_string(pid);
  file_name += "/fd/";
  file_name += std::to_string(memfd);
  // std::cout << file_name << "\n";
  std::ifstream fin(file_name);
  // read /proc/pid/fd/memfd
  std::string line;
  std::stringstream os;
  while (std::getline(fin, line)) {
    os << line << '\n';
  }
  EXPECT_EQ(os.str(), val);
}
