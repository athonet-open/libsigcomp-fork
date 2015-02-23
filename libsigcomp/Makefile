LIBNAME=sigcomp
SRCPATH=./src/

LIBOBJ+=$(SRCPATH)adler32.o
LIBOBJ+=$(SRCPATH)binary_utils.o
LIBOBJ+=$(SRCPATH)compress.o
LIBOBJ+=$(SRCPATH)c_wrapper.o
LIBOBJ+=$(SRCPATH)deflate.o
LIBOBJ+=$(SRCPATH)DeflateCompressor.o
LIBOBJ+=$(SRCPATH)DeflateData.o
LIBOBJ+=$(SRCPATH)DeflateData.Ghost.o
LIBOBJ+=$(SRCPATH)DeflateData.zlib.o
LIBOBJ+=$(SRCPATH)DummyCompressor.o
LIBOBJ+=$(SRCPATH)Mutex.o
LIBOBJ+=$(SRCPATH)PresenceDictionaryState.o
LIBOBJ+=$(SRCPATH)rfc1662_ppfcs16.o
LIBOBJ+=$(SRCPATH)rfc3174_us-sha1.o
LIBOBJ+=$(SRCPATH)SafeObject.o
LIBOBJ+=$(SRCPATH)SigCompBuffer.o
LIBOBJ+=$(SRCPATH)SigCompCompartment.o
LIBOBJ+=$(SRCPATH)SigCompCompressor.o
LIBOBJ+=$(SRCPATH)SigCompCompressorData.o
LIBOBJ+=$(SRCPATH)SigCompCompressorDisp.o
LIBOBJ+=$(SRCPATH)SigCompDecompressorDisp.o
LIBOBJ+=$(SRCPATH)SigCompManager.o
LIBOBJ+=$(SRCPATH)SigCompMessage.o
LIBOBJ+=$(SRCPATH)SigCompState.o
LIBOBJ+=$(SRCPATH)SigCompStateHandler.o
LIBOBJ+=$(SRCPATH)SigCompUDVM.ByteCopy.o
LIBOBJ+=$(SRCPATH)SigCompUDVM.o
LIBOBJ+=$(SRCPATH)SigCompUDVM.Instructions.o
LIBOBJ+=$(SRCPATH)SigCompUDVM.Nack.o
LIBOBJ+=$(SRCPATH)SigCompUDVM.Operands.o
LIBOBJ+=$(SRCPATH)SigCompUDVM.StateManagement.o
LIBOBJ+=$(SRCPATH)SipDictionaryState.o
LIBOBJ+=$(SRCPATH)trees.o
LIBOBJ+=$(SRCPATH)zutil.o
LIBOBJ+=$(SRCPATH)alloc.o

GCCCOMPILE=gcc
GPPCOMPILE=g++
LINK=g++
CFLAGS+=-fPIC
CFLAGS+=-g
LDFLAGS+=-g
#CFLAGS+=-O2
CFLAGS+=-Wno-attributes
CFLAGS+=-DHAVE_CONFIG_H
CPPFLAGS+=-Wno-conversion-null
CPPFLAGS+=-fkeep-inline-functions
LIBLDFLAGS+=-shared
LIBLDFLAGS+=-Wl,-Map=lib.map
INCLUDES+=-I/src/
INCLUDES+=-I./

#g++ -DHAVE_CONFIG_H -I.     -fPIC -g -O2 -fkeep-inline-functions -MT c_wrapper.o -MD -MP -MF .deps/c_wrapper.Tpo -c -o c_wrapper.o
#g++ -fPIC -g -O2 -Wattributes -fkeep-inline-functions -I/src/ -I./ src/c_wrapper.cxx -o src/c_wrapper.o


lib$(LIBNAME).so: $(LIBOBJ)
	$(LINK) $(LIBLDFLAGS) $(LDFLAGS) -o lib$(LIBNAME).so $(LIBOBJ)

%.o: %.c
	$(GCCCOMPILE) $(CFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.cpp
	$(GPPCOMPILE) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.cxx
	$(GPPCOMPILE) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

%.c: %.h

%.cpp: %.h

%.cxx: %.h

clean:
	rm -rf lib$(LIBNAME).so $(LIBOBJ)

install:
	cp lib$(LIBNAME).so /usr/lib/