/******************************************************************************
** File Name: Log30h_IDENTIFY_DEVICE_data_log.c
** Author:
** Creation Time: Mon Dec  4 00:59:24 2017
*/
#include <string.h>
#include "ATA_Vars.h"

#define	QWORD_VALID	(1ull << 63)

#define	REVISION_NUMBER	0x0001

#define	PAGE_NUMBER_02H	0x02
#define	PAGE_NUMBER_03H	0x03
#define	PAGE_NUMBER_04H	0x04
#define	PAGE_NUMBER_05H	0x05
#define	PAGE_NUMBER_06H	0x06
#define	PAGE_NUMBER_07H	0x07
#define	PAGE_NUMBER_08H	0x08

static void ReadLog30h_Page02h_Capacity(void *buf)
{
	uint64	*entry = (uint64 *)buf;

	/* 0..7 - Capacity page information header */
	*entry++ = QWORD_VALID | (PAGE_NUMBER_02H << 16) | REVISION_NUMBER;

	/* 8..15 - Device Capacity */
	*entry++ = QWORD_VALID | ATA_Field(ACCESSIBLE_CAPACITY);

	/* 16..23 - Physcial/Logical Sector Size */
	*entry++ = QWORD_VALID |
		((uint64)ATA_BitGet(bit_LOGICAL_TO_PHYSICAL_SECTOR_RELATIONSHIP_SUPPORTED) << 62) |
		((uint64)ATA_BitGet(bit_LOGICAL_SECTOR_SIZE_SUPPORTED) << 61) |
		((uint64)ATA_Field(ALIGNMENT_ERROR_REPORTING) << 20) |
		((uint64)ATA_Field(LOGICAL_TO_PHYSICAL_SECTOR_RELATIONSHIP) << 16) |
		((uint64)ATA_Field(LOGICAL_SECTOR_OFFSET));

	/* 24..31 - Logical Sector Size */
	*entry++ = QWORD_VALID | ATA_Field(LOGICAL_SECTOR_SIZE);

	/* 32..39 - Nominal Buffer Size */
	*entry++ = QWORD_VALID | ATA_Field(BUFFER_SIZE);
}

