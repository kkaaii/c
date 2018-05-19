/******************************************************************************
** File Name: nvme_cq_entry.h
** Author:
** Creation Time: Thu Jan 11 00:26:30 2018
*/
#ifndef	_NVME_CQ_ENTRY_H
#define	_NVME_CQ_ENTRY_H

/*
** 4.6 Completion Queue Entry
**
** An entry in the Completion Queue is at least 26 bytes in size. The contents
** of Dword 0 are command specific. If a command uses Dword 0, then the
** definition of this Dword is contained within the associated command
** definition. If a command does not use Dword 0, the the field is reserved.
** Dword 1 is reserved. Dword 2 is defined in Figure 27 and Dword 3 is defined
** in Figure 28. Any additional I/O Command Set defined in the future may use
** an alternate Completion Queue entry size or format.
*/

typedef union {
	UINT32	all;
} NVME_CQE_DW0;

typedef union {
	UINT32	all;
	struct {
		UINT32	rsvd0;
	};
} NVME_CQE_DW1;

/*
** Figure 27: Completion Queue Entry: DW 2
*/
typedef	union {
	UINT32	all;
	struct {
		UINT32	SQHD	: 16;	/* SQ Head Pointer */
		UINT32	SQID	: 16;	/* SQ Identifier */
	};
} NVME_CQE_DW2;

/*
** Figure 28: Completion Queue Entry: DW 3
*/
typedef	union {
	UINT32	all;
	struct {
		UINT32	CID	: 16;	/* Command Identifier */
		UINT32	P	: 1;	/* Phase Tag */
		UINT32	SF	: 15;	/* Status Field */
	};
} NVME_CQE_DW3;

/*
** Figure 26: Completion Queue Entry Layout - Admin and NVM Command Set
*/
typedef struct {
	NVME_CQE_DW0	dw0;
	NVME_CQE_DW1	dw1;
	NVME_CQE_DW2	dw2;
	NVME_CQE_DW3	dw3;
} NVME_CQE;

/*
** 4.6.1.1 Status Code Type (SCT)
**
** Completion queue entries indicate a status code type for the type of
** completion being reported. Figure 30 specifies the status code type values
** and descriptions.
**
** Figure 30: Status Code - Status Code Type Values
*/
#define	NVME_SCT_GENERIC_COMMAND_STATUS			0
#define	NVME_SCT_COMMAND_SPECIFIC_STATUS		1
#define	NVME_SCT_MEDIA_AND_DATA_INTEGRITY_ERRORS	2
#define	NVME_SCT_RESERVED_3H				3
#define	NVME_SCT_RESERVED_4H				4
#define	NVME_SCT_RESERVED_5H				5
#define	NVME_SCT_RESERVED_6H				6
#define	NVME_SCT_VENDOR_SPECIFIC			7

/*
** 4.6.1.2 Status Code (SC)
**
** The Status Code (SC) field in the completion queue enetry indicates more
** detailed status information about the completion being reported.
**
** Each Status Code set of values is split into three ranges:
** - 00h-7Fh: Applicable to Admin Command Set, or across multiple command sets
** - 80h-BFh: I/O Command Set Specfic status code
** - C0h-FFh: Vendor Specific status codes
**
** If there are multiple status codes that apply to a particular command
** failure, the controller shall report the status code with the lowest
** numerical value.
*/
#define	SCT(nvmeStatus)	((nvmeStatus) >> 8)
#define	SC(nvmeStatus)	((UINT8)(nvmeStatus))

