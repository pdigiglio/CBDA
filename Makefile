#
# Makefile - simple rules to easily build sources
#

# Main file to compile
MAIN	=
# Common routines to be included
COMMON	= 
MODULES	= $(COMMON)

# Ising/Potts model
#MODULES	+= reticolo ising potts

.PHONY: all distclean clean dist

# Default rule
all: $(MAIN)

# Clean directory from dependences, objects and executable
found_src_cpp  = $(shell find ./* -name "*.cpp")
found = $(found_src_cpp:.cpp=)

found_src_cc = $(shell find ./* -name "*.cc")
found += $(found_src_cc:.cc=)

found_src_h = $(shell find ./* -name "*.h")
found += $(found_src_h:.h=)

found_src_c = $(shell find ./* -name "*.c")
found += $(found_src_c:.c=)

found_src_C = $(shell find ./* -name "*.C")
found += $(found_src_C:.C=)
found += $(found_src_C:.C=_C.so)


clean:
	@-rm --recursive --force --verbose $(addsuffix .o, $(found)) $(addsuffix .so, $(found)) $(addsuffix .d, $(found)) $(found)

#	@echo $(found)

distclean: clean
	@-rm --recursive --force --verbose $(MAIN)  $(found)

# Directory for modules and headers
MDIR	= ../modules
IDIR	= ../include

# Modules direcory
VPATH	= $(MDIR):$(MDIR)/common

# Libraries to be linked in
LBS		= m # math library
LBSPATH	= 

# FIXME Dependence files (append dependences to prerequisite)
ifneq ( $(MAKECMDGOALS), clean )
	-include $(addsuffix .d,$(MODULES) $(MAIN) $(ANALISI))
endif

# Creo l'opzione da passare al compilatore per le librerie: aggiungo
# il prefisso '-l' a tutte le librerie specificate in $(LBS) e il pre-
# fisso '-L' alle directory dove si trovano le librerie
LDFLAGS = $(addprefix -l,$(LBS))
#ifneq($(LBS),)
#	LDFLAGS	+= $(addprefix -l,$(LBS))
#	$(info Adding $(LBS) to LDFLAGS)
#endif
#ifneq ($(LBSPATH),)
#	LDFLAGS += $(addprefix -L,$(LBSPATH))
#endif

# Add ROOT libaries
ROOT = `root-config --libs --cflags`
#LDFLAGS += $(ROOT)

# Creo l'opzione da passare al compilatore per le directory dei file 
# .h "custom". L'opzione '-I-' indica che nelle directory specificate
# in precedenza bisogna cercare soltanto i file locali (cioè non in-
# clusi con '#include < ... >')
INCPATH	=
ifneq ($(IDIR),)
	INCPATH += $(addprefix -I,$(IDIR))
endif

# Creo i file oggetto dei moduli
OBJS	= $(addsuffix .o,$(MODULES) $(MAIN))

SHELL	= /bin/bash
# C/C++ compiler
CC		= gcc
CXX		= g++
# Some cpu-dependent options
MARCH	= core2
MASM	= intel
# standard language
STD		= gnu++11

# Opzioni
CXXFLAGS = -W -Wall -Wextra -Wunreachable-code -Wunused -Wformat-security -Wmissing-noreturn \
		   -O3 -pedantic -std=$(STD) -masm=$(MASM) -march=$(MARCH) -mtune=$(MARCH) -fopenmp -time \
		   $(ROOT)

# Add includes
CXXFLAGS += $(INCPATH)
# Add libraries
CXXFLAGS += $(LDFLAGS)

# Rule to make dependence file(s) for modules
%.d: %.cc
	@echo -e "[`tput setaf 3`depend`tput sgr0`] $(CXX) -MM -ansi -o `tput bold`$@`tput sgr0`" \
		" `tput setaf 2`$<`tput sgr0` $(INCPATH)" # $(CXXFLAGS)"
	@$(CXX) -MM -ansi $< -o $@ $(INCPATH) $(LDFLAGS)

# Rule to make dependence file(s) for main routine
%.d: %.cpp
	@echo -e "[`tput setaf 3`depend`tput sgr0`] $(CXX) -MM -ansi -o `tput bold`$@`tput sgr0`" \
		" `tput setaf 2`$<`tput sgr0` $(INCPATH)" # $(CXXFLAGS)"
	@$(CXX) -MM -ansi $< -o $@ $(INCPATH) $(LDFLAGS)

# Rule to make object files
%.o: %.cc %.d Makefile
	@echo -e "[`tput setaf 4`module`tput sgr0`] $(CXX) -o `tput bold`$@`tput sgr0`" \
		"-c `tput setaf 2`$<`tput sgr0` $(INCPATH)" # $(CXXFLAGS)"
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

# Rule to make object file for main routine
%.o: %.cpp %.d Makefile
	@echo -e "[`tput setaf 4`module`tput sgr0`] $(CXX) -o `tput bold`$@`tput sgr0`" \
		"-c `tput setaf 2`$<`tput sgr0` $(INCPATH)" # $(CXXFLAGS)"
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

$(MAIN): %: $(OBJS) Makefile
	@ echo
	@ echo -e "[`tput setaf 6`info`tput sgr0`] Architettura rilevata:\t\t" \
		`tput setaf 5`` gcc -march=native -Q --help=target | grep --text march | cut -f3 ``tput sgr0`
	@ echo -e "[`tput setaf 6`info`tput sgr0`] Architettura selezionata:\t" `tput setaf 5`$(MARCH)`tput sgr0`
	@ echo -e "[`tput bold``tput setaf 6`$@`tput sgr0`]" \
		"$(CXX) -o `tput bold`$@`tput sgr0` `tput setaf 2`$(OBJS)`tput sgr0` $(CXXFLAGS)\n"
	@$(CXX) $(OBJS) -o $@ $(CXXFLAGS)

# Miscellanea
$(MISC): %: %.o
	@echo -e "[`tput bold``tput setaf 6`$@`tput sgr0`] $(CXX) -c `tput setaf 2`$^`tput sgr0` -o `tput bold`$@`tput sgr0` $(CXXFLAGS)"
	@$(CXX) $^ -o $@ $(CXXFLAGS)

date = $(strip $(shell date "+%Y.%m.%d" ) )
dist: distclean
	cd ..; \
		tar -cvaf lab_calcolo-$(date).tar.gz --exclude-vcs \
		main/ modules/ include/ AUTHORS LICENSE relazione.pdf

# Produce executables
mkxeq: $(MAIN)

# make dependencies
mkdep: $(addsuffix .d,$(MODULES) $(MAIN))
	@echo ""
