#include "CppUTest/TestHarness.h"
#include <stdio.h>

extern "C" {
	#include "../ATA_Vars.c"
	#include "../Cmds/IdentifyDevice.c"
	#include "../Logs/Log30h_IDENTIFY_DEVICE_data_log.c"
}

void (*gReadLog30h[])(void *buf) = {
	NULL,
	NULL,
	ReadLog30h_Page02h_Capacity,
	ReadLog30h_Page03h_Supported_Capabilities,
	ReadLog30h_Page04h_Current_Settings,
	ReadLog30h_Page05h_Strings,
	ReadLog30h_Page06h_Security,
	ReadLog30h_Page07h_Parallel_ATA,
	ReadLog30h_Page08h_Serial_ATA
};

TEST_GROUP(ATA_Vars)
{
	uint8 mBuf[512];
	void checkIdentifyDeviceBit(int iWord, int iBit, int index) {
		uint16	*word = (uint16 *)mBuf;
		uint16	mask = (uint16)BIT(iBit);

		ATA_BitSet(index);
		IdentifyDevice(mBuf);
		CHECK(word[iWord] & mask);

		ATA_BitClr(index);
		IdentifyDevice(mBuf);
		CHECK_FALSE(word[iWord] & mask);
	}

	void checkIdentifyDeviceField(int iWord, int iBit, int bits, int val) {
		uint16	*word = (uint16 *)mBuf;
		uint16	mask = (uint16)((1u << bits) - 1);

		IdentifyDevice(mBuf);
		CHECK_EQUAL(val, (word[iWord] >> iBit) & mask);
	}

	template <typename T>
	void checkIdentifyDeviceValue(int iWord, T expected) {
		T	*actual = (T *)((uint16 *)mBuf + iWord);
		IdentifyDevice(mBuf);
		CHECK_EQUAL(expected, *actual);
	}

	void checkIdentifyDeviceBuffer(int iWord, size_t bytes, void *expected) {
		void	*actual = (uint16 *)mBuf + iWord;
		IdentifyDevice(mBuf);
		MEMCMP_EQUAL(expected, actual, bytes);
	}

	void checkLog30hBit(int page, int iQword, int iBit, int index) {
		void	(*ReadLog30h)(void *) = gReadLog30h[page];
		uint64	*qword = (uint64 *)mBuf;
		uint64	mask = 1ull << iBit;

		ATA_BitSet(index);
		ReadLog30h(mBuf);
		CHECK(qword[iQword] & mask);

		ATA_BitClr(index);
		ReadLog30h(mBuf);
		CHECK_FALSE(qword[iQword] & mask);
	}

	void checkLog30hField(int page, int iQword, int iBit, int bits, uint64 val) {
		void	(*ReadLog30hPageX)(void *) = gReadLog30h[page];
		uint64	*qword = (uint64 *)mBuf;
		uint64	mask = (uint64)((1ull << bits) - 1);

		ReadLog30hPageX(mBuf);
		CHECK_EQUAL(val, (qword[iQword] >> iBit) & mask);
	}

	void checkLog30hBuffer(int page, int iQword, size_t bytes, void *expected) {
		void	(*ReadLog30hPageX)(void *) = gReadLog30h[page];
		void	*actual = (uint64 *)mBuf + iQword;

		ReadLog30hPageX(mBuf);
		MEMCMP_EQUAL(expected, actual, bytes);
	}
};

TEST(ATA_Vars, field_WORLD_WIDE_NAME)
{
#undef	BYTES
#define	BYTES	8
	char	s[] = "87654321        ", *p = s;

	for (int i = BYTES; i > 0; --i, ++p) {
		memcpy(ATA_Field(WORLD_WIDE_NAME), p, BYTES);
		checkLog30hBuffer(PAGE_NUMBER_03H, 56/8, BYTES, p);
		checkIdentifyDeviceBuffer(108, BYTES, p);
	}
}

TEST(ATA_Vars, field_SERIAL_NUMBER)
{
#undef	BYTES
#define	BYTES	20
	char	s[] = "98765432109876543210                    ", *p = s;

	for (int i = BYTES; i > 0; --i, ++p) {
		memcpy(ATA_Field(SERIAL_NUMBER), p, BYTES);
		checkLog30hBuffer(PAGE_NUMBER_05H, 8/8, BYTES, p);
		checkIdentifyDeviceBuffer(10, BYTES, p);
	}
}

TEST(ATA_Vars, field_MODEL_NUMBER)
{
#undef	BYTES
#define	BYTES	40
	char	s[] = "9876543210987654321098765432109876543210                                        ", *p = s;

	for (int i = BYTES; i > 0; --i, ++p) {
		memcpy(ATA_Field(MODEL_NUMBER), p, BYTES);
		checkLog30hBuffer(PAGE_NUMBER_05H, 48/8, BYTES, p);
		checkIdentifyDeviceBuffer(27, BYTES, p);
	}
}

TEST(ATA_Vars, field_FIRMWARE_REVISION)
{
#undef	BYTES
#define	BYTES	8
	char	s[] = "87654321        ", *p = s;

	for (int i = BYTES; i > 0; --i, ++p) {
		memcpy(ATA_Field(FIRMWARE_REVISION), p, BYTES);
		checkLog30hBuffer(PAGE_NUMBER_05H, 32/8, BYTES, p);
		checkIdentifyDeviceBuffer(23, BYTES, p);
	}
}

TEST(ATA_Vars, field_ADDITIONAL_PRODUCT_IDENTIFIER)
{
#undef	BYTES
#define	BYTES	8
	char	s[] = "87654321        ", *p = s;

	for (int i = BYTES; i > 0; --i, ++p) {
		memcpy(ATA_Field(ADDITIONAL_PRODUCT_IDENTIFIER), p, BYTES);
		checkLog30hBuffer(PAGE_NUMBER_05H, 96/8, BYTES, p);
		checkIdentifyDeviceBuffer(170, BYTES, p);
	}
}

TEST(ATA_Vars, field_BUFFER_SIZE)
{
#undef	BITS
#define	BITS	63
	for (int bits = BITS; bits >= 0; --bits) {
		uint64 val = (uint64)((1ull << bits) - 1);
		ATA_Field(BUFFER_SIZE) = val & ((1ull << BITS) - 1);
		checkLog30hField(PAGE_NUMBER_02H, 32/8, 0, BITS, val);
	}
}

TEST(ATA_Vars, field_ACCESSIBLE_CAPACITY)
{
#undef	BITS
#define	BITS	48
	for (int bits = BITS; bits >= 0; --bits) {
		uint64 val = (uint64)((1ull << bits) - 1);
		ATA_Field(ACCESSIBLE_CAPACITY) = val & ((1ull << BITS) - 1);
		checkLog30hField(PAGE_NUMBER_02H, 8/8, 0, BITS, val);
	}

#define	MAX_28BIT_CAPACITY	0x0FFFFFFF
#define	MAX_32BIT_CAPACITY	0xFFFFFFFF
#define	MAX_48BIT_CAPACITY	0xFFFFFFFFFFFFull
	ATA_BitClr(bit_48BIT_SUPPORTED);
	{
		ATA_BitClr(bit_ExtendedNumberOfUserAddressableSectorsSupported);
		{
			ATA_Field(ACCESSIBLE_CAPACITY) = MAX_28BIT_CAPACITY;
			checkIdentifyDeviceValue(60, (uint32)MAX_28BIT_CAPACITY);
			checkIdentifyDeviceValue(100, 0ull);
			checkIdentifyDeviceValue(230, 0ull);
		}
		ATA_BitSet(bit_ExtendedNumberOfUserAddressableSectorsSupported);
		{
			ATA_Field(ACCESSIBLE_CAPACITY) = MAX_28BIT_CAPACITY;
			checkIdentifyDeviceValue(60, (uint32)MAX_28BIT_CAPACITY);
			checkIdentifyDeviceValue(100, 0ull);
			checkIdentifyDeviceValue(230, 0ull);
		}
	}

	ATA_BitSet(bit_48BIT_SUPPORTED);
	{
		ATA_BitClr(bit_ExtendedNumberOfUserAddressableSectorsSupported);
		{
			ATA_Field(ACCESSIBLE_CAPACITY) = MAX_28BIT_CAPACITY;
			checkIdentifyDeviceValue(60, (uint32)MAX_28BIT_CAPACITY);
			checkIdentifyDeviceValue(100, (uint64)MAX_28BIT_CAPACITY);
			checkIdentifyDeviceValue(230, 0ull);

			ATA_Field(ACCESSIBLE_CAPACITY) = MAX_32BIT_CAPACITY;
			checkIdentifyDeviceValue(60, (uint32)MAX_28BIT_CAPACITY);
			checkIdentifyDeviceValue(100, (uint64)MAX_32BIT_CAPACITY);
			checkIdentifyDeviceValue(230, 0ull);

			ATA_Field(ACCESSIBLE_CAPACITY) = MAX_48BIT_CAPACITY;
			checkIdentifyDeviceValue(60, (uint32)MAX_28BIT_CAPACITY);
			checkIdentifyDeviceValue(100, (uint64)MAX_48BIT_CAPACITY);
			checkIdentifyDeviceValue(230, 0ull);
		}
		ATA_BitSet(bit_ExtendedNumberOfUserAddressableSectorsSupported);
		{
			ATA_Field(ACCESSIBLE_CAPACITY) = MAX_28BIT_CAPACITY;
			checkIdentifyDeviceValue(60, (uint32)MAX_28BIT_CAPACITY);
			checkIdentifyDeviceValue(100, (uint64)MAX_28BIT_CAPACITY);
			checkIdentifyDeviceValue(230, (uint64)MAX_28BIT_CAPACITY);

			ATA_Field(ACCESSIBLE_CAPACITY) = MAX_32BIT_CAPACITY;
			checkIdentifyDeviceValue(60, (uint32)MAX_28BIT_CAPACITY);
			checkIdentifyDeviceValue(100, (uint64)MAX_32BIT_CAPACITY);
			checkIdentifyDeviceValue(230, (uint64)MAX_32BIT_CAPACITY);

			ATA_Field(ACCESSIBLE_CAPACITY) = MAX_48BIT_CAPACITY;
			checkIdentifyDeviceValue(60, (uint32)MAX_28BIT_CAPACITY);
			checkIdentifyDeviceValue(100, (uint64)MAX_48BIT_CAPACITY);
			checkIdentifyDeviceValue(230, (uint64)MAX_48BIT_CAPACITY);
		}
	}
}

