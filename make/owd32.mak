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

INCLUDES=-i..\include -i..\src\base\include -i..\src\drivers\include

DEFINES=-DFL_LIBRARY 

RM=del
AS=wasm
CC=wcc386
CXX=wpp386

AFLAGS= -3r -mf -bt=DOS -d1 $(INCLUDES) $(DEFINES)
CFLAGS= -3r -mf -bt=DOS -d3 $(INCLUDES) $(DEFINES)
CXXFLAGS= -3r -mf -bt=DOS -d3 $(INCLUDES) $(DEFINES)

OBJ=..\obj
SRC=..\src
LIB=..\lib

FLTKLIB=$(LIB)\fltkpm.lib

BASEOBJS=&
        $(OBJ)\block.obj &
        $(OBJ)\drvgr.obj &
        $(OBJ)\drvscr.obj &
        $(OBJ)\drvsys.obj &
        $(OBJ)\drvwin.obj &
        $(OBJ)\fl_abort.obj &
        $(OBJ)\fl_ask.obj &
        $(OBJ)\fl.obj &
        $(OBJ)\fl_grab.obj &
        $(OBJ)\fl_rect.obj &
        $(OBJ)\fl_rend.obj &
        $(OBJ)\fl_skin.obj &
        $(OBJ)\flstring.obj &
        $(OBJ)\fl_sys.obj &
        $(OBJ)\fl_utf8.obj &
        $(OBJ)\fnabs.obj &
        $(OBJ)\fnexpand.obj &
        $(OBJ)\fnext.obj &
        $(OBJ)\fnisdir.obj &
        $(OBJ)\fnlist.obj &
        $(OBJ)\fnmatch.obj &
        $(OBJ)\fnsetext.obj &
        $(OBJ)\rint.obj &
        $(OBJ)\scrxywh.obj

BASESRC=$(SRC)\base\src

XUTF8OBJS=&
        $(OBJ)\case.obj &
        $(OBJ)\spacing.obj
    
XUTF8SRC=$(BASESRC)\xutf8

DRIVERCOMMONOBJS=&
        $(OBJ)\cleanup.obj &
        $(OBJ)\fl_timer.obj

DRIVERCOMMONSRC=$(SRC)\drivers\src

DRIVEROBJS=&
        $(OBJ)\fl_caret.obj &
        $(OBJ)\hw.obj &
        $(OBJ)\mouse.obj &
        $(OBJ)\pmdrvscr.obj &
        $(OBJ)\pmdrvsys.obj &
        $(OBJ)\pmdrvwin.obj &
        $(OBJ)\screen.obj &
        $(OBJ)\video.obj &
        $(OBJ)\wm.obj 

DRIVERSRC=$(SRC)\drivers\pm

FLTKOBJS=&
        $(OBJ)\fl_arg.obj &
        $(OBJ)\box.obj &
        $(OBJ)\btn.obj &
        $(OBJ)\btnk.obj &
        $(OBJ)\btnlt.obj &
        $(OBJ)\btnmenu.obj &
        $(OBJ)\btnret.obj &
        $(OBJ)\btnro.obj &
        $(OBJ)\btnrpt.obj &
        $(OBJ)\choice.obj &
        $(OBJ)\compose.obj &
        $(OBJ)\counter.obj &
        $(OBJ)\group.obj &
        $(OBJ)\inpbuf.obj &
        $(OBJ)\inpcur.obj &
        $(OBJ)\inpcho.obj &
        $(OBJ)\inpfile.obj &
        $(OBJ)\input.obj &
        $(OBJ)\input_.obj &
        $(OBJ)\inpvalue.obj &
        $(OBJ)\label.obj &
        $(OBJ)\menuadd.obj &
        $(OBJ)\menubar.obj &
        $(OBJ)\menuglob.obj &
        $(OBJ)\menu.obj &
        $(OBJ)\menu_.obj &
        $(OBJ)\outvalue.obj &
        $(OBJ)\pref.obj &
        $(OBJ)\scrlbar.obj &
        $(OBJ)\shortcut.obj &
        $(OBJ)\slider.obj &
        $(OBJ)\slvalue.obj &
        $(OBJ)\spinner.obj &
        $(OBJ)\textbuf.obj &
        $(OBJ)\textdsp.obj &
        $(OBJ)\texted.obj &
        $(OBJ)\valuator.obj &
        $(OBJ)\widget.obj &
        $(OBJ)\win.obj &
        $(OBJ)\windbl.obj &
        $(OBJ)\winmenu.obj &
        $(OBJ)\winscr.obj &
        $(OBJ)\winsngl.obj

ALLOBJS=$(BASEOBJS) $(DRIVERCOMMONOBJS) $(DRIVEROBJS) $(XUTF8OBJS) $(FLTKOBJS)

all : $(FLTKLIB) .SYMBOlIC

$(FLTKLIB) : $(ALLOBJS)
        wlib $@ @owd32.lbc
	
$(OBJ)\block.obj : $(BASESRC)\block.c  .AUTODEPEND
        *$(CC) $(CFLAGS) -fo=$@ $(BASESRC)\block.c

