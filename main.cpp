#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include "Graph.h"
#include "RandomGraphGenerator.h"

static long long executeSerialBfsAndGetTime(Graph &g) {
    auto start = std::chrono::steady_clock::now();
    g.bfs(0);
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

static long long executeParallelBfsAndGetTime(Graph &g) {
    auto start = std::chrono::steady_clock::now();
    g.parallelBFS(0); // заглушка
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

struct graph_generation_info {
    int size;
    int connections;
};

void run_and_store_experiment_results(std::vector<graph_generation_info> graph_infos, std::ofstream &fw,
                                      std::ofstream &compute_readable) {
    std::mt19937_64 r(42);
    RandomGraphGenerator gen;

    for (auto [size, connections]: graph_infos) {
        std::cout << "--------------------------\n";
        std::cout << "Generating graph of size " << size << " ... wait\n";
        Graph g = gen.generateGraph(r, size, connections);
        std::cout << "Generation completed!\nStarting bfs\n";
        long long serialTime = executeSerialBfsAndGetTime(g);
        long long parallelTime = executeParallelBfsAndGetTime(g);

        fw << "Times for " << size << " vertices and " << connections << " connections: ";
        fw << "\nSerial: " << serialTime;
        fw << "\nParallel: " << parallelTime;
        fw << "\n--------\n";
        fw.flush();

        compute_readable << size << "," << connections << "," << serialTime << "," << parallelTime << "\n";
        compute_readable.flush();
    }

    std::cout << "Done. Results in tmp/results.txt\n";
}

graph_generation_info create_single_dimension_date(int size) {
    return {size, size * 5};
}
int main() {
    try {
        std::vector<graph_generation_info> graph_infos{
#ifndef ONLY_BIG_GRAPH
            create_single_dimension_date(10),
            create_single_dimension_date(100),
            create_single_dimension_date(1000),
            create_single_dimension_date(10000),
            create_single_dimension_date(50000),
            create_single_dimension_date(100000),
            create_single_dimension_date(1000000),
            create_single_dimension_date(2000000),
            create_single_dimension_date(5000000),
            create_single_dimension_date(10000000),
            create_single_dimension_date(15000000),
#endif
            create_single_dimension_date(20000000),
        };

        std::filesystem::create_directories("tmp");
        std::ofstream fw("tmp/results.txt");
        if (!fw) {
            std::cerr << "Failed to open tmp/results.txt for writing\n";
            return 1;
        }
        std::filesystem::create_directories("results");
        std::ofstream compute_readable("results/results-" RESULTS_SUFFIX ".csv");
        if (!compute_readable) {
            std::cerr << "Failed to open tmp/results.txt for writing\n";
            return 1;
        }

        run_and_store_experiment_results(graph_infos, fw, compute_readable);
    } catch (const std::exception &ex) {
        std::cerr << "Exception: " << ex.what() << "\n";
        return 2;
    }
    return 0;
}