TEST(ATA_Vars, field_WRV_MODE3_COUNT)
{
	for (int bits = 32; bits >= 0; --bits) {
		uint32 val = (uint32)((1ull << bits) - 1);
		ATA_Field(WRV_MODE3_COUNT) = val;
		checkLog30hField(PAGE_NUMBER_03H, 40/8, 0, 32, val);
		checkIdentifyDeviceValue(210, val);
	}
}

TEST(ATA_Vars, field_WRV_MODE2_COUNT)
{
	for (int bits = 32; bits >= 0; --bits) {
		uint32 val = (uint32)((1ull << bits) - 1);
		ATA_Field(WRV_MODE2_COUNT) = val;
		checkLog30hField(PAGE_NUMBER_03H, 48/8, 0, 32, val);
		checkIdentifyDeviceValue(212, val);
	}
}

TEST(ATA_Vars, field_UTILIZATION_B)
{
	for (int bits = 32; bits >= 0; --bits) {
		uint32 val = (uint32)((1ull << bits) - 1);
		ATA_Field(UTILIZATION_B) = val;
		checkLog30hField(PAGE_NUMBER_03H, 80/8, 32, 32, val);
	}
}

TEST(ATA_Vars, field_UTILIZATION_A)
{
	for (int bits = 32; bits >= 0; --bits) {
		uint32 val = (uint32)((1ull << bits) - 1);
		ATA_Field(UTILIZATION_A) = val;
		checkLog30hField(PAGE_NUMBER_03H, 80/8, 0, 32, val);
	}
}

TEST(ATA_Vars, field_STREAM_GRANULARITY)
{
	for (int bits = 32; bits >= 0; --bits) {
		uint32 val = (uint32)((1ull << bits) - 1);
		ATA_Field(STREAM_GRANULARITY) = val;
		checkLog30hField(PAGE_NUMBER_04H, 56/8, 0, 32, val);
		checkIdentifyDeviceValue(98, val);
	}
}

TEST(ATA_Vars, field_LOGICAL_SECTOR_SIZE)
{
	for (int bits = 32; bits >= 0; --bits) {
		uint32 val = (uint32)((1ull << bits) - 1);
		ATA_Field(LOGICAL_SECTOR_SIZE) = val;
		checkLog30hField(PAGE_NUMBER_02H, 24/8, 0, 32, val);
		checkIdentifyDeviceValue(117, val);
	}
}

TEST(ATA_Vars, field_ABO_MINIMUM_FRACTION)
{
	for (int bits = 32; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(ABO_MINIMUM_FRACTION) = val;
		checkLog30hField(PAGE_NUMBER_03H, 120/8, 16, 32, val);
	}
}

TEST(ATA_Vars, field_TIME_TO_PERFORMANCE_DEGRATION)
{
#undef	BITS
#define	BITS	16
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(TIME_TO_PERFORMANCE_DEGRATION) = val;
		checkLog30hField(PAGE_NUMBER_04H, 72/8, 16, BITS, val);
	}
}

TEST(ATA_Vars, field_TIME_SCHEDULED_FOR_DEVICE_MAINTENANCE)
{
#undef	BITS
#define	BITS	16
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(TIME_SCHEDULED_FOR_DEVICE_MAINTENANCE) = val;
		checkLog30hField(PAGE_NUMBER_04H, 72/8, 32, BITS, val);
	}
}

TEST(ATA_Vars, field_SUPPORTED_HARDWARE_FEATURE_CONTROL_IDENTIFIER)
{
#undef	BITS
#define	BITS	16
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(SUPPORTED_HARDWARE_FEATURE_CONTROL_IDENTIFIER) = val;
		checkLog30hField(PAGE_NUMBER_08H, 40/8, 16, BITS, val);
	}
}

TEST(ATA_Vars, field_STREAM_MIN_REQUEST_SIZE)
{
#undef	BITS
#define	BITS	16
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(STREAM_MIN_REQUEST_SIZE) = val;
		checkLog30hField(PAGE_NUMBER_04H, 40/8, 0, BITS, val);
		checkIdentifyDeviceValue(95, val);
	}
}

TEST(ATA_Vars, field_STREAM_ACCESS_LATENCY)
{
#undef	BITS
#define	BITS	16
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(STREAM_ACCESS_LATENCY) = val;
		checkLog30hField(PAGE_NUMBER_04H, 48/8, 0, BITS, val);
		checkIdentifyDeviceValue(97, val);
	}
}

TEST(ATA_Vars, field_RECOMMENDED_MULTIWORD_CYCLE_TIME)
{
#undef	BITS
#define	BITS	16
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(RECOMMENDED_MULTIWORD_CYCLE_TIME) = val;
		checkLog30hField(PAGE_NUMBER_07H, 24/8, 16, BITS, val);
		checkIdentifyDeviceValue(66, val);
	}
}

TEST(ATA_Vars, field_PIO_SECTOR_TIME)
{
#undef	BITS
#define	BITS	16
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(PIO_SECTOR_TIME) = val;
		checkLog30hField(PAGE_NUMBER_04H, 32/8, 0, BITS, val);
		checkIdentifyDeviceValue(104, val);
	}
}

TEST(ATA_Vars, field_NOMINAL_MEDIA_ROTATION_RATE)
{
#undef	BITS
#define	BITS	16
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(NOMINAL_MEDIA_ROTATION_RATE) = val;
		checkLog30hField(PAGE_NUMBER_03H, 24/8, 0, BITS, val);
		checkIdentifyDeviceValue(217, val);
	}
}

TEST(ATA_Vars, field_MINIMUM_INACTIVE_TIME)
{
#undef	BITS
#define	BITS	16
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(MINIMUM_INACTIVE_TIME) = val;
		checkLog30hField(PAGE_NUMBER_04H, 72/8, 0, BITS, val);
	}
}

TEST(ATA_Vars, field_MIN_PIO_TRANSFER_TIME_WITHOUT_IORDY)
{
#undef	BITS
#define	BITS	16
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(MIN_PIO_TRANSFER_TIME_WITHOUT_IORDY) = val;
		checkLog30hField(PAGE_NUMBER_07H, 32/8, 0, BITS, val);
		checkIdentifyDeviceValue(67, val);
	}
}

TEST(ATA_Vars, field_MIN_PIO_TRANSFER_TIME_WITH_IORDY)
{
#undef	BITS
#define	BITS	16
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(MIN_PIO_TRANSFER_TIME_WITH_IORDY) = val;
		checkLog30hField(PAGE_NUMBER_07H, 32/8, 16, BITS, val);
		checkIdentifyDeviceValue(68, val);
	}
}

TEST(ATA_Vars, field_MIN_MULTIWORD_CYCLE_TIME)
{
#undef	BITS
#define	BITS	16
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(MIN_MULTIWORD_CYCLE_TIME) = val;
		checkLog30hField(PAGE_NUMBER_07H, 24/8, 0, BITS, val);
		checkIdentifyDeviceValue(65, val);
	}
}

TEST(ATA_Vars, field_MAX_PAGES_PER_DSM_COMMAND)
{
#undef	BITS
#define	BITS	16
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(MAX_PAGES_PER_DSM_COMMAND) = val;
		checkLog30hField(PAGE_NUMBER_03H, 72/8, 16, BITS, val);
		checkIdentifyDeviceValue(105, val);
	}
}

TEST(ATA_Vars, field_MASTER_PASSWORD_IDENTIFIER)
{
#undef	BITS
#define	BITS	16
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(MASTER_PASSWORD_IDENTIFIER) = val;
		checkLog30hField(PAGE_NUMBER_06H, 8/8, 0, BITS, val);
		checkIdentifyDeviceValue(92, val);
	}
}

TEST(ATA_Vars, field_ENHANCED_SECURITY_ERASE_TIME)
{
#undef	BITS
#define	BITS	16
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(ENHANCED_SECURITY_ERASE_TIME) = val;
		checkLog30hField(PAGE_NUMBER_06H, 24/8, 0, BITS, val);
		checkIdentifyDeviceValue(90, val);
	}
}