$(OBJ)\drvgr.obj : $(BASESRC)\drvgr.cxx  .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(BASESRC)\drvgr.cxx
        
$(OBJ)\drvscr.obj : $(BASESRC)\drvscr.cxx  .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(BASESRC)\drvscr.cxx
        
$(OBJ)\drvsys.obj : $(BASESRC)\drvsys.cxx  .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(BASESRC)\drvsys.cxx
        
$(OBJ)\drvwin.obj : $(BASESRC)\drvwin.cxx  .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(BASESRC)\drvwin.cxx
        
$(OBJ)\fl_abort.obj : $(BASESRC)\fl_abort.cxx  .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(BASESRC)\fl_abort.cxx
        
$(OBJ)\fl_ask.obj : $(BASESRC)\fl_ask.cxx  .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(BASESRC)\fl_ask.cxx
        
$(OBJ)\fl.obj : $(BASESRC)\fl.cxx  .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(BASESRC)\fl.cxx
	
$(OBJ)\fl_grab.obj : $(BASESRC)\fl_grab.cxx  .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(BASESRC)\fl_grab.cxx
        
$(OBJ)\fl_rect.obj : $(BASESRC)\fl_rect.cxx  .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(BASESRC)\fl_rect.cxx
        
$(OBJ)\fl_rend.obj : $(BASESRC)\fl_rend.cxx  .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(BASESRC)\fl_rend.cxx
        
$(OBJ)\fl_skin.obj : $(BASESRC)\fl_skin.cxx  .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(BASESRC)\fl_skin.cxx
        
$(OBJ)\flstring.obj : $(BASESRC)\flstring.c  .AUTODEPEND
        *$(CC) $(CFLAGS) -fo=$@ $(BASESRC)\flstring.c
        
$(OBJ)\fl_sys.obj : $(BASESRC)\fl_sys.cxx  .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(BASESRC)\fl_sys.cxx
        
$(OBJ)\fl_utf8.obj : $(BASESRC)\fl_utf8.cxx  .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(BASESRC)\fl_utf8.cxx
        
$(OBJ)\fnabs.obj : $(BASESRC)\fnabs.cxx  .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(BASESRC)\fnabs.cxx
        
$(OBJ)\fnexpand.obj : $(BASESRC)\fnexpand.cxx  .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(BASESRC)\fnexpand.cxx
        
$(OBJ)\fnext.obj : $(BASESRC)\fnext.cxx  .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(BASESRC)\fnext.cxx
        
$(OBJ)\fnisdir.obj : $(BASESRC)\fnisdir.cxx  .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(BASESRC)\fnisdir.cxx
        
$(OBJ)\fnlist.obj : $(BASESRC)\fnlist.cxx  .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(BASESRC)\fnlist.cxx
        
$(OBJ)\fnmatch.obj : $(BASESRC)\fnmatch.cxx  .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(BASESRC)\fnmatch.cxx
        
$(OBJ)\fnsetext.obj : $(BASESRC)\fnsetext.cxx  .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(BASESRC)\fnsetext.cxx
        
$(OBJ)\rint.obj : $(BASESRC)\rint.c .AUTODEPEND
        *$(CC) $(CFLAGS) -fo=$@ $(BASESRC)\rint.c
	
$(OBJ)\scrxywh.obj : $(BASESRC)\scrxywh.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(BASESRC)\scrxywh.cxx

$(OBJ)\case.obj : $(XUTF8SRC)\case.c .AUTODEPEND
        *$(CC) $(CFLAGS) -fo=$@ $(XUTF8SRC)\case.c

$(OBJ)\spacing.obj : $(XUTF8SRC)\spacing.c .AUTODEPEND
        *$(CC) $(CFLAGS) -fo=$@ $(XUTF8SRC)\spacing.c

$(OBJ)\cleanup.obj : $(DRIVERCOMMONSRC)\cleanup.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(DRIVERCOMMONSRC)\cleanup.cxx

$(OBJ)\fl_timer.obj : $(DRIVERCOMMONSRC)\fl_timer.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(DRIVERCOMMONSRC)\fl_timer.cxx
        
$(OBJ)\fl_caret.obj : $(DRIVERSRC)\fl_caret.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(DRIVERSRC)\fl_caret.cxx

$(OBJ)\hw.obj : $(DRIVERSRC)\hw.asm 
        *$(AS) $(AFLAGS) -fo=$@ $(DRIVERSRC)\hw.asm
        
$(OBJ)\mouse.obj : $(DRIVERSRC)\mouse.asm 
        *$(AS) $(AFLAGS) -fo=$@ $(DRIVERSRC)\mouse.asm
        
$(OBJ)\pmdrvscr.obj : $(DRIVERSRC)\pmdrvscr.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(DRIVERSRC)\pmdrvscr.cxx

$(OBJ)\pmdrvsys.obj : $(DRIVERSRC)\pmdrvsys.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(DRIVERSRC)\pmdrvsys.cxx

