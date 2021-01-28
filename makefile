CC=g++
ROOTCFLAGS=`root-config --cflags`
ROOTGLIBS=`root-config --glibs`
CFLAGS=-std=c++11 -g -Wall $(ROOTCFLAGS)

SRCDIR=./src
INCLDIR=./include
OBJDIR=./objs

CPPFLAGS=-I$(INCLDIR)
LDFLAGS=$(ROOTGLIBS)

SRC=$(wildcard $(SRCDIR)/*.cpp)
OBJS=$(SRC:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

DICTPAGES=$(INCLDIR)/SPSPlotMainFrame.h $(INCLDIR)/FileViewFrame.h $(INCLDIR)/ReactionCreationFrame.h $(INCLDIR)/LinkDef_SPSPlot.h
DICT=$(SRCDIR)/SPSPlot_dict.cxx
LIB=$(OBJDIR)/SPSPlot_dict.o

EXE=spsplot

.PHONY: all clean

all: $(EXE)

$(EXE): $(LIB) $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(LIB): $(DICT)
	$(CC) $(CFLAGS) $(CPPFLAGS) -I ./ -o $@ -c $^
	mv $(SRCDIR)/*.pcm ./

$(DICT): $(DICTPAGES)
	rootcling -f $@ $^

clean:
	$(RM) $(OBJS) $(EXE) $(LIB) $(DICT) ./*.pcm

#VPATH:$(SRCDIR)
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $^