static void ReadLog30h_Page03h_Supported_Capabilities(void *buf)
{
	uint64	*entry = (uint64 *)buf;

	/* 0..7 - Supported Capabilities page information header */
	*entry++ = QWORD_VALID | (PAGE_NUMBER_03H << 16) | REVISION_NUMBER;

	/* 8..15 - Supported Capabilities */
	*entry++ = QWORD_VALID |
		((uint64)ATA_BitGet(bit_ADVANCED_BACKGROUND_OPERATION_SUPPORTED) << 54) |
		((uint64)ATA_BitGet(bit_PERSISTENT_SENSE_DATA_REPORTING) << 53) |
		((uint64)ATA_BitGet(bit_SFF8447_REPORTING) << 52) |
		((uint64)ATA_BitGet(bit_DEFINITIVE_ENDING_PATTERN_SUPPORTED) << 51) |
		((uint64)ATA_BitGet(bit_DATA_SET_MANAGEMENT_XL_SUPPORTED) << 50) |
		((uint64)ATA_BitGet(bit_SET_SECTOR_CONFIGURATION_SUPPORTED) << 49) |
		((uint64)ATA_BitGet(bit_ZERO_EXT_SUPPORTED) << 48) |
		((uint64)ATA_BitGet(bit_SUCCESSFUL_NCQ_COMMAND_SENSE_DATA_SUPPORTED) << 47) |
		((uint64)ATA_BitGet(bit_DLC_SUPPORTED) << 46) |
		((uint64)ATA_BitGet(bit_REQUEST_SENSE_DEVICE_FAULT_SUPPORTED) << 45) |
		((uint64)ATA_BitGet(bit_DSN_SUPPORTED) << 44) |
		((uint64)ATA_BitGet(bit_LOW_POWER_STANDBY_SUPPORTED) << 43) |
		((uint64)ATA_BitGet(bit_SET_EPC_POWER_SOURCE_SUPPORTED) << 42) |
		((uint64)ATA_BitGet(bit_AMAX_ADDR_SUPPORTED) << 41) |
		/* 40 Reserved for CFA */
		((uint64)ATA_BitGet(bit_DRAT_SUPPORTED) << 39) |
		((uint64)ATA_BitGet(bit_LPS_MISALIGNMENT_REPORTING_SUPPORTED) << 38) |
		/* 37 Reserved */
		((uint64)ATA_BitGet(bit_READ_BUFFER_DMA_SUPPORTED) << 36) |
		((uint64)ATA_BitGet(bit_WRITE_BUFFER_DMA_SUPPORTED) << 35) |
		/* 34 Reserved */
		((uint64)ATA_BitGet(bit_DOWNLOAD_MICROCODE_DMA_SUPPORTED) << 33) |
		((uint64)ATA_BitGet(bit_28BIT_SUPPORTED) << 32) |
		((uint64)ATA_BitGet(bit_RZAT_SUPPORTED) << 31) |
		/* 30 Reserved */
		((uint64)ATA_BitGet(bit_NOP_SUPPORTED) << 29) |
		((uint64)ATA_BitGet(bit_READ_BUFFER_SUPPORTED) << 28) |
		((uint64)ATA_BitGet(bit_WRITE_BUFFER_SUPPORTED) << 27) |
		/* 26 Reserved */
		((uint64)ATA_BitGet(bit_READ_LOOK_AHEAD_SUPPORTED) << 25) |
		((uint64)ATA_BitGet(bit_VOLATILE_WRITE_CACHE_SUPPORTED) << 24) |
		((uint64)ATA_BitGet(bit_SMART) << 23) |
		((uint64)ATA_BitGet(bit_FLUSH_CACHE_EXT_SUPPORTED) << 22) |
		/* 21 Reserved */
		((uint64)ATA_BitGet(bit_48BIT_SUPPORTED) << 20) |
		/* 19 Reserved */
		((uint64)ATA_BitGet(bit_SPIN_UP_SUPPORTED) << 18) |
		((uint64)ATA_BitGet(bit_PUIS_SUPPORTED) << 17) |
		((uint64)ATA_BitGet(bit_APM_SUPPORTED) << 16) |
		/* 15 Reserved for CFA */
		((uint64)ATA_BitGet(bit_DOWNLOAD_MICROCODE_SUPPORTED) << 14) |
		((uint64)ATA_BitGet(bit_UNLOAD_SUPPORTED) << 13) |
		((uint64)ATA_BitGet(bit_WRITE_FUA_EXT_SUPPORTED) << 12) |
		((uint64)ATA_BitGet(bit_GPL_SUPPORTED) << 11) |
		((uint64)ATA_BitGet(bit_STREAMING_SUPPORTED) << 10) |
		/* 9 Reserved */
		((uint64)ATA_BitGet(bit_SMART_SELF_TEST_SUPPORTED) << 8) |
		((uint64)ATA_BitGet(bit_SMART_ERROR_LOGGING_SUPPORTED) << 7) |
		((uint64)ATA_BitGet(bit_EPC_SUPPORTED) << 6) |
		((uint64)ATA_BitGet(bit_SENSE_DATA_SUPPORTED) << 5) |
		((uint64)ATA_BitGet(bit_FREE_FALL_SUPPORTED) << 4) |
		((uint64)ATA_BitGet(bit_DM_MODE3_SUPPORTED) << 3) |
		((uint64)ATA_BitGet(bit_GPL_DMA_SUPPORTED) << 2) |
		((uint64)ATA_BitGet(bit_WRITE_UNCORRECTABLE_SUPPORTED) << 1) |
		((uint64)ATA_BitGet(bit_WRV_SUPPORTED));

	/* 16..23 - DOWNLOAD MICROCODE Capabilities */
	*entry++ = QWORD_VALID |
		((uint64)ATA_BitGet(bit_DM_CLEARS_NONACTIVATED_DEFERRED_DATA) << 35) |
		((uint64)ATA_BitGet(bit_DM_OFFSETS_DEFERRED_SUPPORTED) << 34) |
		((uint64)ATA_BitGet(bit_DM_IMMEDIATE_SUPPORTED) << 33) |
		((uint64)ATA_BitGet(bit_DM_OFFSETS_IMMEDIATE_SUPPORTED) << 32) |
		((uint64)ATA_Field(DM_MAXIMUM_TRANSFER_SIZE) << 16) |
		((uint64)ATA_Field(DM_MINIMUM_TRANSFER_SIZE));

	/* 24..31 - Nominal Media Rotation Rate */
	*entry++ = QWORD_VALID |
		((uint64)ATA_Field(NOMINAL_MEDIA_ROTATION_RATE));

	/* 32..39 - Form Factor */
	*entry++ = QWORD_VALID |
		((uint64)ATA_Field(NOMINAL_FORM_FACTOR));

	/* 40..47 - Write-Read-Verify Sector Count Mode 3 */
	*entry++ = QWORD_VALID |
		((uint64)ATA_Field(WRV_MODE3_COUNT));

	/* 48..55 - Write-Read-Verify Sector Count Mode 2 */
	*entry++ = QWORD_VALID |
		((uint64)ATA_Field(WRV_MODE2_COUNT));

	/* 56..71 - World wide name */
	memcpy((void *)entry, ATA_Field(WORLD_WIDE_NAME), 8);

	++entry;
	*entry++ = QWORD_VALID;

	/* 72..79 - DATA SET MANAGEMENT */
	*entry++ = QWORD_VALID |
		((uint64)ATA_Field(MAX_PAGES_PER_DSM_COMMAND) << 16) |
		((uint64)ATA_Field(LOGICAL_BLOCK_MARKUPS_SUPPORTED) << 8) |
		((uint64)ATA_BitGet(bit_TRIM_SUPPORTED));

	/* 80..95 - Utilization Per Unit Time */
	*entry++ = ((uint64)ATA_Field(UTILIZATION_B) << 32) |
		((uint64)ATA_Field(UTILIZATION_A));
	*entry++ = QWORD_VALID |
		((uint64)ATA_Field(UTILIZATION_TYPE) << 48) |
		((uint64)ATA_Field(UTILIZATION_UNITS) << 40) |
		((uint64)ATA_Field(UTILIZATION_INTERVAL) << 32);

	/* 96..103 - Utilization Usage Rate Support */
	*entry++ = QWORD_VALID |
		((uint64)ATA_BitGet(bit_SETTING_RATE_BASIS_SUPPORTED) << 23) |
		((uint64)ATA_BitGet(bit_SINCE_POWER_ON_RATE_BASIS_SUPPORTED) << 8) |
		((uint64)ATA_BitGet(bit_POWER_ON_HOURS_RATE_BASIS_SUPPORTED) << 4) |
		((uint64)ATA_BitGet(bit_DATE_TIME_RATE_BASIS_SUPPORTED));

	/* 104..111 - Zoned Capabilities */
	*entry++ = QWORD_VALID |
		((uint64)ATA_Field(ZONED));

	/* 112..119 - Supportd ZAC Capabilities */
	*entry++ = QWORD_VALID |
		((uint64)ATA_BitGet(bit_NON_DATA_RESET_WRITE_POINTERS_EXT_SUPPORTED) << 4) |
		((uint64)ATA_BitGet(bit_NON_DATA_FINISH_ZONE_EXT_SUPPORTED) << 3) |
		((uint64)ATA_BitGet(bit_NON_DATA_CLOSE_ZONE_EXT_SUPPORTED) << 2) |
		((uint64)ATA_BitGet(bit_NON_DATA_OPEN_ZONE_EXT_SUPPORTED) << 1) |
		((uint64)ATA_BitGet(bit_REPORT_ZONES_EXT_SUPPORTED));

	/* 120..127 - Advanced Background Operations Capabilities */
	*entry++ = QWORD_VALID |
		((uint64)ATA_BitGet(bit_ABO_FOREGROUND_MODE_SUPPORTED) << 62) |
		((uint64)ATA_BitGet(bit_ABO_IR_MODE_SUPPORTED) << 61) |
		((uint64)ATA_Field(ABO_MINIMUM_FRACTION) << 16) |
		((uint64)ATA_Field(ABO_MINIMUM_SUPPORTED_TIMELIMIT));

	/* 128..135 - Advanced Background Operations Recommendations */
	*entry++ = QWORD_VALID |
		((uint64)ATA_Field(DEVICE_MAINTENANCE_POLLING_TIME) << 16) |
		((uint64)ATA_Field(ABO_RECOMMENDED_ABO_START_INTERVAL));

	/* 136..143 - Queue Depth */
	*entry++ = QWORD_VALID |
		((uint64)ATA_Field(QUEUE_DEPTH));

	/* 144..151 - Supported SCT Capabilities */
	*entry++ = QWORD_VALID |
		((uint64)ATA_BitGet(bit_SCT_WRITE_SAME_FUNCTION_103_SUPPORTED) << 26) |
		((uint64)ATA_BitGet(bit_SCT_WRITE_SAME_FUNCTION_102_SUPPORTED) << 25) |
		((uint64)ATA_BitGet(bit_SCT_WRITE_SAME_FUNCTION_101_SUPPORTED) << 24) |
		((uint64)ATA_BitGet(bit_SCT_WRITE_SAME_FUNCTION_3_SUPPORTED) << 18) |
		((uint64)ATA_BitGet(bit_SCT_WRITE_SAME_FUNCTION_2_SUPPORTED) << 17) |
		((uint64)ATA_BitGet(bit_SCT_WRITE_SAME_FUNCTION_1_SUPPORTED) << 16) |
		((uint64)ATA_BitGet(bit_SCT_DATA_TABLES_SUPPORTED) << 5) |
		((uint64)ATA_BitGet(bit_SCT_FEATURE_CONTROL_SUPPORTED) << 4) |
		((uint64)ATA_BitGet(bit_SCT_ERROR_RECOVERY_CONTROL_SUPPORTED) << 3) |
		((uint64)ATA_BitGet(bit_SCT_WRITE_SAME_SUPPORTED) << 2) |
		((uint64)ATA_BitGet(bit_SCT_SUPPORTED));

	/* 152..503 - Reserved */

	/* 504..511 - Vendor Specific Supported Capabilities */
}