TEST(ATA_Vars, field_DMA_SECTOR_TIME)
{
#undef	BITS
#define	BITS	16
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(DMA_SECTOR_TIME) = val;
		checkLog30hField(PAGE_NUMBER_04H, 24/8, 0, BITS, val);
		checkIdentifyDeviceValue(96, val);
	}
}

TEST(ATA_Vars, field_DM_MINIMUM_TRANSFER_SIZE)
{
#undef	BITS
#define	BITS	16
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(DM_MINIMUM_TRANSFER_SIZE) = val;
		checkLog30hField(PAGE_NUMBER_03H, 16/8, 0, BITS, val);
		checkIdentifyDeviceValue(234, val);
	}
}

TEST(ATA_Vars, field_DM_MAXIMUM_TRANSFER_SIZE)
{
#undef	BITS
#define	BITS	16
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(DM_MAXIMUM_TRANSFER_SIZE) = val;
		checkLog30hField(PAGE_NUMBER_03H, 16/8, 16, BITS, val);
		checkIdentifyDeviceValue(235, val);
	}
}

TEST(ATA_Vars, field_DEVICE_MAINTENANCE_POLLING_TIME)
{
#undef	BITS
#define	BITS	16
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(DEVICE_MAINTENANCE_POLLING_TIME) = val;
		checkLog30hField(PAGE_NUMBER_03H, 128/8, 16, BITS, val);
	}
}

TEST(ATA_Vars, field_CURRENT_HARDWARE_FEATURE_CONTROL_IDENTIFIER)
{
#undef	BITS
#define	BITS	16
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(CURRENT_HARDWARE_FEATURE_CONTROL_IDENTIFIER) = val;
		checkLog30hField(PAGE_NUMBER_08H, 40/8, 0, BITS, val);
	}
}

TEST(ATA_Vars, field_ABO_RECOMMENDED_ABO_START_INTERVAL)
{
#undef	BITS
#define	BITS	16
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(ABO_RECOMMENDED_ABO_START_INTERVAL) = val;
		checkLog30hField(PAGE_NUMBER_03H, 128/8, 0, BITS, val);
	}
}

TEST(ATA_Vars, field_ABO_MINIMUM_SUPPORTED_TIMELIMIT)
{
#undef	BITS
#define	BITS	16
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(ABO_MINIMUM_SUPPORTED_TIMELIMIT) = val;
		checkLog30hField(PAGE_NUMBER_03H, 120/8, 0, BITS, val);
	}
}

TEST(ATA_Vars, field_WRV_MODE)
{
#undef	BITS
#define	BITS	8
	for (int bits = BITS; bits >= 0; --bits) {
		uint8 val = (uint8)((1u << bits) - 1);
		ATA_Field(WRV_MODE) = val;
		checkLog30hField(PAGE_NUMBER_04H, 16/8, 0, BITS, val);
		checkIdentifyDeviceField(220, 0, BITS, val);
	}
}

TEST(ATA_Vars, field_UTILIZATION_UNITS)
{
#undef	BITS
#define	BITS	8
	for (int bits = BITS; bits >= 0; --bits) {
		uint8 val = (uint8)((1u << bits) - 1);
		ATA_Field(UTILIZATION_UNITS) = val;
		checkLog30hField(PAGE_NUMBER_03H, 88/8, 40, BITS, val);
	}
}

TEST(ATA_Vars, field_UTILIZATION_TYPE)
{
#undef	BITS
#define	BITS	8
	for (int bits = BITS; bits >= 0; --bits) {
		uint8 val = (uint8)((1u << bits) - 1);
		ATA_Field(UTILIZATION_TYPE) = val;
		checkLog30hField(PAGE_NUMBER_03H, 88/8, 48, BITS, val);
	}
}

TEST(ATA_Vars, field_UTILIZATION_INTERVAL)
{
#undef	BITS
#define	BITS	8
	for (int bits = BITS; bits >= 0; --bits) {
		uint8 val = (uint8)((1u << bits) - 1);
		ATA_Field(UTILIZATION_INTERVAL) = val;
		checkLog30hField(PAGE_NUMBER_03H, 88/8, 32, BITS, val);
	}
}

TEST(ATA_Vars, field_LOGICAL_BLOCK_MARKUPS_SUPPORTED)
{
#undef	BITS
#define	BITS	8
	for (int bits = BITS; bits >= 0; --bits) {
		uint8 val = (uint8)((1u << bits) - 1);
		ATA_Field(LOGICAL_BLOCK_MARKUPS_SUPPORTED) = val;
		checkLog30hField(PAGE_NUMBER_03H, 72/8, 8, BITS, val);
	}
}

TEST(ATA_Vars, field_FREE_FALL_SENSITIVITY)
{
#undef	BITS
#define	BITS	8
	for (int bits = BITS; bits >= 0; --bits) {
		uint8 val = (uint8)((1u << bits) - 1);
		ATA_Field(FREE_FALL_SENSITIVITY) = val;
		checkLog30hField(PAGE_NUMBER_04H, 64/8, 0, BITS, val);
		checkIdentifyDeviceField(53, 8, BITS, val);
	}
}

TEST(ATA_Vars, field_DEVSLEEP_EXIT_TIMEOUT)
{
#undef	BITS
#define	BITS	8
	for (int bits = BITS; bits >= 0; --bits) {
		uint8 val = (uint8)((1u << bits) - 1);
		ATA_Field(DEVSLEEP_EXIT_TIMEOUT) = val;
		checkLog30hField(PAGE_NUMBER_08H, 48/8, 8, BITS, val);
	}
}

TEST(ATA_Vars, field_APM_LEVEL)
{
#undef	BITS
#define	BITS	8
	for (int bits = BITS; bits >= 0; --bits) {
		uint8 val = (uint8)((1u << bits) - 1);
		ATA_Field(APM_LEVEL) = val;
		checkLog30hField(PAGE_NUMBER_04H, 16/8, 8, BITS, val);
		checkIdentifyDeviceField(91, 0, BITS, val);
	}
}

TEST(ATA_Vars, field_ABO_STATUS)
{
#undef	BITS
#define	BITS	8
	for (int bits = BITS; bits >= 0; --bits) {
		uint8 val = (uint8)((1u << bits) - 1);
		ATA_Field(ABO_STATUS) = val;
		checkLog30hField(PAGE_NUMBER_04H, 80/8, 0, BITS, val);
	}
}

TEST(ATA_Vars, field_ZONED)
{
#undef	BITS
#define	BITS	2
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(ZONED) = val & ((1 << BITS) - 1);
		checkLog30hField(PAGE_NUMBER_03H, 104/8, 0, BITS, val);
		checkIdentifyDeviceField(69, 0, BITS, val);
	}
}

TEST(ATA_Vars, field_QUEUE_DEPTH)
{
#undef	BITS
#define	BITS	5
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(QUEUE_DEPTH) = val & ((1 << BITS) - 1);
		checkLog30hField(PAGE_NUMBER_03H, 136/8, 0, BITS, val);
		checkIdentifyDeviceField(75, 0, BITS, val);
	}
}

TEST(ATA_Vars, field_POWER_SOURCE)
{
#undef	BITS
#define	BITS	2
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(POWER_SOURCE) = val & ((1 << BITS) - 1);
		checkLog30hField(PAGE_NUMBER_04H, 16/8, 16, BITS, val);
	}
}

TEST(ATA_Vars, field_NORMAL_SECURITY_ERASE_TIME)
{
#undef	BITS
#define	BITS	15
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(NORMAL_SECURITY_ERASE_TIME) = val & ((1 << BITS) - 1);
		checkLog30hField(PAGE_NUMBER_06H, 32/8, 0, BITS, val);
		checkIdentifyDeviceField(89, 0, BITS, val);
	}
}

TEST(ATA_Vars, field_NOMINAL_FORM_FACTOR)
{
#undef	BITS
#define	BITS	4
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(NOMINAL_FORM_FACTOR) = val & ((1 << BITS) - 1);
		checkLog30hField(PAGE_NUMBER_03H, 32/8, 0, BITS, val);
		checkIdentifyDeviceField(168, 0, BITS, val);
	}
}

TEST(ATA_Vars, field_MINIMUM_INACTIVE_TIME_IN_MILLISECONDS)
{
#undef	BITS
#define	BITS	10
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(MINIMUM_INACTIVE_TIME_IN_MILLISECONDS) = val & ((1 << BITS) - 1);
		checkLog30hField(PAGE_NUMBER_04H, 72/8, 48, BITS, val);
	}
}

TEST(ATA_Vars, field_MINIMUM_DEVSLP_ASSERTION_TIME)
{
#undef	BITS
#define	BITS	5
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(MINIMUM_DEVSLP_ASSERTION_TIME) = val & ((1 << BITS) - 1);
		checkLog30hField(PAGE_NUMBER_08H, 48/8, 0, BITS, val);
	}
}

TEST(ATA_Vars, field_LOGICAL_TO_PHYSICAL_SECTOR_RELATIONSHIP)
{
#undef	BITS
#define	BITS	4
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(LOGICAL_TO_PHYSICAL_SECTOR_RELATIONSHIP) = val & ((1 << BITS) - 1);
		checkLog30hField(PAGE_NUMBER_02H, 16/8, 16, BITS, val);
		checkIdentifyDeviceField(106, 0, BITS, val);
	}
}

