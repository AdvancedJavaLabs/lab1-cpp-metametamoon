import matplotlib.pyplot as plt
from collections import namedtuple

Experiment = namedtuple("Experiment", "vertices edges serial_time parallel_time")

for file_name in ["results-optimized.csv", "results-unoptimized.csv"]:
    with open(f"results/{file_name}", "r") as f:
        experiments = []
        experiments.clear()
        print(f"Processing {file_name}; exps: {len(experiments)}")
        for line in f:
            v, e, s, p = map(int, line.split(','))
            experiments.append(Experiment(v, e, s, p))
        xs = [exp.edges for exp in experiments]
        ys_serial = [exp.serial_time for exp in experiments]
        ys_parallel = [exp.parallel_time for exp in experiments]
        plt.plot(xs, ys_serial, 'bs', xs, ys_parallel, 'g^')
        plt.legend(['sequential execution', 'parallel execution'])
        plt.title(f"{file_name} - Time vs Edges")
        plt.xlabel("Edges")
        plt.ylabel("Time (ms)")
        plt.savefig(f"figures/{file_name}.png")
        plt.close()


relevant_experiments = []
thread_counts = [1, 2, 4, 8, 12, 16, 20]
for i in thread_counts:
    with open(f"results/results-unoptimized-{i}.csv", "r") as f:
        experiments = []
        experiments.clear()
        for line in f:
            v, e, s, p = map(int, line.split(','))
            experiments.append(Experiment(v, e, s, p))
        relevant_experiments.append(experiments[-1])
ys = [exp.parallel_time for exp in relevant_experiments]

plt.plot(thread_counts, ys, 'bs')
plt.title(f"Performance as a function of number of threads")
plt.xlabel("Number of threads")
plt.ylabel("Time (ms)")
plt.savefig(f"figures/threads-performance-comparison.png")
plt.close()
