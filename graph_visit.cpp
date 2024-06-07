#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <coroutine>
#include <thread>
#include <atomic>
#include <future>
#include <memory>
#include <map>
#include <chrono>
#include <cassert>

// 图节点结构
struct Node {
  int id;
  std::vector<Node *> successors; // 后继节点
};

void print(const Node &node) {
  std::cout << node.id << ": ";
  for (const auto &successor : node.successors) {
    std::cout << successor->id << ", ";
  }
  std::cout << "\n";
}

// 图结构
class Graph {
  public:
    Graph(int n) : nodes(n) {
      for (int i = 0; i < n; i++) {
        nodes[i].id = i;
      }
    }

    void addEdge(int from, int to) {
      nodes[from].successors.push_back(&nodes[to]);
    }

    const Node& getNode(int id) const {
      return nodes[id];
    }

    int size() const {
      return nodes.size();
    }

    const auto &get_nodes() const {
      return nodes;
    }

  private:
    std::vector<Node> nodes;
};

// 协程任务
struct Visitor {
  struct promise_type {
    Visitor get_return_object() {
      return Visitor{ std::coroutine_handle<promise_type>::from_promise(*this) };
    }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() { std::terminate(); }
  };

  std::coroutine_handle<promise_type> coro;

  Visitor(std::coroutine_handle<promise_type> h) : coro(h) {}
  Visitor(const Visitor &other) = delete;
  Visitor(Visitor &&other) {
    std::swap(coro, other.coro);
  }
  ~Visitor() {
    if (coro && coro.done())
      coro.destroy();
  }

  void resume() {
    if (!coro.done()) coro.resume();
  }
};

// 用于协程的同步机制
struct VisitorAwaitable {
  VisitorAwaitable(const int &depend_count)
    : depend_count(depend_count) {}

  bool await_ready() const noexcept { return depend_count == 0; }
  void await_suspend(std::coroutine_handle<> h) {
  }
  void await_resume() noexcept {}

  private:
    const int &depend_count;
};

Visitor visitNode(const Node &node, std::map<const Node *, int> &depend_map_record) {
  co_await VisitorAwaitable(depend_map_record[&node]);
  std::cout << "visit node begin: " << node.id << "--------\n";
  std::this_thread::sleep_for(std::chrono::milliseconds(node.id * 1000));
  print(node);
  for (const auto &successor : node.successors) {
    depend_map_record[successor] -= 1;
  }
  std::cout << "visit node end: " << node.id << "--------\n";
}

void visitGraph(const Graph &graph) {
  std::map<const Node*, int> depend_map;
  for (const auto &node : graph.get_nodes()) {
    depend_map[&node] = 0;
    print(node);
  }

  for (const auto &node : graph.get_nodes()) {
    for (const auto &successor : node.successors) {
      depend_map[successor] += 1;
    }
  }

  std::map<const Node*, int> depend_map_record(depend_map);
  std::vector<Visitor> results;
  for (auto &it : depend_map_record) {
    std::cout << it.first->id << "!!!!!\n";
    results.emplace_back(std::move(visitNode(*it.first, depend_map_record)));
  }

  for (auto &task : results) {
    if (!task.coro.done()) {
      task.coro.resume();
    }
  }
  std::cout << "??????\n";
  // std::this_thread::sleep_for(std::chrono::seconds(10));
}

int main() {
  Graph graph(5);
  graph.addEdge(0, 1);
  graph.addEdge(0, 3);
  graph.addEdge(1, 2);
  graph.addEdge(3, 2);
  graph.addEdge(2, 4);

  visitGraph(graph);

//  // 需要主线程等待一段时间以确保所有协程完成
//  std::this_thread::sleep_for(std::chrono::seconds(1));
  return 0;
}