typedef enum {
/*
** 4.6.1.2.1 Generic Command Status Definition
**
** Completion queue entries with a Status Code Type of Generic Command Status
** indicate a status value associated with the command that is generic across
** many different types of commands.
**
** Figure 31: Status Code - Generic Command Status Value
*/
	NVME_STATUS_SUCCESSFUL_COMPLETION	= 0x000,
	NVME_STATUS_INVALID_COMMAND_OPCODE	= 0x001,
	NVME_STATUS_INVALID_FIELD_IN_COMMAND	= 0x002,
	NVME_STATUS_COMMAND_ID_CONFLICT		= 0x003,
	NVME_STATUS_DATA_TRANSFER_ERROR		= 0x004,
	NVME_STATUS_COMMANDS_ABORTED_DUE_TO_POWER_LOSS_NOTIFICATION	= 0x005,
	NVME_STATUS_INTERNAL_ERROR		= 0x006,
	NVME_STATUS_COMMAND_ABORT_REQUESTED	= 0x007,
	NVME_STATUS_COMMAND_ABORTED_DUE_TO_SQ_DELETION	= 0x008,
	NVME_STATUS_COMMAND_ABORTED_DUE_TO_FAILED_FUSED_COMMAND	= 0x009,
	NVME_STATUS_COMMAND_ABORTED_DUE_TO_MISSING_FUSED_COMMAND	= 0x00A,
	NVME_STATUS_INVALID_NAMESPACE_OR_FORMAT	= 0x00B,
	NVME_STATUS_COMMAND_SEQUENCE_ERROR	= 0x00C,
	NVME_STATUS_INVALID_SGL_SEGMENT_DESCRIPTOR	= 0x00D,
	NVME_STATUS_INVALID_NUMBER_OF_SGL_DESCRIPTORS	= 0x00E,
	NVME_STATUS_DATA_SGL_LENGTH_INVALID	= 0x00F,
	NVME_STATUS_METADATA_SGL_LENGTH_INVALID	= 0x010,
	NVME_STATUS_SGL_DESCRIPTOR_TYPE_INVALID	= 0x011,
	NVME_STATUS_INVALID_USE_OF_CONTROLLER_MEMORY_BUFFER	= 0x012,
	NVME_STATUS_PRP_OFFSET_INVALID	= 0x013,
	NVME_STATUS_ATOMIC_WRITE_UNIT_EXCEEDED	= 0x014,
	NVME_STATUS_OPERATION_DENIED	= 0x015,
	NVME_STATUS_SGL_OFFSET_INVALID	= 0x016,
	NVME_STATUS_RESERVED_017H	= 0x017,
	NVME_STATUS_HOST_IDENTIFIER_INCONSISTENT_FORMAT	= 0x018,
	NVME_STATUS_KEEP_ALIVE_TIMEOUT_EXPIRED	= 0x019,
	NVME_STATUS_KEEP_ALIVE_TIMEOUT_INVALID	= 0x01A,
	NVME_STATUS_COMMAND_ABORTED_DUE_TO_PREEMPT_AND_ABORT	= 0x01B,
	NVME_STATUS_SANITIZE_FAILED	= 0x01C,
	NVME_STATUS_SANITIZE_IN_PRORESS	= 0x01D,
	NVME_STATUS_SGL_DATA_BLOCK_GRANULARITY_INVALID	= 0x01E,
	NVME_STATUS_COMMAND_NOT_SUPPORTED_FOR_QUEUE_IN_CMB	= 0x014,
/*
** Figure 32: Status Code - Generic Command Status Values, NVM Command Set
*/
	NVME_STATUS_LBA_OUT_OF_RANGE	= 0x080,
	NVME_STATUS_CAPACITY_EXCEEDED	= 0x081,
	NVME_STATUS_NAMESPACE_NOT_READY	= 0x082,
	NVME_STATUS_RESERVATION_CONFLICT	= 0x083,
	NVME_STATUS_FORMAT_IN_PROGRESS	= 0x084,

/*
** 4.6.1.2.2 Command Specific Errors Definition
**
** Completion queue entries with a Status Code Type of Command Specific Errors
** indicate an error that is specific to a particular command opcode. Status
** codes of 0h to 7Fh are for Admin command errors. Status codes of 80h - BFh
** are specific to the selected I/O command set.
**
** Figure 33: Status Code - Command Specific Status Values
*/
	NVME_STATUS_CQ_INVALID	= 0x100,
	NVME_STATUS_INVALID_QID	= 0x101,
	NVME_STATUS_INVALID_QSIZE	= 0x102,
	NVME_STATUS_ABORT_COMMAND_LIMIT_EXCEDED	= 0x103,
	NVME_STATUS_RESERVED_104H	= 0x104,
	NVME_STATUS_AER_LIMIT_EXCEEDED	= 0x105,
	NVME_STATUS_INVALID_FW_SLOT	= 0x106,
	NVME_STATUS_INVALID_FW_IMAGE	= 0x107,
	NVME_STATUS_INVALID_INT_VECTOR	= 0x108,
	NVME_STATUS_INVALID_LOG_PAGE	= 0x109,
	NVME_STATUS_INVALID_FORMAT	= 0x10A,
	NVME_STATUS_FW_ACTIVATION_REQUIRES_CONVENTIONAL_RESET	= 0x10B,
	NVME_STATUS_INVALID_QUEUE_DELETION	= 0x10C,
	NVME_STATUS_FEATURE_NOT_SAVEABLE	= 0x10D,
	NVME_STATUS_FEATURE_NOT_CHANGEABLE	= 0x10E,
	NVME_STATUS_FEATURE_NOT_NS_SPECIFIC	= 0x10F,
	NVME_STATUS_FW_ACTIVATION_REQUIRES_NVM_SUBSYSTEM_RESET	= 0x110,
	NVME_STATUS_FW_ACTIVATION_REQUIRES_RESET	= 0x111,
	NVME_STATUS_FW_ACTIVATION_REQUIRES_MAXIMUM_TIME_VIOLATION	= 0x112,
	NVME_STATUS_FW_ACTIVATION_PROHIBITED	= 0x113,
	NVME_STATUS_OVERLAPPING_RANGE	= 0x114,
	NVME_STATUS_NAMESPACE_INSUFFICIENT_CAPACITY	= 0x115,
	NVME_STATUS_NAMESPACE_ID_UNAVAILABLE	= 0x116,
	NVME_STATUS_RESERVED_117H	= 0x117,
	NVME_STATUS_NAMESPACE_ALREADY_ATTACHED	= 0x118,
	NVME_STATUS_NAMESPACE_IS_PRIVATE	= 0x119,
	NVME_STATUS_NAMESPACE_NOT_ATTACHED	= 0x11A,
	NVME_STATUS_THIN_PROVISIONING_NOT_SUPPORTED	= 0x11B,
	NVME_STATUS_CONTROLLER_LIST_INVALID	= 0x11C,
	NVME_STATUS_DEVICE_SELFTEST_IN_PROGRESS	= 0x11D,
	NVME_STATUS_BOOT_PARTITION_WRITE_PROHIBITED	= 0x11E,
	NVME_STATUS_INVALID_CONTROLLER_ID	= 0x11F,
	NVME_STATUS_INVALID_SECONDARY_CONTROLLER_STATE	= 0x120,
	NVME_STATUS_INVALID_NUMBER_OF_CONTROLLER_RESOURCES	= 0x121,
	NVME_STATUS_INVALID_RESOURCE_ID		= 0x122,

/*
** Figure 34: Status Code - Command Specific Status Values, NVM Command Set
*/
	NVME_STATUS_CONFLICTING_ATTRIBUTES	= 0x180,
	NVME_STATUS_INVALID_PROTECTION_INFO	= 0x181,
	NVME_STATUS_ATTEMPTED_WRITE_TO_READ_ONLY_RANGE	= 0x182,

/*
** 4.6.1.2.3 Media and Data Integrity Errors Definition
**
** Completion queue entries with a Status Code Type of Media and Data
** Integrity Errors indicate an error associated with the command that is due
** to an error associated with the NVM media or data integrity type error.
**
** Figure 35: Status Code - Media and Data Integrity Error Values
*/
	NVME_STATUS_RESERVED_200H	= 0x200,

/*
** Figure 36: Status Code - Media and Data Integrity Error Values, NVM Command
** Set
*/
	NVME_STATUS_WRITE_FAULT	= 0x280,
	NVME_STATUS_UNRECOVERED_READ_ERROR	= 0x281,
	NVME_STATUS_END2END_GUARD_CHECK_ERROR	= 0x282,
	NVME_STATUS_END2END_APPLICATION_TAG_CHECK_ERROR	= 0x283,
	NVME_STATUS_END2END_REFERENCE_TAG_CHECK_ERROR	= 0x284,
	NVME_STATUS_COMPARE_FAILURE	= 0x285,
	NVME_STATUS_ACCESS_DENIED	= 0x286,
	NVME_STATUS_DEALLOCATED_OR_UNWRITTEN_LOGICAL_BLOCK	= 0x287,

/*
** Figure 29: Completion Queue Entry: Status Field
*/
	NVME_STATUS_M	= BIT(14),	/* More */
	NVME_STATUS_DNR	= BIT(15)	/* Do Not Retry */
} NVME_STATUS;