static void ReadLog30h_Page04h_Current_Settings(void *buf)
{
	uint64	*entry = (uint64 *)buf;

	/* 0..7 - Current Settings page information header */
	*entry++ = QWORD_VALID | (PAGE_NUMBER_04H << 16) | REVISION_NUMBER;

	/* 8..15 - Current Settings */
	*entry++ = QWORD_VALID |
		((uint64)ATA_BitGet(bit_FW_ACTIVATION_PENDING) << 19) |
		((uint64)ATA_BitGet(bit_SUCCESSFUL_NCQ_COMMAND_SENSE_DATA_ENABLED) << 18) |
		((uint64)ATA_BitGet(bit_DLC_ENABLED) << 17) |
		((uint64)ATA_BitGet(bit_DSN_ENABLED) << 16) |
		((uint64)ATA_BitGet(bit_EPC_ENABLED) << 15) |
		/* 14 Reserved */
		((uint64)ATA_BitGet(bit_VOLATILE_WRITE_CACHE_ENABLED) << 13) |
		/* 12 Reserved */
		((uint64)ATA_BitGet(bit_REVERTING_TO_DEFAULTS_ENABLED) << 11) |
		((uint64)ATA_BitGet(bit_SENSE_DATA_ENABLED) << 10) |
		/* 9 Reserved */
		((uint64)ATA_BitGet(bit_NON_VOLATILE_WRITE_CACHE) << 8) |
		((uint64)ATA_BitGet(bit_READ_LOOK_AHEAD_ENABLED) << 7) |
		((uint64)ATA_BitGet(bit_SMART_ENABLED) << 6) |
		/* 5 Reserved */
		/* 4 Reserved */
		((uint64)ATA_BitGet(bit_PUIS_ENABLED) << 3) |
		((uint64)ATA_BitGet(bit_APM_ENABLED) << 2) |
		((uint64)ATA_BitGet(bit_FREE_FALL_ENABLED) << 1) |
		((uint64)ATA_BitGet(bit_WRV_ENABLED));

	/* 16..23 - Feature Settings */
	*entry++ = QWORD_VALID |
		((uint64)ATA_Field(POWER_SOURCE) << 16) |
		((uint64)ATA_Field(APM_LEVEL) << 8) |
		((uint64)ATA_Field(WRV_MODE));

	/* 24..31 - DMA Host Interface Sector Times */
	*entry++ = QWORD_VALID | ATA_Field(DMA_SECTOR_TIME);

	/* 32..39 - PIO Host Interface Sector Times */
	*entry++ = QWORD_VALID | ATA_Field(PIO_SECTOR_TIME);

	/* 40..47 - Streaming minimum request size */
	*entry++ = QWORD_VALID | ATA_Field(STREAM_MIN_REQUEST_SIZE);

	/* 48..55 - Streaming access latency */
	*entry++ = QWORD_VALID | ATA_Field(STREAM_ACCESS_LATENCY);

	/* 56..63 - Streaming Performance Granularity */
	*entry++ = QWORD_VALID | ATA_Field(STREAM_GRANULARITY);

	/* 64..71 - Free-fall Control Sensitivity */
	*entry++ = QWORD_VALID | ATA_Field(FREE_FALL_SENSITIVITY);

	/* 72..79 - Device Maintenance Schedule */
	*entry++ = QWORD_VALID |
		((uint64)ATA_Field(MINIMUM_INACTIVE_TIME_IN_MILLISECONDS) << 48) |
		((uint64)ATA_Field(TIME_SCHEDULED_FOR_DEVICE_MAINTENANCE) << 32) |
		((uint64)ATA_Field(TIME_TO_PERFORMANCE_DEGRATION) << 16) |
		((uint64)ATA_Field(MINIMUM_INACTIVE_TIME));

	/* 80..87 - Advanced Background Operations Settings */
	*entry++ = QWORD_VALID | ATA_Field(ABO_STATUS);
}