TEST(ATA_Vars, field_LOGICAL_SECTOR_OFFSET)
{
#undef	BITS
#define	BITS	14
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(LOGICAL_SECTOR_OFFSET) = val & ((1 << BITS) - 1);
		checkLog30hField(PAGE_NUMBER_02H, 16/8, 0, BITS, val);
		checkIdentifyDeviceField(209, 0, BITS, val);
	}
}

TEST(ATA_Vars, field_CURRENT_SERIAL_ATA_SIGNAL_SPEED)
{
#undef	BITS
#define	BITS	3
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(CURRENT_SERIAL_ATA_SIGNAL_SPEED) = val & ((1 << BITS) - 1);
		checkLog30hField(PAGE_NUMBER_08H, 16/8, 0, BITS, val);
		checkIdentifyDeviceField(77, 1, BITS, val);
	}
}

TEST(ATA_Vars, field_ALIGNMENT_ERROR_REPORTING)
{
#undef	BITS
#define	BITS	2
	for (int bits = BITS; bits >= 0; --bits) {
		uint16 val = (uint16)((1u << bits) - 1);
		ATA_Field(ALIGNMENT_ERROR_REPORTING) = val & ((1 << BITS) - 1);
		checkLog30hField(PAGE_NUMBER_02H, 16/8, 20, BITS, val);
		checkIdentifyDeviceField(49, 0, BITS, val);
	}
}

TEST(ATA_Vars, bit_ZERO_EXT_SUPPORTED)
{
	int	index = bit_ZERO_EXT_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 48, index);
}

TEST(ATA_Vars, bit_WRV_SUPPORTED)
{
	int	index = bit_WRV_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 0, index);
	checkIdentifyDeviceBit(119, 1, index);
}

TEST(ATA_Vars, bit_WRV_ENABLED)
{
	int	index = bit_WRV_ENABLED;
	checkLog30hBit(PAGE_NUMBER_04H, 8/8, 0, index);
	checkIdentifyDeviceBit(120, 1, index);
}

TEST(ATA_Vars, bit_WRITE_UNCORRECTABLE_SUPPORTED)
{
	int	index = bit_WRITE_UNCORRECTABLE_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 1, index);
	checkIdentifyDeviceBit(119, 2, index);
	checkIdentifyDeviceBit(120, 2, index);
}

TEST(ATA_Vars, bit_WRITE_FUA_EXT_SUPPORTED)
{
	int	index = bit_WRITE_FUA_EXT_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 12, index);
	checkIdentifyDeviceBit(84, 6, index);
	checkIdentifyDeviceBit(87, 6, index);
}

TEST(ATA_Vars, bit_WRITE_BUFFER_SUPPORTED)
{
	int	index = bit_WRITE_BUFFER_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 27, index);
	checkIdentifyDeviceBit(82, 12, index);
	checkIdentifyDeviceBit(85, 12, index);
}

TEST(ATA_Vars, bit_WRITE_BUFFER_DMA_SUPPORTED)
{
	int	index = bit_WRITE_BUFFER_DMA_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 35, index);
	checkIdentifyDeviceBit(69, 10, index);
}

TEST(ATA_Vars, bit_VOLATILE_WRITE_CACHE_SUPPORTED)
{
	int	index = bit_VOLATILE_WRITE_CACHE_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 24, index);
	checkIdentifyDeviceBit(82, 5, index);
}

TEST(ATA_Vars, bit_VOLATILE_WRITE_CACHE_ENABLED)
{
	int	index = bit_VOLATILE_WRITE_CACHE_ENABLED;
	checkLog30hBit(PAGE_NUMBER_04H, 8/8, 13, index);
	checkIdentifyDeviceBit(85, 5, index);
}

TEST(ATA_Vars, bit_UNLOAD_WHILE_NCQ_COMMANDS_ARE_OUTSTANDING_SUPPORTED)
{
	int	index = bit_UNLOAD_WHILE_NCQ_COMMANDS_ARE_OUTSTANDING_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 10, index);
	checkIdentifyDeviceBit(76, 11, index);
}

TEST(ATA_Vars, bit_UNLOAD_SUPPORTED)
{
	int	index = bit_UNLOAD_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 13, index);
	checkIdentifyDeviceBit(84, 13, index);
	checkIdentifyDeviceBit(87, 13, index);
}

TEST(ATA_Vars, bit_UDMA_MODE6_SUPPORTED)
{
	int	index = bit_UDMA_MODE6_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_07H, 8/8, 22, index);
	checkIdentifyDeviceBit(88, 6, index);
}

TEST(ATA_Vars, bit_UDMA_MODE6_ENABLED)
{
	int	index = bit_UDMA_MODE6_ENABLED;
	checkLog30hBit(PAGE_NUMBER_07H, 8/8, 29, index);
	checkIdentifyDeviceBit(88, 14, index);
}

TEST(ATA_Vars, bit_UDMA_MODE5_SUPPORTED)
{
	int	index = bit_UDMA_MODE5_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_07H, 8/8, 21, index);
	checkIdentifyDeviceBit(88, 5, index);
}

TEST(ATA_Vars, bit_UDMA_MODE5_ENABLED)
{
	int	index = bit_UDMA_MODE5_ENABLED;
	checkLog30hBit(PAGE_NUMBER_07H, 8/8, 28, index);
	checkIdentifyDeviceBit(88, 13, index);
}

TEST(ATA_Vars, bit_UDMA_MODE4_SUPPORTED)
{
	int	index = bit_UDMA_MODE4_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_07H, 8/8, 20, index);
	checkIdentifyDeviceBit(88, 4, index);
}

TEST(ATA_Vars, bit_UDMA_MODE4_ENABLED)
{
	int	index = bit_UDMA_MODE4_ENABLED;
	checkLog30hBit(PAGE_NUMBER_07H, 8/8, 27, index);
	checkIdentifyDeviceBit(88, 12, index);
}

TEST(ATA_Vars, bit_UDMA_MODE3_SUPPORTED)
{
	int	index = bit_UDMA_MODE3_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_07H, 8/8, 19, index);
	checkIdentifyDeviceBit(88, 3, index);
}

TEST(ATA_Vars, bit_UDMA_MODE3_ENABLED)
{
	int	index = bit_UDMA_MODE3_ENABLED;
	checkLog30hBit(PAGE_NUMBER_07H, 8/8, 26, index);
	checkIdentifyDeviceBit(88, 11, index);
}

TEST(ATA_Vars, bit_UDMA_MODE2_SUPPORTED)
{
	int	index = bit_UDMA_MODE2_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_07H, 8/8, 18, index);
	checkIdentifyDeviceBit(88, 2, index);
}

TEST(ATA_Vars, bit_UDMA_MODE2_ENABLED)
{
	int	index = bit_UDMA_MODE2_ENABLED;
	checkLog30hBit(PAGE_NUMBER_07H, 8/8, 25, index);
	checkIdentifyDeviceBit(88, 10, index);
}

TEST(ATA_Vars, bit_UDMA_MODE1_SUPPORTED)
{
	int	index = bit_UDMA_MODE1_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_07H, 8/8, 17, index);
	checkIdentifyDeviceBit(88, 1, index);
}

TEST(ATA_Vars, bit_UDMA_MODE1_ENABLED)
{
	int	index = bit_UDMA_MODE1_ENABLED;
	checkLog30hBit(PAGE_NUMBER_07H, 8/8, 24, index);
	checkIdentifyDeviceBit(88, 9, index);
}

TEST(ATA_Vars, bit_UDMA_MODE0_SUPPORTED)
{
	int	index = bit_UDMA_MODE0_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_07H, 8/8, 16, index);
	checkIdentifyDeviceBit(88, 0, index);
}

TEST(ATA_Vars, bit_UDMA_MODE0_ENABLED)
{
	int	index = bit_UDMA_MODE0_ENABLED;
	checkLog30hBit(PAGE_NUMBER_07H, 8/8, 23, index);
	checkIdentifyDeviceBit(88, 8, index);
}

TEST(ATA_Vars, bit_TRUSTED_COMPUTING_SUPPORTED)
{
	int	index = bit_TRUSTED_COMPUTING_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_06H, 40/8, 0, index);
	checkIdentifyDeviceBit(48, 0, index);
}

TEST(ATA_Vars, bit_TRIM_SUPPORTED)
{
	int	index = bit_TRIM_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 72/8, 0, index);
	checkIdentifyDeviceBit(169, 0, index);
}

TEST(ATA_Vars, bit_SUCCESSFUL_NCQ_COMMAND_SENSE_DATA_SUPPORTED)
{
	int	index = bit_SUCCESSFUL_NCQ_COMMAND_SENSE_DATA_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 47, index);
}

TEST(ATA_Vars, bit_SUCCESSFUL_NCQ_COMMAND_SENSE_DATA_ENABLED)
{
	int	index = bit_SUCCESSFUL_NCQ_COMMAND_SENSE_DATA_ENABLED;
	checkLog30hBit(PAGE_NUMBER_04H, 8/8, 18, index);
}

