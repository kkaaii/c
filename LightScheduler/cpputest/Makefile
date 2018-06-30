ifndef SILENCE
	SILENCE = @
endif

COMPONENT_NAME = LightScheduler
CPPUTEST_HOME = ../../cpputest-3.8
CPPUTEST_EXE_FLAGS = -v -c

CPPFLAGS += -I$(CPPUTEST_HOME)/include
CXXFLAGS += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorNewMacros.h
CFLAGS += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorMallocMacros.h
LD_LIBRARIES = -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt

SRC_DIRS = \
	.\

INCLUDE_DIRS = \
	.\
	..\
	$(CPPUTEST_HOME)/include

include $(CPPUTEST_HOME)/build/MakefileWorker.mk