static void ReadLog30h_Page05h_Strings(void *buf)
{
	uint64	*entry = (uint64 *)buf;
	uint8	*p = (uint8 *)buf;

	/* 0..7 - Strings page information header */
	*entry++ = QWORD_VALID | (PAGE_NUMBER_05H << 16) | REVISION_NUMBER;

	/* 8..27 - SERIAL NUMBER field */
	/* 28..31 Reserved */
	memcpy(p + 8, ATA_Field(SERIAL_NUMBER), 20);

	/* 32..39 - FIRMWARE REVISION field */
	/* 40..47 Reserved */
	memcpy(p + 32, ATA_Field(FIRMWARE_REVISION), 8);

	/* 48..87 - MODEL NUMBER field */
	/* 88..95 Reserved */
	memcpy(p + 48, ATA_Field(MODEL_NUMBER), 40);

	/* 96..103 - ADDITIONAL PRODUCT IDENTIFIER field */
	/* 104..511 Reserved */
	memcpy(p + 96, ATA_Field(ADDITIONAL_PRODUCT_IDENTIFIER), 8);
}

static void ReadLog30h_Page06h_Security(void *buf)
{
	uint64	*entry = (uint64 *)buf;

	/* 0..7 - Security page information header */
	*entry++ = QWORD_VALID | (PAGE_NUMBER_06H << 16) | REVISION_NUMBER;

	/* 8..15 - Master Password Identifier */
	*entry++ = QWORD_VALID | ATA_Field(MASTER_PASSWORD_IDENTIFIER);

	/* 16..23 - Security Status */
	*entry++ = QWORD_VALID |
		((uint64)ATA_BitGet(bit_SECURITY_SUPPORTED) << 6) |
		((uint64)ATA_BitGet(bit_MASTER_PASSWORD_CAPABILITY) << 5) |
		((uint64)ATA_BitGet(bit_ENHANCED_SECURITY_ERASE_SUPPORTED) << 4) |
		((uint64)ATA_BitGet(bit_SECURITY_COUNT_EXPIRED) << 3) |
		((uint64)ATA_BitGet(bit_SECURITY_FROZEN) << 2) |
		((uint64)ATA_BitGet(bit_SECURITY_LOCKED) << 1) |
		(uint64)ATA_BitGet(bit_SECURITY_ENABLED);

	/* 24..31 - Time required for an Enhanced Erase mode SECURITY ERASE UNIT command */
	*entry++ = QWORD_VALID |
		((uint64)ATA_BitGet(bit_ENHANCED_SECURITY_ERASE_TIME_FORMAT) << 15) |
		(uint64)ATA_Field(ENHANCED_SECURITY_ERASE_TIME);

	/* 32..39 - Time required for a Normal Erase mode SECURITY ERASE UNIT command */
	*entry++ = QWORD_VALID |
		((uint64)ATA_BitGet(bit_NORMAL_SECURITY_ERASE_TIME_FORMAT) << 15) |
		(uint64)ATA_Field(NORMAL_SECURITY_ERASE_TIME);

	/* 40..47 - Trusted Computing feature set */
	*entry++ = QWORD_VALID | ATA_BitGet(bit_TRUSTED_COMPUTING_SUPPORTED);

	/* 48..55 - Security Capabilities */
	*entry++ = QWORD_VALID |
		((uint64)ATA_BitGet(bit_RESTRICTED_SANITIZE_OVERRIDES_SECURITY) << 7) |
		((uint64)ATA_BitGet(bit_ACS3_COMMANDS_ALLOWED_BY_SANITIZE) << 6) |
		((uint64)ATA_BitGet(bit_SANITIZE_ANTIFREEZE_LOCK_SUPPORTED) << 5) |
		((uint64)ATA_BitGet(bit_BLOCK_ERASE_SUPPORTED) << 4) |
		((uint64)ATA_BitGet(bit_OVERWRITE_SUPPORTED) << 3) |
		((uint64)ATA_BitGet(bit_CRYPTO_SCRAMBLE_SUPPORTED) << 2) |
		((uint64)ATA_BitGet(bit_SANITIZE_SUPPORTED) << 1) |
		(uint64)ATA_BitGet(bit_ENCRYPT_ALL_SUPPORTED);

	/* 56..511 - Reserved */
}

