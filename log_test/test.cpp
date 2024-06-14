#include "log.h"

float f = 1.1;

int main() {
  float t = 2.1;
  LOG(INFO) << t;
  LOG(WARNING) << t;
  LOG(ERROR) << t;
  LOG(FATAL) << t;
}
