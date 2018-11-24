
INCLUDES=-I../include -I../src/base/include -I../src/drivers/include

DEFINES=-DFL_LIBRARY -D__CURSES__

OBJ=../obj
SRC=../src
LIB=../lib

BASESRC=$(SRC)/base/src

XUTF8SRC=$(BASESRC)/xutf8

DRIVERCOMMONSRC=$(SRC)/drivers/src

DRIVERSRC=$(SRC)/drivers/nc

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

gnu.dep :
	echo "# gnu.dep" > gnu.dep
	find $(SRC) -name "*.cxx" -exec $(CXX) -c $(CXXFLAGS) -MM {} \; >> gnu.dep
	find $(SRC) -name "*.c" -exec $(CC) -c $(CFLAGS) -MM {} \; >> gnu.dep
	find $(BASESRC) -name "*.cxx" -exec $(CXX) -c $(CXXFLAGS) -MM {} \; >> gnu.dep
	find $(BASESRC) -name "*.c" -exec $(CC) -c $(CFLAGS) -MM {} \; >> gnu.dep
	find $(XUTF8SRC) -name "*.cxx" -exec $(CXX) -c $(CXXFLAGS) -MM {} \; >> gnu.dep
	find $(XUTF8SRC) -name "*.c" -exec $(CC) -c $(CFLAGS) -MM {} \; >> gnu.dep
	find $(DRIVERCOMMONSRC) -name "*.cxx" -exec $(CXX) -c $(CXXFLAGS) -MM {} \; >> gnu.dep
	find $(DRIVERCOMMONSRC) -name "*.c" -exec $(CC) -c $(CFLAGS) -MM {} \; >> gnu.dep
	find $(DRIVERSRC) -name "*.cxx" -exec $(CXX) -c $(CXXFLAGS) -MM {} \; >> gnu.dep
	find $(DRIVERSRC) -name "*.c" -exec $(CC) -c $(CFLAGS) -MM {} \; >> gnu.dep
	sed -i "s/\(.*\):/$$\(OBJ\)\/\1 : /" gnu.dep