TEST(ATA_Vars, bit_STREAMING_SUPPORTED)
{
	int	index = bit_STREAMING_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 10, index);
	checkIdentifyDeviceBit(84, 4, index);
}

TEST(ATA_Vars, bit_SPIN_UP_SUPPORTED)
{
	int	index = bit_SPIN_UP_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 18, index);
	checkIdentifyDeviceBit(83, 6, index);
	checkIdentifyDeviceBit(86, 6, index);
}

TEST(ATA_Vars, bit_SOFTWARE_SETTINGS_PRESERVATION_SUPPORTED)
{
	int	index = bit_SOFTWARE_SETTINGS_PRESERVATION_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 23, index);
	checkIdentifyDeviceBit(78, 6, index);
}

TEST(ATA_Vars, bit_SOFTWARE_SETTINGS_PRESERVATION_ENABLED)
{
	int	index = bit_SOFTWARE_SETTINGS_PRESERVATION_ENABLED;
	checkLog30hBit(PAGE_NUMBER_08H, 16/8, 8, index);
	checkIdentifyDeviceBit(79, 6, index);
}

TEST(ATA_Vars, bit_SMART_SELF_TEST_SUPPORTED)
{
	int	index = bit_SMART_SELF_TEST_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 8, index);
	checkIdentifyDeviceBit(84, 1, index);
	checkIdentifyDeviceBit(87, 1, index);
}

TEST(ATA_Vars, bit_SMART_ERROR_LOGGING_SUPPORTED)
{
	int	index = bit_SMART_ERROR_LOGGING_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 7, index);
	checkIdentifyDeviceBit(84, 0, index);
	checkIdentifyDeviceBit(87, 0, index);
}

TEST(ATA_Vars, bit_SMART_ENABLED)
{
	int	index = bit_SMART_ENABLED;
	checkLog30hBit(PAGE_NUMBER_04H, 8/8, 6, index);
	checkIdentifyDeviceBit(85, 0, index);
}

TEST(ATA_Vars, bit_SMART)
{
	int	index = bit_SMART;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 23, index);
	checkIdentifyDeviceBit(82, 0, index);
}

TEST(ATA_Vars, bit_SINCE_POWER_ON_RATE_BASIS_SUPPORTED)
{
	int	index = bit_SINCE_POWER_ON_RATE_BASIS_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 96/8, 8, index);
}

TEST(ATA_Vars, bit_SFF8447_REPORTING)
{
	int	index = bit_SFF8447_REPORTING;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 52, index);
}


TEST(ATA_Vars, bit_SETTING_RATE_BASIS_SUPPORTED)
{
	int	index = bit_SETTING_RATE_BASIS_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 96/8, 23, index);
}

TEST(ATA_Vars, bit_SET_SECTOR_CONFIGURATION_SUPPORTED)
{
	int	index = bit_SET_SECTOR_CONFIGURATION_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 49, index);
}

TEST(ATA_Vars, bit_SET_EPC_POWER_SOURCE_SUPPORTED)
{
	int	index = bit_SET_EPC_POWER_SOURCE_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 42, index);
}

TEST(ATA_Vars, bit_SENSE_DATA_SUPPORTED)
{
	int	index = bit_SENSE_DATA_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 5, index);
	checkIdentifyDeviceBit(119, 6, index);
}

TEST(ATA_Vars, bit_SENSE_DATA_ENABLED)
{
	int	index = bit_SENSE_DATA_ENABLED;
	checkLog30hBit(PAGE_NUMBER_04H, 8/8, 10, index);
	checkIdentifyDeviceBit(120, 6, index);
}

TEST(ATA_Vars, bit_SEND_AND_RECEIVE_QUEUED_COMMANDS_SUPPORTED)
{
	int	index = bit_SEND_AND_RECEIVE_QUEUED_COMMANDS_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 17, index);
	checkIdentifyDeviceBit(77, 6, index);
}

TEST(ATA_Vars, bit_SECURITY_SUPPORTED)
{
	int	index = bit_SECURITY_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_06H, 16/8, 6, index);
	checkIdentifyDeviceBit(128, 0, index);
	checkIdentifyDeviceBit(82, 1, index);
}

TEST(ATA_Vars, bit_SECURITY_LOCKED)
{
	int	index = bit_SECURITY_LOCKED;
	checkLog30hBit(PAGE_NUMBER_06H, 16/8, 1, index);
	checkIdentifyDeviceBit(128, 2, index);
}

TEST(ATA_Vars, bit_SECURITY_FROZEN)
{
	int	index = bit_SECURITY_FROZEN;
	checkLog30hBit(PAGE_NUMBER_06H, 16/8, 2, index);
	checkIdentifyDeviceBit(128, 3, index);
}

TEST(ATA_Vars, bit_SECURITY_ENABLED)
{
	int	index = bit_SECURITY_ENABLED;
	checkLog30hBit(PAGE_NUMBER_06H, 16/8, 0, index);
	checkIdentifyDeviceBit(128, 1, index);
	checkIdentifyDeviceBit(85, 1, index);
}

TEST(ATA_Vars, bit_SECURITY_COUNT_EXPIRED)
{
	int	index = bit_SECURITY_COUNT_EXPIRED;
	checkLog30hBit(PAGE_NUMBER_06H, 16/8, 3, index);
	checkIdentifyDeviceBit(128, 4, index);
}

TEST(ATA_Vars, bit_SCT_WRITE_SAME_SUPPORTED)
{
	int	index = bit_SCT_WRITE_SAME_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 144/8, 2, index);
	checkIdentifyDeviceBit(206, 2, index);
}

TEST(ATA_Vars, bit_SCT_WRITE_SAME_FUNCTION_103_SUPPORTED)
{
	int	index = bit_SCT_WRITE_SAME_FUNCTION_103_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 144/8, 26, index);
}

TEST(ATA_Vars, bit_SCT_WRITE_SAME_FUNCTION_102_SUPPORTED)
{
	int	index = bit_SCT_WRITE_SAME_FUNCTION_102_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 144/8, 25, index);
}

TEST(ATA_Vars, bit_SCT_WRITE_SAME_FUNCTION_101_SUPPORTED)
{
	int	index = bit_SCT_WRITE_SAME_FUNCTION_101_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 144/8, 24, index);
}

TEST(ATA_Vars, bit_SCT_WRITE_SAME_FUNCTION_3_SUPPORTED)
{
	int	index = bit_SCT_WRITE_SAME_FUNCTION_3_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 144/8, 18, index);
}

TEST(ATA_Vars, bit_SCT_WRITE_SAME_FUNCTION_2_SUPPORTED)
{
	int	index = bit_SCT_WRITE_SAME_FUNCTION_2_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 144/8, 17, index);
}

TEST(ATA_Vars, bit_SCT_WRITE_SAME_FUNCTION_1_SUPPORTED)
{
	int	index = bit_SCT_WRITE_SAME_FUNCTION_1_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 144/8, 16, index);
}

TEST(ATA_Vars, bit_SCT_SUPPORTED)
{
	int	index = bit_SCT_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 144/8, 0, index);
	checkIdentifyDeviceBit(206, 0, index);
}

TEST(ATA_Vars, bit_SCT_FEATURE_CONTROL_SUPPORTED)
{
	int	index = bit_SCT_FEATURE_CONTROL_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 144/8, 4, index);
	checkIdentifyDeviceBit(206, 4, index);
}

TEST(ATA_Vars, bit_SCT_ERROR_RECOVERY_CONTROL_SUPPORTED)
{
	int	index = bit_SCT_ERROR_RECOVERY_CONTROL_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 144/8, 3, index);
	checkIdentifyDeviceBit(206, 3, index);
}

TEST(ATA_Vars, bit_SCT_DATA_TABLES_SUPPORTED)
{
	int	index = bit_SCT_DATA_TABLES_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 144/8, 5, index);
	checkIdentifyDeviceBit(206, 5, index);
}

TEST(ATA_Vars, bit_SATA_PHY_EVENT_COUNTERS_LOG_SUPPORTED)
{
	int	index = bit_SATA_PHY_EVENT_COUNTERS_LOG_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 9, index);
	checkIdentifyDeviceBit(76, 10, index);
}

TEST(ATA_Vars, bit_SATA_GEN3_SIGNALING_SPEED_SUPPORTED)
{
	int	index = bit_SATA_GEN3_SIGNALING_SPEED_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 2, index);
	checkIdentifyDeviceBit(76, 3, index);
}

TEST(ATA_Vars, bit_SATA_GEN2_SIGNALING_SPEED_SUPPORTED)
{
	int	index = bit_SATA_GEN2_SIGNALING_SPEED_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 1, index);
	checkIdentifyDeviceBit(76, 2, index);
}

TEST(ATA_Vars, bit_SATA_GEN1_SIGNALING_SPEED_SUPPORTED)
{
	int	index = bit_SATA_GEN1_SIGNALING_SPEED_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 0, index);
	checkIdentifyDeviceBit(76, 1, index);
}

TEST(ATA_Vars, bit_SANITIZE_SUPPORTED)
{
	int	index = bit_SANITIZE_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_06H, 48/8, 1, index);
	checkIdentifyDeviceBit(59, 12, index);
}