$(OBJ)\pmdrvwin.obj : $(DRIVERSRC)\pmdrvwin.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(DRIVERSRC)\pmdrvwin.cxx

$(OBJ)\screen.obj : $(DRIVERSRC)\screen.asm 
        *$(AS) $(AFLAGS) -fo=$@ $(DRIVERSRC)\screen.asm
        
$(OBJ)\video.obj : $(DRIVERSRC)\video.asm 
        *$(AS) $(AFLAGS) -fo=$@ $(DRIVERSRC)\video.asm
        
$(OBJ)\wm.obj : $(DRIVERSRC)\wm.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(DRIVERSRC)\wm.cxx
	
$(OBJ)\fl_arg.obj : $(SRC)\fl_arg.cxx  .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\fl_arg.cxx
	
$(OBJ)\box.obj : $(SRC)\box.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\box.cxx

$(OBJ)\btn.obj : $(SRC)\btn.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\btn.cxx

$(OBJ)\btnk.obj : $(SRC)\btnk.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\btnk.cxx
	
$(OBJ)\btnro.obj : $(SRC)\btnro.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\btnro.cxx

$(OBJ)\btnlt.obj : $(SRC)\btnlt.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\btnlt.cxx

$(OBJ)\btnmenu.obj : $(SRC)\btnmenu.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\btnmenu.cxx

$(OBJ)\btnret.obj : $(SRC)\btnret.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\btnret.cxx

$(OBJ)\btnrpt.obj : $(SRC)\btnrpt.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\btnrpt.cxx
	
$(OBJ)\choice.obj : $(SRC)\choice.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\choice.cxx

$(OBJ)\compose.obj : $(SRC)\compose.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\compose.cxx
	
$(OBJ)\counter.obj : $(SRC)\counter.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\counter.cxx

$(OBJ)\group.obj : $(SRC)\group.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\group.cxx
        
$(OBJ)\inpbuf.obj : $(SRC)\inpbuf.c .AUTODEPEND
        *$(CC) $(CFLAGS) -fo=$@ $(SRC)\inpbuf.c
        
$(OBJ)\inpcur.obj : $(SRC)\inpcur.c .AUTODEPEND
        *$(CC) $(CFLAGS) -fo=$@ $(SRC)\inpcur.c

$(OBJ)\inpcho.obj : $(SRC)\inpcho.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\inpcho.cxx

$(OBJ)\inpfile.obj : $(SRC)\inpfile.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\inpfile.cxx

$(OBJ)\input.obj : $(SRC)\input.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\input.cxx

$(OBJ)\input_.obj : $(SRC)\input_.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\input_.cxx

$(OBJ)\inpvalue.obj : $(SRC)\inpvalue.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\inpvalue.cxx

$(OBJ)\label.obj : $(SRC)\label.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\label.cxx

$(OBJ)\menuadd.obj : $(SRC)\menuadd.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\menuadd.cxx

$(OBJ)\menubar.obj : $(SRC)\menubar.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\menubar.cxx

$(OBJ)\menuglob.obj : $(SRC)\menuglob.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\menuglob.cxx

$(OBJ)\menu.obj : $(SRC)\menu.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\menu.cxx

$(OBJ)\menu_.obj : $(SRC)\menu_.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\menu_.cxx

$(OBJ)\outvalue.obj : $(SRC)\outvalue.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\outvalue.cxx

$(OBJ)\pref.obj : $(SRC)\pref.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\pref.cxx

$(OBJ)\scrlbar.obj : $(SRC)\scrlbar.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\scrlbar.cxx

$(OBJ)\shortcut.obj : $(SRC)\shortcut.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\shortcut.cxx

$(OBJ)\slider.obj : $(SRC)\slider.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\slider.cxx

$(OBJ)\slvalue.obj : $(SRC)\slvalue.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\slvalue.cxx
	
$(OBJ)\spinner.obj : $(SRC)\spinner.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\spinner.cxx

$(OBJ)\textbuf.obj : $(SRC)\textbuf.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\textbuf.cxx

$(OBJ)\textdsp.obj : $(SRC)\textdsp.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\textdsp.cxx

$(OBJ)\texted.obj : $(SRC)\texted.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\texted.cxx

$(OBJ)\valuator.obj : $(SRC)\valuator.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\valuator.cxx

$(OBJ)\widget.obj : $(SRC)\widget.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\widget.cxx

$(OBJ)\win.obj : $(SRC)\win.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\win.cxx

$(OBJ)\windbl.obj : $(SRC)\windbl.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\windbl.cxx

$(OBJ)\winmenu.obj : $(SRC)\winmenu.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\winmenu.cxx

$(OBJ)\winscr.obj : $(SRC)\winscr.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\winscr.cxx

$(OBJ)\winsngl.obj : $(SRC)\winsngl.cxx .AUTODEPEND
        *$(CXX) $(CXXFLAGS) -fo=$@ $(SRC)\winsngl.cxx

        
clean : .SYMBOLIC
        $(RM) $(FLTKLIB)
        $(RM) $(OBJ)\*.obj
        
