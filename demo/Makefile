APPNAME=test
GPP=g++
GCC=gcc
LD=g++
SOURCES=*.c
LIBNAME=sigcomp
LIBPATH=../libsigcomp/

INCLUDES+=-I$(LIBPATH)src/
INCLUDES+=-I$(LIBPATH)

STATICLIBOUT=$(LIBPATH)lib$(LIBNAME).a
LIBDYNAMIC=$(LIBPATH)lib$(LIBNAME).so

CFLAGS+=-Wno-attributes
CFLAGS+=-fkeep-inline-functions
CFLAGS+=-g

#LIBCFLAGS+=-Wall -W
#LIBCFLAGS+=-fPIC
#LIBLDFLAGS+=-shared

all: $(LIBDYNAMIC)
	#$(GPP) $(SOURCES) -o $(APPNAME) $(STATICLIBOUT) $(INCLUDES)
	$(GCC) $(SOURCES) -o $(APPNAME) $(CFLAGS) -L$(LIBPATH) -lsigcomp -ldl $(INCLUDES)

$(LIBDYNAMIC):
	+$(MAKE) -C $(LIBPATH) -f Makefile

clean:
	rm -rf $(APPNAME)
	+$(MAKE) -C $(LIBPATH) -f Makefile clean	

run:
	LD_LIBRARY_PATH=$(LIBPATH) ./$(APPNAME)
	
.PHONY: clean