enum {
/*
** Figure 29: Completion Queue Entry: Status Field
*/
	eSF_DoNotRetry	= BIT(14),
	eSF_More	= BIT(13),

/*
** Figure 31: Status Code - Generic Command Status Values
*/
	/* 000h */eSF_SuccessfulCompletion	= 0x000,
	/* 001h */eSF_InvalidCommandOpcode,
	/* 002h */eSF_InvalidFieldInCommand,
	/* 003h */eSF_CommandIdConflict,
	/* 004h */eSF_DataTransferError,
	/* 005h */eSF_CommandsAbortedDueToPowerLossNotification,
	/* 006h */eSF_InternalError,
	/* 007h */eSF_CommandAbortRequested,
	/* 008h */eSF_CommandAbortedDueToSqDeletion,
	/* 009h */eSF_CommandAbortedDueToFailedFusedCommand,
	/* 00Ah */eSF_CommandAbortedDueToMissingFusedCommand,
	/* 00Bh */eSF_InvalidNamespaceOrFormat,
	/* 00Ch */eSF_CommandSequenceError,
	/* 00Dh */eSF_InvalidSglSegmentDescriptor,
	/* 00Eh */eSF_InvalidNumberOfSglDescriptors,
	/* 00Fh */eSF_DataSglLengthInvalid,
	/* 010h */eSF_MetadataSglLengthInvalid,
	/* 011h */eSF_SglDescriptorTypeInvalid,
	/* 012h */eSF_InvalidUseOfControllerMemoryBuffer,
	/* 013h */eSF_PrpOffsetInvalid,
	/* 014h */eSF_AtomicWriteUnitExceeded,
	/* 015h */eSF_OperationDenied,				/* 1.3 - */
	/* 016h */eSF_SglOffsetInvalid,
	/* 017h */eSF_SglSubTypeInvalid,			/* - 1.3 */
	/* 018h */eSF_HostIdentifierInconsistentFormat,
	/* 019h */eSF_KeepAliveTimeoutExpired,
	/* 01Ah */eSF_KeepAliveTimeoutInvalid,
	/* 01Bh */eSF_CommandAbortedDueToPreemptAndAbort,	/* 1.3 - */
	/* 01Ch */eSF_SanitizeFailed,				/* 1.3 - */
	/* 01Dh */eSF_SanitizeInProgress,			/* 1.3 - */
	/* 01Eh */eSF_SglDataBlockGranularityInvalid,		/* 1.3 - */
	/* 01Fh */eSF_CommandNotSupportedForQueueInCmb,		/* 1.3 - */

/*
** Figure 32: Status Code - Generic Command Status Values, NVM Command Set
*/
	/* 080h */eSF_LbaOutOfRange	= 0x080,
	/* 081h */eSF_CapacityExceeded,
	/* 082h */eSF_NamespaceNotReady,
	/* 083h */eSF_ReservationConflict,
	/* 084h */eSF_FormatInProgress,

/*
** Figure 33: Status Code - Command Specific Status Values
*/
	/* 100h */eSF_CompletionQueueInvalid	= 0x100,
	/* 101h */eSF_InvalidQueueIdentifier,
	/* 102h */eSF_InvalidQueueSize,
	/* 103h */eSF_AbortCommandLimitExceeded,
	/* 104h */eSF_Reserved_0104h,
	/* 105h */eSF_AsynchronousEventRequestLimitExceeded,
	/* 106h */eSF_InvalidFirmwareSlot,
	/* 107h */eSF_InvalidFirmwareImage,
	/* 108h */eSF_InvalidInterruptVector,
	/* 109h */eSF_InvalidLogPage,
	/* 10Ah */eSF_InvalidFormat,
	/* 10Bh */eSF_FirmwareActivationRequiresConventionalReset,
	/* 10Ch */eSF_InvalidQueueDeletion,
	/* 10Dh */eSF_FeatureIdentifierNotSaveable,
	/* 10Eh */eSF_FeatureNotChangeable,
	/* 10Fh */eSF_FeatureNotNamespaceSpecific,
	/* 110h */eSF_FirmwareActivationRequiresNvmSubsystemReset,
	/* 111h */eSF_FirmwareActivationRequiresReset,
	/* 112h */eSF_FirmwareActivationRequiresMaximumTimeViolation,
	/* 113h */eSF_FirmwareActivationProhibited,
	/* 114h */eSF_OverlappingRange,
	/* 115h */eSF_NamespaceInsufficientCapacity,
	/* 116h */eSF_NamespaceIdentifierUnavailable,
	/* 117h */eSF_Reserved_0117h,
	/* 118h */eSF_NamespaceAlreadyAttached,
	/* 119h */eSF_NamespaceIsPrivate,
	/* 11Ah */eSF_NamespaceNotAttached,
	/* 11Bh */eSF_ThinProvisioningNotSupported,
	/* 11Ch */eSF_ControllerListInvalid,
	/* 11Dh */eSF_DeviceSelfTestInProgress,			/* 1.3 - */
	/* 11Eh */eSF_BootPartitionWriteProhibited,		/* 1.3 - */
	/* 11Fh */eSF_InvalidControllerIdentifier,		/* 1.3 - */
	/* 120h */eSF_InvalidSecondaryControllerState,		/* 1.3 - */
	/* 121h */eSF_InvalidNumberOfControllerResources,	/* 1.3 - */
	/* 122h */eSF_InvalidResourceIdentifier,		/* 1.3 - */

/*
** Figure 34: Status Code - Command Specific Status Values, NVM Command Set
*/
	/* 180h */eSF_ConflictingAttributes	= 0x180,
	/* 181h */eSF_InvalidProtectionInformation,
	/* 182h */eSF_AttemptedWriteToReadOnlyRange,

/*
** Figure 36: Status Code - Media and Data Integrity Error Values, NVME Command Set
*/
	/* 280h */eSF_WriteFault	= 0x280,
	/* 281h */eSF_UnrecoveredReadError,
	/* 282h */eSF_EndToEndGuardCheckError,
	/* 283h */eSF_EndToEndApplicationTagCheckError,
	/* 284h */eSF_EndToEndReferenceTagCheckError,
	/* 285h */eSF_CompareFailure,
	/* 286h */eSF_AccessDenied,
	/* 287h */eSF_DeallocatedOrUnwrittenLogicalBlock,
};

#endif	/* _NVME_CQ_ENTRY_H */