TEST(ATA_Vars, bit_SANITIZE_ANTIFREEZE_LOCK_SUPPORTED)
{
	int	index = bit_SANITIZE_ANTIFREEZE_LOCK_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_06H, 48/8, 5, index);
	checkIdentifyDeviceBit(59, 10, index);
}

TEST(ATA_Vars, bit_RZAT_SUPPORTED)
{
	int	index = bit_RZAT_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 31, index);
	checkIdentifyDeviceBit(69, 5, index);
}

TEST(ATA_Vars, bit_REVERTING_TO_DEFAULTS_ENABLED)
{
	int	index = bit_REVERTING_TO_DEFAULTS_ENABLED;
	checkLog30hBit(PAGE_NUMBER_04H, 8/8, 11, index);
}

TEST(ATA_Vars, bit_RESTRICTED_SANITIZE_OVERRIDES_SECURITY)
{
	int	index = bit_RESTRICTED_SANITIZE_OVERRIDES_SECURITY;
	checkLog30hBit(PAGE_NUMBER_06H, 48/8, 7, index);
}

TEST(ATA_Vars, bit_REQUEST_SENSE_DEVICE_FAULT_SUPPORTED)
{
	int	index = bit_REQUEST_SENSE_DEVICE_FAULT_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 45, index);
}

TEST(ATA_Vars, bit_REPORT_ZONES_EXT_SUPPORTED)
{
	int	index = bit_REPORT_ZONES_EXT_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 112/8, 0, index);
}

TEST(ATA_Vars, bit_RECEIPT_OF_HOST_INITIATED_POWER_MANAGEMENT_REQUESTS_SUPPORTED)
{
	int	index = bit_RECEIPT_OF_HOST_INITIATED_POWER_MANAGEMENT_REQUESTS_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 8, index);
	checkIdentifyDeviceBit(76, 9, index);
}

TEST(ATA_Vars, bit_REBUILD_ASSIST_SUPPORTED)
{
	int	index = bit_REBUILD_ASSIST_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 29, index);
	checkIdentifyDeviceBit(78, 11, index);
}

TEST(ATA_Vars, bit_REBUILD_ASSIST_ENABLED)
{
	int	index = bit_REBUILD_ASSIST_ENABLED;
	checkLog30hBit(PAGE_NUMBER_08H, 16/8, 12, index);
	checkIdentifyDeviceBit(79, 11, index);
}

TEST(ATA_Vars, bit_READ_LOOK_AHEAD_SUPPORTED)
{
	int	index = bit_READ_LOOK_AHEAD_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 25, index);
	checkIdentifyDeviceBit(82, 6, index);
}

TEST(ATA_Vars, bit_READ_LOOK_AHEAD_ENABLED)
{
	int	index = bit_READ_LOOK_AHEAD_ENABLED;
	checkLog30hBit(PAGE_NUMBER_04H, 8/8, 7, index);
	checkIdentifyDeviceBit(85, 6, index);
}

TEST(ATA_Vars, bit_READ_LOG_DMA_EXT_AS_EQUIVALENT_TO_READ_LOG_EXT_SUPPORTED)
{
	int	index = bit_READ_LOG_DMA_EXT_AS_EQUIVALENT_TO_READ_LOG_EXT_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 14, index);
	checkIdentifyDeviceBit(76, 15, index);
}

TEST(ATA_Vars, bit_READ_BUFFER_SUPPORTED)
{
	int	index = bit_READ_BUFFER_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 28, index);
	checkIdentifyDeviceBit(82, 13, index);
	checkIdentifyDeviceBit(85, 13, index);
}

TEST(ATA_Vars, bit_READ_BUFFER_DMA_SUPPORTED)
{
	int	index = bit_READ_BUFFER_DMA_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 36, index);
	checkIdentifyDeviceBit(69, 11, index);
}

TEST(ATA_Vars, bit_PUIS_SUPPORTED)
{
	int	index = bit_PUIS_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 17, index);
	checkIdentifyDeviceBit(83, 5, index);
}

TEST(ATA_Vars, bit_PUIS_ENABLED)
{
	int	index = bit_PUIS_ENABLED;
	checkLog30hBit(PAGE_NUMBER_04H, 8/8, 3, index);
	checkIdentifyDeviceBit(86, 5, index);
}

TEST(ATA_Vars, bit_POWER_ON_HOURS_RATE_BASIS_SUPPORTED)
{
	int	index = bit_POWER_ON_HOURS_RATE_BASIS_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 96/8, 4, index);
}

TEST(ATA_Vars, bit_POWER_DISABLE_FEATURE_SUPPORTED)
{
	int	index = bit_POWER_DISABLE_FEATURE_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 27, index);
	checkIdentifyDeviceBit(78, 12, index);
}

TEST(ATA_Vars, bit_POWER_DISABLE_FEATURE_ENABLED)
{
	int	index = bit_POWER_DISABLE_FEATURE_ENABLED;
	checkLog30hBit(PAGE_NUMBER_08H, 16/8, 11, index);
	checkIdentifyDeviceBit(79, 10, index);
}

TEST(ATA_Vars, bit_POWER_DISABLE_FEATURE_ALWAYS_ENABLED)
{
	int	index = bit_POWER_DISABLE_FEATURE_ALWAYS_ENABLED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 28, index);
	checkIdentifyDeviceBit(77, 8, index);
}

TEST(ATA_Vars, bit_PIO_MODE4_IS_SUPPORTED)
{
	int	index = bit_PIO_MODE4_IS_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_07H, 16/8, 1, index);
	checkIdentifyDeviceBit(64, 1, index);
}

TEST(ATA_Vars, bit_PIO_MODE3_IS_SUPPORTED)
{
	int	index = bit_PIO_MODE3_IS_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_07H, 16/8, 0, index);
	checkIdentifyDeviceBit(64, 0, index);
}

TEST(ATA_Vars, bit_PERSISTENT_SENSE_DATA_REPORTING)
{
	int	index = bit_PERSISTENT_SENSE_DATA_REPORTING;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 53, index);
}

TEST(ATA_Vars, bit_OVERWRITE_SUPPORTED)
{
	int	index = bit_OVERWRITE_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_06H, 48/8, 3, index);
	checkIdentifyDeviceBit(59, 14, index);
}

TEST(ATA_Vars, bit_NORMAL_SECURITY_ERASE_TIME_FORMAT)
{
	int	index = bit_NORMAL_SECURITY_ERASE_TIME_FORMAT;
	checkLog30hBit(PAGE_NUMBER_06H, 32/8, 15, index);
	checkIdentifyDeviceBit(89, 15, index);
}

TEST(ATA_Vars, bit_NOP_SUPPORTED)
{
	int	index = bit_NOP_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 29, index);
	checkIdentifyDeviceBit(82, 14, index);
	checkIdentifyDeviceBit(85, 14, index);
}

TEST(ATA_Vars, bit_NONZERO_BUFFER_OFFSETS_SUPPORTED)
{
	int	index = bit_NONZERO_BUFFER_OFFSETS_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 18, index);
	checkIdentifyDeviceBit(78, 1, index);
}

TEST(ATA_Vars, bit_NONZERO_BUFFER_OFFSETS_ENABLED)
{
	int	index = bit_NONZERO_BUFFER_OFFSETS_ENABLED;
	checkLog30hBit(PAGE_NUMBER_08H, 16/8, 3, index);
	checkIdentifyDeviceBit(79, 1, index);
}

TEST(ATA_Vars, bit_NON_VOLATILE_WRITE_CACHE)
{
	int	index = bit_NON_VOLATILE_WRITE_CACHE;
	checkLog30hBit(PAGE_NUMBER_04H, 8/8, 8, index);
	checkIdentifyDeviceBit(69, 2, index);
}

TEST(ATA_Vars, bit_NON_DATA_RESET_WRITE_POINTERS_EXT_SUPPORTED)
{
	int	index = bit_NON_DATA_RESET_WRITE_POINTERS_EXT_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 112/8, 4, index);
}

TEST(ATA_Vars, bit_NON_DATA_OPEN_ZONE_EXT_SUPPORTED)
{
	int	index = bit_NON_DATA_OPEN_ZONE_EXT_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 112/8, 1, index);
}

TEST(ATA_Vars, bit_NON_DATA_FINISH_ZONE_EXT_SUPPORTED)
{
	int	index = bit_NON_DATA_FINISH_ZONE_EXT_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 112/8, 3, index);
}

TEST(ATA_Vars, bit_NON_DATA_CLOSE_ZONE_EXT_SUPPORTED)
{
	int	index = bit_NON_DATA_CLOSE_ZONE_EXT_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 112/8, 2, index);
}

TEST(ATA_Vars, bit_NCQ_STREAMING_SUPPORTED)
{
	int	index = bit_NCQ_STREAMING_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 15, index);
	checkIdentifyDeviceBit(77, 4, index);
}

TEST(ATA_Vars, bit_NCQ_QUEUE_MANAGEMENT_COMMAND_SUPPORTED)
{
	int	index = bit_NCQ_QUEUE_MANAGEMENT_COMMAND_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 16, index);
	checkIdentifyDeviceBit(77, 5, index);
}

