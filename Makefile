# Do not edit normally. Configuration settings are in Makefile.conf.

TARGET_MACHINE := $(shell gcc -dumpmachine)

include Makefile.conf

SHORT_LIBRARY_NAME = datasetturbo
LIBRARY_NAME = lib$(SHORT_LIBRARY_NAME)
VERSION = 0.5
SO_VERSION = 0.5
MAJOR_VERSION = 0

# Autodetect platform based on TARGET_MACHINE
ifneq (,$(findstring x86,$(TARGET_MACHINE)))
DETECTED_CPU=X86
CPU_DESCRIPTION="Detected CPU platform: x86"
else
ifneq (,$(findstring arm,$(TARGET_MACHINE)))
DETECTED_CPU=ARM
CPU_DESCRIPTION="Detected CPU platform: ARM"
else
endif
endif

ifeq (,$(TARGET_ARCH))
ifneq (,$(DETECTED_CPU))
TARGET_ARCH = $(DETECTED_CPU)
else
$(error Could not detect CPU platform.)
endif
endif

# CFLAGS with optional tuning for CPU
OPTCFLAGS = $(TARGET_CPU_FLAGS)

ARM_ASSEMBLER_FLAGS_THUMB = -march=armv7-a -Wa,-march=armv7-a -mthumb -Wa,-mthumb \
 -Wa,-mimplicit-it=always -mthumb-interwork -DCONFIG_THUMB
ARM_ASSEMBLER_FLAGS_NON_THUMB = 

# SIMD configuration (SSE on x86, NEON on ARM).
ifeq ($(TARGET_ARCH), X86)
SIMD_TYPES = SSE2 SSE3 SSSE3 SSE4A # SSE41 SSE42 AVX NEON AVX_SSE4A_FMA4 X86_AVX_FMA
else ifeq ($TARGET_ARCH), ARM)
# ARM NEON is currently not yet functional.
SIMD_TYPES = #NEON
else
$(error Unknown value for TARGET_ARCH.)
endif

# Select the compiled in SIMD types.
ifneq ($(TARGET_ARCH),)
ifneq ($(TARGET_SIMD), NONE)
# When no fixed SIMD architecture is specified, many will be supported and autodetected.
ifeq ($(TARGET_ARCH), X86)
# All types should be configured by default.
SIMD_MODULES = dstSIMDSSE2.o dstSIMDSSE3.o dstSIMDSSSE3.o dstSIMDSSE4A.o # Add the rest.
SIMD_MODULES += dstSIMDSSE2Stream.o dstSIMDSSE3Stream.o dstSIMDSSSE3Stream.o dstSIMDSSE4AStream.o
else
ifeq ($(TARGET_ARCH), ARM)
# ARM NEON is currently not yet functional.
SIMD_MODULES = #dstSIMDNEON.o
TARGET_SIMD = NONE
endif
endif
endif
endif

# Enforce a fixed SIMD level when indicated.
FIXED_SIMD_TYPE =
FIXED_NO_SIMD =
ifneq ($(TARGET_SIMD),)
ifeq ($(TARGET_SIMD), NONE)
# This flag has been moved to the automatically generated file dstConfigParams.h.
# OPTCFLAGS +=-DDST_NO_SIMD
FIXED_NO_SIMD = DST_NO_SIMD
else
# This flag has been moved to the automatically generated file dstConfigParams.h.
# OPTCFLAGS += -DDST_FIXED_SIMD

ifeq ($(TARGET_SIMD), X86_SSE2)
OPTCFLAGS += -msse2
FIXED_SIMD_TYPE = SSE2
else
ifeq ($(TARGET_SIMD), X86_SSE3)
OPTCFLAGS += -msse3
FIXED_SIMD_TYPE = SSE3
else
ifeq ($(TARGET_SIMD), X86_SSSE3)
OPTCFLAGS += -mssse3
FIXED_SIMD_TYPE = SSE3
else
ifeq ($(TARGET_SIMD), X86_SSE4A)
# AMD's SSE4A does not support SSSE3.
OPTCFLAGS += -msse4a
FIXED_SIMD_TYPE = SSE4A
else
ifeq ($(TARGET_SIMD), X86_SSE41)
OPTCFLAGS += -mssse41
FIXED_SIMD_TYPE = SSE41
else
ifeq ($(TARGET_SIMD), X86_SSE42)
OPTCFLAGS += -msse42
FIXED_SIMD_TYPE = SSE42
else
ifeq ($(TARGET_SIMD), X86_AVX)
OPTCFLAGS += -mavx
FIXED_SIMD_TYPE = AVX
else
ifeq ($(TARGET_SIMD), ARM_NEON)
OPTCFLAGS += -mfpu=neon
FIXED_SIMD_TYPE = NEON
else
ifeq ($(TARGET_SIMD), X86_AVX_SSE4A_FMA4)
OPTCFLAGS += -mavx -msse4a -mfma4
FIXED_SIMD_TYPE = AVX_SSE4A_FMA4
else
ifeq ($(TARGET_SIMD), X86_AVX_FMA)
OPTCFLAGS += -mavx -mfma
FIXED_SIMD_TYPE = AVX_FMA
else
ifneq ($(TARGET_SIMD),)
$(error Not a valid SIMD type.)
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
# This flag has been moved to the automatically generated file dstConfigParams.h.
# OPTFLAGS += -DDST_FIXED_SIMD_$(FIXED_SIMD_TYPE)
SIMD_MODULES = dstSIMD$(FIXED_SIMD_TYPE).o dstSIMD$(FIXED_SIMD_TYPE)Stream.o

