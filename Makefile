# Name:                pcap2tap
# Date:                2025.02.15
# Version:             0.2.3
# Brief:               Play pcap to TAP.
# License:             Apache License 2.0
# Author(s):           Deminets

APPNAME = pcap2tap
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.cpp.o)
DEPS = $(SRCS:.cpp=.cpp.dep)

# $(info SRC=$(SRCS))

CFLAGS = -O2 -Wall
CXXFLAGS = -O2 -Wall -g -lpcap
CXX = g++


PHONY := all
all: ${APPNAME}


$(APPNAME): $(OBJS)
	@echo "LINK $@"
	@$(CXX) $(CXXFLAGS) -o $@ $(OBJS)


-include $(DEPS)

%.cpp.dep: %.cpp
	@echo "DEP $<"
	@$(CXX) -MM -MT $@ $(CXXFLAGS) -c $< -o $@


%.cpp.o: %.cpp %.cpp.dep
	@echo "CPP $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@


PHONY += clean
clean:
	rm -f *.c.o *.cpp.o *.cpp.dep ${APPNAME}


.DEFAULT:
	$(warning !!!WARNING!!!: not found rule for target = '$(@)')
	@:

.PHONY : $(PHONY)
