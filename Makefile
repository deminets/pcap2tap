# Name:                pcap2tap
# Date:                2025.02.15
# Version:             0.2.3
# Brief:               Play pcap to TAP.
# License:             Apache License 2.0
# Author(s):           Deminets

APPNAME = pcap2tap

BLDIR := $(CURDIR)
define SRCDIR
$(abspath $(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
endef

SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(subst $(SRCDIR),$(BLDIR),$(SRCS:.cpp=.cpp.o))
DEPS := $(subst $(SRCDIR),$(BLDIR),$(SRCS:.cpp=.cpp.dep))


CFLAGS = -O2 -Wall
CXXFLAGS = -O2 -Wall -g -lpcap
CXX = g++


PHONY := all
all: $(BLDIR)/${APPNAME}


$(BLDIR)/$(APPNAME): $(DEPS) $(OBJS)
	@echo "LINK $(notdir $@)"
	@$(CXX) $(CXXFLAGS) -o $@ $(OBJS)


$(BLDIR)/%.cpp.dep: $(SRCDIR)/%.cpp
	@echo "DEP $(notdir $<)"
	@$(CXX) -MM -MT $@ $(CXXFLAGS) -c $< -o $@


$(BLDIR)/%.cpp.o: $(SRCDIR)/%.cpp $(BLDIR)/%.cpp.dep
	@echo "CPP $(notdir $<)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@


ifneq (clean,$(filter clean,$(MAKECMDGOALS)))
-include $(DEPS)
endif


PHONY += clean
clean:
	@echo "CLEAN"
	@rm -f $(BLDIR)/*.c.o $(BLDIR)/*.cpp.o $(BLDIR)/*.cpp.dep $(BLDIR)/${APPNAME}


.DEFAULT:
	$(warning !!!WARNING!!!: not found rule for target = '$(@)')
	@:

.PHONY : $(PHONY)
