# Do not edit normally. Configuration settings are in Makefile.conf.

include Makefile.conf

SHORT_LIBRARY_NAME = datasetturbo
LIBRARY_NAME = lib$(SHORT_LIBRARY_NAME)
VERSION = 0.3
SO_VERSION = 0.3
MAJOR_VERSION = 0

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
# Shared library.
LIBRARY_OBJECT = $(LIBRARY_NAME).so.$(VERSION)
INSTALL_TARGET = install_shared
LIBRARY_DEPENDENCY =
TEST_PROGRAM_LFLAGS = -l$(SHORT_LIBRARY_NAME)
CFLAGS_LIB = $(CFLAGS) -fPIC -fvisibility=hidden -DDST_SHARED -DDST_SHARED_EXPORTS
CFLAGS_TEST = $(CFLAGS)
else
# Static or static debug version.
ifeq ($(LIBRARY_CONFIGURATION), DEBUG)
LIBRARY_OBJECT = $(LIBRARY_NAME)_dbg.a
else
LIBRARY_OBJECT = $(LIBRARY_NAME).a
endif
# install_static also works for debugging library
INSTALL_TARGET = install_static
LIBRARY_DEPENDENCY = $(LIBRARY_OBJECT)
TEST_PROGRAM_LFLAGS = $(LIBRARY_OBJECT)
CFLAGS_LIB = $(CFLAGS)
CFLAGS_TEST = $(CFLAGS)
endif

LIBRARY_LIBS = -lpthread -lm -lc

LIBRARY_MODULE_OBJECTS = dstMisc.o dstRandom.o dstRNGCMWC.o dstThread.o dstSIMDDotMatrix.o \
	dstVectorMath.o dstVectorMathSIMD.o dstMatrixMath.o dstMatrixMathSIMD.o
LIBRARY_HEADER_FILES = dstConfig.h dstRandom.h dstDynamicArray.h dstTimer.h dstThread.h \
	dstSIMD.h dstSIMDDot.h dstSIMDMatrix.h dstSIMDSSE2.h \
	dstMath.h dstVectorMath.h dstColor.h dstVectorMathSIMD.h dstMatrixMath.h dstMatrixMathSIMD.h
TEST_PROGRAMS = test-random test-array

LIBRARY_PKG_CONFIG_FILE = datasetturbo.pc

default : library

library : $(LIBRARY_OBJECT) $(LIBRARY_PKG_CONFIG_FILE)

$(LIBRARY_NAME).so.$(VERSION) : $(LIBRARY_MODULE_OBJECTS) $(LIBRARY_HEADER_FILES)
	g++ -shared -Wl,-soname,$(LIBRARY_NAME).so.$(SO_VERSION) -fPIC -o $(LIBRARY_OBJECT) \
$(LIBRARY_MODULE_OBJECTS) $(LIBRARY_LIBS)
	@echo Run '(sudo) make install to install.'

$(LIBRARY_NAME).a : $(LIBRARY_MODULE_OBJECTS) $(LIBRARY_HEADER_FILES)
	ar r $(LIBRARY_OBJECT) $(LIBRARY_MODULE_OBJECTS)
	@echo 'Run (sudo) make install to install, or make test to build the test programs.'

$(LIBRARY_NAME)_dbg.a : $(LIBRARY_MODULE_OBJECTS) $(LIBRARY_HEADER_FILES)
	ar r $(LIBRARY_OBJECT) $(LIBRARY_MODULE_OBJECTS)
	@echo 'The library is compiled with debugging enabled.'

install : $(INSTALL_TARGET) install_headers install_pkgconfig

install_headers : $(LIBRARY_HEADER_FILES)
	mkdir -p $(HEADER_FILE_INSTALL_DIR)/DataSetTurbo
	@for x in $(LIBRARY_HEADER_FILES); do \
	echo Installing $(HEADER_FILE_INSTALL_DIR)/DataSetTurbo/$$x.; \
	install -m 0644 $$x $(HEADER_FILE_INSTALL_DIR)/DataSetTurbo/$$x; done

install_shared : $(LIBRARY_OBJECT)
	install -m 0644 $(LIBRARY_OBJECT) $(SHARED_LIB_DIR)/$(LIBRARY_OBJECT)
	ln -sf $(SHARED_LIB_DIR)/$(LIBRARY_OBJECT) $(SHARED_LIB_DIR)/$(LIBRARY_NAME).so
#	ln -sf $(SHARED_LIB_DIR)/$(LIBRARY_OBJECT) $(SHARED_LIB_DIR)/$(LIBRARY_NAME).so.$(MAJOR_VERSION)
	@echo 'Run make test to build the test programs.'

install_static : $(LIBRARY_OBJECT)
	install -m 0644 $(LIBRARY_OBJECT) $(STATIC_LIB_DIR)/$(LIBRARY_OBJECT)

install_pkgconfig : $(LIBRARY_PKG_CONFIG_FILE)
	install -m 0644 datasetturbo.pc $(PKG_CONFIG_INSTALL_DIR)/pkgconfig/$(LIBRARY_PKG_CONFIG_FILE) 

uninstall_libs :
	@echo Removing all versions of libdatasetturbo in /usr/lib.
	rm -f /usr/lib/libdatasetturbo.*
	@echo Removing all versions of libdatasetturbo in $(STATIC_LIB_DIR) and $(SHARED_LIB_DIR)
	rm -f $(STATIC_LIB_DIR)/libdatasetturbo.*
	rm -f $(SHARED_LIB_DIR)/libdatasetturbo.*

test : $(TEST_PROGRAMS)

test-random :  $(LIBRARY_DEPENDENCY) test-random.cpp dstRandom.h
	g++ $(CFLAGS_TEST) test-random.cpp -o test-random -lfgen $(TEST_PROGRAM_LFLAGS) -lm 

test-array :  $(LIBRARY_DEPENDENCY) test-array.cpp dstDynamicArray.h
	g++ $(CFLAGS_TEST) test-array.cpp -o test-array $(TEST_PROGRAM_LFLAGS) -lm

$(LIBRARY_PKG_CONFIG_FILE) : Makefile.conf Makefile
	@echo Generating datasetturbo.pc.
#	@echo libdir=$(LIB_INSTALL_DIR) > datasetturbo.pc
#	@echo includedir=${HEADER_FILE_INSTALL_DIR} > datasetturbo.pc
	@echo Name: datasetturbo > datasetturbo.pc
	@echo Version: $(VERSION) >> datasetturbo.pc
	@echo Description: DataSetTurbo library >> datasetturbo.pc
	@echo Requires: $(PKG_CONFIG_REQUIREMENTS) >> datasetturbo.pc
	@echo Libs: -ldatasetturbo >> datasetturbo.pc
	@echo Cflags: -I$(HEADER_FILE_INSTALL_DIR)/DataSetTurbo >> datasetturbo.pc

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
        # Make sure Makefile.conf and Makefile are dependency for all modules.
	for x in $(LIBRARY_MODULE_OBJECTS); do \
	echo $$x : Makefile.conf Makefile >> .depend; done
	echo '# Module dependencies' >> .depend
	g++ -MM test-random.cpp >> .depend

include .depend
