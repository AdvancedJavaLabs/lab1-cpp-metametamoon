CXX=clang++-16
SOURCES=main.cpp Graph.cpp RandomGraphGenerator.cpp
SOURCES_WITH_ERROR=main.cpp GraphTainted.cpp RandomGraphGenerator.cpp
all: bin/main bin/main-sanitized bin/main-sanitized-with-error bin/main-unoptimized bin/main-unoptimized-1 bin/main-unoptimized-2 bin/main-unoptimized-4 bin/main-unoptimized-8 bin/main-unoptimized-12 bin/main-unoptimized-16 bin/main-unoptimized-20

bin/main: $(SOURCES)
	$(CXX) -std=c++20 -O3 $(SOURCES) -DTHREAD_COUNT=12 -DRESULTS_SUFFIX=\"optimized\" -o $@

bin/main-unoptimized: $(SOURCES)
	clang++-16 -std=c++20 -O0 $(SOURCES) -DTHREAD_COUNT=12 -DRESULTS_SUFFIX=\"unoptimized\" -o $@

bin/main-sanitized: $(SOURCES)
	clang++-16 -std=c++20 -Og -g $(SOURCES) -fsanitize=thread -DTHREAD_COUNT=12 -DRESULTS_SUFFIX=\"sanitized\" -o $@

bin/main-sanitized-with-error: $(SOURCES_WITH_ERROR)
	clang++-16 -std=c++20 -Og -g $(SOURCES_WITH_ERROR) -fsanitize=thread -DTHREAD_COUNT=12 -DRESULTS_SUFFIX=\"sanitized-with-error\" -o $@

bin/main-with-error: $(SOURCES_WITH_ERROR)
	clang++-16 -std=c++20 -O0 $(SOURCES_WITH_ERROR) -DTHREAD_COUNT=12 -DRESULTS_SUFFIX=\"sanitized-with-error\" -o $@

bin/main-unoptimized-1: $(SOURCES)
	clang++-16 -std=c++20 -O0 $(SOURCES) -DTHREAD_COUNT=1 -DRESULTS_SUFFIX=\"unoptimized-1\" -DONLY_BIG_GRAPH -o $@

bin/main-unoptimized-2: $(SOURCES)
	clang++-16 -std=c++20 -O0 $(SOURCES) -DTHREAD_COUNT=2 -DRESULTS_SUFFIX=\"unoptimized-2\" -DONLY_BIG_GRAPH -o $@

bin/main-unoptimized-4: $(SOURCES)
	clang++-16 -std=c++20 -O0 $(SOURCES) -DTHREAD_COUNT=4 -DRESULTS_SUFFIX=\"unoptimized-4\" -DONLY_BIG_GRAPH -o $@

bin/main-unoptimized-8: $(SOURCES)
	clang++-16 -std=c++20 -O0 $(SOURCES) -DTHREAD_COUNT=8 -DRESULTS_SUFFIX=\"unoptimized-8\" -DONLY_BIG_GRAPH -o $@

bin/main-unoptimized-12: $(SOURCES)
	clang++-16 -std=c++20 -O0 $(SOURCES) -DTHREAD_COUNT=12 -DRESULTS_SUFFIX=\"unoptimized-12\" -DONLY_BIG_GRAPH -o $@

bin/main-unoptimized-16: $(SOURCES)
	clang++-16 -std=c++20 -O0 $(SOURCES) -DTHREAD_COUNT=16 -DRESULTS_SUFFIX=\"unoptimized-16\" -DONLY_BIG_GRAPH -o $@

bin/main-unoptimized-20: $(SOURCES)
	clang++-16 -std=c++20 -O0 $(SOURCES) -DTHREAD_COUNT=20 -DRESULTS_SUFFIX=\"unoptimized-20\" -DONLY_BIG_GRAPH -o $@

clean:
	rm bin/*