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

CPPFLAGS+=-Wno-attributes
CPPFLAGS+=-Wno-conversion-null
CPPFLAGS+=-fkeep-inline-functions

LIBCFLAGS+=-Wall -W
LIBCFLAGS+=-fPIC
LIBLDFLAGS+=-shared

all: $(LIBDYNAMIC)
	#$(GPP) $(SOURCES) -o $(APPNAME) $(STATICLIBOUT) $(INCLUDES)
	$(GCC) $(SOURCES) -o $(APPNAME) $(CPPFLAGS) -L$(LIBPATH) -lsigcomp $(INCLUDES)

$(LIBDYNAMIC):
	+$(MAKE) -C $(LIBPATH) -f Makefile

clean:
	rm -rf $(APPNAME)
	+$(MAKE) -C $(LIBPATH) -f Makefile clean	

run:
	LD_LIBRARY_PATH=$(LIBPATH) ./$(APPNAME)
	
.PHONY: clean