/******************************************************************************
** File Name: IdentifyDevice.c
** Author:
** Creation Time: Tue Nov 28 02:54:18 2017
*/
#include "ATA_Vars.h"
#include "ATA_Command.h"
#include "IdentifyDevice.h"

static inline void ReportCapacity28Bit(uint32 capacity)
{
	int	iWord = IDFY_W060_W061_TOTAL_SECTORS_28BIT;
	IDFY_SetDword(iWord, capacity);
}

static inline void ReportCapacity48Bit(uint64 capacity)
{
	int	iWord = IDFY_W100_W103_NUMBER_OF_USER_ADDRESSABLE_LOGICAL_SECTORS;
	IDFY_SetQword(iWord, capacity);
}

static inline void ReportCapacityExtended48Bit(uint64 capacity)
{
	int	iWord = IDFY_W230_W233_EXTENDED_NUMBER_OF_USER_ADDRESSABLE_SECTORS;
	IDFY_SetQword(iWord, capacity);
}


static void ReportCapacity(void)
{
	if (!ATA_BitGet(bit_48BIT_SUPPORTED)) {
		ReportCapacity28Bit(ATA_Field(ACCESSIBLE_CAPACITY));
	} else if (!ATA_BitGet(bit_ExtendedNumberOfUserAddressableSectorsSupported)) {
		ReportCapacity48Bit(ATA_Field(ACCESSIBLE_CAPACITY));
		if (ATA_Field(ACCESSIBLE_CAPACITY) <= 0x0FFFFFFF)
			ReportCapacity28Bit(ATA_Field(ACCESSIBLE_CAPACITY));
		else
			ReportCapacity28Bit(0x0FFFFFFF);
	} else {
		ReportCapacityExtended48Bit(ATA_Field(ACCESSIBLE_CAPACITY));
		if (ATA_Field(ACCESSIBLE_CAPACITY) <= 0x0FFFFFFF) {
			ReportCapacity28Bit(ATA_Field(ACCESSIBLE_CAPACITY));
			ReportCapacity48Bit(ATA_Field(ACCESSIBLE_CAPACITY));
		} else if (ATA_Field(ACCESSIBLE_CAPACITY) <= 0xFFFFFFFF) {
			ReportCapacity28Bit(0x0FFFFFFF);
			ReportCapacity48Bit(ATA_Field(ACCESSIBLE_CAPACITY));
		} else {
			ReportCapacity28Bit(0x0FFFFFFF);
			ReportCapacity48Bit(0xFFFFFFFF);
		}
	}
}

