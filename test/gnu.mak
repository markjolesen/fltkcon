#
# gnu.mak
#
# License CC0 PUBLIC DOMAIN
#
# To the extent possible under law, Mark J. Olesen has waived all copyright
# and related or neighboring rights to fltkcon Library. This work is published
# from: United States.
#

INCLUDES=-I../include

DEFINES=-D__CURSES__

LIB=../lib

FLTKLIB=$(LIB)/fltkcon.a

CXXFLAGS=\
        $(INCLUDES)\
	$(DEFINES)\
        -Wall\
        -ansi\
        -pedantic\
        -ggdb

EXES=tmenubar

EXES2=\
    talign\
    tinpfile\
    tinput\
    tmenubar\
    tscroll\
    ttexted\
    tvaluato

LIBS=-L ../lib $(FLTKLIB) -l curses

.PHONY : all
all : $(EXES)

talign : talign.o
	$(CXX) $(CXXFLAGS) -o $@ $? $(LIBS)

tinpfile : tinpfile.o
	$(CXX) $(CXXFLAGS) -o $@ $? $(LIBS)

tinput : tinput.o
	$(CXX) $(CXXFLAGS) -o $@ $? $(LIBS)

tmenubar : tmenubar.o
	$(CXX) $(CXXFLAGS) -o $@ $? $(LIBS)

tscroll : tscroll.o
	$(CXX) $(CXXFLAGS) -o $@ $? $(LIBS)

ttexted : ttexted.o
	$(CXX) $(CXXFLAGS) -o $@ $? $(LIBS)

tvaluato : tvaluato.o
	$(CXX) $(CXXFLAGS) -o $@ $? $(LIBS)

%.o : %.cxx
	${CXX} -c ${CXXFLAGS} -o $@ $<

.PHONY : clean
clean : 
	rm -f $(EXES)
	rm -f *.o
