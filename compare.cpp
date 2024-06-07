#include <iostream>
#include <string>
#include <compare>
#include <ranges>
struct A {
  std::strong_ordering operator<=>(const A &other) const {
    auto len = std::min(str.size(), other.str.size());
    for (int i = 0; i < len; i++) {
      if (str[i] < other.str[i])
        return std::strong_ordering::less;
      if (str[i] > other.str[i])
        return std::strong_ordering::greater;
    }
    if (str.size() > other.str.size())
        return std::strong_ordering::greater;
    if (str.size() < other.str.size())
        return std::strong_ordering::less;
    return std::strong_ordering::equal;
  }
  std::string str;
};
int main() {
  auto res = int(3) <=> int(2);
//  std::cout << (res < 0) << std::endl;
//  std::cout << (res > 2) << std::endl;

  A a1{"ab"}, a2{"b"};
  std::cout << (a1 < a2) <<"\n";
  std::cout << (a1 > a2) <<"\n";
}
