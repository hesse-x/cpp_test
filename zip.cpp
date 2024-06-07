#include <ranges>
#include <array>
#include <string_view>
#include <iostream>
template <std::ranges::range... Args>
auto zip(Args&&... args) {
  return std::views::zip(std::forward<Args>(args)...);
}

template<typename T>
concept non_range = !std::ranges::range<T>;

template <non_range... Args>
auto zip(Args&&... args) {
  return std::tuple<Args&&...>(std::forward<Args>(args)...);
}

int main() {
  std::string_view a("abc");
  std::array<int, 3> b = {1, 2, 3};
  auto view = zip(a, b);
  for (auto val : zip(a, b)) {
    auto [i1, i2] = val;
    std::cout << std::get<0>(val) << ": " << std::get<1>(val) << std::endl;
  }
  std::cout << "------------" << std::endl;
  auto [v1, v2] = zip(1, 3);
  std::cout << v1 << ": " << v2 << std::endl;
}
