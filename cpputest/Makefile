ifndef SILENCE
	SILENCE = @
endif

COMPONENT_NAME = c
CPPUTEST_HOME = ../cpputest-3.8
CPPUTEST_EXE_FLAGS = -v -c

CPPFLAGS += -I$(CPPUTEST_HOME)/include
CXXFLAGS += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorNewMacros.h
CFLAGS += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorMallocMacros.h
LD_LIBRARIES = -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt

SRC_DIRS = \
	./\
	../SinglyLinkedList	\
	../CircularlySinglyLinkedList	\
	../BinaryIndexedTree	\
	../SegmentTree		\
	../tree		\
	./Mock \
	../misc/gen_forward	\

INCLUDE_DIRS = \
	./Mock \
	../SinglyLinkedList	\
	../CircularlySinglyLinkedList	\
	../BinaryIndexedTree	\
	../SegmentTree		\
	../tree		\
	../misc/gen_forward	\
	$(CPPUTEST_HOME)/include

include $(CPPUTEST_HOME)/build/MakefileWorker.mk