endif	# TARGET_SIMD != NONE
endif	# TARGET_SIMD != <empty>
# When certain SIMD support is guaranteed to be available, the library will be compiled
# as such. When DST_NO_SIMD is defined, SIMD code is excluded entirely.
# Otherwise, SIMD support is auto-detected.

# Compile time configuration definitions
OPT_DEFINES =

# Memcpy, memory alignment
ifeq ($(PREFER_LIBC_MEMCPY), YES)
OPT_DEFINES += -DDST_PREFER_LIBC_MEMCPY
endif
ifeq ($(UNALIGNED_MEMORY_ACCESS), YES)
OPT_DEFINES += -DDST_UNALIGNED_MEMORY_ACCESS
endif


ifeq ($(LIBRARY_CONFIGURATION), DEBUG)
# Adding some optimization even when compiling for debugging can help compilation
# problems related to arguments that need to immediate constants.
OPTCFLAGS += -O -ggdb
else
OPTCFLAGS += -Ofast -ffast-math
endif

CFLAGS = -Wall -pipe -I. $(OPTCFLAGS) $(OPT_DEFINES)

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

LIBRARY_CPP_MODULE_OBJECTS = dstMisc.o dstRandom.o dstRNGCMWC.o dstThread.o \
	dstVectorMath.o dstMatrixMath.o dstCpuInfo.o dstDotMatrixNoSIMD.o \
	$(SIMD_MODULES) dstMatrixMathSIMD.o
LIBRARY_ASM_MODULE_OBJECTS = dstARMMemset.o
LIBRARY_MODULE_OBJECTS = $(LIBRARY_CPP_MODULE_OBJECTS) $(LIBRARY_ASM_MODULE_OBJECTS)
LIBRARY_HEADER_FILES = dstConfig.h dstMisc.h dstRandom.h dstDynamicArray.h dstQueue.h \
	dstTimer.h dstThread.h \
	dstSIMD.h dstSIMDDot.h dstSIMDMatrix.h dstSIMDSSE2.h dstSIMDFuncs.h \
	dstMath.h dstMemory.h \
	dstVectorMath.h dstColor.h dstVectorMathSIMD.h dstMatrixMath.h dstMatrixMathSIMD.h \
	dstConfigParams.h
TEST_PROGRAMS = test-random test-array
TEST_PROGRAMS += test-simd

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

test-random :  $(LIBRARY_DEPENDENCY) test-random.cpp
	g++ $(CFLAGS_TEST) test-random.cpp -o test-random -lfgen $(TEST_PROGRAM_LFLAGS) -lm 

test-array :  $(LIBRARY_DEPENDENCY) test-array.cpp
	g++ $(CFLAGS_TEST) test-array.cpp -o test-array $(TEST_PROGRAM_LFLAGS) -lm

test-simd :  $(LIBRARY_DEPENDENCY) test-simd.cpp
	g++ $(CFLAGS_TEST) test-simd.cpp -o test-simd $(TEST_PROGRAM_LFLAGS) -lpthread -lm

dstConfigParams.h : Makefile.conf Makefile
	{ echo '// Automatically generated'; \
	for x in $(FIXED_SIMD_TYPE); do \
		echo '#define DST_FIXED_SIMD'; \
		echo '#define DST_FIXED_SIMD_$(FIXED_SIMD_TYPE)'; \
	done; \
	for x in $(FIXED_NO_SIMD); do \
		echo '#define DST_NO_SIMD'; \
	done; } > dstConfigParams.h

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
	@for y in $(SIMD_TYPES); do \
		SSEMATCH=`echo $\$< | grep $$y | wc -l`;  \
		if [ $$SSEMATCH != 0 ]; then \
			TOLOWERCMD='tr '\''[:upper:]'\'' '\''[:lower:]'\'''; \
			SSENAMELOWER=`echo $$y | $$TOLOWERCMD`; \
			SSEFLAGS='-m'$$SSENAMELOWER' -DDST_SIMD_MODE_'$$y; \
		fi; \
	done; \
	STREAMMATCH=`echo $\$< | grep Stream | wc -l`; \
	if [ $$STREAMMATCH != 0 ]; then \
		STREAMFLAGS='-DDST_SIMD_MODE_STREAM'; \
	fi; \
	CMD='g++ -c '$$SSEFLAGS' '$$STREAMFLAGS' $(CFLAGS_LIB) $< -o $@'; \
	echo $$CMD; $$CMD

.S.o :
	@ARM=`cat /proc/cpuinfo | grep ARMv | wc -l`; \
	if [ ARM > 0 ]; then \
	THUMB=`echo | gcc -dM -E - | grep __thumb2__ | wc -l`; \
	if [ THUMB > 0 ]; then \
		x=$(ARM_THUMB ASSEMBLER_FLAGS); else \
		x=$(ARM_NON_THUMB_ASSEMBLER_FLAGS); \
	fi; \
fi; \
g++ -c $(CFLAGS_LIB) $x $< -o $@

dep :
	rm -f .depend
	make .depend

.depend: Makefile.conf Makefile dstConfigParams.h
	g++ -MM $(CFLAGS_LIB) $(patsubst %.o,%.cpp,$(LIBRARY_CPP_MODULE_OBJECTS)) >> .depend
	g++ -MM $(CFLAGS_LIB) $(patsubst %.o,%.S,$(LIBRARY_ASM_MODULE_OBJECTS)) >> .depend
        # Make sure Makefile.conf and Makefile are dependency for all modules.
	for x in $(LIBRARY_MODULE_OBJECTS); do \
	echo $$x : Makefile.conf Makefile >> .depend; done
	echo '# Module dependencies' >> .depend
	g++ -MM test-random.cpp >> .depend
	g++ -MM test-array.cpp >> .depend
	g++ -MM test-simd.cpp >> .depend

include .depend