void ATA_Command_IdentifyDevice(void)
{
	int	iWord;

	/*=====================================================================
	** Word 0 - General Configuration
	**     15 - 0 = ATA device
	**   14:8 - Retired
	**    7:6 - Obsolete
	**    5:3 - Retired
	**      2 - Incomplete response
	**      1 - Retired
	**      0 - Reserved
	*/
	iWord = IDFY_W000_GENERAL_CONFIGURATION;

	/*=====================================================================
	** Word 1 - Obsolete
	*/

	/*=====================================================================
	** Word 2 - Specific configuration
	*/
	iWord = IDFY_W002_SPECIFIC_CONFIGURATION;

	/*=====================================================================
	** Word 3 - Obsolete
	*/

	/*=====================================================================
	** Word 4..5 - Retired
	*/

	/*=====================================================================
	** Word 6 - Obsolete
	*/

	/*=====================================================================
	** Word 7..8 - Reserved for CFA
	*/

	/*=====================================================================
	** Word 9 - Retired
	*/

	/*=====================================================================
	** Word 10..19 - Serial number
	*/
	iWord = IDFY_W010_W019_SERIAL_NUMBER;
	IDFY_Copy(iWord, ATA_Field(SERIAL_NUMBER));

	/*=====================================================================
	** Word 20..21 - Retired
	*/

	/*=====================================================================
	** Word 22 - Obsolete
	*/

	/*=====================================================================
	** Word 23..26 - Firmware revision
	*/
	iWord = IDFY_W023_W026_FIRMWARE_REVISION;
	IDFY_Copy(iWord, ATA_Field(FIRMWARE_REVISION));

	/*=====================================================================
	** Word 27..46 - Model number
	*/
	iWord = IDFY_W027_W046_MODEL_NUMBER;
	IDFY_Copy(iWord, ATA_Field(MODEL_NUMBER));

	/*=====================================================================
	** Word 47 - Obsolete
	*/

	/*=====================================================================
	** Word 48 - Trusted Computing feature set options
	**      15 - Shall be cleared to zero
	**      14 - Shall be set to one
	**    13:1 - Reserved for the Trusted Computing Group
	**       0 - Trusted Computing feature set is supported
	*/
	iWord = IDFY_W048_TRUSTED_COMPUTING_FEATURE_SET_OPTIONS;
	IDFY_SetBit(iWord, 14, 1);
	IDFY_SetBit(iWord,  0, ATA_BitGet(bit_TRUSTED_COMPUTING_SUPPORTED));

	/*=====================================================================
	** Word 49 - Capabilities
	**   15:14 - Reserved for the IDENTIFY PACKET DEVICE command
	**      13 - 1 = Standby timer values as specified in this standard
	**               are supported
	**           0 = Standby timer values shall be vendor specific
	**      12 - Reserved for the IDENTIFY PACKET DEVICE command
	**      11 - 1 = IORDY supported
	**           0 = IORDY may be supported
	**      10 - IORDY may be disabled
	**       9 - Shall be set to one (i.e., LBA is supported)
	**       8 - DMA supported
	**     7:2 - Reserved
	**     1:0 - Long Physical Sector Alignment Error reporting
	*/
	iWord = IDFY_W049_CAPABILITIES;
	IDFY_SetBit(iWord, 11, ATA_BitGet(bit_IORDY_SUPPORTED));
	IDFY_SetBit(iWord, 10, ATA_BitGet(bit_IORDY_DISABLE_SUPPORTED));
	IDFY_SetBit(iWord,  9, 1);
	IDFY_SetBit(iWord,  8, ATA_BitGet(bit_DMA_SUPPORTED));
	IDFY_SetField(iWord, 0, ATA_Field(ALIGNMENT_ERROR_REPORTING));

	/*=====================================================================
	** Word 50 - Capabilities
	**      15 - Shall be cleared to zero
	**      14 - Shall be set to one
	**    13:2 - Reserved
	**       1 - Obsolete
	**       0 - 1 = There is a minimum Standby timer value and it is
	**               vendor specific
	**           0 = There is no minimum Standby timer value
	*/
	iWord = IDFY_W050_CAPABILITIES;
	IDFY_SetBit(iWord, 14, 1);

	/*=====================================================================
	** Word 51..52 - Obsolete
	*/

	/*=====================================================================
	** Word 53
	**    15:8 - Free-fall Control Sensitivity
	**     7:3 - Reserved
	**       2 - the fields reported in word 88 are valid
	**       1 - the fields reported in words 64..70 are valid
	**       0 - Obsolete
	*/
	iWord = IDFY_W053;
	IDFY_SetField(iWord, 8, ATA_Field(FREE_FALL_SENSITIVITY));

	/*=====================================================================
	** Word 54..58 - Obsolete
	*/

	/*=====================================================================
	** Word 59
	**      15 - The BLOCK ERASE EXT command is supported
	**      14 - The OVERWRITE EXT command is supported
	**      13 - The CRYPTO SCRAMBLE EXT command is supported
	**      12 - The Sanitize feature set is supported
	**      11 - 1 = The commands allowed during a sanitize operation are
	**               as specified by this standard
	**           0 = The commands allowed during s snaitize operation are
	**               as specified by ACS-2
	**      10 - The SANITIZE ANTIFREEZE LOCK EXT command is supported
	**       9 - Reserved
	**     8:0 - Obsolete
	*/
	iWord = IDFY_W059;
	IDFY_SetBit(iWord, 15, ATA_BitGet(bit_BLOCK_ERASE_SUPPORTED));
	IDFY_SetBit(iWord, 14, ATA_BitGet(bit_OVERWRITE_SUPPORTED));
	IDFY_SetBit(iWord, 13, ATA_BitGet(bit_CRYPTO_SCRAMBLE_SUPPORTED));
	IDFY_SetBit(iWord, 12, ATA_BitGet(bit_SANITIZE_SUPPORTED));
	IDFY_SetBit(iWord, 11, ATA_BitGet(bit_ACS3_COMMANDS_ALLOWED_BY_SANITIZE));
	IDFY_SetBit(iWord, 10, ATA_BitGet(bit_SANITIZE_ANTIFREEZE_LOCK_SUPPORTED));

	/*=====================================================================
	** Word 60..61 - Total number of user addressable logical sectors for
	**               28-bit commands
	*/
	iWord = IDFY_W060_W061_TOTAL_SECTORS_28BIT;
	IDFY_SetDword(iWord, min(0x0FFFFFFF, ATA_Field(ACCESSIBLE_CAPACITY)));

	/*=====================================================================
	** Word 62 - Obsolete
	*/

	/*=====================================================================
	** Word 63
	**   15:11 - Reserved
	**      10 - Multiword DMA mode 2 is selected
	**       9 - Multiword DMA mode 1 is selected
	**       8 - Multiword DMA mode 0 is selected
	**     7:3 - Reserved
	**       2 - Multiword DMA mode 2 and below are supported
	**       1 - Multiword DMA mode 1 and below are supported
	**       0 - Multiword DMA mode 0 is supported
	*/
	iWord = IDFY_W063;
	IDFY_SetBit(iWord, 10, ATA_BitGet(bit_MULTIWORD_DMA_MODE2_ENABLED));
	IDFY_SetBit(iWord,  9, ATA_BitGet(bit_MULTIWORD_DMA_MODE1_ENABLED));
	IDFY_SetBit(iWord,  8, ATA_BitGet(bit_MULTIWORD_DMA_MODE0_ENABLED));
	IDFY_SetBit(iWord,  2, ATA_BitGet(bit_MULTIWORD_DMA_MODE2_SUPPORTED));
	IDFY_SetBit(iWord,  1, ATA_BitGet(bit_MULTIWORD_DMA_MODE1_SUPPORTED));
	IDFY_SetBit(iWord,  0, ATA_BitGet(bit_MULTIWORD_DMA_MODE0_SUPPORTED));

	/*=====================================================================
	** Word 64
	**    15:2 - Reserved
	**     1:0 - PIO mode 3 and mode 4 supported
	*/
	iWord = IDFY_W064;
	IDFY_SetBit(iWord,  1, ATA_BitGet(bit_PIO_MODE4_IS_SUPPORTED));
	IDFY_SetBit(iWord,  0, ATA_BitGet(bit_PIO_MODE3_IS_SUPPORTED));

	/*=====================================================================
	** Word 65 - Minimum Multiword DMA transfer cycle time per word
	*/
	iWord = IDFY_W065_MIN_MULTIWORD_CYCLE_TIME;
	IDFY_SetWord(iWord, ATA_Field(MIN_MULTIWORD_CYCLE_TIME));

	/*=====================================================================
	** Word 66 - Manufacturer's recommended Multiword DMA transfer
	**           cycle time
	*/
	iWord = IDFY_W066_RECOMMENDED_MULTIWORD_CYCLE_TIME;
	IDFY_SetWord(iWord, ATA_Field(RECOMMENDED_MULTIWORD_CYCLE_TIME));

	/*=====================================================================
	** Word 67 - Minimum PIO transfer cycle time without flow control
	*/
	iWord = IDFY_W067_MIN_PIO_TRANSFER_TIME_WITHOUT_IORDY;
	IDFY_SetWord(iWord, ATA_Field(MIN_PIO_TRANSFER_TIME_WITHOUT_IORDY));

	/*=====================================================================
	** Word 68 - Minimum PIO transfer cycle time with IORDY flow control
	*/
	iWord = IDFY_W068_MIN_PIO_TRANSFER_TIME_WITH_IORDY;
	IDFY_SetWord(iWord, ATA_Field(MIN_PIO_TRANSFER_TIME_WITH_IORDY));

	/*=====================================================================
	** Word 69 - Additional Supported
	**      15 - Reserved for CFA
	**      14 - Deterministic data in trimmed LBA range(s) is supported
	**      13 - Long Physical Sector Alignment Error Reporting Control
	**           is supported
	**      12 - Obsolete
	**      11 - READ BUFFER DMA is supported
	**      10 - WRITE BUFFER DMA is supported
	**       9 - Obsolete
	**       8 - DOWNLOAD MICROCODE DMA is supported
	**       7 - Reserved for IEEE 1667
	**       6 - 0 = Optional ATA device 28-bit commands supported
	**       5 - Trimmed LBA range(s) returning zeroed data is supported
	**       4 - Device Encrypts All User Data on the device
	**       3 - Extended Number of User Addressable Sectors is supported
	**       2 - All write cache is non-volatile
	**     1:0 - Zoned Capabilities
	*/
	iWord = IDFY_W069_ADDITIONAL_SUPPORTED;
	IDFY_SetBit(iWord, 14, ATA_BitGet(bit_DRAT_SUPPORTED));
	IDFY_SetBit(iWord, 13, ATA_BitGet(bit_LPS_MISALIGNMENT_REPORTING_SUPPORTED));
	IDFY_SetBit(iWord, 11, ATA_BitGet(bit_READ_BUFFER_DMA_SUPPORTED));
	IDFY_SetBit(iWord, 10, ATA_BitGet(bit_WRITE_BUFFER_DMA_SUPPORTED));
	IDFY_SetBit(iWord,  8, ATA_BitGet(bit_DOWNLOAD_MICROCODE_DMA_SUPPORTED));
	IDFY_SetBit(iWord,  6, ATA_BitGet(bit_28BIT_SUPPORTED));
	IDFY_SetBit(iWord,  5, ATA_BitGet(bit_RZAT_SUPPORTED));
	IDFY_SetBit(iWord,  4, ATA_BitGet(bit_ENCRYPT_ALL_SUPPORTED));
	IDFY_SetBit(iWord,  2, ATA_BitGet(bit_NON_VOLATILE_WRITE_CACHE));
	IDFY_SetField(iWord, 0, ATA_Field(ZONED));

	/*=====================================================================
	** Word 70 - Reserved
	*/

	/*=====================================================================
	** Word 71..74 - Reserved for the IDENTIFY PACKET DEVICE command
	*/

	/*=====================================================================
	** Word 75 - Queue depth
	**    15:5 - Reserved
	**     4:0 - Maximum queue depth - 1
	*/
	iWord = IDFY_W075_QUEUE_DEPTH;
	IDFY_SetField(iWord, 0, ATA_Field(QUEUE_DEPTH));

	/*=====================================================================
	** Word 76 - Serial ATA Capabilities
	**      15 - Supports READ LOG DMA EXT as equivalent to READ LOG EXT
	**      14 - Supports Device Automatic Partial to Slumber transitions
	**      13 - Supports Host Automatic Partial to Slumber transitions
	**      12 - Supports NCQ priority information
	**      11 - Supports Unload while NCQ commands are outstanding
	**      10 - Supports the SATA Phy Event Counters log
	**       9 - Supports receipt of host initiated power management requests
	**       8 - Supports the NCQ feature set
	**     7:4 - Reserved for Serial ATA
	**       3 - Supports SATA Gen3 Signaling Speeed (6.0Gb/s)
	**       2 - Supports SATA Gen2 Signaling Speeed (3.0Gb/s)
	**       1 - Supports SATA Gen1 Signaling Speeed (1.5Gb/s)
	**       0 - Shall be cleared to zero
	*/
	iWord = IDFY_W076_SERIAL_ATA_CAPABILITIES;
	IDFY_SetBit(iWord, 15, ATA_BitGet(bit_READ_LOG_DMA_EXT_AS_EQUIVALENT_TO_READ_LOG_EXT_SUPPORTED));
	IDFY_SetBit(iWord, 14, ATA_BitGet(bit_DEVICE_AUTOMATIC_PARTIAL_TO_SLUMBER_TRANSITIONS_SUPPORTED));
	IDFY_SetBit(iWord, 13, ATA_BitGet(bit_HOST_AUTOMATIC_PARTIAL_TO_SLUMBER_TRANSITIONS_SUPPORTED));
	IDFY_SetBit(iWord, 12, ATA_BitGet(bit_NCQ_PRIORITY_INFORMATION_SUPPORTED));
	IDFY_SetBit(iWord, 11, ATA_BitGet(bit_UNLOAD_WHILE_NCQ_COMMANDS_ARE_OUTSTANDING_SUPPORTED));
	IDFY_SetBit(iWord, 10, ATA_BitGet(bit_SATA_PHY_EVENT_COUNTERS_LOG_SUPPORTED));
	IDFY_SetBit(iWord,  9, ATA_BitGet(bit_RECEIPT_OF_HOST_INITIATED_POWER_MANAGEMENT_REQUESTS_SUPPORTED));
	IDFY_SetBit(iWord,  8, ATA_BitGet(bit_NCQ_FEATURE_SET_SUPPORTED));
	IDFY_SetBit(iWord,  3, ATA_BitGet(bit_SATA_GEN3_SIGNALING_SPEED_SUPPORTED));
	IDFY_SetBit(iWord,  2, ATA_BitGet(bit_SATA_GEN2_SIGNALING_SPEED_SUPPORTED));
	IDFY_SetBit(iWord,  1, ATA_BitGet(bit_SATA_GEN1_SIGNALING_SPEED_SUPPORTED));

	/*=====================================================================
	** Word 77 - Serial ATA Additional Capabilities
	**    15:9 - Reserved for Serial ATA
	**       8 - Power Disable feature always enabled
	**       7 - Supports DevSleep to ReducedPwrState
	**       6 - Supports RECEIVE FPDMA QUEUED and SEND FPDMA QUEUEUD commands
	**       5 - Supports NCQ NON-DATA command
	**       4 - Supports NCQ Streaming
	**     3:1 - Coded value indicating current negotiated Serial ATA signal speed
	**       0 - Shall be cleared to zero
	*/
	iWord = IDFY_W077_SERIAL_ATA_ADDITIONAL_CAPABILITIES;
	IDFY_SetBit(iWord,  8, ATA_BitGet(bit_POWER_DISABLE_FEATURE_ALWAYS_ENABLED));
	IDFY_SetBit(iWord,  7, ATA_BitGet(bit_DEVSLEEP_TO_REDUCEDPWRSTATE_CAPABILITY_SUPPORTED));
	IDFY_SetBit(iWord,  6, ATA_BitGet(bit_SEND_AND_RECEIVE_QUEUED_COMMANDS_SUPPORTED));
	IDFY_SetBit(iWord,  5, ATA_BitGet(bit_NCQ_QUEUE_MANAGEMENT_COMMAND_SUPPORTED));
	IDFY_SetBit(iWord,  4, ATA_BitGet(bit_NCQ_STREAMING_SUPPORTED));
	IDFY_SetField(iWord,  1, ATA_Field(CURRENT_SERIAL_ATA_SIGNAL_SPEED));

	/*=====================================================================
	** Word 78 - Serial ATA features supported
	**   15:13 - Reserved for Serial ATA
	**      12 - Power Disable feature supported
	**      11 - Device supports Rebuild Assist feature set
	**      10 - Reserved for Serial ATA
	**       9 - Device supports Hybrid Information
	**       8 - Device Sleep feature supported
	**       7 - Device supports NCQ Autosense
	**       6 - Device supports Software Setings Preservation
	**       5 - Device supports Hardware Feature Control
	**       4 - Device supports in-order data delivery
	**       3 - Device supports initiating power management
	**       2 - Device supports DMA Setup auto-activation
	**       1 - Device supports nonzero buffer offsets
	**       0 - Shall be cleared to zero
	*/
	iWord = IDFY_W078_SERIAL_ATA_FEATURES_SUPPORTED;
	IDFY_SetBit(iWord, 12, ATA_BitGet(bit_POWER_DISABLE_FEATURE_SUPPORTED));
	IDFY_SetBit(iWord, 11, ATA_BitGet(bit_REBUILD_ASSIST_SUPPORTED));
	IDFY_SetBit(iWord,  9, ATA_BitGet(bit_HYBRID_INFORMATION_SUPPORTED));
	IDFY_SetBit(iWord,  8, ATA_BitGet(bit_DEVICE_SLEEP_SUPPORTED));
	IDFY_SetBit(iWord,  7, ATA_BitGet(bit_NCQ_AUTOSENSE_SUPPORTED));
	IDFY_SetBit(iWord,  6, ATA_BitGet(bit_SOFTWARE_SETTINGS_PRESERVATION_SUPPORTED));
	IDFY_SetBit(iWord,  5, ATA_BitGet(bit_HARDWARE_FEATURE_CONTROL_SUPPORTED));
	IDFY_SetBit(iWord,  4, ATA_BitGet(bit_IN_ORDER_DATA_DELIVERY_SUPPORTED));
	IDFY_SetBit(iWord,  3, ATA_BitGet(bit_DEVICE_INITIATED_POWER_MANAGEMENT_SUPPORTED));
	IDFY_SetBit(iWord,  2, ATA_BitGet(bit_DMA_SETUP_AUTO_ACTIVATION_SUPPORTED));
	IDFY_SetBit(iWord,  1, ATA_BitGet(bit_NONZERO_BUFFER_OFFSETS_SUPPORTED));

	/*=====================================================================
	** Word 79 - Serial ATA features enabled
	**   15:12 - Reserved for Serial ATA
	**      11 - Rebuild Assist feature set enabled
	**      10 - Power Disable feature enabled
	**       9 - Hybrid Information enabled
	**       8 - Device Sleep feature enabled
	**       7 - Automatic Partial to Slumber transition enabled
	**       6 - Software Setings Preservation enabled
	**       5 - Hardware Feature Control enabled
	**       4 - In-order data delivery enabled
	**       3 - Device initiated power management enabled
	**       2 - DMA Setup auto-activation enabled
	**       1 - Nonzero buffer offsets enabled
	**       0 - Shall be cleared to zero
	*/
	iWord = IDFY_W079_SERIAL_ATA_FEATURES_ENABLED;
	IDFY_SetBit(iWord, 11, ATA_BitGet(bit_REBUILD_ASSIST_ENABLED));
	IDFY_SetBit(iWord, 10, ATA_BitGet(bit_POWER_DISABLE_FEATURE_ENABLED));
	IDFY_SetBit(iWord,  9, ATA_BitGet(bit_HYBRID_INFORMATION_ENABLED));
	IDFY_SetBit(iWord,  8, ATA_BitGet(bit_DEVICE_SLEEP_ENABLED));
	IDFY_SetBit(iWord,  7, ATA_BitGet(bit_AUTOMATIC_PARTIAL_TO_SLUMBER_TRANSITIONS_ENABLED));
	IDFY_SetBit(iWord,  6, ATA_BitGet(bit_SOFTWARE_SETTINGS_PRESERVATION_ENABLED));
	IDFY_SetBit(iWord,  5, ATA_BitGet(bit_HARDWARE_FEATURE_CONTROL_IS_ENABLED));
	IDFY_SetBit(iWord,  4, ATA_BitGet(bit_IN_ORDER_DATA_DELIVERY_ENABLED));
	IDFY_SetBit(iWord,  3, ATA_BitGet(bit_DEVICE_INITIATED_POWER_MANAGEMENT_ENABLED));
	IDFY_SetBit(iWord,  2, ATA_BitGet(bit_DMA_SETUP_AUTO_ACTIVATION_ENABLED));
	IDFY_SetBit(iWord,  1, ATA_BitGet(bit_NONZERO_BUFFER_OFFSETS_ENABLED));

	/*=====================================================================
	** Word 80 - Major version number
	**   15:12 - Reserved
	**      11 - supports ACS-4
	**      10 - supports ACS-3
	**       9 - supports ACS-2
	**       8 - supports ATA8-ACS
	**       7 - Obsolete
	**       6 - Obsolete
	**       5 - Obsolete
	**       4 - Obsolete
	**       3 - Obsolete
	**       2 - Obsolete
	**       1 - Obsolete
	**       0 - Reserved
	*/

	/*=====================================================================
	** Word 81 - Minor version number
	*/

	/*=====================================================================
	** Word 82 - Commands and feature sets supported
	**      15 - Obsolete
	**      14 - The NOP command is supported
	**      13 - The READ BUFFER command is supported
	**      12 - The WRITE BUFFER command is supported
	**   11:10 - Obsolete
	**       9 - Shall be cleared to zero (i.e., the DEVICE RESET command
	**           is not supported)
	**     8:7 - Obsolete
	**       6 - Read look-ahead is supported
	**       5 - The volatile write cache is supported
	**       4 - Shall be cleared to zero (i.e., the PACKET feature set
	**           is not supported)
	**       3 - Shall be set to one (i.e., the Power Management feature
	**           set is supported)
	**       2 - Obsolete
	**       1 - The Security feature set is supported
	**       0 - The SMART feature set is supported
	*/
	iWord = IDFY_W082_COMMANDS_AND_FEATURE_SETS_SUPPORTED;
	IDFY_SetBit(iWord, 14, ATA_BitGet(bit_NOP_SUPPORTED));
	IDFY_SetBit(iWord, 13, ATA_BitGet(bit_READ_BUFFER_SUPPORTED));
	IDFY_SetBit(iWord, 12, ATA_BitGet(bit_WRITE_BUFFER_SUPPORTED));
	IDFY_SetBit(iWord,  6, ATA_BitGet(bit_READ_LOOK_AHEAD_SUPPORTED));
	IDFY_SetBit(iWord,  5, ATA_BitGet(bit_VOLATILE_WRITE_CACHE_SUPPORTED));
	IDFY_SetBit(iWord,  3, 1);
	IDFY_SetBit(iWord,  1, ATA_BitGet(bit_SECURITY_SUPPORTED));
	IDFY_SetBit(iWord,  0, ATA_BitGet(bit_SMART));

	/*=====================================================================
	** Word 83 - Commands and feature sets supported
	**      15 - Shall be cleared to zero
	**      14 - Shall be set to one
	**      13 - The FLUSH CACHE EXT command is supported
	**      12 - Shall be set to one (i.e., the FLUSH CACHE command is
	**           supported)
	**      11 - Obsolete
	**      10 - The 48-bit Address feature set is supported
	**     9:7 - Obsolete
	**       6 - SET FEATURES subcommand is required to spin-up after power-up
	**       5 - The PUIS feature set is supported
	**       4 - Obsolete
	**       3 - The APM feature set is supported
	**       2 - Reserved for CFA
	**       1 - Obsolete
	**       0 - The DOWNLOAD MICROCODE command is supported
	*/
	iWord = IDFY_W083_COMMANDS_AND_FEATURE_SETS_SUPPORTED;
	IDFY_SetBit(iWord, 14, 1);
	IDFY_SetBit(iWord, 13, ATA_BitGet(bit_FLUSH_CACHE_EXT_SUPPORTED));
	IDFY_SetBit(iWord, 12, 1);
	IDFY_SetBit(iWord, 10, ATA_BitGet(bit_48BIT_SUPPORTED));
	IDFY_SetBit(iWord,  6, ATA_BitGet(bit_SPIN_UP_SUPPORTED));
	IDFY_SetBit(iWord,  5, ATA_BitGet(bit_PUIS_SUPPORTED));
	IDFY_SetBit(iWord,  3, ATA_BitGet(bit_APM_SUPPORTED));
	IDFY_SetBit(iWord,  0, ATA_BitGet(bit_DOWNLOAD_MICROCODE_SUPPORTED));

	/*=====================================================================
	** Word 84 - Commands and feature sets supported
	**      15 - Shall be cleared to zero
	**      14 - Shall be set to one
	**      13 - The IDLE IMMEDIATE command with UNLOAD feature is supported
	**    12:9 - Obsolete
	**       8 - Shall be set to one (i.e., the World Wide Name is supported)
	**       7 - Obsolete
	**       6 - The WRITE DMA FUA EXT command is supported
	**       5 - The GPL feature set is supported
	**       4 - The Streaming feature set is supported
	**       3 - Obsolete
	**       2 - Reserved
	**       1 - The SMART self-test is supported
	**       0 - SMART error logging is supported
	*/
	iWord = IDFY_W084_COMMANDS_AND_FEATURE_SETS_SUPPORTED;
	IDFY_SetBit(iWord, 14, 1);
	IDFY_SetBit(iWord, 13, ATA_BitGet(bit_UNLOAD_SUPPORTED));
	IDFY_SetBit(iWord,  8, 1);
	IDFY_SetBit(iWord,  6, ATA_BitGet(bit_WRITE_FUA_EXT_SUPPORTED));
	IDFY_SetBit(iWord,  5, ATA_BitGet(bit_GPL_SUPPORTED));
	IDFY_SetBit(iWord,  4, ATA_BitGet(bit_STREAMING_SUPPORTED));
	IDFY_SetBit(iWord,  1, ATA_BitGet(bit_SMART_SELF_TEST_SUPPORTED));
	IDFY_SetBit(iWord,  0, ATA_BitGet(bit_SMART_ERROR_LOGGING_SUPPORTED));

	/*=====================================================================
	** Word 85 - Commands and feature sets supported or enabled
	**      15 - Obsolete
	**      14 - The NOP command is supported
	**      13 - The READ BUFFER command is supported
	**      12 - The WRITE BUFFER command is supported
	**   11:10 - Obsolete
	**       9 - Shall be cleared to zero (i.e., the DEVICE RESET command
	**           is not supported)
	**     8:7 - Obsolte
	**       6 - Read look-ahead is enabled
	**       5 - The volatile write cache is enabled
	**       4 - Shall be cleared to zero (i.e., the PACKET feature set
	**           is not supported)
	**       3 - Shall be set to one (i.e., the Power Management feature
	**           set is supported)
	**       2 - Obsolete
	**       1 - The Security feature set is enabled
	**       0 - The SMART feature set is enabled
	*/
	iWord = IDFY_W085_COMMANDS_AND_FEATURE_SETS_SUPPORTED_OR_ENABLED;
	IDFY_SetBit(iWord, 14, ATA_BitGet(bit_NOP_SUPPORTED));
	IDFY_SetBit(iWord, 13, ATA_BitGet(bit_READ_BUFFER_SUPPORTED));
	IDFY_SetBit(iWord, 12, ATA_BitGet(bit_WRITE_BUFFER_SUPPORTED));
	IDFY_SetBit(iWord,  6, ATA_BitGet(bit_READ_LOOK_AHEAD_ENABLED));
	IDFY_SetBit(iWord,  5, ATA_BitGet(bit_VOLATILE_WRITE_CACHE_ENABLED));
	IDFY_SetBit(iWord,  3, 1);
	IDFY_SetBit(iWord,  1, ATA_BitGet(bit_SECURITY_ENABLED));
	IDFY_SetBit(iWord,  0, ATA_BitGet(bit_SMART_ENABLED));

	/*=====================================================================
	** Word 86 - Commands and feature sets supported or enabled
	**      15 - Words 119..120 are valid
	**      14 - Reserved
	**      13 - FLUSH CACHE EXT command supported
	**      12 - FLUSH CACHE command supported
	**      11 - Obsolete
	**      10 - The 48-bit Address feature set is supported
	**     9:7 - Obsolete
	**       6 - SET FEATURES subcommand is required to spin-up after power-up
	**       5 - The PUIS faeture set is enabled
	**       4 - Obsolete
	**       3 - The APM feature set is enabled
	**       2 - Reserved for CFA
	**       1 - Obsolete
	**       0 - The DOWNLOAD MICROCODE command is supported
	*/
	iWord = IDFY_W086_COMMANDS_AND_FEATURE_SETS_SUPPORTED_OR_ENABLED;
	IDFY_SetBit(iWord, 13, ATA_BitGet(bit_FLUSH_CACHE_EXT_SUPPORTED));
	IDFY_SetBit(iWord, 12, 1);
	IDFY_SetBit(iWord, 10, ATA_BitGet(bit_48BIT_SUPPORTED));
	IDFY_SetBit(iWord,  6, ATA_BitGet(bit_SPIN_UP_SUPPORTED));
	IDFY_SetBit(iWord,  5, ATA_BitGet(bit_PUIS_ENABLED));
	IDFY_SetBit(iWord,  3, ATA_BitGet(bit_APM_ENABLED));
	IDFY_SetBit(iWord,  0, ATA_BitGet(bit_DOWNLOAD_MICROCODE_SUPPORTED));

	/*=====================================================================
	** Word 87 - Commands and feature sets supported or enabled
	**      15 - Shall be cleared to zero
	**      14 - Shall be set to one
	**      13 - The IDLE IMMEDIATE command with UNLOAD feature is supported
	**    12:9 - Obsolete
	**       8 - Shall be set to one (i.e., thw World Wide Name is supported)
	**       7 - Obsolete
	**       6 - The WRITE DMA FUA EXT command is supported
	**       5 - The GPL feature set is uspported
	**     4:3 - Obsolete
	**       2 - Media serial number is valid
	**       1 - SMART self-test is supported
	**       0 - SMART error logging is supported
	*/
	iWord = IDFY_W087_COMMANDS_AND_FEATURE_SETS_SUPPORTED_OR_ENABLED;
	IDFY_SetBit(iWord, 14, 1);
	IDFY_SetBit(iWord, 13, ATA_BitGet(bit_UNLOAD_SUPPORTED));
	IDFY_SetBit(iWord,  8, 1);
	IDFY_SetBit(iWord,  6, ATA_BitGet(bit_WRITE_FUA_EXT_SUPPORTED));
	IDFY_SetBit(iWord,  5, ATA_BitGet(bit_GPL_SUPPORTED));
	IDFY_SetBit(iWord,  1, ATA_BitGet(bit_SMART_SELF_TEST_SUPPORTED));
	IDFY_SetBit(iWord,  0, ATA_BitGet(bit_SMART_ERROR_LOGGING_SUPPORTED));

	/*=====================================================================
	** Word 88 - Ultra DMA modes
	**      15 - Reserved
	**      14 - Ultra DMA mode 6 is selected
	**      13 - Ultra DMA mode 5 is selected
	**      12 - Ultra DMA mode 4 is selected
	**      11 - Ultra DMA mode 3 is selected
	**      10 - Ultra DMA mode 2 is selected
	**       9 - Ultra DMA mode 1 is selected
	**       8 - Ultra DMA mode 0 is selected
	**       7 - Reserved
	**       6 - Ultra DMA mode 6 and below are supported
	**       5 - Ultra DMA mode 5 and below are supported
	**       4 - Ultra DMA mode 4 and below are supported
	**       3 - Ultra DMA mode 3 and below are supported
	**       2 - Ultra DMA mode 2 and below are supported
	**       1 - Ultra DMA mode 1 and below are supported
	**       0 - Ultra DMA mode 0 is supported
	*/
	iWord = IDFY_W088_ULTRA_DMA_MODES;
	IDFY_SetBit(iWord, 14, ATA_BitGet(bit_UDMA_MODE6_ENABLED));
	IDFY_SetBit(iWord, 13, ATA_BitGet(bit_UDMA_MODE5_ENABLED));
	IDFY_SetBit(iWord, 12, ATA_BitGet(bit_UDMA_MODE4_ENABLED));
	IDFY_SetBit(iWord, 11, ATA_BitGet(bit_UDMA_MODE3_ENABLED));
	IDFY_SetBit(iWord, 10, ATA_BitGet(bit_UDMA_MODE2_ENABLED));
	IDFY_SetBit(iWord,  9, ATA_BitGet(bit_UDMA_MODE1_ENABLED));
	IDFY_SetBit(iWord,  8, ATA_BitGet(bit_UDMA_MODE0_ENABLED));
	IDFY_SetBit(iWord,  6, ATA_BitGet(bit_UDMA_MODE6_SUPPORTED));
	IDFY_SetBit(iWord,  5, ATA_BitGet(bit_UDMA_MODE5_SUPPORTED));
	IDFY_SetBit(iWord,  4, ATA_BitGet(bit_UDMA_MODE4_SUPPORTED));
	IDFY_SetBit(iWord,  3, ATA_BitGet(bit_UDMA_MODE3_SUPPORTED));
	IDFY_SetBit(iWord,  2, ATA_BitGet(bit_UDMA_MODE2_SUPPORTED));
	IDFY_SetBit(iWord,  1, ATA_BitGet(bit_UDMA_MODE1_SUPPORTED));
	IDFY_SetBit(iWord,  0, ATA_BitGet(bit_UDMA_MODE0_SUPPORTED));

	/*=====================================================================
	** Word 89 - Time required for a Normal Erase mode SECURITY ERASE UNIT
	**           command
	*/
	iWord = IDFY_W089_NORMAL_SECURITY_ERASE_TIME;
	IDFY_SetBit(iWord, 15, ATA_BitGet(bit_NORMAL_SECURITY_ERASE_TIME_FORMAT));
	IDFY_SetField(iWord, 0, ATA_Field(NORMAL_SECURITY_ERASE_TIME));

	/*=====================================================================
	** Word 90 - Time required for an Enhanced Erase mode SECURITY ERASE
	**           UNIT command
	*/
	iWord = IDFY_W090_ENHANCED_SECURITY_ERASE_TIME;
	IDFY_SetWord(iWord, ATA_Field(ENHANCED_SECURITY_ERASE_TIME));

	/*=====================================================================
	** Word 91
	**    15:8 - Reserved
	**     7:0 - Current APM level value
	*/
	iWord = IDFY_W091_CURRENT_ADVANCED_POWER_MANAGEMENT_LEVEL_VALUE;
	IDFY_SetField(iWord, 0, ATA_Field(APM_LEVEL));

	/*=====================================================================
	** Word 92 - Master Password Identifier
	*/
	iWord = IDFY_W092_MASTER_PASSWORD_IDENTIFIER;
	IDFY_SetWord(iWord, ATA_Field(MASTER_PASSWORD_IDENTIFIER));

	/*=====================================================================
	** Word 93 - Hardware reset results
	**           For SATA devices, word 93 shall be set to the value 0000h
	*/
	iWord = IDFY_W093_HARDWARE_RESET_RESULTS;

	/*=====================================================================
	** Word 94 - Obsolete
	*/

	/*=====================================================================
	** Word 95 - Stream Minimum Request Size
	*/
	iWord = IDFY_W095_STREAM_MINIMUM_REQUEST_SIZE;
	IDFY_SetWord(iWord, ATA_Field(STREAM_MIN_REQUEST_SIZE));

	/*=====================================================================
	** Word 96 - Streaming Transfer Time - DMA
	*/
	iWord = IDFY_W096_STREAMING_TRANSFER_TIME_DMA;
	IDFY_SetWord(iWord, ATA_Field(DMA_SECTOR_TIME));

	/*=====================================================================
	** Word 97 - Streaming Access Latency - DMA and PIO
	*/
	iWord = IDFY_W097_STREAMING_ACCESS_LATENCY;
	IDFY_SetWord(iWord, ATA_Field(STREAM_ACCESS_LATENCY));

	/*=====================================================================
	** Word 98..99 - Streaming Performance Granularity
	*/
	iWord = IDFY_W098_W099_STREAMING_PERFORMANCE_GRANULARITY;
	IDFY_SetDword(iWord, ATA_Field(STREAM_GRANULARITY));

	/*=====================================================================
	** Word 100..103 - Number of User Addressable Logical Sectors
	*/
	iWord = IDFY_W100_W103_NUMBER_OF_USER_ADDRESSABLE_LOGICAL_SECTORS;

	/*=====================================================================
	** Word 104 - Streaming Transfer Time - PIO
	*/
	iWord = IDFY_W104_STREAMING_TRANSFER_TIME_PIO;
	IDFY_SetWord(iWord, ATA_Field(PIO_SECTOR_TIME));

	/*=====================================================================
	** Word 105 - Maximum number of 512-byte blocks per DATA SET
	**            MANAGEMENT command
	*/
	iWord = IDFY_W105_MAX_PAGES_PER_DSM_COMMAND;
	IDFY_SetWord(iWord, ATA_Field(MAX_PAGES_PER_DSM_COMMAND));

	/*=====================================================================
	** Word 106 - Physical sector size / logical sector size
	**       15 - Shall be cleared to zero
	**       14 - Shall be set to one
	**       13 - Device has multiple logical sectors per physcial sector
	**       12 - Device Logical Sector longer than 256 words
	**     11:4 - Reserved
	**      3:0 - 2^x logical sectors per physical sector
	*/
	iWord = IDFY_W106_PHYSICAL_SECTOR_SIZE_LOGICAL_SECTOR_SIZE;
	IDFY_SetBit(iWord, 14, 1);
	IDFY_SetBit(iWord, 13, ATA_BitGet(bit_LOGICAL_TO_PHYSICAL_SECTOR_RELATIONSHIP_SUPPORTED));
	IDFY_SetBit(iWord, 12, ATA_BitGet(bit_LOGICAL_SECTOR_SIZE_SUPPORTED));
	IDFY_SetField(iWord, 0, ATA_Field(LOGICAL_TO_PHYSICAL_SECTOR_RELATIONSHIP));

	/*=====================================================================
	** Word 107 - Inter-seek delay for ISO/IEC 7779 standard acoustic
	**            testing
	*/

	/*=====================================================================
	** Word 108..111 - World wide name
	*/
	iWord = IDFY_W108_W111_WORLD_WIDE_NAME;
	IDFY_Copy(iWord, ATA_Field(WORLD_WIDE_NAME));

	/*=====================================================================
	** Word 112..115 - Reserved
	*/

	/*=====================================================================
	** Word 116 - Obsolete
	*/

	/*=====================================================================
	** Word 117..118 - Logical sector size
	*/
	iWord = IDFY_W117_W118_LOGICAL_SECTOR_SIZE;
	IDFY_SetWord(iWord, ATA_Field(LOGICAL_SECTOR_SIZE));

	/*=====================================================================
	** Word 119 - Commands and feature sets supported
	**       15 - Shall be cleared to zero
	**       14 - Shall be set to one
	**    13:10 - Reserved
	**        9 - DSN feature set is supported
	**        8 - Accessible Max Addres Configuration feature set is supported
	**        7 - EPC feature set is supported
	**        6 - Sense Data Reporting feature set is supported
	**        5 - The Free-fall Control feature set is supported
	**        4 - Downlaod Microcode mode 3 is supported
	**        3 - The READ LOG DMA EXT command and WRITE LOG DMA EXT
	**            command are supported
	**        2 - The WRITE UNCORRECTABLE EXT command is supported
	**        1 - The Write-Read-Verify feature set is supported
	**        0 - Obsolete
	*/
	iWord = IDFY_W119_COMMANDS_AND_FEATURE_SETS_SUPPORTED;
	IDFY_SetBit(iWord, 14, 1);
	IDFY_SetBit(iWord,  9, ATA_BitGet(bit_DSN_SUPPORTED));
	IDFY_SetBit(iWord,  8, ATA_BitGet(bit_AMAX_ADDR_SUPPORTED));
	IDFY_SetBit(iWord,  7, ATA_BitGet(bit_EPC_SUPPORTED));
	IDFY_SetBit(iWord,  6, ATA_BitGet(bit_SENSE_DATA_SUPPORTED));
	IDFY_SetBit(iWord,  5, ATA_BitGet(bit_FREE_FALL_SUPPORTED));
	IDFY_SetBit(iWord,  4, ATA_BitGet(bit_DM_MODE3_SUPPORTED));
	IDFY_SetBit(iWord,  3, ATA_BitGet(bit_GPL_DMA_SUPPORTED));
	IDFY_SetBit(iWord,  2, ATA_BitGet(bit_WRITE_UNCORRECTABLE_SUPPORTED));
	IDFY_SetBit(iWord,  1, ATA_BitGet(bit_WRV_SUPPORTED));

	/*=====================================================================
	** Word 120 - Commands and feature sets supported or enabled
	**       15 - Shall be cleared to zero
	**       14 - Shall be set to one
	**    13:10 - Reserved
	**        9 - DSN feature set is enabled
	**        8 - Reserved
	**        7 - EPC feature set is enabled
	**        6 - Sense Data Reporting feature set is enabled
	**        5 - The Free-fall Control feature set is enabled
	**        4 - Downlaod Microcode mode 3 is supported
	**        3 - The READ LOG DMA EXT command and WRITE LOG DMA EXT
	**            command are supported
	**        2 - The WRITE UNCORRECTABLE EXT command is supported
	**        1 - The Write-Read-Verify feature set is enabled
	**        0 - Obsolete
	*/
	iWord = IDFY_W120_COMMANDS_AND_FEATURE_SETS_SUPPORTED_OR_ENABLED;
	IDFY_SetBit(iWord, 14, 1);
	IDFY_SetBit(iWord,  9, ATA_BitGet(bit_DSN_ENABLED));
	IDFY_SetBit(iWord,  7, ATA_BitGet(bit_EPC_ENABLED));
	IDFY_SetBit(iWord,  6, ATA_BitGet(bit_SENSE_DATA_ENABLED));
	IDFY_SetBit(iWord,  5, ATA_BitGet(bit_FREE_FALL_ENABLED));
	IDFY_SetBit(iWord,  4, ATA_BitGet(bit_DM_MODE3_SUPPORTED));
	IDFY_SetBit(iWord,  3, ATA_BitGet(bit_GPL_DMA_SUPPORTED));
	IDFY_SetBit(iWord,  2, ATA_BitGet(bit_WRITE_UNCORRECTABLE_SUPPORTED));
	IDFY_SetBit(iWord,  1, ATA_BitGet(bit_WRV_ENABLED));

	/*=====================================================================
	** Word 121..126 - Reserved for expanded supported and enabled settings
	*/

	/*=====================================================================
	** Word 127 - Obsolete
	*/

	/*=====================================================================
	** Word 128 - Security status
	**     15:9 - Reserved
	**        8 - Master Password Capability: 0 = High, 1 = Maximum
	**      7:6 - Reserved
	**        5 - Enahnced security erase supported
	**        4 - Security count expired
	**        3 - Security frozen
	**        2 - Security locked
	**        1 - Security enabled
	**        0 - Security supported
	*/
	iWord = IDFY_W128_SECURITY_STATUS;
	IDFY_SetBit(iWord,  8, ATA_BitGet(bit_MASTER_PASSWORD_CAPABILITY));
	IDFY_SetBit(iWord,  5, ATA_BitGet(bit_ENHANCED_SECURITY_ERASE_SUPPORTED));
	IDFY_SetBit(iWord,  4, ATA_BitGet(bit_SECURITY_COUNT_EXPIRED));
	IDFY_SetBit(iWord,  3, ATA_BitGet(bit_SECURITY_FROZEN));
	IDFY_SetBit(iWord,  2, ATA_BitGet(bit_SECURITY_LOCKED));
	IDFY_SetBit(iWord,  1, ATA_BitGet(bit_SECURITY_ENABLED));
	IDFY_SetBit(iWord,  0, ATA_BitGet(bit_SECURITY_SUPPORTED));

	/*=====================================================================
	** Word 129..159 - Vendor specific
	*/

	/*=====================================================================
	** Word 160..167 - Reserved for CFA
	*/

	/*=====================================================================
	** Word 168
	**     15:4 - Reserved
	**      3:0 - Device Nominal Form Factor
	*/
	iWord = IDFY_W168;
	IDFY_SetField(iWord, 0, ATA_Field(NOMINAL_FORM_FACTOR));

	/*=====================================================================
	** Word 169 - DATA SET MANAGEMENT command support
	**     15:1 - Reserved
	**        0 - The TRIM bit in the DATA SET MANAGEMENT command is supported
	*/
	iWord = IDFY_W169_DATA_SET_MANAGEMENT_COMMAND_SUPPORT;
	IDFY_SetBit(iWord,  0, ATA_BitGet(bit_TRIM_SUPPORTED));

	/*=====================================================================
	** Word 170..173 - Additional Product Identifier
	*/
	iWord = IDFY_W170_W173_ADDITIONAL_PRODUCT_IDENTIFIER;
	IDFY_Copy(iWord, ATA_Field(ADDITIONAL_PRODUCT_IDENTIFIER));

	/*=====================================================================
	** Word 174..175 - Reserved
	*/

	/*=====================================================================
	** Word 176..205 - Current media serial number
	*/
	iWord = IDFY_W176_W205_CURRENT_MEDIA_SERIAL_NUMBER;

	/*=====================================================================
	** Word 206 - SCT Command Transport
	**    15:12 - Vendor Specific
	**     11:8 - Reserved
	**        7 - Reserved for Serial ATA
	**        6 - Reserved
	**        5 - The SCT Data Tables command is supported
	**        4 - The SCT Feature Control command is supported
	**        3 - The SCT Error Recovery Control command is supported
	**        2 - The SCT Write Same command is supported
	**        1 - Obsolete
	**        0 - The SCT Command Transport is supported
	*/
	iWord = IDFY_W206_SCT_COMMAND_TRANSPORT;
	IDFY_SetBit(iWord,  5, ATA_BitGet(bit_SCT_DATA_TABLES_SUPPORTED));
	IDFY_SetBit(iWord,  4, ATA_BitGet(bit_SCT_FEATURE_CONTROL_SUPPORTED));
	IDFY_SetBit(iWord,  3, ATA_BitGet(bit_SCT_ERROR_RECOVERY_CONTROL_SUPPORTED));
	IDFY_SetBit(iWord,  2, ATA_BitGet(bit_SCT_WRITE_SAME_SUPPORTED));
	IDFY_SetBit(iWord,  0, ATA_BitGet(bit_SCT_SUPPORTED));

	/*=====================================================================
	** Word 207..208 - Reserved
	*/

	/*=====================================================================
	** Word 209 - Alignment of logical sectors within a physical sector
	**       15 - Shall be cleared to zero
	**       14 - Shall be set to one
	**     13:0 - Logical sector offset within the first physical sector
	**            where the first logical sector is placed
	*/
	if (ATA_BitGet(bit_LOGICAL_TO_PHYSICAL_SECTOR_RELATIONSHIP_SUPPORTED)) {
		iWord = IDFY_W209_ALIGNMENT_OF_LOGICAL_BLOCKS_WITHIN_PHYSICAL_BLOCK;
		IDFY_SetBit(iWord, 14, 1);
		IDFY_SetField(iWord, 0, ATA_Field(LOGICAL_SECTOR_OFFSET));
	}

	/*=====================================================================
	** Word 210..211 - Write-Read-Verify Sector Mode 3 Count
	*/
	iWord = IDFY_W210_W211_WRITE_READ_VERIFY_SECTOR_MODE3_COUNT;
	IDFY_SetDword(iWord, ATA_Field(WRV_MODE3_COUNT));

	/*=====================================================================
	** Word 212..213 - Write-Read-Verify Sector Mode 2 Count
	*/
	iWord = IDFY_W212_W213_WRITE_READ_VERIFY_SECTOR_MODE2_COUNT;
	IDFY_SetDword(iWord, ATA_Field(WRV_MODE2_COUNT));

	/*=====================================================================
	** Word 214..216 - Obsolete
	*/

	/*=====================================================================
	** Word 217 - Nominal media rotation rate
	*/
	iWord = IDFY_W217_NOMINAL_MEDIA_ROTATION_RATE;
	IDFY_SetWord(iWord, ATA_Field(NOMINAL_MEDIA_ROTATION_RATE));

	/*=====================================================================
	** Word 218 - Reserved
	*/

	/*=====================================================================
	** Word 219 - Obsolete
	*/

	/*=====================================================================
	** Word 220
	**     15:8 - Reserved
	**      7:0 - Write-Read-Verify feature set current mode
	*/
	iWord = IDFY_W220;
	IDFY_SetField(iWord, 0, ATA_Field(WRV_MODE));

	/*=====================================================================
	** Word 221 - Reserved
	*/

	/*=====================================================================
	** Word 222 - Transport major version number
	**            0000h or FFFFh = device does not report version
	**    15:12 - Transport Type
	**            0h = Parallel
	**            1h = Serial
	**            Eh = PCIe
	**            All others = Reserved
	**
	**            Parallel          Serial                  PCIe
	**     11:9 - Reserved          Reserved                Reserved
	**        8 - Reserved          SATA 3.3                Reserved
	**        7 - Reserved          SATA 3.2                Reserved
	**        6 - Reserved          SATA 3.1                Reserved
	**        5 - Reserved          SATA 3.0                Reserved
	**        4 - Reserved          SATA 2.6                Reserved
	**        3 - Reserved          SATA 2.5                Reserved
	**        2 - Reserved          SATA II: Extensions     Reserved
	**        1 - ATA/ATAPI-7       SATA 1.0a               Reserved
	**        0 - ATA8-APT          ATA8-AST                Reserved
	*/

	/*=====================================================================
	** Word 223 - Transport minor version number
	*/

	/*=====================================================================
	** Word 224..229 - Reserved
	*/

	/*=====================================================================
	** Word 230..233 - Extended Number of User Addressable Sectors
	*/

	/*=====================================================================
	** Word 234 - Minimum number of 512-byte data blocks per Download
	**            Microcode operation
	*/
	iWord = IDFY_W234_DM_MINIMUM_TRANSFER_SIZE;
	IDFY_SetWord(iWord, ATA_Field(DM_MINIMUM_TRANSFER_SIZE));

	/*=====================================================================
	** Word 235 - Maximum number of 512-byte data blocks per Download
	**            Microcode operation
	*/
	iWord = IDFY_W235_DM_MAXIMUM_TRANSFER_SIZE;
	IDFY_SetWord(iWord, ATA_Field(DM_MAXIMUM_TRANSFER_SIZE));

	/*=====================================================================
	** Word 236..254 - Reserved
	*/

	/*=====================================================================
	** Word 255 - Integrity word
	**     15:8 - Checksum
	**      7:0 - Checksum Validity Indicator
	*/
}

