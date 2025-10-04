#include "Graph.h"
#include <algorithm>
#include <atomic>
#include <queue>
#include <thread>


Graph::Graph(int vertices) : V(vertices), adjList(vertices) {
}

void Graph::addEdge(int src, int dest) {
    if (src < 0 || dest < 0 || src >= V || dest >= V) return;
    auto &vec = adjList[src];
    if (std::find(vec.begin(), vec.end(), dest) == vec.end()) {
        vec.push_back(dest);
    }
}

using i64 = int64_t;

template<typename T>
struct safe_vector {
    std::vector<T> vec; // must have enough vec.size to store all the elements
    i64 actual_size = 0;

    void push_back(T const &element) {
        auto index = actual_size++;
        vec.at(index) = element;
    }
};

struct my_thread_work {
    i64 begin;
    i64 end; // exclusive
};

// final is exlusive
std::vector<my_thread_work> split_range(i64 size, i64 thread_count) {
    std::vector<my_thread_work> result;
    i64 single_thread_work_size = size / thread_count;
    result.reserve(thread_count);
    for (i64 i = 0; i < thread_count - 1; i++) {
        result.push_back({i * single_thread_work_size, (i + 1) * single_thread_work_size});
    }
    result.push_back({(thread_count - 1) * single_thread_work_size, size});
    return result;
}


void Graph::parallelBFS(int startVertex) {
    std::vector<std::atomic_bool> visited(V);
    for (i64 i = 0; i < V; ++i) {
        visited[i] = false;
    }
    safe_vector<int> current_level{std::vector<int>(V, 0)};
    safe_vector<int> next_level{std::vector<int>(V, 0)};
    current_level.push_back(startVertex);
    auto const &myAdjList = adjList;
    while (true) {
        auto handle = [&current_level, &visited, &next_level, &myAdjList](i64 begin, i64 end) {
            for (i64 i = begin; i < end; ++i) {
                for (int v: myAdjList.at(current_level.vec.at(i))) {
                    auto isVisited = visited.at(v).exchange(true);
                    if (!isVisited) {
                        next_level.push_back(v);
                    }
                }
            }
        };
        std::vector<std::thread> threads;
        auto my_thread_works = split_range(current_level.actual_size, THREAD_COUNT);
        for (auto work_range: my_thread_works) {
            auto work = [work_range, &handle]() { handle(work_range.begin, work_range.end); };
            threads.emplace_back(work);
        }
        for (auto &thread: threads) {
            thread.join();
        }

        if (next_level.actual_size == 0) break;
        std::swap(current_level.vec, next_level.vec);
        current_level.actual_size = next_level.actual_size;
        next_level.actual_size = 0;
    }
}

void Graph::bfs(int startVertex) {
    if (startVertex < 0 || startVertex >= V) return;
    std::vector<char> visited(V, 0);
    std::queue<int> q;

    visited[startVertex] = 1;
    q.push(startVertex);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int n: adjList[u]) {
            if (!visited[n]) {
                visited[n] = 1;
                q.push(n);
            }
        }
    }
}

int Graph::vertices() const { return V; }
