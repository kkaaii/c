TARGET=app
CC=gcc
RM=rm

SOURCES=\
	main.c\
	host.c\
	device.c\
	pcie.c\
	platform_linux.c\
	../src/nvme/nvme_queue.c\
	../src/host/nvme_host.c\
	../src/host/admin/identify_host.c\
	../src/host/admin/get_logpage_host.c\
	../src/host/admin/fw_download_host.c\
	../src/host/admin/fw_commit_host.c\
	../src/host/admin/features_host.c\
	../src/device/nvme_device.c\
	../src/device/admin/identify_device.c\
	../src/device/admin/get_logpage_device.c\
	../src/device/admin/fw_download_device.c\
	../src/device/admin/fw_commit_device.c\
        ../src/device/admin/features_device.c\
	../src/tests/identify_test.c\
	../src/tests/get_logpage_test.c\
	../src/tests/fw_download_test.c\
	../src/tests/fw_commit_test.c\
	../src/tests/get_features_test.c\
	../src/tests/set_features_test.c\

CFLAGS+=-I ../inc -I ../inc/host -I ../inc/device
LFLAGS+=-lpthread

OBJS=$(SOURCES:.c=.o)

.PHONY = all clean clobber

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

clean:
	-$(RM) $(OBJS)

clobber: clean
	-$(RM) $(TARGET)