static void ReadLog30h_Page07h_Parallel_ATA(void *buf)
{
	uint64	*entry = (uint64 *)buf;

	/* 0..7 - Parallel ATA page information header */
	*entry++ = QWORD_VALID | (PAGE_NUMBER_07H << 16) | REVISION_NUMBER;

	/* 8..15 - Parallel ATA Capabilities */
	*entry++ = QWORD_VALID |
		((uint64)ATA_BitGet(bit_IORDY_SUPPORTED) << 38) |
		((uint64)ATA_BitGet(bit_IORDY_DISABLE_SUPPORTED) << 37) |
		((uint64)ATA_BitGet(bit_DMA_SUPPORTED) << 36) |
		((uint64)ATA_BitGet(bit_MULTIWORD_DMA_MODE2_ENABLED) << 35) |
		((uint64)ATA_BitGet(bit_MULTIWORD_DMA_MODE1_ENABLED) << 34) |
		((uint64)ATA_BitGet(bit_MULTIWORD_DMA_MODE0_ENABLED) << 33) |
		((uint64)ATA_BitGet(bit_MULTIWORD_DMA_MODE2_SUPPORTED) << 32) |
		((uint64)ATA_BitGet(bit_MULTIWORD_DMA_MODE1_SUPPORTED) << 31) |
		((uint64)ATA_BitGet(bit_MULTIWORD_DMA_MODE0_SUPPORTED) << 30) |
		((uint64)ATA_BitGet(bit_UDMA_MODE6_ENABLED) << 29) |
		((uint64)ATA_BitGet(bit_UDMA_MODE5_ENABLED) << 28) |
		((uint64)ATA_BitGet(bit_UDMA_MODE4_ENABLED) << 27) |
		((uint64)ATA_BitGet(bit_UDMA_MODE3_ENABLED) << 26) |
		((uint64)ATA_BitGet(bit_UDMA_MODE2_ENABLED) << 25) |
		((uint64)ATA_BitGet(bit_UDMA_MODE1_ENABLED) << 24) |
		((uint64)ATA_BitGet(bit_UDMA_MODE0_ENABLED) << 23) |
		((uint64)ATA_BitGet(bit_UDMA_MODE6_SUPPORTED) << 22) |
		((uint64)ATA_BitGet(bit_UDMA_MODE5_SUPPORTED) << 21) |
		((uint64)ATA_BitGet(bit_UDMA_MODE4_SUPPORTED) << 20) |
		((uint64)ATA_BitGet(bit_UDMA_MODE3_SUPPORTED) << 19) |
		((uint64)ATA_BitGet(bit_UDMA_MODE2_SUPPORTED) << 18) |
		((uint64)ATA_BitGet(bit_UDMA_MODE1_SUPPORTED) << 17) |
		((uint64)ATA_BitGet(bit_UDMA_MODE0_SUPPORTED) << 16);
		/* 15:0 Reserved */

	/* 16..23 - PIO Modes Supported */
	*entry++ = QWORD_VALID |
		((uint64)ATA_BitGet(bit_PIO_MODE4_IS_SUPPORTED) << 1) |
		((uint64)ATA_BitGet(bit_PIO_MODE3_IS_SUPPORTED));

	/* 24..31 - Multiword DMA transfer cycle time */
	*entry++ = QWORD_VALID |
		((uint64)ATA_Field(RECOMMENDED_MULTIWORD_CYCLE_TIME) << 16) |
		((uint64)ATA_Field(MIN_MULTIWORD_CYCLE_TIME));

	/* 32..39 - Minimum PIO transfer cycle time */
	*entry++ = QWORD_VALID |
		((uint64)ATA_Field(MIN_PIO_TRANSFER_TIME_WITH_IORDY) << 16) |
		((uint64)ATA_Field(MIN_PIO_TRANSFER_TIME_WITHOUT_IORDY));

	/* 40..47 - Set Transfer Mode */
	*entry = QWORD_VALID | ATA_Field(TRANSFER_MODE);

#if 0
	/* 48..55 - Parallel ATA Hardware Reset Result */
	*entry = QWORD_VALID |
		((uint64)ATA_BitGet(bit_CBLID) << 55) |
		((uint64)ATA_BitGet(bit_D1_PDIAG) << 15) |
		((uint64)ATA_Field(D1_DEVICE_NUMBER_DETECT) << 8) |
		((uint64)ATA_BitGet(bit_D0_PDIAG) << 7) |
		((uint64)ATA_BitGet(bit_D0_DASP) << 6) |
		((uint64)ATA_BitGet(bit_D0_D1_SELECTION) << 5) |
		((uint64)ATA_BitGet(bit_D0_DIAGNOSTICS) << 4) |
		((uint64)ATA_Field(D0_DEVICE_NUMBER_DETECT));
#endif
}

