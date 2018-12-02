#
# owd32.mak
#
# License CC0 PUBLIC DOMAIN
#
# To the extent possible under law, Mark J. Olesen has waived all copyright
# and related or neighboring rights to fltkcon Library. This work is published
# from: United States.
#

INCLUDES=-i..\include

DEFINES=

SYSTEM=NT

RM=del
CXX=wpp386

CXXFLAGS= -3r -mf -bt=NT -d3 $(INCLUDES) $(DEFINES)

OBJ=..\obj
SRC=..\src
LIB=..\lib

FLTKLIB=$(LIB)\fltkms.lib

EXES= &
    talign.exe &
    tinpfile.exe &
    tinput.exe &
    tmenubar.exe &
    tscroll.exe &
    ttexted.exe &
    tvaluato.exe

all : $(EXES) .SYMBOLIC    

.cxx.obj : .AUTODEPEND
	*$(CXX) $< -fo=$^@ $(CXXFLAGS)

talign.exe : talign.obj
	*wlink NAME $^@ DEBUG ALL SYSTEM $(SYSTEM) FILE $<, $(FLTKLIB)

tinpfile.exe : tinpfile.obj
	*wlink NAME $^@ DEBUG ALL SYSTEM $(SYSTEM) FILE $<, $(FLTKLIB)

tinput.exe : tinput.obj
	*wlink NAME $^@ DEBUG ALL SYSTEM $(SYSTEM) FILE $<, $(FLTKLIB)

tmenubar.exe : tmenubar.obj
	*wlink NAME $^@ DEBUG ALL SYSTEM $(SYSTEM) FILE $<, $(FLTKLIB)

tscroll.exe : tscroll.obj
	*wlink NAME $^@ DEBUG ALL SYSTEM $(SYSTEM) FILE $<, $(FLTKLIB)

ttexted.exe : ttexted.obj
	*wlink NAME $^@ DEBUG ALL SYSTEM $(SYSTEM) FILE $<, $(FLTKLIB)

tvaluato.exe : tvaluato.obj
	*wlink NAME $^@ DEBUG ALL SYSTEM $(SYSTEM) FILE $<, $(FLTKLIB)

clean : .SYMBOLIC
	@rm *.obj
	@rm *.exe
