# Do not edit normally. Configuration settings are in Makefile.conf.

include Makefile.conf

SHORT_LIBRARY_NAME = datasetturbo
LIBRARY_NAME = lib$(SHORT_LIBRARY_NAME)
VERSION = 0.2
VERSION_MAJOR = 0

# CFLAGS with optional tuning for CPU
OPTCFLAGS =
ifeq ($(TARGET_CPU), CORTEX_A7)
OPTCFLAGS += -mcpu=cortex-a7 -mfpu=vfpv4
endif
ifeq ($(TARGET_CPU), CORTEX_A8)
OPTCFLAGS += -mcpu=cortex-a8
endif
ifeq ($(TARGET_CPU), CORTEX_A9)
OPTCFLAGS += -mcpu=cortex-a9
endif

# SIMD configuration (SSE on x86).
ifeq ($(TARGET_SIMD), X86_SSE3)
OPTCFLAGS += -msse3 -DUSE_SSE2 -DUSE_SSE3
else
ifeq ($(TARGET_SIMD), X86_SSE2)
OPTCFLAGS += -msse2 -DUSE_SSE2
endif
endif
ifeq ($(TARGET_SIMD), ARM_NEON)
OPTCFLAGS +=-DUSE_ARM_NEON -mneon
endif
ifeq ($(TARGET_SIMD), NONE)
OPTCFLAGS +=-DNO_SIMD
endif

ifeq ($(LIBRARY_CONFIGURATION), DEBUG)
# Adding some optimization even when compiling for debugging can help compilation
# problems related to arguments that need to immediate constants.
OPTCFLAGS += -O -ggdb
else
OPTCFLAGS += -Ofast -ffast-math
endif

CFLAGS = -Wall -pipe -I. $(OPTCFLAGS)

ifeq ($(LIBRARY_CONFIGURATION), SHARED)
LIBRARY_OBJECT = $(LIBRARY_NAME).so.$(VERSION)
INSTALL_TARGET = install_shared
LIBRARY_DEPENDENCY = $(SHARED_LIB_DIR)/$(LIBRARY_OBJECT)
TEST_PROGRAM_LFLAGS = -l$(SHORT_LIBRARY_NAME)
CFLAGS_LIB = $(CFLAGS) -fPIC -fvisibility=hidden -DDST_SHARED -DDST_SHARED_EXPORTS
CFLAGS_TEST = $(CFLAGS)
else
ifeq ($(LIBRARY_CONFIGURATION), DEBUG)
LIBRARY_OBJECT = $(LIBRARY_NAME)_dbg.a
else
LIBRARY_OBJECT = $(LIBRARY_NAME).a
endif
# install_static also works for debugging library
INSTALL_TARGET = install_static
LIBRARY_DEPENDENCY = $(LIBRARY_OBJECT) $(BACKEND_OBJECT)
TEST_PROGRAM_LFLAGS = $(LIBRARY_OBJECT)
CFLAGS_LIB = $(CFLAGS)
CFLAGS_TEST = $(CFLAGS)
endif

LIBRARY_MODULE_OBJECTS = dstRandom.o dstRNGCMWC.o dstSIMDDotMatrix.o
LIBRARY_HEADER_FILES = dstConfig.h dstRandom.h dstDynamicArray.h dstTimer.h \
	dstSIMD.h dstSIMDDot.h dstSIMDMatrix.h dstSIMDSSE2.h
TEST_PROGRAMS = test-random test-array

default : library

library : $(LIBRARY_OBJECT)

$(LIBRARY_NAME).so.$(VERSION) : $(LIBRARY_MODULE_OBJECTS) $(LIBRARY_HEADER_FILES)
	g++ -shared -Wl,-soname,$(LIBRARY_OBJECT) -fPIC -o $(LIBRARY_OBJECT) \
$(LIBRARY_MODULE_OBJECTS) -lc -lm
	@echo Run '(sudo) make install to install.'

$(LIBRARY_NAME).a : $(LIBRARY_MODULE_OBJECTS) $(LIBRARY_HEADER_FILES)
	ar r $(LIBRARY_OBJECT) $(LIBRARY_MODULE_OBJECTS)
	@echo 'Run (sudo) make install to install, or make test to build the test programs.'

$(LIBRARY_NAME)_dbg.a : $(LIBRARY_MODULE_OBJECTS) $(LIBRARY_HEADER_FILES)
	ar r $(LIBRARY_OBJECT) $(LIBRARY_MODULE_OBJECTS)
	@echo 'The library is compiled with debugging enabled.'

install : $(INSTALL_TARGET) install_headers

install_headers : $(LIBRARY_HEADER_FILES)
	@for x in $(LIBRARY_HEADER_FILES); do \
	echo Installing $$x.; \
	install -m 0644 $$x $(INCLUDE_DIR)/$$x; done

install_shared : $(LIBRARY_OBJECT)
	install -m 0644 $(LIBRARY_OBJECT) $(SHARED_LIB_DIR)/$(LIBRARY_OBJECT)
	ln -sf $(SHARED_LIB_DIR)/$(LIBRARY_OBJECT) $(SHARED_LIB_DIR)/$(LIBRARY_NAME).so
	ln -sf $(SHARED_LIB_DIR)/$(LIBRARY_OBJECT) $(SHARED_LIB_DIR)/$(LIBRARY_NAME).so.$(VERSION_MAJOR)
	@echo 'Run make test to build the test programs.'

install_static : $(LIBRARY_OBJECT)
	install -m 0644 $(LIBRARY_OBJECT) $(STATIC_LIB_DIR)/$(LIBRARY_OBJECT)

test : $(TEST_PROGRAMS)

test-random :  $(LIBRARY_DEPENDENCY) test-random.cpp dstRandom.h
	g++ $(CFLAGS_TEST) test-random.cpp -o test-random -lfgen $(TEST_PROGRAM_LFLAGS) -lm 

test-array :  $(LIBRARY_DEPENDENCY) test-array.cpp dstDynamicArray.h
	g++ $(CFLAGS_TEST) test-array.cpp -o test-array $(TEST_PROGRAM_LFLAGS) -lm 

clean :
	rm -f $(LIBRARY_MODULE_OBJECTS)
	rm -f $(TEST_PROGRAMS)
	rm -f $(LIBRARY_NAME).so.$(VERSION)
	rm -f $(LIBRARY_NAME).a
	rm -f $(LIBRARY_NAME)_dbg.a

.cpp.o :
	g++ -c $(CFLAGS_LIB) $< -o $@

dep :
	rm -f .depend
	make .depend

.depend: Makefile.conf Makefile
	g++ -MM $(patsubst %.o,%.cpp,$(LIBRARY_MODULE_OBJECTS)) >> .depend
        # Make sure Makefile.conf is a dependency for all modules.
	for x in $(LIBRARY_MODULE_OBJECTS); do \
	echo $$x : Makefile.conf >> .depend; done
	echo '# Module dependencies' >> .depend
	g++ -MM test-random.cpp >> .depend

include .depend