TEST(ATA_Vars, bit_NCQ_PRIORITY_INFORMATION_SUPPORTED)
{
	int	index = bit_NCQ_PRIORITY_INFORMATION_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 11, index);
	checkIdentifyDeviceBit(76, 12, index);
}

TEST(ATA_Vars, bit_NCQ_FEATURE_SET_SUPPORTED)
{
	int	index = bit_NCQ_FEATURE_SET_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 7, index);
	checkIdentifyDeviceBit(76, 8, index);
}

TEST(ATA_Vars, bit_NCQ_AUTOSENSE_SUPPORTED)
{
	int	index = bit_NCQ_AUTOSENSE_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 24, index);
	checkIdentifyDeviceBit(78, 7, index);
}

TEST(ATA_Vars, bit_MULTIWORD_DMA_MODE2_SUPPORTED)
{
	int	index = bit_MULTIWORD_DMA_MODE2_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_07H, 8/8, 32, index);
	checkIdentifyDeviceBit(63, 2, index);
}

TEST(ATA_Vars, bit_MULTIWORD_DMA_MODE2_ENABLED)
{
	int	index = bit_MULTIWORD_DMA_MODE2_ENABLED;
	checkLog30hBit(PAGE_NUMBER_07H, 8/8, 35, index);
	checkIdentifyDeviceBit(63, 10, index);
}

TEST(ATA_Vars, bit_MULTIWORD_DMA_MODE1_SUPPORTED)
{
	int	index = bit_MULTIWORD_DMA_MODE1_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_07H, 8/8, 31, index);
	checkIdentifyDeviceBit(63, 1, index);
}

TEST(ATA_Vars, bit_MULTIWORD_DMA_MODE1_ENABLED)
{
	int	index = bit_MULTIWORD_DMA_MODE1_ENABLED;
	checkLog30hBit(PAGE_NUMBER_07H, 8/8, 34, index);
	checkIdentifyDeviceBit(63, 9, index);
}

TEST(ATA_Vars, bit_MULTIWORD_DMA_MODE0_SUPPORTED)
{
	int	index = bit_MULTIWORD_DMA_MODE0_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_07H, 8/8, 30, index);
	checkIdentifyDeviceBit(63, 0, index);
}

TEST(ATA_Vars, bit_MULTIWORD_DMA_MODE0_ENABLED)
{
	int	index = bit_MULTIWORD_DMA_MODE0_ENABLED;
	checkLog30hBit(PAGE_NUMBER_07H, 8/8, 33, index);
	checkIdentifyDeviceBit(63, 8, index);
}

TEST(ATA_Vars, bit_MASTER_PASSWORD_CAPABILITY)
{
	int	index = bit_MASTER_PASSWORD_CAPABILITY;
	checkLog30hBit(PAGE_NUMBER_06H, 16/8, 5, index);
	checkIdentifyDeviceBit(128, 8, index);
}

TEST(ATA_Vars, bit_LPS_MISALIGNMENT_REPORTING_SUPPORTED)
{
	int	index = bit_LPS_MISALIGNMENT_REPORTING_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 38, index);
	checkIdentifyDeviceBit(69, 13, index);
}

TEST(ATA_Vars, bit_LOW_POWER_STANDBY_SUPPORTED)
{
	int	index = bit_LOW_POWER_STANDBY_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 43, index);
}

TEST(ATA_Vars, bit_LOGICAL_TO_PHYSICAL_SECTOR_RELATIONSHIP_SUPPORTED)
{
	int	index = bit_LOGICAL_TO_PHYSICAL_SECTOR_RELATIONSHIP_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_02H, 16/8, 62, index);
	checkIdentifyDeviceBit(106, 13, index);
}

TEST(ATA_Vars, bit_LOGICAL_SECTOR_SIZE_SUPPORTED)
{
	int	index = bit_LOGICAL_SECTOR_SIZE_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_02H, 16/8, 61, index);
	checkIdentifyDeviceBit(106, 12, index);
}

TEST(ATA_Vars, bit_IORDY_SUPPORTED)
{
	int	index = bit_IORDY_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_07H, 8/8, 38, index);
	checkIdentifyDeviceBit(49, 11, index);
}

TEST(ATA_Vars, bit_IORDY_DISABLE_SUPPORTED)
{
	int	index = bit_IORDY_DISABLE_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_07H, 8/8, 37, index);
	checkIdentifyDeviceBit(49, 10, index);
}

TEST(ATA_Vars, bit_IN_ORDER_DATA_DELIVERY_SUPPORTED)
{
	int	index = bit_IN_ORDER_DATA_DELIVERY_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 21, index);
	checkIdentifyDeviceBit(78, 4, index);
}

TEST(ATA_Vars, bit_IN_ORDER_DATA_DELIVERY_ENABLED)
{
	int	index = bit_IN_ORDER_DATA_DELIVERY_ENABLED;
	checkLog30hBit(PAGE_NUMBER_08H, 16/8, 6, index);
	checkIdentifyDeviceBit(79, 4, index);
}

TEST(ATA_Vars, bit_HYBRID_INFORMATION_SUPPORTED)
{
	int	index = bit_HYBRID_INFORMATION_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 27, index);
	checkIdentifyDeviceBit(78, 9, index);
}

TEST(ATA_Vars, bit_HYBRID_INFORMATION_ENABLED)
{
	int	index = bit_HYBRID_INFORMATION_ENABLED;
	checkLog30hBit(PAGE_NUMBER_08H, 16/8, 13, index);
	checkIdentifyDeviceBit(79, 9, index);
}

TEST(ATA_Vars, bit_HOST_AUTOMATIC_PARTIAL_TO_SLUMBER_TRANSITIONS_SUPPORTED)
{
	int	index = bit_HOST_AUTOMATIC_PARTIAL_TO_SLUMBER_TRANSITIONS_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 12, index);
	checkIdentifyDeviceBit(76, 13, index);
}

TEST(ATA_Vars, bit_HARDWARE_FEATURE_CONTROL_SUPPORTED)
{
	int	index = bit_HARDWARE_FEATURE_CONTROL_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 22, index);
	checkIdentifyDeviceBit(78, 5, index);
}

TEST(ATA_Vars, bit_HARDWARE_FEATURE_CONTROL_IS_ENABLED)
{
	int	index = bit_HARDWARE_FEATURE_CONTROL_IS_ENABLED;
	checkLog30hBit(PAGE_NUMBER_08H, 16/8, 7, index);
	checkIdentifyDeviceBit(79, 5, index);
}

TEST(ATA_Vars, bit_GPL_SUPPORTED)
{
	int	index = bit_GPL_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 11, index);
	checkIdentifyDeviceBit(84, 5, index);
	checkIdentifyDeviceBit(87, 5, index);
}

TEST(ATA_Vars, bit_GPL_DMA_SUPPORTED)
{
	int	index = bit_GPL_DMA_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 2, index);
	checkIdentifyDeviceBit(119, 3, index);
	checkIdentifyDeviceBit(120, 3, index);
}

TEST(ATA_Vars, bit_FREE_FALL_SUPPORTED)
{
	int	index = bit_FREE_FALL_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 4, index);
	checkIdentifyDeviceBit(119, 5, index);
}

TEST(ATA_Vars, bit_FREE_FALL_ENABLED)
{
	int	index = bit_FREE_FALL_ENABLED;
	checkLog30hBit(PAGE_NUMBER_04H, 8/8, 1, index);
	checkIdentifyDeviceBit(120, 5, index);
}

TEST(ATA_Vars, bit_FLUSH_CACHE_EXT_SUPPORTED)
{
	int	index = bit_FLUSH_CACHE_EXT_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 22, index);
	checkIdentifyDeviceBit(83, 13, index);
	checkIdentifyDeviceBit(86, 13, index);
}

TEST(ATA_Vars, bit_ExtendedNumberOfUserAddressableSectorsSupported)
{
	int	index = bit_ExtendedNumberOfUserAddressableSectorsSupported;
	checkIdentifyDeviceBit(69, 3, index);
}

TEST(ATA_Vars, bit_EPC_SUPPORTED)
{
	int	index = bit_EPC_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 6, index);
	checkIdentifyDeviceBit(119, 7, index);
}

TEST(ATA_Vars, bit_EPC_ENABLED)
{
	int	index = bit_EPC_ENABLED;
	checkLog30hBit(PAGE_NUMBER_04H, 8/8, 15, index);
	checkIdentifyDeviceBit(120, 7, index);
}

TEST(ATA_Vars, bit_ENHANCED_SECURITY_ERASE_TIME_FORMAT)
{
	int	index = bit_ENHANCED_SECURITY_ERASE_TIME_FORMAT;
	checkLog30hBit(PAGE_NUMBER_06H, 24/8, 15, index);
}

TEST(ATA_Vars, bit_ENHANCED_SECURITY_ERASE_SUPPORTED)
{
	int	index = bit_ENHANCED_SECURITY_ERASE_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_06H, 16/8, 4, index);
	checkIdentifyDeviceBit(128, 5, index);
}

TEST(ATA_Vars, bit_ENCRYPT_ALL_SUPPORTED)
{
	int	index = bit_ENCRYPT_ALL_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_06H, 48/8, 0, index);
	checkIdentifyDeviceBit(69, 4, index);
}