static void ReadLog30h_Page08h_Serial_ATA(void *buf)
{
	uint64	*entry = (uint64 *)buf;

	/* 0..7 - Serial ATA page information header */
	*entry++ = QWORD_VALID | (PAGE_NUMBER_08H << 16) | REVISION_NUMBER;

	/* 8..15 - SATA Capabilities */
	*entry++ = QWORD_VALID |
		((uint64)ATA_BitGet(bit_REBUILD_ASSIST_SUPPORTED) << 29) |
		((uint64)ATA_BitGet(bit_POWER_DISABLE_FEATURE_ALWAYS_ENABLED) << 28) |
		((uint64)ATA_BitGet(bit_POWER_DISABLE_FEATURE_SUPPORTED) << 27) |
		((uint64)ATA_BitGet(bit_HYBRID_INFORMATION_SUPPORTED) << 27) | 
		((uint64)ATA_BitGet(bit_DEVSLEEP_TO_REDUCEDPWRSTATE_CAPABILITY_SUPPORTED) << 26) | 
		((uint64)ATA_BitGet(bit_DEVICE_SLEEP_SUPPORTED) << 25) | 
		((uint64)ATA_BitGet(bit_NCQ_AUTOSENSE_SUPPORTED) << 24) | 
		((uint64)ATA_BitGet(bit_SOFTWARE_SETTINGS_PRESERVATION_SUPPORTED) << 23) | 
		((uint64)ATA_BitGet(bit_HARDWARE_FEATURE_CONTROL_SUPPORTED) << 22) | 
		((uint64)ATA_BitGet(bit_IN_ORDER_DATA_DELIVERY_SUPPORTED) << 21) | 
		((uint64)ATA_BitGet(bit_DEVICE_INITIATED_POWER_MANAGEMENT_SUPPORTED) << 20) | 
		((uint64)ATA_BitGet(bit_DMA_SETUP_AUTO_ACTIVATION_SUPPORTED) << 19) | 
		((uint64)ATA_BitGet(bit_NONZERO_BUFFER_OFFSETS_SUPPORTED) << 18) | 
		((uint64)ATA_BitGet(bit_SEND_AND_RECEIVE_QUEUED_COMMANDS_SUPPORTED) << 17) | 
		((uint64)ATA_BitGet(bit_NCQ_QUEUE_MANAGEMENT_COMMAND_SUPPORTED) << 16) | 
		((uint64)ATA_BitGet(bit_NCQ_STREAMING_SUPPORTED) << 15) | 
		((uint64)ATA_BitGet(bit_READ_LOG_DMA_EXT_AS_EQUIVALENT_TO_READ_LOG_EXT_SUPPORTED) << 14) | 
		((uint64)ATA_BitGet(bit_DEVICE_AUTOMATIC_PARTIAL_TO_SLUMBER_TRANSITIONS_SUPPORTED) << 13) | 
		((uint64)ATA_BitGet(bit_HOST_AUTOMATIC_PARTIAL_TO_SLUMBER_TRANSITIONS_SUPPORTED) << 12) | 
		((uint64)ATA_BitGet(bit_NCQ_PRIORITY_INFORMATION_SUPPORTED) << 11) | 
		((uint64)ATA_BitGet(bit_UNLOAD_WHILE_NCQ_COMMANDS_ARE_OUTSTANDING_SUPPORTED) << 10) | 
		((uint64)ATA_BitGet(bit_SATA_PHY_EVENT_COUNTERS_LOG_SUPPORTED) << 9) | 
		((uint64)ATA_BitGet(bit_RECEIPT_OF_HOST_INITIATED_POWER_MANAGEMENT_REQUESTS_SUPPORTED) << 8) | 
		((uint64)ATA_BitGet(bit_NCQ_FEATURE_SET_SUPPORTED) << 7) | 
		((uint64)ATA_BitGet(bit_SATA_GEN3_SIGNALING_SPEED_SUPPORTED) << 2) | 
		((uint64)ATA_BitGet(bit_SATA_GEN2_SIGNALING_SPEED_SUPPORTED) << 1) | 
		((uint64)ATA_BitGet(bit_SATA_GEN1_SIGNALING_SPEED_SUPPORTED)); 

	/* 16..23 - Current SATA Settings */
	*entry++ = QWORD_VALID |
		((uint64)ATA_BitGet(bit_HYBRID_INFORMATION_ENABLED) << 13) |
		((uint64)ATA_BitGet(bit_REBUILD_ASSIST_ENABLED) << 12) |
		((uint64)ATA_BitGet(bit_POWER_DISABLE_FEATURE_ENABLED) << 11) |
		((uint64)ATA_BitGet(bit_DEVICE_SLEEP_ENABLED) << 10) | 
		((uint64)ATA_BitGet(bit_AUTOMATIC_PARTIAL_TO_SLUMBER_TRANSITIONS_ENABLED) << 9) |
		((uint64)ATA_BitGet(bit_SOFTWARE_SETTINGS_PRESERVATION_ENABLED) << 8) | 
		((uint64)ATA_BitGet(bit_HARDWARE_FEATURE_CONTROL_IS_ENABLED) << 7) | 
		((uint64)ATA_BitGet(bit_IN_ORDER_DATA_DELIVERY_ENABLED) << 6) | 
		((uint64)ATA_BitGet(bit_DEVICE_INITIATED_POWER_MANAGEMENT_ENABLED) << 5) | 
		((uint64)ATA_BitGet(bit_DMA_SETUP_AUTO_ACTIVATION_ENABLED) << 4) | 
		((uint64)ATA_BitGet(bit_NONZERO_BUFFER_OFFSETS_ENABLED) << 3) | 
		((uint64)ATA_Field(CURRENT_SERIAL_ATA_SIGNAL_SPEED));

	/* 24..39 - Reserved for Serial ATA */
	entry++;
	entry++;

	/* 40..41 - CURRENT HARDWARE FEATURE CONTROL IDENTIFIER field */
	/* 42..43 - SUPPORTED HARDWARE FEATURE CONTROL IDENTIFIER field */
	/* 44..47 - Reserved */
	*entry++ = 0 |
		((uint64)ATA_Field(SUPPORTED_HARDWARE_FEATURE_CONTROL_IDENTIFIER) << 16) |
		((uint64)ATA_Field(CURRENT_HARDWARE_FEATURE_CONTROL_IDENTIFIER));

	/* 48..55 - Device Sleep Timing variables */
	*entry++ = 0 |
		((uint64)ATA_BitGet(bit_DEVSLP_TIMING_VARIABLES_SUPPORTED) << 63) |
		((uint64)ATA_Field(DEVSLEEP_EXIT_TIMEOUT) << 8) |
		((uint64)ATA_Field(MINIMUM_DEVSLP_ASSERTION_TIME));

	/* 56..511 - Reserved for SATA */
}

