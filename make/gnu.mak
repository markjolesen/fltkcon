#
# Top-level makefile for the Fast Light Tool Kit (FLTK).
#
# Copyright 2018 The fltkcon authors
# Copyright 2018 The FDOSTUI authors
# Copyright 2017-2018 The fltkal authors
# Copyright 1998-2017 by Bill Spitzak and others.
# 
#                               FLTK License
#                             December 11, 2001
#  
#  The FLTK library and included programs are provided under the terms
#  of the GNU Library General Public License (LGPL) with the following
#  exceptions:
#  
#      1. Modifications to the FLTK configure script, config
#         header file, and makefiles by themselves to support
#         a specific platform do not constitute a modified or
#         derivative work.
#  
#        The authors do request that such modifications be
#        contributed to the FLTK project - send all contributions
#        through the "Software Trouble Report" on the following page:
#   
#             http://www.fltk.org/str.php
#  
#      2. Widgets that are subclassed from FLTK widgets do not
#         constitute a derivative work.
#  
#      3. Static linking of applications and widgets to the
#         FLTK library does not constitute a derivative work
#         and does not require the author to provide source
#         code for the application or widget, use the shared
#         FLTK libraries, or link their applications or
#         widgets against a user-supplied version of FLTK.
#  
#         If you link the application or widget to a modified
#         version of FLTK, then the changes to FLTK must be
#         provided under the terms of the LGPL in sections
#         1, 2, and 4.
#  
#      4. You do not have to provide a copy of the FLTK license
#         with programs that are linked to the FLTK library, nor
#         do you have to identify the FLTK license in your
#         program or documentation as required by section 6
#         of the LGPL.
#  
#         However, programs must still identify their use of FLTK.
#         The following example statement can be included in user
#         documentation to satisfy this requirement:
#  
#             [program/widget] is based in part on the work of
#             the FLTK project (http://www.fltk.org).
#  
#      This library is free software; you can redistribute it and/or
#      modify it under the terms of the GNU Library General Public
#      License as published by the Free Software Foundation; either
#      version 2 of the License, or (at your option) any later version.
#  
#      This library is distributed in the hope that it will be useful,
#      but WITHOUT ANY WARRANTY; without even the implied warranty of
#      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#      Library General Public License for more details.
#  
#      You should have received a copy of the GNU Library General Public
#      License along with FLTK.  If not, see <http://www.gnu.org/licenses/>.
# 

INCLUDES=-I../include -I../src/base/include -I../src/drivers/include

DEFINES=-DFL_LIBRARY -D__CURSES__

OBJ=../obj
SRC=../src
LIB=../lib

FLTKLIB=$(LIB)/fltkcon.a

RANLIB=ranlib

CFLAGS=\
        $(INCLUDES)\
	$(DEFINES)\
        -Wall\
        -ansi\
        -std=c99\
        -pedantic\
        -ggdb

CXXFLAGS=\
        $(INCLUDES)\
	$(DEFINES)\
        -Wall\
        -ansi\
        -pedantic\
        -ggdb

BASEOBJS=\
        $(OBJ)/block.o \
        $(OBJ)/drvgr.o \
        $(OBJ)/drvscr.o \
        $(OBJ)/drvsys.o \
        $(OBJ)/drvwin.o \
        $(OBJ)/fl_abort.o \
        $(OBJ)/fl_ask.o \
        $(OBJ)/fl.o \
        $(OBJ)/fl_grab.o \
        $(OBJ)/fl_rect.o \
        $(OBJ)/fl_rend.o \
        $(OBJ)/fl_skin.o \
        $(OBJ)/flstring.o \
        $(OBJ)/fl_sys.o \
        $(OBJ)/fl_utf8.o \
        $(OBJ)/fnabs.o \
        $(OBJ)/fnexpand.o \
        $(OBJ)/fnext.o \
        $(OBJ)/fnisdir.o \
        $(OBJ)/fnlist.o \
        $(OBJ)/fnmatch.o \
        $(OBJ)/fnsetext.o \
        $(OBJ)/rint.o \
        $(OBJ)/scrxywh.o

BASESRC=$(SRC)/base/src

XUTF8OBJS=\
        $(OBJ)/case.o \
        $(OBJ)/spacing.o
    
XUTF8SRC=$(BASESRC)/xutf8

DRIVERCOMMONOBJS=\
        $(OBJ)/cleanup.o \
        $(OBJ)/fl_caret.o \
        $(OBJ)/fl_timer.o \
        $(OBJ)/wm.o 

DRIVERCOMMONSRC=$(SRC)/drivers/src

DRIVEROBJS=\
	$(OBJ)/ncdrvscr.o\
	$(OBJ)/ncdrvsys.o\
	$(OBJ)/ncdrvwin.o\
	$(OBJ)/ncwm.o\
	$(OBJ)/screen.o

DRIVERSRC=$(SRC)/drivers/nc

FLTKOBJS=\
        $(OBJ)/fl_arg.o \
        $(OBJ)/box.o \
        $(OBJ)/btn.o \
        $(OBJ)/btnk.o \
        $(OBJ)/btnlt.o \
        $(OBJ)/btnmenu.o \
        $(OBJ)/btnret.o \
        $(OBJ)/btnro.o \
        $(OBJ)/btnrpt.o \
        $(OBJ)/choice.o \
        $(OBJ)/compose.o \
        $(OBJ)/counter.o \
        $(OBJ)/group.o \
        $(OBJ)/inpbuf.o \
        $(OBJ)/inpcur.o \
        $(OBJ)/inpcho.o \
        $(OBJ)/inpfile.o \
        $(OBJ)/input.o \
        $(OBJ)/input_.o \
        $(OBJ)/inpvalue.o \
        $(OBJ)/label.o \
        $(OBJ)/menuadd.o \
        $(OBJ)/menubar.o \
        $(OBJ)/menuglob.o \
        $(OBJ)/menu.o \
        $(OBJ)/menu_.o \
        $(OBJ)/outvalue.o \
        $(OBJ)/pref.o \
        $(OBJ)/scrlarea.o \
        $(OBJ)/scrlbar.o \
        $(OBJ)/scroll.o \
        $(OBJ)/shortcut.o \
        $(OBJ)/slider.o \
        $(OBJ)/slvalue.o \
        $(OBJ)/spinner.o \
        $(OBJ)/textbuf.o \
        $(OBJ)/textdsp.o \
        $(OBJ)/texted.o \
        $(OBJ)/valuator.o \
        $(OBJ)/widget.o \
        $(OBJ)/win.o \
        $(OBJ)/windbl.o \
        $(OBJ)/winmenu.o \
        $(OBJ)/winscr.o \
        $(OBJ)/winsngl.o

ALLOBJS=$(BASEOBJS) $(DRIVERCOMMONOBJS) $(DRIVEROBJS) $(XUTF8OBJS) $(FLTKOBJS)

VPATH=$(SRC):$(BASESRC):$(XUTF8SRC):$(DRIVERCOMMONSRC):$(DRIVERSRC)

.PHONY : all
all : $(FLTKLIB)

$(FLTKLIB) : $(ALLOBJS)
	$(AR) rvc $@ $?
	$(RANLIB) $@

$(OBJ)/%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@ 2> $(addsuffix .err,$@)

$(OBJ)/%.o : %.cxx
	$(CXX) -c $(CXXFLAGS) $< -o $@ 2> $(addsuffix .err,$@)

-include gnu.dep