TEST(ATA_Vars, bit_DSN_SUPPORTED)
{
	int	index = bit_DSN_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 44, index);
	checkIdentifyDeviceBit(119, 9, index);
}

TEST(ATA_Vars, bit_DSN_ENABLED)
{
	int	index = bit_DSN_ENABLED;
	checkLog30hBit(PAGE_NUMBER_04H, 8/8, 16, index);
	checkIdentifyDeviceBit(120, 9, index);
}

TEST(ATA_Vars, bit_DRAT_SUPPORTED)
{
	int	index = bit_DRAT_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 39, index);
	checkIdentifyDeviceBit(69, 14, index);
}

TEST(ATA_Vars, bit_DOWNLOAD_MICROCODE_SUPPORTED)
{
	int	index = bit_DOWNLOAD_MICROCODE_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 14, index);
	checkIdentifyDeviceBit(83, 0, index);
	checkIdentifyDeviceBit(86, 0, index);
}

TEST(ATA_Vars, bit_DOWNLOAD_MICROCODE_DMA_SUPPORTED)
{
	int	index = bit_DOWNLOAD_MICROCODE_DMA_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 33, index);
	checkIdentifyDeviceBit(69, 8, index);
}

TEST(ATA_Vars, bit_DMA_SUPPORTED)
{
	int	index = bit_DMA_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_07H, 8/8, 36, index);
	checkIdentifyDeviceBit(49, 8, index);
}

TEST(ATA_Vars, bit_DMA_SETUP_AUTO_ACTIVATION_SUPPORTED)
{
	int	index = bit_DMA_SETUP_AUTO_ACTIVATION_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 19, index);
	checkIdentifyDeviceBit(78, 2, index);
}

TEST(ATA_Vars, bit_DMA_SETUP_AUTO_ACTIVATION_ENABLED)
{
	int	index = bit_DMA_SETUP_AUTO_ACTIVATION_ENABLED;
	checkLog30hBit(PAGE_NUMBER_08H, 16/8, 4, index);
	checkIdentifyDeviceBit(79, 2, index);
}

TEST(ATA_Vars, bit_DM_OFFSETS_IMMEDIATE_SUPPORTED)
{
	int	index = bit_DM_OFFSETS_IMMEDIATE_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 16/8, 32, index);
}

TEST(ATA_Vars, bit_DM_OFFSETS_DEFERRED_SUPPORTED)
{
	int	index = bit_DM_OFFSETS_DEFERRED_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 16/8, 34, index);
}

TEST(ATA_Vars, bit_DM_MODE3_SUPPORTED)
{
	int	index = bit_DM_MODE3_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 3, index);
	checkIdentifyDeviceBit(119, 4, index);
	checkIdentifyDeviceBit(120, 4, index);
}

TEST(ATA_Vars, bit_DM_IMMEDIATE_SUPPORTED)
{
	int	index = bit_DM_IMMEDIATE_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 16/8, 33, index);
}

TEST(ATA_Vars, bit_DM_CLEARS_NONACTIVATED_DEFERRED_DATA)
{
	int	index = bit_DM_CLEARS_NONACTIVATED_DEFERRED_DATA;
	checkLog30hBit(PAGE_NUMBER_03H, 16/8, 35, index);
}

TEST(ATA_Vars, bit_DLC_SUPPORTED)
{
	int	index = bit_DLC_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 46, index);
}

TEST(ATA_Vars, bit_DLC_ENABLED)
{
	int	index = bit_DLC_ENABLED;
	checkLog30hBit(PAGE_NUMBER_04H, 8/8, 17, index);
}

TEST(ATA_Vars, bit_DEVSLP_TIMING_VARIABLES_SUPPORTED)
{
	int	index = bit_DEVSLP_TIMING_VARIABLES_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 48/8, 63, index);
}

TEST(ATA_Vars, bit_DEVSLEEP_TO_REDUCEDPWRSTATE_CAPABILITY_SUPPORTED)
{
	int	index = bit_DEVSLEEP_TO_REDUCEDPWRSTATE_CAPABILITY_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 26, index);
	checkIdentifyDeviceBit(77, 7, index);
}

TEST(ATA_Vars, bit_DEVICE_SLEEP_SUPPORTED)
{
	int	index = bit_DEVICE_SLEEP_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 25, index);
	checkIdentifyDeviceBit(78, 8, index);
}

TEST(ATA_Vars, bit_DEVICE_SLEEP_ENABLED)
{
	int	index = bit_DEVICE_SLEEP_ENABLED;
	checkLog30hBit(PAGE_NUMBER_08H, 16/8, 10, index);
	checkIdentifyDeviceBit(79, 8, index);
}

TEST(ATA_Vars, bit_DEVICE_INITIATED_POWER_MANAGEMENT_SUPPORTED)
{
	int	index = bit_DEVICE_INITIATED_POWER_MANAGEMENT_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 20, index);
	checkIdentifyDeviceBit(78, 3, index);
}

TEST(ATA_Vars, bit_DEVICE_INITIATED_POWER_MANAGEMENT_ENABLED)
{
	int	index = bit_DEVICE_INITIATED_POWER_MANAGEMENT_ENABLED;
	checkLog30hBit(PAGE_NUMBER_08H, 16/8, 5, index);
	checkIdentifyDeviceBit(79, 3, index);
}

TEST(ATA_Vars, bit_DEVICE_AUTOMATIC_PARTIAL_TO_SLUMBER_TRANSITIONS_SUPPORTED)
{
	int	index = bit_DEVICE_AUTOMATIC_PARTIAL_TO_SLUMBER_TRANSITIONS_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_08H, 8/8, 13, index);
	checkIdentifyDeviceBit(76, 14, index);
}

TEST(ATA_Vars, bit_DEFINITIVE_ENDING_PATTERN_SUPPORTED)
{
	int	index = bit_DEFINITIVE_ENDING_PATTERN_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 51, index);
}

TEST(ATA_Vars, bit_DATE_TIME_RATE_BASIS_SUPPORTED)
{
	int	index = bit_DATE_TIME_RATE_BASIS_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 96/8, 0, index);
}

TEST(ATA_Vars, bit_DATA_SET_MANAGEMENT_XL_SUPPORTED)
{
	int	index = bit_DATA_SET_MANAGEMENT_XL_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 50, index);
}

TEST(ATA_Vars, bit_CRYPTO_SCRAMBLE_SUPPORTED)
{
	int	index = bit_CRYPTO_SCRAMBLE_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_06H, 48/8, 2, index);
	checkIdentifyDeviceBit(59, 13, index);
}

TEST(ATA_Vars, bit_BLOCK_ERASE_SUPPORTED)
{
	int	index = bit_BLOCK_ERASE_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_06H, 48/8, 4, index);
	checkIdentifyDeviceBit(59, 15, index);
}

TEST(ATA_Vars, bit_AMAX_ADDR_SUPPORTED)
{
	int	index = bit_AMAX_ADDR_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 41, index);
	checkIdentifyDeviceBit(119, 8, index);
}

TEST(ATA_Vars, bit_AUTOMATIC_PARTIAL_TO_SLUMBER_TRANSITIONS_ENABLED)
{
	int	index = bit_AUTOMATIC_PARTIAL_TO_SLUMBER_TRANSITIONS_ENABLED;
	checkLog30hBit(PAGE_NUMBER_08H, 16/8, 9, index);
	checkIdentifyDeviceBit(79, 7, index);
}

TEST(ATA_Vars, bit_APM_SUPPORTED)
{
	int	index = bit_APM_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 16, index);
	checkIdentifyDeviceBit(83, 3, index);
}

TEST(ATA_Vars, bit_APM_ENABLED)
{
	int	index = bit_APM_ENABLED;
	checkLog30hBit(PAGE_NUMBER_04H, 8/8, 2, index);
	checkIdentifyDeviceBit(86, 3, index);
}

TEST(ATA_Vars, bit_ADVANCED_BACKGROUND_OPERATION_SUPPORTED)
{
	int	index = bit_ADVANCED_BACKGROUND_OPERATION_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 54, index);
}

TEST(ATA_Vars, bit_ACS3_COMMANDS_ALLOWED_BY_SANITIZE)
{
	int	index = bit_ACS3_COMMANDS_ALLOWED_BY_SANITIZE;
	checkLog30hBit(PAGE_NUMBER_06H, 48/8, 6, index);
	checkIdentifyDeviceBit(59, 11, index);
}

TEST(ATA_Vars, bit_ABO_IR_MODE_SUPPORTED)
{
	int	index = bit_ABO_IR_MODE_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 120/8, 61, index);
}

TEST(ATA_Vars, bit_ABO_FOREGROUND_MODE_SUPPORTED)
{
	int	index = bit_ABO_FOREGROUND_MODE_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 120/8, 62, index);
}

TEST(ATA_Vars, bit_48BIT_SUPPORTED)
{
	int	index = bit_48BIT_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 20, index);
	checkIdentifyDeviceBit(83, 10, index);
	checkIdentifyDeviceBit(86, 10, index);
}

TEST(ATA_Vars, bit_28BIT_SUPPORTED)
{
	int	index = bit_28BIT_SUPPORTED;
	checkLog30hBit(PAGE_NUMBER_03H, 8/8, 32, index);
	checkIdentifyDeviceBit(69, 6, index);
}
