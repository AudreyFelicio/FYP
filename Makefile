CC = gcc
CXX = g++

CFLAGS := $(CFLAGS) -O3 -Wall -Wextra -pedantic -Werror -std=c18 -pthread
CXXFLAGS := $(CXXFLAGS) -O3 -Wall -Wextra -pedantic -Werror -std=c++20 -pthread

all: generator bruteforce best_input relative_order approx_median

generator:
	$(CXX) $(CXXFLAGS) -o build/generator generator.cpp

bruteforce:
	$(CXX) $(CXXFLAGS) -o build/bruteforce bruteforce.cpp

best_input:
	$(CXX) $(CXXFLAGS) -o build/best_input best_from_input.cpp

relative_order:
	$(CXX) $(CXXFLAGS) -o build/relative relative_order.cpp

approx_median:
	$(CXX) $(CXXFLAGS) -o build/approx approx_median.cpp

naive_local_search:
	$(CXX) $(CXXFLAGS) -o build/naive_local naive_local_search.cpp

.PHONY: clean
clean:
	rm -f *.o build/*

# dependency handling
# https://make.mad-scientist.net/papers/advanced-auto-dependency-generation/#tldr

DEPDIR := .deps
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$<.d

COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
COMPILE.cpp = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

%.c.o: %.c
%.c.o: %.c $(DEPDIR)/%.c.d | $(DEPDIR)
	$(COMPILE.c) $(OUTPUT_OPTION) $<

%.cpp.o: %.cpp
%.cpp.o: %.cpp $(DEPDIR)/%.cpp.d | $(DEPDIR)
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

$(DEPDIR): ; @mkdir -p $@

DEPFILES := $(SRCS:%=$(DEPDIR)/%.d) $(DEPDIR)/client.c.d
$(DEPFILES):

include $(wildcard $(DEPFILES))
