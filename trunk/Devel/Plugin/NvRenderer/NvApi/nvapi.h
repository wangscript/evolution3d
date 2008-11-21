 /***************************************************************************\
|*                                                                           *|
|*      Copyright 2005-2006 NVIDIA Corporation.  All rights reserved.        *|
|*                                                                           *|
|*   NOTICE TO USER:                                                         *|
|*                                                                           *|
|*   This source code is subject to NVIDIA ownership rights under U.S.       *|
|*   and international Copyright laws.                                       *|
|*                                                                           *|
|*   This software and the information contained herein is PROPRIETARY       *|
|*   and CONFIDENTIAL to NVIDIA and is being provided under the terms        *|
|*   and conditions of a Non-Disclosure Agreement. Any reproduction or       *|
|*   disclosure to any third party without the express written consent of    *|
|*   NVIDIA is prohibited.                                                   *|
|*                                                                           *|
|*   NVIDIA MAKES NO REPRESENTATION ABOUT THE SUITABILITY OF THIS SOURCE     *|
|*   CODE FOR ANY PURPOSE. IT IS PROVIDED "AS IS" WITHOUT EXPRESS OR         *|
|*   IMPLIED WARRANTY OF ANY KIND.  NVIDIA DISCLAIMS ALL WARRANTIES WITH     *|
|*   REGARD TO THIS SOURCE CODE, INCLUDING ALL IMPLIED WARRANTIES OF         *|
|*   MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR          *|
|*   PURPOSE. IN NO EVENT SHALL NVIDIA BE LIABLE FOR ANY SPECIAL,            *|
|*   INDIRECT, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES          *|
|*   WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN      *|
|*   AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING     *|
|*   OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOURCE      *|
|*   CODE.                                                                   *|
|*                                                                           *|
|*   U.S. Government End Users. This source code is a "commercial item"      *|
|*   as that term is defined at 48 C.F.R. 2.101 (OCT 1995), consisting       *| 
|*   of "commercial computer software" and "commercial computer software     *|
|*   documentation" as such terms are used in 48 C.F.R. 12.212 (SEPT 1995)   *|
|*   and is provided to the U.S. Government only as a commercial end item.   *|
|*   Consistent with 48 C.F.R.12.212 and 48 C.F.R. 227.7202-1 through        *|
|*   227.7202-4 (JUNE 1995), all U.S. Government End Users acquire the       *|
|*   source code with only those rights set forth herein.                    *|
|*                                                                           *|
|*                                                                           *|
 \***************************************************************************/
///////////////////////////////////////////////////////////////////////////////
//
//$Date$
// File: nvapi.h
//
// NvAPI provides an interface to NVIDIA devices. This file contains the 
// interface constants, structure definitions and function prototypes.
//
// Target Profile: NDA-developer
// Target OS-Arch: winxp_x86
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _NVAPI_H
#define _NVAPI_H

#pragma pack(push,8) // Make sure we have consistent structure packings

#ifdef __cplusplus
extern "C" {
#endif

// ====================================================
// Universal NvAPI Definitions
// ====================================================
#ifdef _WIN32
#define NVAPI_INTERFACE extern NvAPI_Status __cdecl 
#else
#define NVAPI_INTERFACE extern NvAPI_Status __cdecl 
#endif

typedef unsigned __int64 NvU64;
typedef unsigned long    NvU32;
typedef int              NvS32;
typedef unsigned short   NvU16;
typedef unsigned char    NvU8;

#define DECLARE_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name

// NVAPI Handles - These handles are retrieved from various calls and passed in to others in NvAPI
//                 These are meant to be opaque types.  Do not assume they correspond to indices, HDCs,
//                 display indexes or anything else.
//
//                 Most handles remain valid until a display re-configuration (display mode set) or GPU
//                 reconfiguration (going into or out of SLI modes) occurs.  If NVAPI_HANDLE_INVALIDATED
//                 is received by an app, it should discard all handles, and re-enumerate them.
//
DECLARE_HANDLE(NvDisplayHandle);            // Display Device driven by NVidia GPU(s) (an attached display)
DECLARE_HANDLE(NvUnAttachedDisplayHandle);  // Unattached Display Device driven by NVidia GPU(s)
DECLARE_HANDLE(NvLogicalGpuHandle);         // One or more physical GPUs acting in concert (SLI)
DECLARE_HANDLE(NvPhysicalGpuHandle);        // A single physical GPU

#define NVAPI_DEFAULT_HANDLE        0

#define NVAPI_GENERIC_STRING_MAX    4096
#define NVAPI_LONG_STRING_MAX       256
#define NVAPI_SHORT_STRING_MAX      64

typedef struct 
{
    NvS32   sX;
    NvS32   sY;
    NvS32   sWidth;
    NvS32   sHeight;
} NvSBox;

#define NVAPI_MAX_PHYSICAL_GPUS             64
#define NVAPI_MAX_LOGICAL_GPUS              64
#define NVAPI_MAX_AVAILABLE_GPU_TOPOLOGIES  256
#define NVAPI_MAX_GPU_TOPOLOGIES            NVAPI_MAX_PHYSICAL_GPUS
#define NVAPI_MAX_GPU_PER_TOPOLOGY          8
#define NVAPI_MAX_DISPLAY_HEADS             2
#define NVAPI_MAX_DISPLAYS                  NVAPI_MAX_PHYSICAL_GPUS * NVAPI_MAX_DISPLAY_HEADS

typedef char NvAPI_String[NVAPI_GENERIC_STRING_MAX];
typedef char NvAPI_LongString[NVAPI_LONG_STRING_MAX];
typedef char NvAPI_ShortString[NVAPI_SHORT_STRING_MAX];

// =========================================================================================
// NvAPI Version Definition
// Maintain per structure specific version define using the MAKE_NVAPI_VERSION macro.
// Usage: #define NV_GENLOCK_STATUS_VER  MAKE_NVAPI_VERSION(NV_GENLOCK_STATUS, 1)
// =========================================================================================
#define MAKE_NVAPI_VERSION(typeName,ver) (NvU32)(sizeof(typeName) | ((ver)<<16))
#define GET_NVAPI_VERSION(ver) (NvU32)((ver)>>16)
#define GET_NVAPI_SIZE(ver) (NvU32)((ver) & 0xffff)

// ====================================================
// NvAPI Status Values
//    All NvAPI functions return one of these codes.
// ====================================================


typedef enum 
{
    NVAPI_OK                                    =  0,      // Success
    NVAPI_ERROR                                 = -1,      // Generic error
    NVAPI_LIBRARY_NOT_FOUND                     = -2,      // nvapi.dll can not be loaded
    NVAPI_NO_IMPLEMENTATION                     = -3,      // not implemented in current driver installation
    NVAPI_API_NOT_INTIALIZED                    = -4,      // NvAPI_Initialize has not been called (successfully)
    NVAPI_INVALID_ARGUMENT                      = -5,      // invalid argument
    NVAPI_NVIDIA_DEVICE_NOT_FOUND               = -6,      // no NVIDIA display driver was found
    NVAPI_END_ENUMERATION                       = -7,      // no more to enum
    NVAPI_INVALID_HANDLE                        = -8,      // invalid handle
    NVAPI_INCOMPATIBLE_STRUCT_VERSION           = -9,      // an argument's structure version is not supported
    NVAPI_HANDLE_INVALIDATED                    = -10,     // handle is no longer valid (likely due to GPU or display re-configuration)
    NVAPI_OPENGL_CONTEXT_NOT_CURRENT            = -11,     // no NVIDIA OpenGL context is current (but needs to be)
    NVAPI_EXPECTED_LOGICAL_GPU_HANDLE           = -100,    // expected a logical GPU handle for one or more parameters
    NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE          = -101,    // expected a phyiscal GPU handle for one or more parameters
    NVAPI_EXPECTED_DISPLAY_HANDLE               = -102,    // expected an NV display handle for one or more parameters
    NVAPI_INVALID_COMBINATION                   = -103,    // used in some commands to indicate that the combination of parameters is not validexpected an NV display handle for one or more parameters
    NVAPI_NOT_SUPPORTED                         = -104,    // Requested feature not supported in the selected GPU
    NVAPI_PORTID_NOT_FOUND                      = -105,    // NO port ID found for I2C transaction
    NVAPI_EXPECTED_UNATTACHED_DISPLAY_HANDLE    = -106,    // expected an unattached display handle as one of the input param
    NVAPI_INVALID_PERF_LEVEL                    = -107,    // invalid perf level 
    NVAPI_DEVICE_BUSY                           = -108,    // device is busy, request not fulfilled
    NVAPI_NV_PERSIST_FILE_NOT_FOUND             = -109,    // NV persist file is not found
    NVAPI_PERSIST_DATA_NOT_FOUND                = -110,    // NV persist data is not found
    NVAPI_EXPECTED_TV_DISPLAY                   = -111,    // expected TV output display
    NVAPI_EXPECTED_TV_DISPLAY_ON_DCONNECTOR     = -112,    // expected TV output on D Connector - HDTV_EIAJ4120.
    NVAPI_NO_ACTIVE_SLI_TOPOLOGY                = -113,    // SLI is not supported for this device
    NVAPI_SLI_RENDERING_MODE_NOTALLOWED         = -114,    // setup of SLI rendering mode is not possible right now
    NVAPI_EXPECTED_DIGITAL_FLAT_PANEL           = -115,    // expected digital flat panel
    NVAPI_ARGUMENT_EXCEED_MAX_SIZE              = -116,    // argument exceeds expected size
    NVAPI_DEVICE_SWITCHING_NOT_ALLOWED          = -117,    // inhibit ON due to one of the flags in NV_GPU_DISPLAY_CHANGE_INHIBIT or SLI Active
    NVAPI_TESTING_CLOCKS_NOT_SUPPORTED          = -118,    // testing clocks not supported
    NVAPI_UNKNOWN_UNDERSCAN_CONFIG              = -119,    // the specified underscan config is from an unknown source (e.g. INF)
    NVAPI_DATA_NOT_FOUND                        = -120,    // Requested data was not found
} NvAPI_Status;


///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_Initialize
//
//   DESCRIPTION: Initializes NVAPI library. This must be called before any 
//                other NvAPI_ function.
//
// RETURN STATUS: NVAPI_ERROR            Something is wrong during the initialization process (generic error)
//                NVAPI_LIBRARYNOTFOUND  Can not load nvapi.dll
//                NVAPI_OK                  Initialized
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_Initialize();

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GetErrorMessage
//
//   DESCRIPTION: converts an NvAPI error code into a null terminated string
//
// RETURN STATUS: null terminated string (always, never NULL)
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GetErrorMessage(NvAPI_Status nr,NvAPI_ShortString szDesc);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GetInterfaceVersionString
//
//   DESCRIPTION: Returns a string describing the version of the NvAPI library.
//                Contents of the string are human readable.  Do not assume a fixed
//                format.
//
// RETURN STATUS: User readable string giving info on NvAPI's version
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GetInterfaceVersionString(NvAPI_ShortString szDesc);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GetDisplayDriverVersion
//
//   DESCRIPTION: Returns a struct that describes aspects of the display driver
//                build.
//
// RETURN STATUS: NVAPI_ERROR or NVAPI_OK
//
///////////////////////////////////////////////////////////////////////////////
typedef struct 
{
    NvU32              version;             // Structure version
    NvU32              drvVersion;           
    NvU32              bldChangeListNum;     
    NvAPI_ShortString  szBuildBranchString; 
    NvAPI_ShortString  szAdapterString;
} NV_DISPLAY_DRIVER_VERSION;
#define NV_DISPLAY_DRIVER_VERSION_VER  MAKE_NVAPI_VERSION(NV_DISPLAY_DRIVER_VERSION,1)
NVAPI_INTERFACE NvAPI_GetDisplayDriverVersion(NvDisplayHandle hNvDisplay, NV_DISPLAY_DRIVER_VERSION *pVersion);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_EnumNvidiaDisplayHandle
//
//   DESCRIPTION: Returns the handle of the NVidia display specified by the enum 
//                index (thisEnum). The client should keep enumerating until it
//                returns NVAPI_END_ENUMERATION.
//
//                Note: Display handles can get invalidated on a modeset, so the calling applications need to 
//                renum the handles after every modeset.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: either the handle pointer is NULL or enum index too big
//                NVAPI_OK: return a valid NvDisplayHandle based on the enum index
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia device found in the system
//                NVAPI_END_ENUMERATION: no more display device to enumerate.
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_EnumNvidiaDisplayHandle(NvU32 thisEnum, NvDisplayHandle *pNvDispHandle);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_EnumNvidiaUnAttachedDisplayHandle
//
//   DESCRIPTION: Returns the handle of the NVidia UnAttached display specified by the enum 
//                index (thisEnum). The client should keep enumerating till it
//                return error.
//
//                Note: Display handles can get invalidated on a modeset, so the calling applications need to 
//                renum the handles after every modeset.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: either the handle pointer is NULL or enum index too big
//                NVAPI_OK: return a valid NvDisplayHandle based on the enum index
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia device found in the system
//                NVAPI_END_ENUMERATION: no more display device to enumerate.
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_EnumNvidiaUnAttachedDisplayHandle(NvU32 thisEnum, NvUnAttachedDisplayHandle *pNvUnAttachedDispHandle);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_EnumPhysicalGPUs
//
//   DESCRIPTION: Returns an array of physical GPU handles.
//
//                Each handle represents a physical GPU present in the system.
//                That GPU may be part of a SLI configuration, or not be visible to the OS directly.
//
//                At least 1 GPU must be present in the system and running an NV display driver.
//
//                The array nvGPUHandle will be filled with physical GPU handle values.  The returned
//                gpuCount determines how many entries in the array are valid.
//
//                Note: In drivers older than 105.00, all physical GPU handles get invalidated on a modeset. So the calling applications 
//                      need to renum the handles after every modeset. 
//                      With drivers 105.00 and up all physical GPU handles are constant.
//                      Physical GPU handles are constant as long as the GPUs are not physically moved and the SBIOS VGA order is unchanged.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: nvGPUHandle or pGpuCount is NULL
//                NVAPI_OK: one or more handles were returned
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_EnumPhysicalGPUs(NvPhysicalGpuHandle nvGPUHandle[NVAPI_MAX_PHYSICAL_GPUS], NvU32 *pGpuCount);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_EnumLogicalGPUs
//
//   DESCRIPTION: Returns an array of logical GPU handles.
//
//                Each handle represents one or more GPUs acting in concert as a single graphics device.
//
//                At least 1 GPU must be present in the system and running an NV display driver.
//
//                The array nvGPUHandle will be filled with logical GPU handle values.  The returned
//                gpuCount determines how many entries in the array are valid.
//
//                Note: All logical GPUs handles get invalidated on a GPU topology change, so the calling application is required to 
//                renum the logical GPU handles to get latest physical handle mapping after every GPU topology change activated 
//                by a call to NvAPI_SetGpuTopologies.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: nvGPUHandle or pGpuCount is NULL
//                NVAPI_OK: one or more handles were returned
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_EnumLogicalGPUs(NvLogicalGpuHandle nvGPUHandle[NVAPI_MAX_LOGICAL_GPUS], NvU32 *pGpuCount);
 
///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GetPhysicalGPUsFromDisplay
//
//   DESCRIPTION: Returns an array of physical GPU handles associated with the specified display.
//
//                At least 1 GPU must be present in the system and running an NV display driver.
//
//                The array nvGPUHandle will be filled with physical GPU handle values.  The returned
//                gpuCount determines how many entries in the array are valid.
//
//                If the display corresponds to more than one physical GPU, the first GPU returned
//                is the one with the attached active output.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: hNvDisp is not valid; nvGPUHandle or pGpuCount is NULL
//                NVAPI_OK: one or more handles were returned
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GetPhysicalGPUsFromDisplay(NvDisplayHandle hNvDisp, NvPhysicalGpuHandle nvGPUHandle[NVAPI_MAX_PHYSICAL_GPUS], NvU32 *pGpuCount);
 
///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GetPhysicalGPUFromUnAttachedDisplay
//
//   DESCRIPTION: Returns a physical GPU handle associated with the specified unattached display.
//
//                At least 1 GPU must be present in the system and running an NV display driver.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: hNvUnAttachedDisp is not valid or pPhysicalGpu is NULL.
//                NVAPI_OK: one or more handles were returned
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GetPhysicalGPUFromUnAttachedDisplay(NvUnAttachedDisplayHandle hNvUnAttachedDisp, NvPhysicalGpuHandle *pPhysicalGpu);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_CreateDisplayFromUnAttachedDisplay
//
//   DESCRIPTION: The unattached display handle is converted to a active attached display handle.
//
//                At least 1 GPU must be present in the system and running an NV display driver.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: hNvUnAttachedDisp is not valid or pNvDisplay is NULL.
//                NVAPI_OK: one or more handles were returned
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_CreateDisplayFromUnAttachedDisplay(NvUnAttachedDisplayHandle hNvUnAttachedDisp, NvDisplayHandle *pNvDisplay);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GetLogicalGPUFromDisplay
//
//   DESCRIPTION: Returns the logical GPU handle associated with the specified display.
//
//                At least 1 GPU must be present in the system and running an NV display driver.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: hNvDisp is not valid; pLogicalGPU is NULL
//                NVAPI_OK: one or more handles were returned
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GetLogicalGPUFromDisplay(NvDisplayHandle hNvDisp, NvLogicalGpuHandle *pLogicalGPU);
 
///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GetLogicalGPUFromPhysicalGPU
//
//   DESCRIPTION: Returns the logical GPU handle associated with specified physical GPU handle.
//
//                At least 1 GPU must be present in the system and running an NV display driver.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: hPhysicalGPU is not valid; pLogicalGPU is NULL
//                NVAPI_OK: one or more handles were returned
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GetLogicalGPUFromPhysicalGPU(NvPhysicalGpuHandle hPhysicalGPU, NvLogicalGpuHandle *pLogicalGPU);
   
///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GetPhysicalGPUsFromLogicalGPU
//
//   DESCRIPTION: Returns the physical GPU handles associated with the specified logical GPU handle.
//
//                At least 1 GPU must be present in the system and running an NV display driver.
//
//                The array hPhysicalGPU will be filled with physical GPU handle values.  The returned
//                gpuCount determines how many entries in the array are valid.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: hLogicalGPU is not valid; hPhysicalGPU is NULL
//                NVAPI_OK: one or more handles were returned
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//                NVAPI_EXPECTED_LOGICAL_GPU_HANDLE: hLogicalGPU was not a logical GPU handle
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GetPhysicalGPUsFromLogicalGPU(NvLogicalGpuHandle hLogicalGPU,NvPhysicalGpuHandle hPhysicalGPU[NVAPI_MAX_PHYSICAL_GPUS], NvU32 *pGpuCount);
   
///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GetAssociatedNvidiaDisplayHandle
//
//   DESCRIPTION: Returns the handle of the NVidia display that is associated
//                with the display name given.  Eg: "\\DISPLAY1"
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: either argument is NULL
//                NVAPI_OK: *pNvDispHandle is now valid
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia device maps to that display name
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GetAssociatedNvidiaDisplayHandle(const char *szDisplayName, NvDisplayHandle *pNvDispHandle);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GetAssociatedNvidiaDisplayName
//
//   DESCRIPTION: Returns the display name given.  Eg: "\\DISPLAY1" using the NVidia display handle
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: either argument is NULL
//                NVAPI_OK: *pNvDispHandle is now valid
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia device maps to that display name
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GetAssociatedNvidiaDisplayName(NvDisplayHandle NvDispHandle, NvAPI_ShortString szDisplayName);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GetUnAttachedAssociatedDisplayName
//
//   DESCRIPTION: Returns the display name given.  Eg: "\\DISPLAY1" using the NVidia unattached display handle
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: either argument is NULL
//                NVAPI_OK: *pNvDispHandle is now valid
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia device maps to that display name
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GetUnAttachedAssociatedDisplayName(NvUnAttachedDisplayHandle hNvUnAttachedDisp, NvAPI_ShortString szDisplayName);



///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_EnableHWCursor
//
//   DESCRIPTION: Enable hardware cursor support
//
// RETURN STATUS: NVAPI_ERROR or NVAPI_OK
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_EnableHWCursor(NvDisplayHandle hNvDisplay);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_DisableHWCursor
//
//   DESCRIPTION: Disable hardware cursor support
//
// RETURN STATUS: NVAPI_ERROR or NVAPI_OK
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_DisableHWCursor(NvDisplayHandle hNvDisplay);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GetVBlankCounter
//
//   DESCRIPTION: get vblank counter
//
// RETURN STATUS: NVAPI_ERROR or NVAPI_OK
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GetVBlankCounter(NvDisplayHandle hNvDisplay, NvU32 *pCounter);

///////////////////////////////////////////////////////////////////////////////
// FUNCTION NAME: NvAPI_OverrideRefreshRate
//   DESCRIPTION: Override the refresh rate on the given  display/outputsMask.
//                The new refresh rate can be applied right away in this API call or deferred to happen with the
//                next OS modeset. The override is only good for one modeset (doesn't matter it's deferred or immediate).
//               
//
//         INPUT: hNvDisplay - the NVidia display handle. It can be NVAPI_DEFAULT_HANDLE or a handle
//                             enumerated from NvAPI_EnumNVidiaDisplayHandle().
//                
//                outputsMask - a set of bits that identify all target outputs which are associated with the NVidia 
//                              display handle to apply the refresh rate override. Note when SLI is enabled,  the
//                              outputsMask only applies to the GPU that is driving the display output.
//
//                refreshRate - the override value. "0.0" means cancel the override.
//                              
//
//                bSetDeferred - "0": apply the refresh rate override immediately in this API call.
//                               "1":  apply refresh rate at the next OS modeset.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: hNvDisplay or outputsMask is invalid
//                NVAPI_OK: the refresh rate override is correct set
//                NVAPI_ERROR: the operation failed
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_SetRefreshRateOverride(NvDisplayHandle hNvDisplay, NvU32 outputsMask, float refreshRate, NvU32 bSetDeferred);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GetAssociatedDisplayOutputId
//
//   DESCRIPTION: Gets the active outputId associated with the display handle.
//
//    PARAMETERS: hNvDisplay(IN) - NVIDIA Display selection. It can be NVAPI_DEFAULT_HANDLE or a handle enumerated from NvAPI_EnumNVidiaDisplayHandle().
//                outputId(OUT)  - The active display output id associated with the selected display handle hNvDisplay.
//                                 The outputid will have only one bit set. In case of clone or span this  will indicate the display
//                                 outputId of the primay display that the GPU is driving.
// RETURN STATUS: NVAPI_OK: call successful.
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found.
//                NVAPI_EXPECTED_DISPLAY_HANDLE: hNvDisplay is not a valid display handle.
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GetAssociatedDisplayOutputId(NvDisplayHandle hNvDisplay, NvU32 *pOutputId);


///////////////////////////////////////////////////////////////////////////////
// FUNCTION NAME:   NvAPI_GetDisplayPortInfo
//
// DESCRIPTION:     This API returns the current DP related into on the specified device(monitor)
//
// PARAMETERS:      hNvDisplay(IN) - NVIDIA Display selection. It can be NVAPI_DEFAULT_HANDLE or a handle enumerated from NvAPI_EnumNVidiaDisplayHandle().
//                  outputId(IN)   - The display output id. If it's "0" then the default outputId from NvAPI_GetAssociatedDisplayOutputId() will be used.
//                  pInfo(OUT)     - The display port info
//
// RETURN STATUS: 
//                  NVAPI_OK - completed request
//                  NVAPI_ERROR - miscellaneous error occurred
//                  NVAPI_INVALID_ARGUMENT: Invalid input parameter.
//
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    NV_DP_1_62GBPS            = 6,
    NV_DP_2_70GBPS            = 0xA,
} NV_DP_LINK_RATE;

typedef enum
{
    NV_DP_1_LANE              = 1,
    NV_DP_2_LANE              = 2,
    NV_DP_4_LANE              = 4,
} NV_DP_LANE_COUNT;

typedef enum
{
    NV_DP_COLOR_SPACE_RGB     = 0,
    NV_DP_COLOR_SPACE_YUV601,
    NV_DP_COLOR_SPACE_YUV709,
} NV_DP_COLOR_SPACE;

typedef enum
{
    NV_DP_DYNAMIC_RANGE_VESA  = 0,
    NV_DP_DYNAMIC_RANGE_CEA,
} NV_DP_DYNAMIC_RANGE;

typedef enum
{
    NV_DP_BPP_DEFAULT         = 0,
    NV_DP_BPP_16_422,
    NV_DP_BPP_18_444,
    NV_DP_BPP_20_422,
    NV_DP_BPP_24_422,
    NV_DP_BPP_24_444,
    NV_DP_BPP_30_444,
    NV_DP_BPP_32_422,
    NV_DP_BPP_36_444,
    NV_DP_BPP_48_444,
} NV_DP_BPP;

typedef struct
{
    NvU32               version;                     // structure version
    NvU32               dpcd_ver;                    // the DPCD version of the monitor
    NV_DP_LINK_RATE     maxLinkRate;                 // the max supported link rate
    NV_DP_LANE_COUNT    maxLaneCount;                // the max supported lane count
    NV_DP_LINK_RATE     curLinkRate;                 // the current link rate
    NV_DP_LANE_COUNT    curLaneCount;                // the current lane count
    NV_DP_COLOR_SPACE   colorSpace;                  // the current color space
    NV_DP_DYNAMIC_RANGE dynamicRange;                // the dynamic range
    NV_DP_BPP           bpp;                         // the current bit-per-pixel;
    NvU32               isDp                   : 1;  // if the monitor is driven by display port 
    NvU32               isInternalDp           : 1;  // if the monitor is driven by NV Dp transmitter
 } NV_DISPLAY_PORT_INFO; 

#define NV_DISPLAY_PORT_INFO_VER  MAKE_NVAPI_VERSION(NV_DISPLAY_PORT_INFO,1)

NVAPI_INTERFACE NvAPI_GetDisplayPortInfo(NvDisplayHandle hNvDisplay, NvU32 outputId, NV_DISPLAY_PORT_INFO *pInfo);


///////////////////////////////////////////////////////////////////////////////
// FUNCTION NAME:   NvAPI_SetDisplayPort
//
// DESCRIPTION:     This API is used to setup DP related configurations.
//
// PARAMETERS:      hNvDisplay(IN) - NVIDIA display handle. It can be NVAPI_DEFAULT_HANDLE or a handle enumerated from NvAPI_EnumNVidiaDisplayHandle().
//                  outputId(IN)   - This display output ID, when it's "0" it means the default outputId generated from the return of NvAPI_GetAssociatedDisplayOutputId().
//                  pCfg(IN)       - The display port config structure. If pCfg is NULL, it means to use the driver's default value to setup.
//
// RETURN STATUS: 
//                  NVAPI_OK - completed request
//                  NVAPI_ERROR - miscellaneous error occurred
//                  NVAPI_INVALID_ARGUMENT: Invalid input parameter.
///////////////////////////////////////////////////////////////////////////////
typedef struct
{
    NvU32               version;                     // structure version
    NV_DP_LINK_RATE     linkRate;                    // the link rate
    NV_DP_LANE_COUNT    laneCount;                   // the lane count
    NV_DP_COLOR_SPACE   colorSpace;                  // the color space to set
    NV_DP_DYNAMIC_RANGE dynamicRange;                // the dynamic range
    NV_DP_BPP           bpp;                         // bit-per-pixel;
} NV_DISPLAY_PORT_CONFIG;

#define NV_DISPLAY_PORT_CONFIG_VER  MAKE_NVAPI_VERSION(NV_DISPLAY_PORT_CONFIG,1)

NVAPI_INTERFACE NvAPI_SetDisplayPort(NvDisplayHandle hNvDisplay, NvU32 outputId, NV_DISPLAY_PORT_CONFIG *pCfg);


///////////////////////////////////////////////////////////////////////////////
// FUNCTION NAME:   NvAPI_GetHDMISupportInfo
//
// DESCRIPTION:     This API returns the current infoframe data on the specified device(monitor)
//
// PARAMETERS:      hNvDisplay(IN) - NVIDIA Display selection. It can be NVAPI_DEFAULT_HANDLE or a handle enumerated from NvAPI_EnumNVidiaDisplayHandle().
//                  outputId(IN)   - The display output id. If it's "0" then the default outputId from NvAPI_GetAssociatedDisplayOutputId() will be used.
//                  pInfo(OUT)     - The monitor and GPU's HDMI support info
//
// RETURN STATUS: 
//                  NVAPI_OK - completed request
//                  NVAPI_ERROR - miscellaneous error occurred
//                  NVAPI_INVALID_ARGUMENT: Invalid input parameter.
//
///////////////////////////////////////////////////////////////////////////////
typedef struct
{
    NvU32      version;                     // structure version
    NvU32      isGpuHDMICapable       : 1;  // if the GPU can handle HDMI
    NvU32      isMonUnderscanCapable  : 1;  // if the monitor supports underscan
    NvU32      isMonBasicAudioCapable : 1;  // if the monitor supports basic audio
    NvU32      isMonYCbCr444Capable   : 1;  // if YCbCr 4:4:4 is supported
    NvU32      isMonYCbCr422Capable   : 1;  // if YCbCr 4:2:2 is supported
    NvU32      isMonHDMI              : 1;  // if the monitor is HDMI (with IEEE's HDMI registry ID)
    NvU32      EDID861ExtRev;               // the revision number of the EDID 861 extension
 } NV_HDMI_SUPPORT_INFO; 

#define NV_HDMI_SUPPORT_INFO_VER  MAKE_NVAPI_VERSION(NV_HDMI_SUPPORT_INFO,1)

NVAPI_INTERFACE NvAPI_GetHDMISupportInfo(NvDisplayHandle hNvDisplay, NvU32 outputId, NV_HDMI_SUPPORT_INFO *pInfo);

///////////////////////////////////////////////////////////////////////////////
// FUNCTION NAME:   NvAPI_GetInfoFrame
//
// DESCRIPTION:     This API returns the current infoframe data on the specified device(monitor)
//
// PARAMETERS:      hNvDisplay(IN) - NVIDIA Display selection. It can be NVAPI_DEFAULT_HANDLE or a handle enumerated from NvAPI_EnumNVidiaDisplayHandle().
//                  outputId(IN)   - The display output id. If it's "0" then the default outputId from NvAPI_GetAssociatedDisplayOutputId() will be used.
//                  type(IN)       - The type of infoframe to set
//                  pInfoFrame(IN) - The infoframe data
//
// RETURN STATUS: 
//                  NVAPI_OK - completed request
//                  NVAPI_ERROR - miscellaneous error occurred
//                  NVAPI_INVALID_ARGUMENT: Invalid input parameter.
//
///////////////////////////////////////////////////////////////////////////////
typedef enum _NV_INFOFRAME_TYPE
{
    NV_INFOFRAME_TYPE_AVI   = 2,
    NV_INFOFRAME_TYPE_SPD   = 3,
    NV_INFOFRAME_TYPE_AUDIO = 4,
    NV_INFOFRAME_TYPE_MS    = 5,
} NV_INFOFRAME_TYPE;

typedef struct
{
    NvU8 type;
    NvU8 version;
    NvU8 length;
} NV_INFOFRAME_HEADER;

// since this is for Windows OS so far, we use this bit little endian defination
// to handle the translation
typedef struct
{
    // byte 1
    NvU8 channelCount     : 3;
    NvU8 rsvd_bits_byte1  : 1;
    NvU8 codingType       : 4;

    // byte 2
    NvU8 sampleSize       : 2;
    NvU8 sampleRate       : 3;
    NvU8 rsvd_bits_byte2  : 3;

    // byte 3
    NvU8  byte3;

    // byte 4
    NvU8  speakerPlacement;

    // byte 5
    NvU8 rsvd_bits_byte5  : 3;
    NvU8 levelShift       : 4;
    NvU8 downmixInhibit   : 1;

    // byte 6~10
    NvU8 rsvd_byte6;
    NvU8 rsvd_byte7;
    NvU8 rsvd_byte8;
    NvU8 rsvd_byte9;
    NvU8 rsvd_byte10;

}NV_AUDIO_INFOFRAME;

typedef struct
{
    // byte 1
    NvU8 scanInfo                : 2;
    NvU8 barInfo                 : 2;
    NvU8 activeFormatInfoPresent : 1;
    NvU8 colorSpace              : 2;
    NvU8 rsvd_bits_byte1         : 1;

    // byte 2
    NvU8 activeFormatAspectRatio : 4;
    NvU8 picAspectRatio          : 2;
    NvU8 colorimetry             : 2;

    // byte 3
    NvU8 nonuniformScaling       : 2;
    NvU8 rsvd_bits_byte3         : 6;

    // byte 4
    NvU8 vic                     : 7;
    NvU8 rsvd_bits_byte4         : 1;

    // byte 5
    NvU8 pixelRepeat             : 4;
    NvU8 rsvd_bits_byte5         : 4;

    // byte 6~13 
    NvU8 topBarLow;
    NvU8 topBarHigh;
    NvU8 bottomBarLow;
    NvU8 bottomBarHigh;
    NvU8 leftBarLow;
    NvU8 leftBarHigh;
    NvU8 rightBarLow;
    NvU8 rightBarHigh;

} NV_VIDEO_INFOFRAME;

typedef struct
{
    NV_INFOFRAME_HEADER    header;
    union
    {
        NV_AUDIO_INFOFRAME audio;
        NV_VIDEO_INFOFRAME video;
    }u;
} NV_INFOFRAME;
NVAPI_INTERFACE NvAPI_GetInfoFrame(NvDisplayHandle hNvDisplay, NvU32 outputId, NV_INFOFRAME_TYPE type, NV_INFOFRAME *pInfoFrame);

///////////////////////////////////////////////////////////////////////////////
// FUNCTION NAME:   NvAPI_SetInfoFrame
//
// DESCRIPTION:     This API returns the current infoframe data on the specified device(monitor)
//
// PARAMETERS:      hNvDisplay(IN) - NVIDIA Display selection. It can be NVAPI_DEFAULT_HANDLE or a handle enumerated from NvAPI_EnumNVidiaDisplayHandle().
//                  outputId(IN)   - The display output id. If it's "0" then the default outputId from NvAPI_GetAssociatedDisplayOutputId() will be used.
//                  type(IN)       - The type of infoframe to set
//                  pInfoFrame(IN) - The infoframe data, NULL mean reset to the default value.
//
// RETURN STATUS: 
//                  NVAPI_OK - completed request
//                  NVAPI_ERROR - miscellaneous error occurred
//                  NVAPI_INVALID_ARGUMENT: Invalid input parameter.
//
///////////////////////////////////////////////////////////////////////////////

NVAPI_INTERFACE NvAPI_SetInfoFrame(NvDisplayHandle hNvDisplay, NvU32 outputId, NV_INFOFRAME_TYPE type, NV_INFOFRAME *pInfoFrame);


typedef struct
{
    NvU32   version;            //IN version info  
    NvU32   currentLevel;       //OUT current DVC level
    NvU32   minLevel;           //OUT min range level
    NvU32   maxLevel;           //OUT max range level
} NV_DISPLAY_DVC_INFO;

#define NV_DISPLAY_DVC_INFO_VER  MAKE_NVAPI_VERSION(NV_DISPLAY_DVC_INFO,1)

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GetDVCInfo
//
//   DESCRIPTION: Retrieves the Digital Vibrance Control(DVC) information of the selected display.
//
//    PARAMETERS: hNvDisplay(IN) - NVIDIA Display selection. It can be NVAPI_DEFAULT_HANDLE or a handle enumerated from NvAPI_EnumNVidiaDisplayHandle().
//                outputId(IN)   - One of the selected outputId retrieved from NvAPI_GPU_GetActiveOutputs or NvAPI_GetView, to identify the target 
//                                 output in case multiple targets are associated with the selected hNvDisplay.
//                                 Can be NULL to pick the display output id associated with hNvDisplay.
//                pDVCInfo(OUT)  - The returned DVC information.
//
// RETURN STATUS: NVAPI_OK: call successful.
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found.
//                NVAPI_EXPECTED_DISPLAY_HANDLE: hNvDisplay is not a valid display handle.
//                NVAPI_INVALID_ARGUMENT: pDVCInfo is NULL.
//                NVAPI_INCOMPATIBLE_STRUCT_VERSION - the version of the NV_DISPLAY_DVC_INFO struct is not supported
//                NVAPI_NOT_SUPPORTED - DVC feature is not supported on the selected GPU
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GetDVCInfo(NvDisplayHandle hNvDisplay, NvU32 outputId, NV_DISPLAY_DVC_INFO *pDVCInfo);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_SetDVCLevel
//
//   DESCRIPTION: Sets the DVC level for the selected display.
//
//    PARAMETERS: hNvDisplay(IN) - NVIDIA Display selection. It can be NVAPI_DEFAULT_HANDLE or a handle enumerated from NvAPI_EnumNVidiaDisplayHandle().
//                outputId(IN)   - One of the selected outputId retrieved from NvAPI_GPU_GetActiveOutputs or NvAPI_GetView, to identify the target 
//                                 output in case multiple targets are associated with the selected hNvDisplay.
//                                 Can be NULL to pick the display output id associated with hNvDisplay.
//                level(IN)      - The new level to apply. Value should be within the range of min and max.
//
// RETURN STATUS: NVAPI_OK: call successful.
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found.
//                NVAPI_EXPECTED_DISPLAY_HANDLE: hNvDisplay is not a valid display handle.
//                NVAPI_NOT_SUPPORTED - DVC is not supported on the selected GPU
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_SetDVCLevel(NvDisplayHandle hNvDisplay, NvU32 outputId, NvU32 level);

typedef struct
{
    NvU32   version;            //IN version info
    NvU32   currentLevel;       //OUT current Image Sharpening level
    NvU32   minLevel;           //OUT min range level
    NvU32   maxLevel;           //OUT max range level
} NV_DISPLAY_IMAGE_SHARPENING_INFO;

#define NV_DISPLAY_IMAGE_SHARPENING_INFO_VER  MAKE_NVAPI_VERSION(NV_DISPLAY_IMAGE_SHARPENING_INFO,1)
///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GetImageSharpeningInfo
//
//   DESCRIPTION: Retrieves the Image Sharpening information of the selected display.
//
//    PARAMETERS: hNvDisplay(IN) - NVIDIA Display selection. It can be NVAPI_DEFAULT_HANDLE or a handle enumerated from NvAPI_EnumNVidiaDisplayHandle().
//                outputId(IN)   - One of the selected outputId retrieved from NvAPI_GPU_GetActiveOutputs or NvAPI_GetView, to identify the target 
//                                 output in case multiple targets are associated with the selected hNvDisplay.
//                                 Can be NULL to pick the display output id associated with hNvDisplay.
//                pImageSharpeningInfo(OUT)  - The returned Image Sharpening information.
//
// RETURN STATUS: NVAPI_OK: call successful.
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found.
//                NVAPI_EXPECTED_DISPLAY_HANDLE: hNvDisplay is not a valid display handle.
//                NVAPI_INVALID_ARGUMENT: pImageSharpeningInfo is NULL.
//                NVAPI_INCOMPATIBLE_STRUCT_VERSION - the version of the NV_DISPLAY_IMAGE_SHARPENING_INFO struct is not supported
//                NVAPI_NOT_SUPPORTED - Image Sharpening is not supported on the selected GPU
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GetImageSharpeningInfo(NvDisplayHandle hNvDisplay, NvU32 outputId, NV_DISPLAY_IMAGE_SHARPENING_INFO *pImageSharpeningInfo);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_SetImageSharpeningLevel
//
//   DESCRIPTION: Sets the Image Sharpening level for the selected display.
//
//    PARAMETERS: hNvDisplay(IN) - NVIDIA Display selection. It can be NVAPI_DEFAULT_HANDLE or a handle enumerated from NvAPI_EnumNVidiaDisplayHandle().
//                outputId(IN)   - One of the selected outputId retrieved from NvAPI_GPU_GetActiveOutputs or NvAPI_GetView, to identify the target 
//                                 output in case multiple targets are associated with the selected hNvDisplay.
//                                 Can be NULL to pick the display output id associated with hNvDisplay.
//                level(IN)      - The new level to apply. Value should be within the range of min and max.
//
// RETURN STATUS: NVAPI_OK: call successful.
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found.
//                NVAPI_EXPECTED_DISPLAY_HANDLE: hNvDisplay is not a valid display handle.
//                NVAPI_NOT_SUPPORTED - Image Sharpening is not supported on the selected GPU
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_SetImageSharpeningLevel(NvDisplayHandle hNvDisplay, NvU32 outputId, NvU32 level);



//-----------------------------------------------------------------------------
// DirectX APIs
//-----------------------------------------------------------------------------
DECLARE_HANDLE(NVDX_ObjectHandle);  // DX Objects
static const NVDX_ObjectHandle NVDX_OBJECT_NONE = 0;



//-----------------------------------------------------------------------------
// Direct3D9 APIs
//-----------------------------------------------------------------------------

#if defined(_D3D9_H_) && defined(__cplusplus)

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_GetSurfaceHandle
//
//   DESCRIPTION: Get the handle of a given surface. This handle uniquely 
//                identify the surface through all NvAPI entries.
//
//         INPUT: pSurface - Surface to be identified
//
//        OUTPUT: pHandle - Will be filled by the return handle
//
//          DEMO: PrimTexture
//
// RETURN STATUS: an int which could be an NvAPI status or DX HRESULT code
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_D3D9_GetSurfaceHandle(IDirect3DSurface9 *pSurface, 
                                        NVDX_ObjectHandle *pHandle);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_GetTextureHandle
//
//   DESCRIPTION: Get the handle of a given DX9 texture.
//
//         INPUT: pTexture - Surface to be identified
//
//        OUTPUT: pHandle - Will be filled by the return handle
//
// RETURN STATUS: an int which could be an NvAPI status or DX HRESULT code
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_D3D9_GetTextureHandle(IDirect3DTexture9 *pTexture, 
                                        NVDX_ObjectHandle *pHandle);


///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_GpuSyncGetHandleSize
//
//   DESCRIPTION: returns size of the init and copy sync handles. These handles are
//                then allocated and initialized to zero by the application
//
//        OUTPUT: pInitHandleSize - size of GpuSync init handle
//                pMapHandleSize - size of GpuSync copy handle
//
// RETURN STATUS: an int which could be an NvAPI status or DX HRESULT code
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_D3D9_GpuSyncGetHandleSize(IDirect3DDevice9 *pDev, 
                                            unsigned int *pInitHandleSize,
                                            unsigned int *pMapHandleSize);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_GpuSyncInit
//
//   DESCRIPTION: setup sync functionality
//
// RETURN STATUS: an int which could be an NvAPI status or DX HRESULT code
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_D3D9_GpuSyncInit(IDirect3DDevice9 *pDev, 
                                            void * syncInitData);


///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_GpuSyncEnd
//
//   DESCRIPTION: tear down sync structures
//
// RETURN STATUS: an int which could be an NvAPI status or DX HRESULT code
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_D3D9_GpuSyncEnd(IDirect3DDevice9 *pDev, 
                                            void * syncData);


///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_GpuSyncMapTexBuffer
//
//   DESCRIPTION: map a texture to receive ogl data
//
// RETURN STATUS: an int which could be an NvAPI status or DX HRESULT code
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_D3D9_GpuSyncMapTexBuffer(IDirect3DDevice9 *pDev, 
                                            IDirect3DTexture9 *pTexture,
                                            void * syncData);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_GpuSyncMapVertexBuffer
//
//   DESCRIPTION: map a vertex buffer to recieve ogl data
//
// RETURN STATUS: an int which could be an NvAPI status or DX HRESULT code
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_D3D9_GpuSyncMapVertexBuffer(IDirect3DDevice9 *pDev, 
                                            IDirect3DVertexBuffer9 *pVertexBuffer,
                                            void * syncData);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_GpuSyncAcquire
//
//   DESCRIPTION: semaphore acquire for synchronization control of mapped buffer
//
//         INPUT: accessMode - acquire mapped buffer read/write access
//
// RETURN STATUS: an int which could be an NvAPI status or DX HRESULT code
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_D3D9_GpuSyncAcquire(IDirect3DDevice9 *pDev, 
                                            void * syncData);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_GpuSyncRelease
//
//   DESCRIPTION: semaphore release for synchronization control of mapped buffer
//
//         INPUT: accessMode - release mapped buffer read/write access
//
// RETURN STATUS: an int which could be an NvAPI status or DX HRESULT code
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_D3D9_GpuSyncRelease(IDirect3DDevice9 *pDev, 
                                            void * syncData);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_GetCurrentRenderTargetHandle
//
//   DESCRIPTION: Get the handle of current RT/ZB.
//
//         INPUT: pDev - Device whose current ZB to be identified
//
//        OUTPUT: pHandle - Will be filled by the return handle
//
// RETURN STATUS: an int which could be an NvAPI status or DX HRESULT code
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_D3D9_GetCurrentRenderTargetHandle(IDirect3DDevice9 *pDev,
                                            NVDX_ObjectHandle *pHandle);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_GetCurrentZBufferHandle
//
//   DESCRIPTION: 
//
// RETURN STATUS: an int which could be an NvAPI status or DX HRESULT code
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_D3D9_GetCurrentZBufferHandle(IDirect3DDevice9 *pDev,
                                               NVDX_ObjectHandle *pHandle);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_GetIndexBufferHandle
//
//   DESCRIPTION: Get the handle of a given DX9 index buffer.
//
//         INPUT: pIndexBuffer - Index Buffer to be identified
//
//        OUTPUT: pHandle - Will be filled by the return handle
//
// RETURN STATUS: an int which could be an NvAPI status or DX HRESULT code
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_D3D9_GetIndexBufferHandle(IDirect3DIndexBuffer9 *pIndexBuffer, 
                                        NVDX_ObjectHandle *pHandle);


///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_GetVertexBufferHandle
//
//   DESCRIPTION: Get the handle of a given DX9 vertex buffer.
//
//         INPUT: pVertexBuffer - Vertex Buffer to be identified
//
//        OUTPUT: pHandle - Will be filled by the return handle
//
// RETURN STATUS: an int which could be an NvAPI status or DX HRESULT code
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_D3D9_GetVertexBufferHandle(IDirect3DVertexBuffer9 *pVertexBuffer, 
                                        NVDX_ObjectHandle *pHandle);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_AliasPrimaryAsTexture
//
//   DESCRIPTION: Create an texture that is an alias of current device's primary surface
//
//         INPUT:  pDev       The device to get primary surface from
//                 dwIndex    The index to the primary flipchain of device (usually 0)
// 
//        OUTPUT:  ppTexture  Fill with the texture created
//                 pHandle    If non-NULL, fill with the NVDX handle of the created texture
//
//          DEMO:  PrimTexture
//
// RETURN STATUS: an int which could be an NvAPI status or DX HRESULT code
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_D3D9_AliasPrimaryAsTexture(IDirect3DDevice9 *pDev, 
                                            NvU32 dwIndex,
                                            IDirect3DTexture9 **ppTexture,
                                            NVDX_ObjectHandle *pHandle = 0);


///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_PresentSurfaceToDesktop
//
//   DESCRIPTION: Present a given surface to the desktop. This interface can be
//                used to start a fullscreen flipping mode even within windowed D3D app.
//
//         INPUT: pDev              The device (display) to present to surfaceHandle   The surface
//                                  handle obtained from NVD3D9_GetSurfaceHandle
//                                  NOTE: NVDX_OBJECT_NONE means restore
//                dwFlipFlags       Flags to indicate SYNC mode (other bits reserved and must be 0)
//                dwExcludeDevices  This is a bitmask (usually 0) to indicate which device
//                                  will be EXCLUDED from this present. This is only
//                                  effective when used in a clone mode configuration where
//                                  the application wants one monitor to show the specially
//                                  rendered screen and the other normal desktop.
//
//          NOTE: It is applications responsibility to determine which devices are
//                available on the current clone config, through nvcpl interfaces.
//
//          DEMO: PrimTexture
//
// RETURN STATUS: an int which could be an NvAPI status or DX HRESULT code
//
///////////////////////////////////////////////////////////////////////////////

// Parameter used for dwFlipFlags (All other bits reserved)
#define NV_FLIPFLAG_VSYNC               0x00000001  // SYNCMODE         (bit 0:1) - 0:NOSYNC, 1:VSYNC, 2:HSYNC
#define NV_FLIPFLAG_HSYNC               0x00000002
#define NV_FLIPFLAG_TRIPLEBUFFERING     0x00000004  // TRIPLEBUFFERING  (bit 2)   - 0: DoubleBuffer, 1:TripleBuffer or more

NVAPI_INTERFACE NvAPI_D3D9_PresentSurfaceToDesktop(IDirect3DDevice9 *pDev, 
                                               NVDX_ObjectHandle surfaceHandle, 
                                               NvU32 dwFlipFlags,
                                               NvU32 dwExcludeDevices = 0);

///////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION_NAME: NvAPI_D3D9_CreateVideoBegin
//
//   DESCRIPTION: Signals the driver that the application next will create a set of 
//                D3DFMT_X8R8G8B8 render targets for overlay use. The call will fail 
//                if the driver/hardware doesn't support this mode, in which case the 
//                application should fall back to the traditional overlay (with driver 
//                internal overlay buffers). If this call returns successfully, then 
//                before the driver sees _CreateVideoEnd, all D3DFMT_A8R8G8B8 render 
//                targets will be allocated as overlay surfaces. 
//                See _CreateVideo, NV_CVFLAG_EXTERNAL_OVERLAY flag for more details.
//                This interface is only available on Windows Vista.
//
//         INPUT: pDev              The device (display) to present to
//
// RETURN STATUS: an int which could be an NvAPI status or DX HRESULT code
//
///////////////////////////////////////////////////////////////////////////////

NVAPI_INTERFACE NvAPI_D3D9_CreateVideoBegin(IDirect3DDevice9 *pDev);

///////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION_NAME: NvAPI_D3D9_CreateVideoEnd
//
//   DESCRIPTION: Signals the driver that the application has finished creating 
//                D3DFMT_X8R8G8B8 render targets for overlay.
//                See _CreateVideo, NV_CVFLAG_EXTERNAL_OVERLAY flag for more details.
//                This interface is only available on Windows Vista.
//
//         INPUT: pDev              The device (display) to present to
//
// RETURN STATUS: an int which could be an NvAPI status or DX HRESULT code
//
///////////////////////////////////////////////////////////////////////////////

NVAPI_INTERFACE NvAPI_D3D9_CreateVideoEnd(IDirect3DDevice9 *pDev);

///////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION_NAME: NvAPI_D3D9_CreateVideo
//
//   DESCRIPTION: Allocates and initializes video resources for playback within a D3D9 context
//
//         INPUT: pDev              The device (display) to present to
//                dwVersion         Version of the CreateVideoParams structure
//                dwCVFlags         Initialization flags (see defines for description)
//                dwFlipQueueHint   If overlay is allocated, indication of the desired number of flips
//                                  to be queued (minimum of 1).  A larger number is better for performance,
//                                  but also consumes more frame buffer resources.  This number should be
//                                  set according to the number of fields/frames that can be decoded in
//                                  advance of presentation.  Eg., if the decoder cannot decode ahead and
//                                  must display immediately after decoding a frame, then set this to 1.
//                                  If the decoder is decoding field content one frame at a time, then it
//                                  is decoding one field ahead and should set this to 2.  If the decoder
//                                  can decode n flips ahead, then set this to n+1.  If the GPU has
//                                  constrained frame buffer resources, then set this number lower, and also
//                                  reduce the decode-ahead pipeline resources accordingly.
//                dwMaxSrcWidth     Maximum video source width
//                dwMaxSrcHeight    Maximum video source height
//                dwNumOvlSurfs     (VER2 only) Number of surfaces that the application intends to use for  
//                                  overlay (also refer to the NV_CVFLAG_EXTERNAL_OVERLAY flag)
//                hOvlSurfs         (VER2 only) Handles to the surfaces that the application intends to use
//                                  for overlay (also refer to the NV_CVFLAG_EXTERNAL_OVERLAY flag)
//
// RETURN STATUS: an int which could be an NvAPI status or DX HRESULT code
//
///////////////////////////////////////////////////////////////////////////////

#define NV_CVFLAG_OVERLAY                   0x00000001      // Overlay will be used
#define NV_CVFLAG_OVERLAY_REINIT            0x00000002      // Change source size or flip queue hint once overlay
                                                            // resources have already been allocated.  Be wary
                                                            // of upward allocating new resources, if there is
                                                            // a failure, be prepared to deallocate everything
                                                            // and start from scratch.
#define NV_CVFLAG_EXTERNAL_OVERLAY          0x00000004      // Use overlay surfaces allocated by the application. This flag
                                                            // is supported only in NV_DX_CREATE_VIDEO_PARAMS_VER2 and above 
                                                            // on Windows Vista.
                                                            //
                                                            // The application sets this flag to indicate that the driver should
                                                            // not allocate any internal overlay buffers, and the first
                                                            // dwNumOvlSurfs surfaces in pCVParams->hOvlSurfs[] will be used for
                                                            // overlay scan-out directly. The driver will validate these surfaces
                                                            // against the following conditions, and fail the call otherwise:
                                                            //
                                                            // 1. No less than NV_CV_MIN_OVERLAY_SURFACE_NUMBER and no more than
                                                            //    NV_CV_MAX_OVERLAY_SURFACE_NUMBER should be passed in
                                                            // 2. Only surface handles of D3DFMT_A8R8G8B8 render targets created
                                                            //    inside a pair of _CreateVideoBegin ()/_CreateVideoEnd() should be
                                                            //    used
                                                            // 3. The size of the surfaces must be identical to the target display
                                                            //
                                                            // If all of the above condictions are met for each of the surfaces,
                                                            // then the driver will not allocate any internal overlay buffers. Upon
                                                            // _PresentVideo(), the source surface will be used for overlay scan-
                                                            // out directly without the driver copying the content to an overlay
                                                            // buffer first.
#define NV_CVFLAG_2ND_FORCE_DISABLE         0x00010000      // Forces full screen video on secondary device to be
                                                            // disabled in multihead modes, regardless of user setting
#define NV_CVFLAG_2ND_FORCE_ENABLE          0x00020000      // Forces full screen video on secondary device to be
                                                            // enabled on the default device in multihead modes, 
                                                            // regardless of user setting
#define NV_CVFLAG_2ND_FORCE_ENABLE1         0x00040000      // Forces full screen video on secondary device to be
                                                            // enabled on the primary device in clone mode, 
                                                            // regardless of user setting
#define NV_CVFLAG_2ND_COMMANDEER            0x00100000      // If another application owns the secondary full screen
                                                            // device, forcibly take possession of it.
#define NV_CVFLAG_SECONDARY_DISPLAY         0x01000000      // Is the target display secondary display when in Dual-view mode

typedef struct
{
    NvU32 version;
    NvU32 cvFlags;
    NvU32 flipQueueHint;
    NvU32 maxSrcWidth;
    NvU32 maxSrcHeight;
} NV_DX_CREATE_VIDEO_PARAMS1;

#define NV_CV_MIN_OVERLAY_SURFACE_NUMBER    2
#define NV_CV_MAX_OVERLAY_SURFACE_NUMBER    6
typedef struct
{
    NvU32 version;
    NvU32 cvFlags;
    NvU32 flipQueueHint;
    NvU32 maxSrcWidth;
    NvU32 maxSrcHeight;
    NvU32 dwNumOvlSurfs;
    NVDX_ObjectHandle hOvlSurfs[NV_CV_MAX_OVERLAY_SURFACE_NUMBER];
} NV_DX_CREATE_VIDEO_PARAMS2;

typedef NV_DX_CREATE_VIDEO_PARAMS2  NV_DX_CREATE_VIDEO_PARAMS;

#define NV_DX_CREATE_VIDEO_PARAMS_VER1  MAKE_NVAPI_VERSION(NV_DX_CREATE_VIDEO_PARAMS1,1)

#define NV_DX_CREATE_VIDEO_PARAMS_VER2  MAKE_NVAPI_VERSION(NV_DX_CREATE_VIDEO_PARAMS2,2)

#define NV_DX_CREATE_VIDEO_PARAMS_VER   NV_DX_CREATE_VIDEO_PARAMS_VER2

NVAPI_INTERFACE NvAPI_D3D9_CreateVideo(IDirect3DDevice9 *pDev, 
                                       NV_DX_CREATE_VIDEO_PARAMS *pCVParams);

///////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION_NAME: NvAPI_D3D9_FreeVideo
//
//   DESCRIPTION: Releases all video resources
//
//         INPUT: pDev              The device (display) to present to
//
// RETURN STATUS: an int which could be an NvAPI status or DX HRESULT code
//
///////////////////////////////////////////////////////////////////////////////

NVAPI_INTERFACE NvAPI_D3D9_FreeVideo(IDirect3DDevice9 *pDev);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_PresentVideo
//
//   DESCRIPTION: Signals a final and complete frame ready for presentation.
//                Can optionally render to the overlay, but should be called
//                regardless of whether any actual rendering occurs.  If the
//                user has enabled full screen video in a multihead mode,
//                this frame will also be rendered on the secondary device.
//
//          NOTE: Use NV_DX_PRESENT_VIDEO_PARAMS_VER in the "version" method for both 32 bit and 64 bit
//                callers.  For older drivers that do not support 64 bit callers, 32 bit callers
//                should use the evaluation of MAKE_NVAPI_VERSION(NV_DX_PRESENT_VIDEO_PARAMS1,1)
//                in that field.
//
//         INPUT: pDev              The device (display) to present to
//                dwVersion         Version of the PresentVideoParams structure
//                surfaceHandle     The surface handle obtained from NvAPI_D3D9_GetSurfaceHandle
//                                  or NvAPI_D3D9_GetCurrentRenderTargetHandle
//                dwPVFlags         Presentation flags (see defines for description)
//                dwColourKey       Colour key to use if NV_PVFLAG_DST_KEY is set
//                qwTimeStamp*      If NV_PVFLAG_USE_STAMP is set, time in ns when the frame is to be presented
//                                  If NV_PVFLAG_SET_STAMP is set, set the current time to this, and present on next vblank
//                dwFlipRate        Set to the current flip rate
//                                  Set to zero if the frame to be presented is a still frame
//                srcUnclipped      Unclipped source rectangle of the entire frame of data
//                srcClipped        Cropped source rectangle.  It is the caller's responsibility to crop
//                                  the source if the desktop crops the destination.
//                dst               Destination rectangle (in desktop coordinates) of the overlay.  It is the
//                                  caller's responsibility to crop the destination against the desktop.
//
// RETURN STATUS: an int which could be an NvAPI status or DX HRESULT code. 
//
//                NVAPI_DEVICE_BUSY - This return value indicates the flip request was dropped 
//                because the hardware flip queue was too deep, thus no more present requests can be 
//                accepted at the moment. 
//
//                When external overlay is used (for "external overlay" please refer to NvAPI_D3D9_CreateVideoBegin,  
//                NvAPI_D3D9_CreateVideoEnd, and VER2-only descriptions of NvAPI_D3D9_CreateVideo), 
//                a failure in _PresentVideo means the current front (on-screen) buffer will remain the 
//                unchanged. In this case the application should be careful not to render to the current 
//                front buffer, as it will cause video tearing. The application could re-try presenting 
//                the same frame during the next vsync, or continue with rendering the next frame to a 
//                back buffer.
//
///////////////////////////////////////////////////////////////////////////////

// PresentVideo flags
#define NV_PVFLAG_ODD           0x00000001      // Field is odd
#define NV_PVFLAG_EVEN          0x00000002      // Field is even
#define NV_PVFLAG_PROTECTED     0x00000004      // Indicates that this frame is protected and guarantees full 
                                                // screen video will not display this frame on any secondary device.
                                                // Conversely, not setting this indicates an unprotected frame
#define NV_PVFLAG_PROGRESSIVE   0x00000008      // Indicates progressive frame.  If the odd or even flags are set
                                                // in conjunction with this, it indicates the original field that
                                                // generated this deinterlaced frame, and attempts to synchronize
                                                // this presentation to the corresponding display field of an
                                                // interlaced display
#define NV_PVFLAG_SHOW          0x00000010      // Show the overlay
                                                // If the app is minimized or obscured, continue to call NvAPI_D3D9_PresentVideo
                                                // for every complete frame without this flag set.
                                                // If enabled, unprotected video will continue to play full screen
                                                // on the secondary device, using the pixel aspect cached from
                                                // the last time a frame was shown.  To change the pixel aspect while hidden,
                                                // the caller must "show" a frame at least once with a new clipped source and 
                                                // destination rectangle.  This shown frame can be rendered invisible with
                                                // appropriate selection of colour key.
#define NV_PVFLAG_FAST_MOVE     0x00000020      // Move overlay position without waiting for vblank.
                                                // The only parameters used are dwDstX, dwDstY, and NV_PVFLAG_SHOW.
#define NV_PVFLAG_WAIT          0x00000040      // If set, blocking flip, wait until flip queue can accept another flip.
                                                // A non-blocking flip will return an error if flip cannot be queued yet.
#define NV_PVFLAG_REPEAT        0x00000080      // Video data is completely unchanged from the previous flip (used for telecine)
#define NV_PVFLAG_DST_KEY       0x00000100      // Use destination colour key.
#define NV_PVFLAG_FULLSCREEN    0x00000200      // Indicates that the overlay is playing fullscreen on the desktop.
                                                // This bit is used to automatically overscan the image on TV's.
#define NV_PVFLAG_SET_STAMP     0x00001000      // Set the current time.
#define NV_PVFLAG_USE_STAMP     0x00002000      // If set, use timestamps.
                                                // If not set, flip on the next vblank.

typedef struct 
{
    NvU32 version;
    NVDX_ObjectHandle surfaceHandle;
    NvU32 pvFlags;
    NvU32 colourKey;
    NvU32 timeStampLow;
    NvU32 timeStampHigh;
    NvU32 flipRate;
    NvSBox srcUnclipped;
    NvSBox srcClipped;
    NvSBox dst;
} NV_DX_PRESENT_VIDEO_PARAMS1;

typedef NV_DX_PRESENT_VIDEO_PARAMS1 NV_DX_PRESENT_VIDEO_PARAMS;

#define NV_DX_PRESENT_VIDEO_PARAMS_VER  MAKE_NVAPI_VERSION(NV_DX_PRESENT_VIDEO_PARAMS1,2)

NVAPI_INTERFACE NvAPI_D3D9_PresentVideo(IDirect3DDevice9 *pDev, 
                                        NV_DX_PRESENT_VIDEO_PARAMS *pPVParams);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_Video_GetVideoCapabilities
//
//   DESCRIPTION: Get the GPU's video processing capabilities. The caller is responsible for furnishing
//                the inputs within the NVAPI_VIDEO_CAPS_PACKET. This packet shall contain the output 
//                consisting of the supported video features for the current configuration. The number of 
//                NVAPI_VIDEO_CAPS will match the number of input video streams for which the capabilites
//                have been requested. Multiple supported video profiles may be returned that would include
//                a combination of various video features.
//                 
//         INPUT: pDev                          The device (display) to present to
//                version                       version of the NV_DX_VIDEO_CAPS structure
//                videoCapsPacket               NVAPI_VIDEO_CAPS_PACKET containing both the 
//                                              input Video Source information for which the capabilities
//                                              are requested, as well as the output available 
//                                              supported video features for the current configuration.
//
// RETURN STATUS: an int which could be an NvAPI status or DX HRESULT code
//
///////////////////////////////////////////////////////////////////////////////

#define NV_MAX_HEADS        4   // Maximum heads, each with NVAPI_DESKTOP_RES resolution
#define NV_MAX_VID_STREAMS  4   // Maximum input video streams, each with a NVAPI_VIDEO_SRC_INFO
#define NV_MAX_VID_PROFILES 4   // Maximum output video profiles supported

typedef enum _NV_CODEC
{
    NV_CODEC_TYPE_NONE,     
    NV_CODEC_TYPE_MPEG2, 
    NV_CODEC_TYPE_H264,
    NV_CODEC_TYPE_VC1,
} NV_CODEC;

// current video surface to be played back
typedef struct _NVAPI_VIDEO_SRC_INFO
{
    NvU32       srcWidth;   // Input video width 
    NvU32       srcHeight;  // Input video height
    NV_CODEC    codecType;  // One of the available NV_CODEC's
    NvU32       reserved1;  // reserved for future expansion
    NvU64       reserved2;
    NvU64       reserved3;
} NVAPI_VIDEO_SRC_INFO;

// current desktop resolution
typedef struct _NVAPI_DESKTOP_RES
{
    NvU32 width;            // current resolution width
    NvU32 height;           // current resolution height
    NvU32 bitsPerPixel;     // current resolution depth
    NvU32 refreshRate;      // current display refresh rate
    NvU64 reserved1;        // reserved for future expansion
    NvU64 reserved2;
} NVAPI_DESKTOP_RES;

#define NV_DEINTERLACE_PIXADAPTIVE_BIT      0   // HW Pixel adaptive deinterlacing available
#define NV_VID_ENHANCE_EDGE_ENHANCE_BIT     0   // Edge enhancement present
#define NV_VID_ENHANCE_NOISE_REDUCTION_BIT  1   // Noise reduction present 
#define NV_COLOR_CTRL_PROCAMP_BIT           0   // ProCamp is supported
#define NV_COLOR_CTRL_COLOR_TEMP_BIT        1   // Color temp control supported 
#define NV_GAMMA_Y_BIT                      0   // Y-Gamma controls present
#define NV_GAMMA_RGB_BIT                    1   // RGB-Gamma controls present
#define NV_MISC_CAPS_INV_TELECINE_BIT       0   // Inverse telecine is available

// available video post-process features
typedef struct _NVAPI_VIDEO_PROCESSING_CAPS 
{
    NvU64               deinterlaceMode;    // Possible deinterlace modes supported, "check NV_DEINTERLACE_xxx bits"
    NvU64               videoEnhance;       // Possible HW postproc enhancements e.g. NR, EE, "check NV_VID_ENHANCE_xxx bits"
    NvU64               colorControl;       // Color control is supported, "check NV_COLOR_xxx bits"
    NvU64               gamma;              // Available gamma conversions, "check NV_GAMMA_xxx bits"
    NvU64               miscCaps;           // Miscellaneous post processing caps supported, "check NV_MISC_CAPS_xxx bits"
    NvU64               reserved1;          // reserved for future use
    NvU64               reserved2;
    NvU64               reserved3;
} NVAPI_VIDEO_PROCESSING_CAPS;

#define NV_VID_FEATURE_NO_SYNC_FLIPS_BIT            0   // when this bit is set, APP needs to Lock the RGB render target
                                                        // before calling RGB overlay to present video.
                                                        // In general this bit is set for G7x, and not set for G8x and later chips
#define NV_VID_FEATURE_HALF_RES_ON_INTERLACED_BIT   1   // When this bit is set, it means that GPU can only show half resolution
                                                        // video on interlaced display, which is true for G7x. Needs special
                                                        // workaround by app to show full-resolution video.
#define NV_PERF_LEVEL_RED_BIT                       0   // when this bit is set, it means the GPU can NOT support HD/BD playback
#define NV_PERF_LEVEL_YELLOW_BIT                    1   // when this bit is set, it means the GPU meets the minimum requirement for HD/BD playback
#define NV_PERF_LEVEL_GREEN_BIT                     2   // when this bit is set, it means the GPU can support HD/BD playback without problem

// available video decode and post-process features
typedef struct _NVAPI_VIDEO_CAPS
{
    NvU32                       maxFlipRate;    // Maximum flipping rate supported 
    NV_CODEC                    hwDecode;       // Supported codec for HW decoding
    NvU64                       vidFeature;     // Indicates presence of special video processing features
    NvU32                       perfLevel;      // GPU dependent level; can be red, yellow, or green
    NvU32                       numVidProfiles; // Number of returned video profiles              
    NvU32                       maxResPixels;   // Number of pixels in the max screen resolution supported at maxFlipRate for the input codecType
                                                // (refer to codecType field of the NVAPI_VIDEO_SRC_INFO structure)
    NvU32                       reserved1;
    NVAPI_VIDEO_PROCESSING_CAPS vidProcCaps[NV_MAX_VID_PROFILES];
} NVAPI_VIDEO_CAPS;

#define NVAPI_VIDEO_CAPS_PACKET_VER 1
#define RENDER_MODE_DWM_BIT         0
#define RENDER_MODE_OVERLAY_BIT     1   

typedef struct _NVAPI_VIDEO_CAPS_PACKET
{              
    NvU32                   packetVer;                      // (IN) Packet version that needs to match NVAPI_VIDEO_CAPS_PACKET_VER
    NvU32                   numVidStreams;                  // (IN) Number of streams for which the video caps are requested
    NVAPI_VIDEO_SRC_INFO    vidSrcInfo[NV_MAX_VID_STREAMS]; // (IN) Video source info
    NVAPI_DESKTOP_RES       res[NV_MAX_HEADS];              // (IN) Current desktop resolution(s)
    NvU64                   renderMode;                     // (IN) Requested rendering mode for the video source
    NvU64                   reserved1;                      // (IN) reserved for future expansion
    NvU64                   reserved2;                      // (IN) 

    NVAPI_VIDEO_CAPS        videoCaps[NV_MAX_VID_STREAMS];  // (OUT) Available video features for requested video streams
} NVAPI_VIDEO_CAPS_PACKET;

typedef struct _NV_DX_VIDEO_CAPS
{
    NvU32                   version;                        // (IN) NVAPI version that matched NV_DX_VIDEO_CAPS_VER
    NVAPI_VIDEO_CAPS_PACKET videoCapsPacket;                // (IN/OUT)
} NV_DX_VIDEO_CAPS;

#define NV_DX_VIDEO_CAPS_VER  MAKE_NVAPI_VERSION(NV_DX_VIDEO_CAPS,1)

NVAPI_INTERFACE NvAPI_D3D9_GetVideoCapabilities(IDirect3DDevice9 *pDev, 
                                                NV_DX_VIDEO_CAPS *pVideoCaps);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_RestoreDesktop
//
//   DESCRIPTION: 
//
// RETURN STATUS: NVAPI_ERROR or NVAPI_OK
//
// NOTE: This is not an interface, this is just a short-hand helper
//
///////////////////////////////////////////////////////////////////////////////
inline int NvAPI_D3D9_RestoreDesktop(IDirect3DDevice9 *pDev)
{
    return NvAPI_D3D9_PresentSurfaceToDesktop(pDev,NVDX_OBJECT_NONE,0);
}


///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_AliasPrimaryFromDevice
//
//   DESCRIPTION: Create an alias surface from the given pDevFrom's primary swap chain.
//
// INPUT:   pDevTo      Where new surfaces created in
//          pDevFrom    Where the surfaces aliased from
//          dwIndex     Index to the primary flipchain of pDevFrom
//          
// OUTPUT:  ppSurf      Filled with new surface pointer (to be released by caller)
//          pHandle     (optional) If non-NULL, filled with SurfaceHandle of the surface
//                      Same can be achieved by calling NVD3D9_GetSurfaceHandle afterwards
// 
// DEMO:    Multihead
//
// RETURN STATUS: an int which could be an NvAPI status or DX HRESULT code
//
NVAPI_INTERFACE NvAPI_D3D9_AliasPrimaryFromDevice(IDirect3DDevice9 *pDevTo, 
                                              IDirect3DDevice9 *pDevFrom, 
                                              NvU32 dwIndex,
                                              IDirect3DSurface9 **ppSurf,
                                              NVDX_ObjectHandle *pHandle = 0);
///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_SetResourceHint
//
//   DESCRIPTION: General purpose function for passing down various resource 
//                related hints to the driver. Hints are divided into categories
//                and types within each cateroy. 
//
//  INPUT: pDev - valid device context
//         obj  - previously obtained HV resource handle 
//         dwHintCategory - category of the hints
//         dwHintType     - a hint within this category
//         *pdwHintValue  - pointer to location containing hint value
//  
//  OUTPUT:
//         *dwHintValue   - receives previous value of this hint.
//
// RETURN STATUS: an int which could be an NvAPI status or DX HRESULT code

// Avaliable categories / hint names:
//     Sli:
typedef enum _NVAPI_SETRESOURCEHINT_CATEGORY
{
    NvApiHints_Sli = 1,

}  NVAPI_SETRESOURCEHINT_CATEGORY;


// types of Sli hints
  // InterframeAwareForTexturing tells the driver to discard any interframe
  // dirty state (skip inter-gpu transfers) on this object when texturing from it.
  // other operations (clear, render, blit, cpu lock) are not affected by this bit.
  // The value of the bit is a max difference (in frames) of the last modifications
  // of the current and golden-copy gpus, which is allowed to get discarded.
  // For example, if the value is 1 then inactive buffer will be discarded only if it was
  // modified on the same or previous frame and will be transfered to active gpu otherwise.
  //
  // default value: zero

typedef enum _NVAPI_SETRESOURCEHINT_SLI_HINTS
{
    NvApiHints_Sli_InterframeAwareForTexturing = 1,

}  NVAPI_SETRESOURCEHINT_SLI_HINTS;

//
// end of categories

NVAPI_INTERFACE NvAPI_D3D9_SetResourceHint(IDirect3DDevice9 *pDev, NVDX_ObjectHandle obj, 
                            NVAPI_SETRESOURCEHINT_CATEGORY dwHintCategory, NvU32 dwHintName, NvU32 *pdwHintValue);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_Lock
//
//   DESCRIPTION: Lock and Unlock a given surface identified by handle. This
//                function can provide CPU access to all object including
//                RTs, ZBs, textures, VBs and IBs.
// NOTE: 
// (a) If an object can be accessed with normal DX9 means, please do not use this
// (b) Lock should be called right before CPU access, and Unlock called right after
//     the access is done. Any 3D rendering or state change may cause the locked
//     surface to be lost. When that happens, trying to access the cached CPU 
//     address may causing app to crash.
//
// RETURN STATUS: an int which could be an NvAPI status or DX HRESULT code
//
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
#define NV_ACCESSFLAG_READONLY  0x00000001 // TBD: do these go with dwLockFlags?
#define NV_ACCESSFLAG_DISCARD   0x00000002
NVAPI_INTERFACE NvAPI_D3D9_Lock(IDirect3DDevice9 *pDev, NVDX_ObjectHandle obj, NvU32 dwLockFlags,
                            void **ppAddress, NvU32 *pPitch);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_Unlock
//
//   DESCRIPTION: 
//
// RETURN STATUS: an int which could be an NvAPI status or DX HRESULT code
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_D3D9_Unlock(IDirect3DDevice9 *pDev, NVDX_ObjectHandle obj);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_LockForCUDA
//
//   DESCRIPTION: Lock a given surface identified by handle. This
//                function can provide access to all objects including
//                RTs, ZBs, textures, VBs and IBs.
// NOTE: 
//     LockForCUDA is analogous to LockForCPU, except that the memory will be
//     read/written by CUDA (compute running in a separate channel) as opposed 
//     to the CPU.  It should be called right before giving access to CUDA
//     and Unlock called right after the access is done.
//
// RETURN STATUS: NvAPI status
//
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
NVAPI_INTERFACE NvAPI_D3D9_LockForCUDA(IDirect3DDevice9 *pDev, NVDX_ObjectHandle obj, NvU32 dwLockFlags,
                            NvU32 *pClient, NvU32 *pHandle, NvU64 *pOffset, NvU32 *pSize);


//-----------------------------------------------------------------------------
// Purevideo Post Processing controls, defines and datastructures
//-----------------------------------------------------------------------------
#ifndef NV_VIDEO_POSTPROCESSING_DEFINE
#define NV_VIDEO_POSTPROCESSING_DEFINE

typedef struct
{
    NvU32 value;        
    NvU32 min;        
    NvU32 max;        
} NV_VIDEO_POSTPROCESSING_RANGE_VALUE;


typedef enum
{
    NV_VIDEO_POSTPROCESSING_CAPS_FEATURE_SUPPORTED          = 0x00,  
    NV_VIDEO_POSTPROCESSING_CAPS_FEATURE_NOT_SUPPORTED      = 0x01,  
} NV_VIDEO_POSTPROCESSING_CAPS;

//Deinterlace 

typedef enum _NV_VIDEO_POSTPROCESSING_DEINTERLACE_FEATURE
{
    NV_VIDEO_POSTPROCESSING_DEINTERLACE_FEATURE_DISABLE           = 0x00,       
    NV_VIDEO_POSTPROCESSING_DEINTERLACE_FEATURE_ENABLE            = 0x01,       
} NV_VIDEO_POSTPROCESSING_DEINTERLACE_FEATURE;

typedef enum _NV_VIDEO_POSTPROCESSING_DEINTERLACE_ALG
{
    NV_VIDEO_POSTPROCESSING_DEINTERLACE_ALG_BOB              = 0x01,
    NV_VIDEO_POSTPROCESSING_DEINTERLACE_ALG_WEAVE            = 0x02,
    NV_VIDEO_POSTPROCESSING_DEINTERLACE_ALG_SIMPLE_ADAPTIVE  = 0x03,
    NV_VIDEO_POSTPROCESSING_DEINTERLACE_ALG_GRADIENT_SIMPLE  = 0x04,      
    NV_VIDEO_POSTPROCESSING_DEINTERLACE_ALG_GRADIENT_FULL    = 0x05,
    NV_VIDEO_POSTPROCESSING_DEINTERLACE_ALG_ADVANCED         = 0x06,
} NV_VIDEO_POSTPROCESSING_DEINTERLACE_ALG;

typedef enum _NV_VIDEO_POSTPROCESSING_DEINTERLACE_ENHANCE
{
    NV_VIDEO_POSTPROCESSING_DEINTERLACE_ENHANCE_DIR_SPATIAL_DEFAULT  = 0x01,
    NV_VIDEO_POSTPROCESSING_DEINTERLACE_ENHANCE_DIR_SPATIAL_SIMPLE   = 0x02,
    NV_VIDEO_POSTPROCESSING_DEINTERLACE_ENHANCE_DIR_SPATIAL_ADVANCED = 0x03,
} NV_VIDEO_POSTPROCESSING_DEINTERLACE_ENHANCE;

typedef enum _NV_VIDEO_POSTPROCESSING_DEINTERLACE_MISC
{
    NV_VIDEO_POSTPROCESSING_DEINTERLACE_MISC_NONE = 0x00,  // none so far

} NV_VIDEO_POSTPROCESSING_DEINTERLACE_MISC;

typedef struct
{
    NV_VIDEO_POSTPROCESSING_CAPS caps;
    NV_VIDEO_POSTPROCESSING_DEINTERLACE_FEATURE feature;
    NV_VIDEO_POSTPROCESSING_DEINTERLACE_ALG     alg;
    NV_VIDEO_POSTPROCESSING_DEINTERLACE_ENHANCE enhance;
    NV_VIDEO_POSTPROCESSING_DEINTERLACE_MISC    misc;
} NV_VIDEO_POSTPROCESSING_DEINTERLACE_INFO;


//Scaling

typedef enum _NV_VIDEO_POSTPROCESSING_SCALING_FEATURE
{
    NV_VIDEO_POSTPROCESSING_SCALING_FEATURE_DISABLE            = 0x00,       
    NV_VIDEO_POSTPROCESSING_SCALING_FEATURE_ENABLE             = 0x01,       
} NV_VIDEO_POSTPROCESSING_SCALING_FEATURE;

typedef enum _NV_VIDEO_POSTPROCESSING_SCALING_ALG
{
    NV_VIDEO_POSTPROCESSING_SCALING_ALG_DEFAULT                = 0x01,
    NV_VIDEO_POSTPROCESSING_SCALING_ALG_4x4FILTER              = 0x02,
} NV_VIDEO_POSTPROCESSING_SCALING_ALG;

typedef enum _NV_VIDEO_POSTPROCESSING_SCALING_ENHANCE
{
    NV_VIDEO_POSTPROCESSING_SCALING_ENHANCE_NONE               = 0x00,   // None so far 
} NV_VIDEO_POSTPROCESSING_SCALING_ENHANCE;

typedef enum _NV_VIDEO_POSTPROCESSING_SCALING_MISC
{
    NV_VIDEO_POSTPROCESSING_SCALING_MISC_NONE                  = 0x00,  // none so far
} NV_VIDEO_POSTPROCESSING_SCALING_MISC;

typedef struct
{
    NV_VIDEO_POSTPROCESSING_CAPS caps;
    NV_VIDEO_POSTPROCESSING_SCALING_FEATURE feature;
    NV_VIDEO_POSTPROCESSING_SCALING_ALG     alg;
    NV_VIDEO_POSTPROCESSING_SCALING_ENHANCE enhance;
    NV_VIDEO_POSTPROCESSING_SCALING_MISC    misc;
} NV_VIDEO_POSTPROCESSING_SCALING_INFO;


//Cadence

typedef enum _NV_VIDEO_POSTPROCESSING_CADENCE_FEATURE
{
    NV_VIDEO_POSTPROCESSING_CADENCE_FEATURE_DISABLE            = 0x00,       
    NV_VIDEO_POSTPROCESSING_CADENCE_FEATURE_ENABLE             = 0x01,       
} NV_VIDEO_POSTPROCESSING_CADENCE_FEATURE;

typedef enum _NV_VIDEO_POSTPROCESSING_CADENCE_ALG
{
    NV_VIDEO_POSTPROCESSING_CADENCE_ALG_NONE                   = 0x00, //None so far
} NV_VIDEO_POSTPROCESSING_CADENCE_ALG;

typedef enum _NV_VIDEO_POSTPROCESSING_CADENCE_ENHANCE
{
    NV_VIDEO_POSTPROCESSING_CADENCE_ENHANCE_NONE               = 0x00,   
    NV_VIDEO_POSTPROCESSING_CADENCE_ENHANCE_VOF                = 0x01,   
} NV_VIDEO_POSTPROCESSING_CADENCE_ENHANCE;

typedef enum _NV_VIDEO_POSTPROCESSING_CADENCE_MISC
{
    NV_VIDEO_POSTPROCESSING_CADENCE_MISC_NONE                  = 0x00,  // none so far
} NV_VIDEO_POSTPROCESSING_CADENCE_MISC;

typedef struct
{
    NV_VIDEO_POSTPROCESSING_CAPS caps;
    NV_VIDEO_POSTPROCESSING_CADENCE_FEATURE feature;
    NV_VIDEO_POSTPROCESSING_CADENCE_ALG     alg;
    NV_VIDEO_POSTPROCESSING_CADENCE_ENHANCE enhance;
    NV_VIDEO_POSTPROCESSING_CADENCE_MISC    misc;
} NV_VIDEO_POSTPROCESSING_CADENCE_INFO;



//Edge Enhance

typedef enum _NV_VIDEO_POSTPROCESSING_EE_FEATURE
{
    NV_VIDEO_POSTPROCESSING_EE_FEATURE_DISABLE            = 0x00,       
    NV_VIDEO_POSTPROCESSING_EE_FEATURE_ENABLE             = 0x01,       
} NV_VIDEO_POSTPROCESSING_EE_FEATURE;

typedef enum _NV_VIDEO_POSTPROCESSING_EE_ALG
{
    NV_VIDEO_POSTPROCESSING_EE_ALG_NONE                   = 0x00, //None so far
} NV_VIDEO_POSTPROCESSING_EE_ALG;

typedef enum _NV_VIDEO_POSTPROCESSING_EE_ENHANCE
{
    NV_VIDEO_POSTPROCESSING_EE_ENHANCE_NONE                = 0x00,   
} NV_VIDEO_POSTPROCESSING_EE_ENHANCE;

typedef enum _NV_VIDEO_POSTPROCESSING_EE_MISC
{
    NV_VIDEO_POSTPROCESSING_EE_MISC_NONE                  = 0x00,  // none so far
} NV_VIDEO_POSTPROCESSING_EE_MISC;

typedef struct
{
    NV_VIDEO_POSTPROCESSING_CAPS caps;
    NV_VIDEO_POSTPROCESSING_EE_FEATURE     feature;
    NV_VIDEO_POSTPROCESSING_EE_ALG         alg ;
    NV_VIDEO_POSTPROCESSING_EE_ENHANCE     enhance;
    NV_VIDEO_POSTPROCESSING_EE_MISC        misc;
    NV_VIDEO_POSTPROCESSING_RANGE_VALUE    rangeValue;
} NV_VIDEO_POSTPROCESSING_EE_INFO;

//Noise Reduction

typedef enum _NV_VIDEO_POSTPROCESSING_NR_FEATURE
{
    NV_VIDEO_POSTPROCESSING_NR_FEATURE_DISABLE            = 0x00,       
    NV_VIDEO_POSTPROCESSING_NR_FEATURE_ENABLE             = 0x01,       
} NV_VIDEO_POSTPROCESSING_NR_FEATURE;

typedef enum _NV_VIDEO_POSTPROCESSING_NR_ALG
{
    NV_VIDEO_POSTPROCESSING_NR_ALG_NONE                   = 0x00, //None so far
} NV_VIDEO_POSTPROCESSING_NR_ALG;

typedef enum _NV_VIDEO_POSTPROCESSING_NR_ENHANCE
{
    NV_VIDEO_POSTPROCESSING_NR_ENHANCE_NONE                = 0x00,   
} NV_VIDEO_POSTPROCESSING_NR_ENHANCE;

typedef enum _NV_VIDEO_POSTPROCESSING_NR_MISC
{
    NV_VIDEO_POSTPROCESSING_NR_MISC_NONE                  = 0x00,  // none so far
} NV_VIDEO_POSTPROCESSING_NR_MISC;

typedef struct
{
    NV_VIDEO_POSTPROCESSING_CAPS caps;
    NV_VIDEO_POSTPROCESSING_NR_FEATURE     feature;
    NV_VIDEO_POSTPROCESSING_NR_ALG         alg;
    NV_VIDEO_POSTPROCESSING_NR_ENHANCE     enhance;
    NV_VIDEO_POSTPROCESSING_NR_MISC        misc;
    NV_VIDEO_POSTPROCESSING_RANGE_VALUE    rangeValue;
} NV_VIDEO_POSTPROCESSING_NR_INFO;


//Over Drive

typedef enum _NV_VIDEO_POSTPROCESSING_OVERDRIVE_FEATURE
{
    NV_VIDEO_POSTPROCESSING_OVERDRIVE_FEATURE_DISABLE            = 0x00,       
    NV_VIDEO_POSTPROCESSING_OVERDRIVE_FEATURE_ENABLE             = 0x01,       
} NV_VIDEO_POSTPROCESSING_OVERDRIVE_FEATURE;

typedef enum _NV_VIDEO_POSTPROCESSING_OVERDRIVE_ALG
{
    NV_VIDEO_POSTPROCESSING_OVERDRIVE_ALG_NONE                   = 0x00, //None so far
} NV_VIDEO_POSTPROCESSING_OVERDRIVE_ALG;

typedef enum _NV_VIDEO_POSTPROCESSING_OVERDRIVE_ENHANCE
{
    NV_VIDEO_POSTPROCESSING_OVERDRIVE_ENHANCE_NONE                = 0x00,   
} NV_VIDEO_POSTPROCESSING_OVERDRIVE_ENHANCE;

typedef enum _NV_VIDEO_POSTPROCESSING_OVERDRIVE_MISC
{
    NV_VIDEO_POSTPROCESSING_OVERDRIVE_MISC_NONE                  = 0x00,  // none so far
} NV_VIDEO_POSTPROCESSING_OVERDRIVE_MISC;

typedef struct
{
    NV_VIDEO_POSTPROCESSING_CAPS caps;
    NV_VIDEO_POSTPROCESSING_OVERDRIVE_FEATURE feature;
    NV_VIDEO_POSTPROCESSING_OVERDRIVE_ALG     alg;
    NV_VIDEO_POSTPROCESSING_OVERDRIVE_ENHANCE enhance;
    NV_VIDEO_POSTPROCESSING_OVERDRIVE_MISC    misc;
} NV_VIDEO_POSTPROCESSING_OVERDRIVE_INFO;


//Postprocessing info struct 
typedef struct 
{
    NvU32 version;                                              // structure version
    NV_VIDEO_POSTPROCESSING_DEINTERLACE_INFO  deinterlaceInfo;  // deinterlace info struct
    NV_VIDEO_POSTPROCESSING_SCALING_INFO      scalingInfo;      // scaling info struct
    NV_VIDEO_POSTPROCESSING_CADENCE_INFO      cadenceInfo;      // cadence info struct
    NV_VIDEO_POSTPROCESSING_NR_INFO           nrInfo;           // noise reduction info struct
    NV_VIDEO_POSTPROCESSING_EE_INFO           eeInfo;           // edge enhance info struct
    NV_VIDEO_POSTPROCESSING_OVERDRIVE_INFO    overDriveInfo;    // overdrive info struct
} NV_VIDEO_POSTPROCESSING_CTRL_INFO;
#define NV_VIDEO_POSTPROCESSING_CTRL_NVAPI_VER MAKE_NVAPI_VERSION(NV_VIDEO_POSTPROCESSING_CTRL_INFO,1)

#endif 


///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_Video_PostProcessing_GetDefault
//
//   DESCRIPTION: Queries the video postprocessing default control info
//
// RETURN STATUS: NVAPI_ERROR or NVAPI_OK
//
///////////////////////////////////////////////////////////////////////////////

NVAPI_INTERFACE NvAPI_D3D9_Video_PostProcessing_GetDefault(IDirect3DDevice9 *pDev, NV_VIDEO_POSTPROCESSING_CTRL_INFO* pVideoPPCtrlInfo);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_Video_PostProcessing_Get
//
//   DESCRIPTION: Queries the current video postprocessing  control info
//
// RETURN STATUS: NVAPI_ERROR or NVAPI_OK
//
///////////////////////////////////////////////////////////////////////////////

NVAPI_INTERFACE NvAPI_D3D9_Video_PostProcessing_Get(IDirect3DDevice9 *pDev,  NV_VIDEO_POSTPROCESSING_CTRL_INFO* pVideoPPCtrlInfo);


///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_Video_PostProcessing_Set
//
//   DESCRIPTION: Sets the current video postprocessing  control info
//
// RETURN STATUS: NVAPI_ERROR or NVAPI_OK
//
///////////////////////////////////////////////////////////////////////////////

NVAPI_INTERFACE NvAPI_D3D9_Video_PostProcessing_Set(IDirect3DDevice9 *pDev,  NV_VIDEO_POSTPROCESSING_CTRL_INFO* pVideoPPCtrlInfo);



//Split screen info struct 

typedef enum _NV_VIDEO_POSTPROCESSING_SPLITSCREEN_TYPE
{
    NV_VIDEO_POSTPROCESSING_SPLITSCREEN_TYPE_DISABLE   = 0x00,            
    NV_VIDEO_POSTPROCESSING_SPLITSCREEN_TYPE_NORMAL    = 0x01,            
    NV_VIDEO_POSTPROCESSING_SPLITSCREEN_TYPE_REPEATED  = 0x02,            
    NV_VIDEO_POSTPROCESSING_SPLITSCREEN_TYPE_ON_MIRROR = 0x03,            
} NV_VIDEO_POSTPROCESSING_SPLITSCREEN_TYPE;


typedef struct 
{
    NvU32 version;                              // structure version
    NV_VIDEO_POSTPROCESSING_CAPS caps;
    NV_VIDEO_POSTPROCESSING_SPLITSCREEN_TYPE type;
} NV_VIDEO_POSTPROCESSING_SPLITSCREEN_CTRL_INFO;
#define NV_VIDEO_POSTPROCESSING_SPLITSCREEN_CTRL_NVAPI_VER MAKE_NVAPI_VERSION(NV_VIDEO_POSTPROCESSING_SPLITSCREEN_CTRL_INFO,1)

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_Video_PostProcessing_SplitScreen_GetDefault
//
//   DESCRIPTION: Queries the split screen default control info
//
// RETURN STATUS: NVAPI_ERROR or NVAPI_OK
//
///////////////////////////////////////////////////////////////////////////////

NVAPI_INTERFACE NvAPI_D3D9_Video_PostProcessing_SplitScreen_GetDefault(IDirect3DDevice9 *pDev, NV_VIDEO_POSTPROCESSING_SPLITSCREEN_CTRL_INFO* pVideoPPSplitScreenCtrlInfo);


///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_Video_PostProcessing_SplitScreen_Get
//
//   DESCRIPTION: Queries the video post processing split screen control  info
//
// RETURN STATUS: NVAPI_ERROR or NVAPI_OK
//
///////////////////////////////////////////////////////////////////////////////

NVAPI_INTERFACE NvAPI_D3D9_Video_PostProcessing_SplitScreen_Get(IDirect3DDevice9 *pDev, NV_VIDEO_POSTPROCESSING_SPLITSCREEN_CTRL_INFO* pVideoPPSplitScreenCtrlInfo);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_D3D9_Video_PostProcessing_SplitScreen_Set
//
//   DESCRIPTION: Sets the video post processing split screen control  info
//
// RETURN STATUS: NVAPI_ERROR or NVAPI_OK
//
///////////////////////////////////////////////////////////////////////////////

NVAPI_INTERFACE NvAPI_D3D9_Video_PostProcessing_SplitScreen_Set(IDirect3DDevice9 *pDev, NV_VIDEO_POSTPROCESSING_SPLITSCREEN_CTRL_INFO* pVideoPPSplitScreenCtrlInfo);


///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME:   NvAPI_D3D9_GetSLIInfo
//
// DESCRIPTION:     Returns the SLI information for D3D device. The NVAPI_NO_ACTIVE_SLI_TOPOLOGY value is
//                  returned when SLI is unsupported for this device.
//
// RETURN STATUS:
//                  NVAPI_OK - completed request
//                  NVAPI_NO_ACTIVE_SLI_TOPOLOGY - SLI is not supported for this device
//                  NVAPI_ERROR - error occurred
//
///////////////////////////////////////////////////////////////////////////////
typedef enum _NV_SLI_MODE
{
    NV_SLI_RENDERING_MODE_AUTOSELECT  = 0,     // AutoSelect
    NV_SLI_RENDERING_MODE_AFR         = 1,     // Alternate Frames
    NV_SLI_RENDERING_MODE_SFR         = 2,     // Split Frame
    NV_SLI_RENDERING_MODE_SINGLE      = 3      // Single GPU
} NV_SLI_MODE;

typedef struct
{
    NvU32 version;                       // Structure version

    NV_SLI_MODE         mode;            // [OUT] Current SLI mode
    NvU32               gpus;            // [OUT] Number of GPUs
} NV_SLI_INFO;
#define NV_SLI_INFO_VER  MAKE_NVAPI_VERSION(NV_SLI_INFO,1)

NVAPI_INTERFACE NvAPI_D3D9_GetSLIInfo(IDirect3DDevice9 *pDev, NV_SLI_INFO *pSliInfo);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME:   NvAPI_D3D9_SetSLIMode
//
// DESCRIPTION:     Sets the SLI rendering mode for D3D device. The NVAPI_SLI_RENDERING_MODE_NOTALLOWED
//                  return status occurs when SLI mode is set in the middle of a rendering. An app may try
//                  to recreate/reset device and perform the call again. The normal way to setup the SLI mode
//                  is immediately after CreateDevice() or Reset(). The NVAPI_NO_ACTIVE_SLI_TOPOLOGY value is
//                  returned when SLI is unsupported for this device.
//
// RETURN STATUS:
//                  NVAPI_OK - completed request
//                  NVAPI_NO_ACTIVE_SLI_TOPOLOGY - SLI is not supported for this device
//                  NVAPI_SLI_RENDERING_MODE_NOTALLOWED - setup of SLI mode is not possible right now
//                  NVAPI_INVALID_ARGUMENT - invalid rendering mode
//                  NVAPI_ERROR - error occurred
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_D3D9_SetSLIMode(IDirect3DDevice9 *pDev, NV_SLI_MODE SliMode);

///////////////////////////////////////////////////////////////////////////////
// FUNCTION NAME:   NvAPI_D3D9_VideoSurfaceEncryptionControl
//
// DESCRIPTION:     This API lets caller to query GPU capabilities for handling
//                  encrypted uncompressed video surfaces, to enable and dis-
//                  able video surface encryption, and to configure the para-
//                  meters used in the encryption algorithms.
//
// RETURN STATUS:   NVAPI_OK - completed request
//                  NVAPI_ERROR - miscellaneous error occurred
//                  NVAPI_INVALID_ARGUMENT - invalid argument
//                  NVAPI_INCOMPATIBLE_STRUCT_VERSION - the version of 
//                      NVAPI_VIDSURF_ENCRYPT_CTRL_PARAMS struct not supported
//
///////////////////////////////////////////////////////////////////////////////

// Video Surface Encryption Control Commands
typedef enum 
{
    NVAPI_VIDSURF_ENCRYPT_CMD_NULL              = 0x00, // Null command
    NVAPI_VIDSURF_ENCRYPT_CMD_GET_GUID_COUNT    = 0x01, // Get GUID count
    NVAPI_VIDSURF_ENCRYPT_CMD_GET_GUIDS         = 0x02, // Get GUIDs
    NVAPI_VIDSURF_ENCRYPT_CMD_SET_GUID          = 0x03, // Set GUID
    NVAPI_VIDSURF_ENCRYPT_CMD_ENABLE            = 0x04, // Enable encryption
    NVAPI_VIDSURF_ENCRYPT_CMD_SET_KEY           = 0x05, // Set key
    NVAPI_VIDSURF_ENCRYPT_CMD_SET_IV            = 0x06  // Set IV
} NVAPI_VIDSURF_ENCRYPT_COMMANDS;

#define NVAPI_VIDSURF_ENCRYPT_MAX_GUIDS        (4)

typedef struct
{
    NvU32   dwGuidCount;                                // (OUT)
} NVAPI_VIDSURF_ENCRYPT_CTRL_GET_GUID_COUNT;

typedef struct
{
    GUID    guids[NVAPI_VIDSURF_ENCRYPT_MAX_GUIDS];     // (OUT)
} NVAPI_VIDSURF_ENCRYPT_CTRL_GET_GUIDS;

typedef struct
{
    GUID    guid;                                       // (IN)
} NVAPI_VIDSURF_ENCRYPT_CTRL_SET_GUID;

typedef struct
{
    IDirect3DSurface9  *pSurface;                       // D3D9 surface ptr (IN)
    BOOL                bEnable;                        // (IN)
} NVAPI_VIDSURF_ENCRYPT_CTRL_ENABLE;

typedef struct
{
    NvU32   dwEncryptKeyProtectionMode;
    NvU32   dwEncryptKeyProtectionIdentifier;
    NvU32   dwKey[4];                                   // (IN)
} NVAPI_VIDSURF_ENCRYPT_CTRL_SET_KEY;

typedef struct
{
    NvU32   dwEncryptKeyProtectionMode;
    NvU32   dwEncryptKeyProtectionIdentifier;
    NvU32   dwIV[4];                                    // (IN)
} NVAPI_VIDSURF_ENCRYPT_CTRL_SET_IV;

typedef struct
{
    NvU32               version;                        // structure version (IN)
    NvU32               dwCommand;                      // command (IN)
    union
    {
        NVAPI_VIDSURF_ENCRYPT_CTRL_GET_GUID_COUNT       GetGuidCount;
        NVAPI_VIDSURF_ENCRYPT_CTRL_GET_GUIDS            GetGuids;
        NVAPI_VIDSURF_ENCRYPT_CTRL_SET_GUID             SetGuid;
        NVAPI_VIDSURF_ENCRYPT_CTRL_ENABLE               Enable;
        NVAPI_VIDSURF_ENCRYPT_CTRL_SET_KEY              SetKey;
        NVAPI_VIDSURF_ENCRYPT_CTRL_SET_IV               SetIV;
    };
} NVAPI_VIDSURF_ENCRYPT_CTRL_PARAMS;

#define NVAPI_VIDSURF_ENCRYPT_CTRL_PARAMS_VER \
    MAKE_NVAPI_VERSION(NVAPI_VIDSURF_ENCRYPT_CTRL_PARAMS,1)

NVAPI_INTERFACE NvAPI_D3D9_VideoSurfaceEncryptionControl(IDirect3DDevice9 *pDev, 
    NVAPI_VIDSURF_ENCRYPT_CTRL_PARAMS *pVidSurfEncrCtrlParams);

#endif //defined(_D3D9_H_) && defined(__cplusplus)

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GPU_GetShaderPipeCount
//
//   DESCRIPTION: Retrieves the number of Shader Pipes on the GPU
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: pCount is NULL
//                NVAPI_OK: *pCount is set
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//                NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE: hPhysicalGpu was not a physical GPU handle
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_GetShaderPipeCount(NvPhysicalGpuHandle hPhysicalGpu,NvU32 *pCount);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GPU_GetAllOutputs
//
//   DESCRIPTION: Returns set of all GPU-output identifiers as a bitmask.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: hPhysicalGpu or pOutputsMask is NULL
//                NVAPI_OK: *pOutputsMask contains a set of GPU-output identifiers
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//                NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE: hPhysicalGpu was not a physical GPU handle
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_GetAllOutputs(NvPhysicalGpuHandle hPhysicalGpu,NvU32 *pOutputsMask);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GPU_GetConnectedOutputs
//
//   DESCRIPTION: Same as NvAPI_GPU_GetAllOutputs but returns only the set of GPU-output 
//                identifiers that are connected to display devices.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: hPhysicalGpu or pOutputsMask is NULL
//                NVAPI_OK: *pOutputsMask contains a set of GPU-output identifiers
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//                NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE: hPhysicalGpu was not a physical GPU handle
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_GetConnectedOutputs(NvPhysicalGpuHandle hPhysicalGpu, NvU32 *pOutputsMask);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GPU_GetConnectedOutputsWithLidState
//
//   DESCRIPTION: Similar to NvAPI_GPU_GetConnectedOutputs this API returns the connected display identifiers that are connected 
//                as a output mask but unlike NvAPI_GPU_GetConnectedOutputs this API "always" reflects the Lid State in the output mask.
//                Thus if you expect the LID close state to be available in the connection mask use this API.
//                If LID is closed then this API will remove the LID panel from the connected display identifiers. 
//                If LID is open then this API will reflect the LID panel in the connected display identifiers. 
//                Note:This API should be used on laptop systems and on systems where LID state is required in the connection output mask. 
//                     On desktop systems the returned identifiers will match NvAPI_GPU_GetConnectedOutputs.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: hPhysicalGpu or pOutputsMask is NULL
//                NVAPI_OK: *pOutputsMask contains a set of GPU-output identifiers
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//                NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE: hPhysicalGpu was not a physical GPU handle
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_GetConnectedOutputsWithLidState(NvPhysicalGpuHandle hPhysicalGpu, NvU32 *pOutputsMask);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GPU_GetActiveOutputs
//
//   DESCRIPTION: Same as NvAPI_GPU_GetAllOutputs but returns only the set of GPU-output 
//                identifiers that are actively driving display devices.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: hPhysicalGpu or pOutputsMask is NULL
//                NVAPI_OK: *pOutputsMask contains a set of GPU-output identifiers
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//                NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE: hPhysicalGpu was not a physical GPU handle
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_GetActiveOutputs(NvPhysicalGpuHandle hPhysicalGpu, NvU32 *pOutputsMask);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GPU_GetEDID
//
//   DESCRIPTION: Returns the EDID data for the specified GPU handle and connection bit mask.
//                displayOutputId should have exactly 1 bit set to indicate a single display.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: pEDID is NULL; displayOutputId has 0 or > 1 bits set.
//                NVAPI_OK: *pEDID contains valid data.
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found.
//                NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE: hPhysicalGpu was not a physical GPU handle.
//                NVAPI_DATA_NOT_FOUND: requested display does not contain an EDID
//
///////////////////////////////////////////////////////////////////////////////
#define NV_EDID_DATA_SIZE   256
#define NV_EDID_V1_DATA_SIZE   256
typedef struct
{
    NvU32   version;        //structure version
    NvU8    EDID_Data[NV_EDID_DATA_SIZE];
    NvU32   sizeofEDID;
} NV_EDID;

#define NV_EDID_VER         MAKE_NVAPI_VERSION(NV_EDID,2)
NVAPI_INTERFACE NvAPI_GPU_GetEDID(NvPhysicalGpuHandle hPhysicalGpu, NvU32 displayOutputId, NV_EDID *pEDID);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GPU_GetOutputType
//
//   DESCRIPTION: Give a physical GPU handle and a single outputId (exactly 1 bit set), this API
//                returns the output type.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: hPhysicalGpu, outputId or pOutputsMask is NULL; or outputId has > 1 bit set
//                NVAPI_OK: *pOutputType contains a NvGpuOutputType value
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//                NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE: hPhysicalGpu was not a physical GPU handle
//
///////////////////////////////////////////////////////////////////////////////
typedef enum _NV_GPU_OUTPUT_TYPE
{
    NVAPI_GPU_OUTPUT_UNKNOWN  = 0,
    NVAPI_GPU_OUTPUT_CRT      = 1,     // CRT display device
    NVAPI_GPU_OUTPUT_DFP      = 2,     // Digital Flat Panel display device
    NVAPI_GPU_OUTPUT_TV       = 3,     // TV display device
} NV_GPU_OUTPUT_TYPE;

NVAPI_INTERFACE NvAPI_GPU_GetOutputType(NvPhysicalGpuHandle hPhysicalGpu, NvU32 outputId, NV_GPU_OUTPUT_TYPE *pOutputType);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GPU_ValidateOutputCombination
//
//   DESCRIPTION: This call is used to determine if a set of GPU outputs can be active 
//                simultaneously.  While a GPU may have <n> outputs, they can not typically
//                all be active at the same time due to internal resource sharing.
//
//                Given a physical GPU handle and a mask of candidate outputs, this call
//                will return NVAPI_OK if all of the specified outputs can be driven
//                simultaneously.  It will return NVAPI_INVALID_COMBINATION if they cannot.
//                
//                Use NvAPI_GPU_GetAllOutputs() to determine which outputs are candidates.
//
// RETURN STATUS: NVAPI_OK: combination of outputs in outputsMask are valid (can be active simultaneously)
//                NVAPI_INVALID_COMBINATION: combination of outputs in outputsMask are NOT valid
//                NVAPI_INVALID_ARGUMENT: hPhysicalGpu or outputsMask does not have at least 2 bits set
//                NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE: hPhysicalGpu was not a physical GPU handle
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_ValidateOutputCombination(NvPhysicalGpuHandle hPhysicalGpu, NvU32 outputsMask);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GPU_GetConnectorInfo
//
//   DESCRIPTION: Given a physical GPU handle and a single outputId (exactly 1 bit set),
//                this API fills the NV_GPU_CONNECTOR_INFO with connector specific data. 
//                Note:If outputId is a connected or active then the current attached connector information is returned.
//                If there is no connector attached for the outputId then all possible connections on the board are returned.
//                Some TV outputs may have multiple connectors attached or it could have ambiguous connector layout on the board.
//                In that case the connector[] array will list all connectors but doesn't
//                indicate which one is 'active'. To get the active TV connector use NvAPI_GetTVOutputInfo.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: hPhysicalGpu, outputId or pOutputsMask is NULL; or outputId has > 1 bit set
//                NVAPI_OK: *pConnectorInfo contains valid NV_GPU_CONNECTOR_INFO data
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//                NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE: hPhysicalGpu was not a physical GPU handle
//                NVAPI_INCOMPATIBLE_STRUCT_VERSION - NV_GPU_CONNECTOR_INFO version not compatible with driver
//
///////////////////////////////////////////////////////////////////////////////

typedef enum _NV_GPU_CONNECTOR_TYPE
{
    NVAPI_GPU_CONNECTOR_VGA_15_PIN                      = 0x00000000,
    NVAPI_GPU_CONNECTOR_TV_COMPOSITE                    = 0x00000010,
    NVAPI_GPU_CONNECTOR_TV_SVIDEO                       = 0x00000011,
    NVAPI_GPU_CONNECTOR_TV_HDTV_COMPONENT               = 0x00000013,
    NVAPI_GPU_CONNECTOR_TV_SCART                        = 0x00000014,
    NVAPI_GPU_CONNECTOR_TV_COMPOSITE_SCART_ON_EIAJ4120  = 0x00000016,
    NVAPI_GPU_CONNECTOR_TV_HDTV_EIAJ4120                = 0x00000017,
    NVAPI_GPU_CONNECTOR_PC_POD_HDTV_YPRPB               = 0x00000018,
    NVAPI_GPU_CONNECTOR_PC_POD_SVIDEO                   = 0x00000019,
    NVAPI_GPU_CONNECTOR_PC_POD_COMPOSITE                = 0x0000001A,
    NVAPI_GPU_CONNECTOR_DVI_I_TV_SVIDEO                 = 0x00000020,
    NVAPI_GPU_CONNECTOR_DVI_I_TV_COMPOSITE              = 0x00000021,
    NVAPI_GPU_CONNECTOR_DVI_I                           = 0x00000030,
    NVAPI_GPU_CONNECTOR_DVI_D                           = 0x00000031,
    NVAPI_GPU_CONNECTOR_ADC                             = 0x00000032,
    NVAPI_GPU_CONNECTOR_LFH_DVI_I_1                     = 0x00000038,
    NVAPI_GPU_CONNECTOR_LFH_DVI_I_2                     = 0x00000039,
    NVAPI_GPU_CONNECTOR_SPWG                            = 0x00000040,
    NVAPI_GPU_CONNECTOR_OEM                             = 0x00000041,
    NVAPI_GPU_CONNECTOR_HDMI_A                          = 0x00000061,
    NVAPI_GPU_CONNECTOR_UNKNOWN                         = 0xFFFFFFFF,
} NV_GPU_CONNECTOR_TYPE;

typedef enum _NV_GPU_CONNECTOR_PLATFORM
{
    NVAPI_GPU_CONNECTOR_PLATFORM_DEFAULT_ADD_IN_CARD   = 0x00000000,
    NVAPI_GPU_CONNECTOR_PLATFORM_TWO_PLATE_ADD_IN_CARD = 0x00000001,
    NVAPI_GPU_CONNECTOR_PLATFORM_MOBILE_ADD_IN_CARD    = 0x00000008,
    NVAPI_GPU_CONNECTOR_PLATFORM_MOBILE_BACK           = 0x00000010,
    NVAPI_GPU_CONNECTOR_PLATFORM_MOBILE_BACK_LEFT      = 0x00000011,
    NVAPI_GPU_CONNECTOR_PLATFORM_MOBILE_BACK_DOCK      = 0x00000018,
    NVAPI_GPU_CONNECTOR_PLATFORM_MAINBOARD_DEFAULT     = 0x00000020,
    NVAPI_GPU_CONNECTOR_PLATFORM_UNKNOWN               = 0xFFFFFFFF,
} NV_GPU_CONNECTOR_PLATFORM;

typedef struct
{
    NV_GPU_CONNECTOR_TYPE   type;           // connector type
    NvU32                   locationIndex;  // connector location
} NV_GPU_CONNECTOR_DATA;

#define NV_API_MAX_CONNECTOR_PER_OUTPUT     4

typedef struct
{
    NvU32                       version;            // structure version
    NV_GPU_CONNECTOR_PLATFORM   connectorPlatform;  // connector platform
    NvU32                       connectorCount;     // num valid entries in connector[]
    NV_GPU_CONNECTOR_DATA       connector[NV_API_MAX_CONNECTOR_PER_OUTPUT];
} NV_GPU_CONNECTOR_INFO;

#define NV_GPU_CONNECTOR_INFO_VER  MAKE_NVAPI_VERSION(NV_GPU_CONNECTOR_INFO,1)

NVAPI_INTERFACE NvAPI_GPU_GetConnectorInfo(NvPhysicalGpuHandle hPhysicalGpu, NvU32 outputId, NV_GPU_CONNECTOR_INFO *pConnectorInfo);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GPU_GetFullName
//
//   DESCRIPTION: Retrieves the full GPU name as an ascii string.  Eg: "Quadro FX 1400"
//
// RETURN STATUS: NVAPI_ERROR or NVAPI_OK
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_GetFullName(NvPhysicalGpuHandle hPhysicalGpu, NvAPI_ShortString szName);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GPU_GetPCIIdentifiers
//
//   DESCRIPTION: Returns the PCI identifiers associated this this GPU.
//                  DeviceId - the internal PCI device identifier for the GPU.
//                  SubSystemId - the internal PCI subsystem identifier for the GPU.
//                  RevisionId - the internal PCI device-specific revision identifier for the GPU.
//                  ExtDeviceId - the external PCI device identifier for the GPU.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: hPhysicalGpu or an argument is NULL
//                NVAPI_OK: arguments are populated with PCI identifiers
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//                NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE: hPhysicalGpu was not a physical GPU handle
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_GetPCIIdentifiers(NvPhysicalGpuHandle hPhysicalGpu,NvU32 *pDeviceId,NvU32 *pSubSystemId,NvU32 *pRevisionId,NvU32 *pExtDeviceId);
    
typedef enum _NV_GPU_BUS_TYPE
{
    NVAPI_GPU_BUS_TYPE_UNDEFINED    = 0,
    NVAPI_GPU_BUS_TYPE_PCI          = 1,
    NVAPI_GPU_BUS_TYPE_AGP          = 2,
    NVAPI_GPU_BUS_TYPE_PCI_EXPRESS  = 3,
    NVAPI_GPU_BUS_TYPE_FPCI         = 4,
} NV_GPU_BUS_TYPE;
///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GPU_GetBusType
//
//   DESCRIPTION: Returns the type of bus associated this this GPU.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: hPhysicalGpu or pBusType is NULL
//                NVAPI_OK: *pBusType contains bus identifier
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//                NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE: hPhysicalGpu was not a physical GPU handle
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_GetBusType(NvPhysicalGpuHandle hPhysicalGpu,NV_GPU_BUS_TYPE *pBusType);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GPU_GetIRQ
//
//   DESCRIPTION: Returns the interrupt number associated this this GPU.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: hPhysicalGpu or pIRQ is NULL
//                NVAPI_OK: *pIRQ contains interrupt number
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//                NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE: hPhysicalGpu was not a physical GPU handle
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_GetIRQ(NvPhysicalGpuHandle hPhysicalGpu,NvU32 *pIRQ);
    
///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GPU_GetVbiosRevision
//
//   DESCRIPTION: Returns the revision of the video bios associated this this GPU.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: hPhysicalGpu or pBiosRevision is NULL
//                NVAPI_OK: *pBiosRevision contains revision number
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//                NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE: hPhysicalGpu was not a physical GPU handle
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_GetVbiosRevision(NvPhysicalGpuHandle hPhysicalGpu,NvU32 *pBiosRevision);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GPU_GetVbiosOEMRevision
//
//   DESCRIPTION: Returns the OEM revision of the video bios associated this this GPU.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: hPhysicalGpu or pBiosRevision is NULL
//                NVAPI_OK: *pBiosRevision contains revision number
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//                NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE: hPhysicalGpu was not a physical GPU handle
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_GetVbiosOEMRevision(NvPhysicalGpuHandle hPhysicalGpu,NvU32 *pBiosRevision);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GPU_GetVbiosVersionString
//
//   DESCRIPTION: Returns the full bios version string in the form of xx.xx.xx.xx.yy where
//                the xx numbers come from NvAPI_GPU_GetVbiosRevision and yy comes from 
//                NvAPI_GPU_GetVbiosOEMRevision.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: hPhysicalGpu is NULL
//                NVAPI_OK: szBiosRevision contains version string
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//                NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE: hPhysicalGpu was not a physical GPU handle
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_GetVbiosVersionString(NvPhysicalGpuHandle hPhysicalGpu,NvAPI_ShortString szBiosRevision);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GPU_GetAGPAperture
//
//   DESCRIPTION: Returns AGP aperture in megabytes
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: pSize is NULL
//                NVAPI_OK: call successful
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//                NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE: hPhysicalGpu was not a physical GPU handle
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_GetAGPAperture(NvPhysicalGpuHandle hPhysicalGpu,NvU32 *pSize);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GPU_GetCurrentAGPRate
//
//   DESCRIPTION: Returns the current AGP Rate (1 = 1x, 2=2x etc, 0 = AGP not present)
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: pRate is NULL
//                NVAPI_OK: call successful
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//                NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE: hPhysicalGpu was not a physical GPU handle
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_GetCurrentAGPRate(NvPhysicalGpuHandle hPhysicalGpu,NvU32 *pRate);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GPU_GetCurrentPCIEDownstreamWidth
//
//   DESCRIPTION: Returns the number of PCIE lanes being used for the PCIE interface 
//                downstream from the GPU.
//
//                On systems that do not support PCIE, the maxspeed for the root link
//                will be zero.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: pWidth is NULL
//                NVAPI_OK: call successful
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//                NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE: hPhysicalGpu was not a physical GPU handle
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_GetCurrentPCIEDownstreamWidth(NvPhysicalGpuHandle hPhysicalGpu,NvU32 *pWidth);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GPU_GetPhysicalFrameBufferSize
//
//   DESCRIPTION: Returns the physical size of framebuffer in Kb.  This does NOT include any
//                system RAM that may be dedicated for use by the GPU.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: pSize is NULL
//                NVAPI_OK: call successful
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//                NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE: hPhysicalGpu was not a physical GPU handle
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_GetPhysicalFrameBufferSize(NvPhysicalGpuHandle hPhysicalGpu,NvU32 *pSize);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GPU_GetVirtualFrameBufferSize
//
//   DESCRIPTION: Returns the virtual size of framebuffer in Kb.  This includes the physical RAM plus any
//                system RAM that has been dedicated for use by the GPU.
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: pSize is NULL
//                NVAPI_OK: call successful
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//                NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE: hPhysicalGpu was not a physical GPU handle
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_GetVirtualFrameBufferSize(NvPhysicalGpuHandle hPhysicalGpu,NvU32 *pSize);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GPU_GetQuadroStatus
//
//   DESCRIPTION: Retrieves the Quadro status for the GPU (1 if Quadro, 0 if GeForce)
//
// RETURN STATUS: NVAPI_ERROR or NVAPI_OK
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_GetQuadroStatus(NvPhysicalGpuHandle hPhysicalGpu, NvU32 *pStatus); 


//Performance table overclocking 

typedef enum _NV_GPU_PERF_CLOCK_DOMAIN_ID
{
    NVAPI_GPU_PERF_CLOCK_DOMAIN_NV      = 0,
    NVAPI_GPU_PERF_CLOCK_DOMAIN_M       = 4,

} NV_GPU_PERF_CLOCK_DOMAIN_ID;

#define NVAPI_MAX_GPU_PERF_CLOCKS       32

#define NVAPI_MAX_PERF_CLOCK_LEVELS     12
#define NVAPI_TARGET_ALL_PERF_LEVELS    0xffffffff

#define NV_PERF_CLOCK_LEVEL_STATE_DEFAULT                   0x00000000 //level is in its default state
#define NV_PERF_CLOCK_LEVEL_STATE_OVERCLOCKED               0x00000001 //level is overclocked
#define NV_PERF_CLOCK_LEVEL_STATE_DESKTOP                   0x00000002 //2d desktop perf level
#define NV_PERF_CLOCK_LEVEL_STATE_PERFORMANCE               0x00000004 //3d applications perf level
#define NV_PERF_CLOCK_LEVEL_STATE_TEST                      0x00000008 //test the new clocks for this level. Does not apply.
#define NV_PERF_CLOCK_LEVEL_STATE_TEST_SUCCESS              0x00000010 //test result
      
#define NV_PERF_CLOCK_GPU_STATE_DEFAULT                     0x00000000 //default state
#define NV_PERF_CLOCK_GPU_STATE_DYNAMIC_SUPPORTED           0x00000001 //gpu supports dynamic performance level transitions
#define NV_PERF_CLOCK_GPU_STATE_DESKTOP                     0x00000002 //gpu in desktop level
#define NV_PERF_CLOCK_GPU_STATE_PERFORMANCE                 0x00000004 //gpu in performance level
#define NV_PERF_CLOCK_GPU_STATE_ACTIVE_CLOCKING_SUPPORTED   0x00000008 //active clocking supported
#define NV_PERF_CLOCK_GPU_STATE_ACTIVE_CLOCKING_ENABLE      0x00000010 //enable active clocking
#define NV_PERF_CLOCK_GPU_STATE_ACTIVE_CLOCKING_DISABLE     0x00000020 //disable active clocking
#define NV_PERF_CLOCK_GPU_STATE_MEMCLK_CONTROL_DISABLED     0x00000040 //memory clock control disabled
#define NV_PERF_CLOCK_GPU_STATE_GFXCLK_CONTROL_DISABLED     0x00000080 //core clock control disabled
#define NV_PERF_CLOCK_GPU_STATE_SET_DEFERRED                0x00000100 //No immediate perf transitions. Deferred until perf triggers kick in.
#define NV_PERF_CLOCK_GPU_STATE_TESTING_CLOCKS_SUPPORTED    0x00000200 //testing clocks supported

typedef struct
{
    NvU32   version;                                //IN perf clock table version
    NvU32   levelCount;                             //number of the performance levels. count increases everytime a level is overclocked
    NvU32   gpuPerflevel;                           //OUT the current perf level. This is a dynamic level which can possibly change on every call
    NvU32   domainCount;                            //IN/OUT number of domains
    NvU32   gpuPerfFlags;                           //IN/OUT gpu flags - one of the flags defined in NV_PERF_CLOCK_GPU_STATE
    struct
    {
        NvU32   level;                              //IN/OUT performance level indicator, range 0 to levelCount - 1.
        NvU32   flags;                              //IN/OUT per level flags - one of more flags defined in NV_PERF_CLOCK_LEVEL_STATE 
        struct
        {
            NV_GPU_PERF_CLOCK_DOMAIN_ID domainId;       //IN/OUT current domain indicator - one of the ids from NV_GPU_PERF_CLOCK_DOMAIN_ID
            NvU32                       domainFlags;    //reserved unused domain flags 
            NvU32                       currentFreq;    //IN/OUT current clock KHz
            NvU32                       defaultFreq;    //default clock KHz
            NvU32                       minFreq;        //min KHz
            NvU32                       maxFreq;        //max KHz
            NvU32                       bSetClock:1;    //IN if set during NvAPI_GPU_SetPerfClocks call, this domain currentFreq will be applied
        } domain[NVAPI_MAX_GPU_PERF_CLOCKS];
    } perfLevel[NVAPI_MAX_PERF_CLOCK_LEVELS];
    
} NV_GPU_PERF_CLOCK_TABLE;

#define NV_GPU_PERF_CLOCK_TABLE_VER  MAKE_NVAPI_VERSION(NV_GPU_PERF_CLOCK_TABLE,1)

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME:   NvAPI_GPU_GetPerfClocks
//
// DESCRIPTION:     Retrieves the performance clock table information for one or all the supported levels.
//
// PARAMETERS:      hPhysicalGPU(IN) - GPU selection.
//                  level(IN) - Specific level selection. Zero for all levels. Number of levels increases with overclocking of the levels.
//                  pPerfClkTable(OUT) - Table of performance levels retrieved.
//
// RETURN STATUS: 
//    NVAPI_OK - completed request
//    NVAPI_ERROR - miscellaneous error occurred
//    NVAPI_HANDLE_INVALIDATED - handle passed has been invalidated (see user guide)
//    NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE - handle passed is not a physical GPU handle
//    NVAPI_INCOMPATIBLE_STRUCT_VERSION - the version of the PERF_CLOCK_TABLE struct is not supported
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_GetPerfClocks(NvPhysicalGpuHandle hPhysicalGpu, NvU32 level, NV_GPU_PERF_CLOCK_TABLE *pPerfClkTable);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME:   NvAPI_GPU_SetPerfClocks
//
// DESCRIPTION:     Overclock a specific level in the performance table or overclock all levels with bSetClock set.
//
// PARAMETERS:      hPhysicalGPU(IN) - GPU selection.
//                  level(IN) - Specific level selection. Zero for all levels. Number of levels increases with overclocking of the levels.
//                  pPerfClkTable(IN) - Table of performance levels to set.
//                  Any other than DEFAULT for GPU and Level flags - gpuPerfFlags and level flags gets applied.
//                  If bSetClock is set, currentFreq gets applied.
//                  Overclocking DOMAIN_NV requires simulteneous overclocking of DOMAIN_M, else overclocking will fail.
//
// RETURN STATUS: 
//    NVAPI_OK - completed request
//    NVAPI_ERROR - miscellaneous error occurred
//    NVAPI_HANDLE_INVALIDATED - handle passed has been invalidated (see user guide)
//    NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE - handle passed is not a physical GPU handle
//    NVAPI_INCOMPATIBLE_STRUCT_VERSION - the version of the PERF_CLOCK_TABLE struct is not supported
//    NVAPI_TESTING_CLOCKS_NOT_SUPPORTED - testing clocks not supported
//
///////////////////////////////////////////////////////////////////////////////

NVAPI_INTERFACE NvAPI_GPU_SetPerfClocks(NvPhysicalGpuHandle hPhysicalGpu, NvU32 level, NV_GPU_PERF_CLOCK_TABLE *pPerfClkTable);

///////////////////////////////////////////////////////////////////////////////////
//  GPU Cooler Controls
//  Provides ability to Get and Set the fan level or equivalent cooler levels for various target devices associated with the GPU

#define NVAPI_MAX_COOLERS_PER_GPU   3
#define NVAPI_MIN_COOLER_LEVEL      0
#define NVAPI_MAX_COOLER_LEVEL      100
#define NVAPI_MAX_COOLER_LEVELS 24

typedef enum 
{
    NVAPI_COOLER_TYPE_NONE = 0,
    NVAPI_COOLER_TYPE_FAN,
    NVAPI_COOLER_TYPE_WATER,
    NVAPI_COOLER_TYPE_LIQUID_NO2,
} NV_COOLER_TYPE;

typedef enum 
{
    NVAPI_COOLER_CONTROLLER_NONE = 0,
    NVAPI_COOLER_CONTROLLER_ADI,
    NVAPI_COOLER_CONTROLLER_INTERNAL,
} NV_COOLER_CONTROLLER;

typedef enum
{
    NVAPI_COOLER_POLICY_NONE = 0,
    NVAPI_COOLER_POLICY_MANUAL,                     //Manual adjustment of cooler level. Gets applied right away independent of temperature or performance level.
    NVAPI_COOLER_POLICY_PERF,                       //GPU performance controls the cooler level.
    NVAPI_COOLER_POLICY_TEMPERATURE_DISCRETE = 4,   //Discrete thermal levels control the cooler level.
    NVAPI_COOLER_POLICY_TEMPERATURE_CONTINUOUS = 8, //Cooler level adjusted at continuous thermal levels.
    NVAPI_COOLER_POLICY_HYBRID,                     //Hybrid of performance and temperature levels.
} NV_COOLER_POLICY;

typedef enum
{
    NVAPI_COOLER_TARGET_NONE = 0,
    NVAPI_COOLER_TARGET_GPU,
    NVAPI_COOLER_TARGET_MEMORY,
    NVAPI_COOLER_TARGET_POWER_SUPPLY = 4,
    NVAPI_COOLER_TARGET_ALL = 7                    //This cooler cools all of the components related to its target gpu.
} NV_COOLER_TARGET;

typedef enum
{
    NVAPI_COOLER_CONTROL_NONE = 0,
    NVAPI_COOLER_CONTROL_TOGGLE,                   //ON/OFF
    NVAPI_COOLER_CONTROL_VARIABLE,                 //Suppports variable control.
} NV_COOLER_CONTROL;

typedef enum
{
    NVAPI_INACTIVE = 0,             //inactive or unsupported
    NVAPI_ACTIVE = 1,               //active and spinning in case of fan
} NV_COOLER_ACTIVITY_LEVEL;

typedef struct
{
    NvU32   version;                //structure version 
    NvU32   count;                  //number of associated coolers with the selected GPU
    struct 
    {
        NV_COOLER_TYPE              type;               //type of cooler - FAN, WATER, LIQUID_NO2...
        NV_COOLER_CONTROLLER        controller;         //internal, ADI...
        NvU32                       defaultMinLevel;    //the min default value % of the cooler
        NvU32                       defaultMaxLevel;    //the max default value % of the cooler
        NvU32                       currentMinLevel;    //the current allowed min value % of the cooler
        NvU32                       currentMaxLevel;    //the current allowed max value % of the cooler
        NvU32                       currentLevel;       //the current value % of the cooler
        NV_COOLER_POLICY            defaultPolicy;      //cooler control policy - auto-perf, auto-thermal, manual, hybrid...
        NV_COOLER_POLICY            currentPolicy;      //cooler control policy - auto-perf, auto-thermal, manual, hybrid...
        NV_COOLER_TARGET            target;             //cooling target - GPU, memory, chipset, powersupply, canoas...
        NV_COOLER_CONTROL           controlType;        //toggle or variable
        NV_COOLER_ACTIVITY_LEVEL    active;             //is the cooler active - fan spinning...
    } cooler[NVAPI_MAX_COOLERS_PER_GPU];

} NV_GPU_GETCOOLER_SETTINGS;

#define NV_GPU_GETCOOLER_SETTINGS_VER  MAKE_NVAPI_VERSION(NV_GPU_GETCOOLER_SETTINGS,1)

typedef struct 
{
    NvU32   version;        //structure version 
    struct 
    {
        NvU32               currentLevel;           //the new value % of the cooler
        NV_COOLER_POLICY    currentPolicy;          //the new cooler control policy - auto-perf, auto-thermal, manual, hybrid...
    } cooler[NVAPI_MAX_COOLERS_PER_GPU];

} NV_GPU_SETCOOLER_LEVEL;

#define NV_GPU_SETCOOLER_LEVEL_VER  MAKE_NVAPI_VERSION(NV_GPU_SETCOOLER_LEVEL,1)

typedef struct
{
    NvU32               version;        //structure version
    NV_COOLER_POLICY    policy;         //selected policy to update the cooler levels for, example NVAPI_COOLER_POLICY_PERF
    struct 
    {
        NvU32 levelId;      // level indicator for a policy
        NvU32 currentLevel; // new cooler level for the selected policy level indicator.
        NvU32 defaultLevel; // default cooler level for the selected policy level indicator.
    } policyCoolerLevel[NVAPI_MAX_COOLER_LEVELS];
} NV_GPU_COOLER_POLICY_TABLE;

#define NV_GPU_COOLER_POLICY_TABLE_VER MAKE_NVAPI_VERSION(NV_GPU_COOLER_POLICY_TABLE,1)

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME:   NvAPI_GPU_GetCoolerSettings
//
// DESCRIPTION:     Retrieves the cooler information of all coolers or a specific cooler associated with the selected GPU.
//                  Coolers are indexed 0 to NVAPI_MAX_COOLERS_PER_GPU-1.
//                  To retrieve specific cooler info set the coolerIndex to the appropriate cooler index. 
//                  To retrieve info for all cooler set coolerIndex to NVAPI_COOLER_TARGET_ALL. 
//
// PARAMETERS :     hPhysicalGPU(IN) - GPU selection.
//                  coolerIndex(IN)  - Explict cooler index selection. 
//                  pCoolerInfo(OUT) - Array of cooler settings.
//
// RETURN STATUS: 
//    NVAPI_OK - completed request
//    NVAPI_ERROR - miscellaneous error occurred
//    NVAPI_INVALID_ARGUMENT - pCoolerInfo is NULL
//    NVAPI_HANDLE_INVALIDATED - handle passed has been invalidated (see user guide)
//    NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE - handle passed is not a physical GPU handle
//    NVAPI_INCOMPATIBLE_STRUCT_VERSION - the version of the INFO struct is not supported
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_GetCoolerSettings(NvPhysicalGpuHandle hPhysicalGpu, NvU32 coolerIndex, NV_GPU_GETCOOLER_SETTINGS *pCoolerInfo);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME:   NvAPI_GPU_SetCoolerLevels
//
// DESCRIPTION:     Set the cooler levels for all coolers or a specific cooler associated with the selected GPU.
//                  Coolers are indexed 0 to NVAPI_MAX_COOLERS_PER_GPU-1. Every cooler level with non-zero currentpolicy gets applied.           
//                  The new level should be in the range of minlevel and maxlevel retrieved from GetCoolerSettings API or between 
//                  and NVAPI_MIN_COOLER_LEVEL to MAX_COOLER_LEVEL.
//                  To set level for a specific cooler set the coolerIndex to the appropriate cooler index. 
//                  To set level for all coolers set coolerIndex to NVAPI_COOLER_TARGET_ALL. 
// NOTE:            To lock the fan speed independent of the temperature or performance changes set the cooler currentPolicy to 
//                  NVAPI_COOLER_POLICY_MANUAL else set it to the current policy retrieved from the GetCoolerSettings API.
// PARAMETERS:      hPhysicalGPU(IN) - GPU selection.
//                  coolerIndex(IN)  - Explict cooler index selection.
//                  pCoolerLevels(IN) - Updated cooler level and cooler policy.
//
// RETURN STATUS: 
//    NVAPI_OK - completed request
//    NVAPI_ERROR - miscellaneous error occurred
//    NVAPI_INVALID_ARGUMENT - pCoolerLevels is NULL
//    NVAPI_HANDLE_INVALIDATED - handle passed has been invalidated (see user guide)
//    NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE - handle passed is not a physical GPU handle
//    NVAPI_INCOMPATIBLE_STRUCT_VERSION - the version of the INFO struct is not supported
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_SetCoolerLevels(NvPhysicalGpuHandle hPhysicalGpu, NvU32 coolerIndex, NV_GPU_SETCOOLER_LEVEL *pCoolerLevels);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME:   NvAPI_GPU_RestoreCoolerSettings
//
// DESCRIPTION:     Restore the modified cooler settings to NVIDIA defaults.
//
// PARAMETERS:      hPhysicalGPU(IN) - GPU selection.
//                  pCoolerIndex(IN) - Array containing absolute cooler indexes to restore. Pass NULL restore all coolers.
//                  CoolerCount - Number of coolers to restore.
//
// RETURN STATUS: 
//    NVAPI_OK - completed request
//    NVAPI_ERROR - miscellaneous error occurred
//    NVAPI_HANDLE_INVALIDATED - handle passed has been invalidated (see user guide)
//    NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE - handle passed is not a physical GPU handle
//    NVAPI_INCOMPATIBLE_STRUCT_VERSION - the version of the INFO struct is not supported
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_RestoreCoolerSettings(NvPhysicalGpuHandle hPhysicalGpu, NvU32 *pCoolerIndex, NvU32 coolerCount);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME:   NvAPI_GPU_GetCoolerPolicyTable
//
// DESCRIPTION:     Retrieves the table of cooler and policy levels for the selected policy. Supported only for NVAPI_COOLER_POLICY_PERF.
//
// PARAMETERS:      hPhysicalGPU(IN) - GPU selection.
//                  coolerIndex(IN) - cooler index selection.
//                  pCoolerTable(OUT) - Table of policy levels and associated cooler levels.
//                  count(OUT) - Count of the number of valid levels for the selected policy.
//
// RETURN STATUS: 
//    NVAPI_OK - completed request
//    NVAPI_ERROR - miscellaneous error occurred
//    NVAPI_HANDLE_INVALIDATED - handle passed has been invalidated (see user guide)
//    NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE - handle passed is not a physical GPU handle
//    NVAPI_INCOMPATIBLE_STRUCT_VERSION - the version of the INFO struct is not supported
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_GetCoolerPolicyTable(NvPhysicalGpuHandle hPhysicalGpu, NvU32 coolerIndex, NV_GPU_COOLER_POLICY_TABLE *pCoolerTable, NvU32 *count);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME:   NvAPI_GPU_SetCoolerPolicyTable
//
// DESCRIPTION:     Restore the modified cooler settings to NVIDIA defaults. Supported only for NVAPI_COOLER_POLICY_PERF.
//
// PARAMETERS:      hPhysicalGPU(IN) - GPU selection.
//                  coolerIndex(IN) - cooler index selection.
//                  pCoolerTable(IN) - Updated table of policy levels and associated cooler levels. Every non-zero policy level gets updated.
//                  count(IN) - Number of valid levels in the policy table.
//
// RETURN STATUS: 
//    NVAPI_OK - completed request
//    NVAPI_ERROR - miscellaneous error occurred
//    NVAPI_HANDLE_INVALIDATED - handle passed has been invalidated (see user guide)
//    NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE - handle passed is not a physical GPU handle
//    NVAPI_INCOMPATIBLE_STRUCT_VERSION - the version of the INFO struct is not supported
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_SetCoolerPolicyTable(NvPhysicalGpuHandle hPhysicalGpu, NvU32 coolerIndex, NV_GPU_COOLER_POLICY_TABLE *pCoolerTable, NvU32 count);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME:   NvAPI_GPU_RestoreCoolerPolicyTable
//
// DESCRIPTION:     Restores the perf table policy levels to the defaults.
//
// PARAMETERS:      hPhysicalGPU(IN) - GPU selection.
//                  coolerIndex(IN) - cooler index selection.
//                  pCoolerIndex(IN) - Array containing absolute cooler indexes to restore. Pass NULL restore all coolers.
//                  coolerCount - Number of coolers to restore.
//                  policy - restore for the selected policy
//
// RETURN STATUS: 
//    NVAPI_OK - completed request
//    NVAPI_ERROR - miscellaneous error occurred
//    NVAPI_HANDLE_INVALIDATED - handle passed has been invalidated (see user guide)
//    NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE - handle passed is not a physical GPU handle
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_RestoreCoolerPolicyTable(NvPhysicalGpuHandle hPhysicalGpu, NvU32 *pCoolerIndex, NvU32 coolerCount, NV_COOLER_POLICY policy);

///////////////////////////////////////////////////////////////////////////////////
//  Thermal API
//  Provides ability to get temperature levels from the various thermal sensors associated with the GPU

#define NVAPI_MAX_THERMAL_SENSORS_PER_GPU 3

typedef enum 
{
    NVAPI_THERMAL_TARGET_NONE          = 0,
    NVAPI_THERMAL_TARGET_GPU           = 1,
    NVAPI_THERMAL_TARGET_MEMORY        = 2,
    NVAPI_THERMAL_TARGET_POWER_SUPPLY  = 4,
    NVAPI_THERMAL_TARGET_BOARD         = 8,
    NVAPI_THERMAL_TARGET_ALL           = 15,
    NVAPI_THERMAL_TARGET_UNKNOWN       = -1,
} NV_THERMAL_TARGET;

typedef enum
{
    NVAPI_THERMAL_CONTROLLER_NONE = 0,
    NVAPI_THERMAL_CONTROLLER_GPU_INTERNAL,  
    NVAPI_THERMAL_CONTROLLER_ADM1032,
    NVAPI_THERMAL_CONTROLLER_MAX6649,       
    NVAPI_THERMAL_CONTROLLER_MAX1617,      
    NVAPI_THERMAL_CONTROLLER_LM99,      
    NVAPI_THERMAL_CONTROLLER_LM89,         
    NVAPI_THERMAL_CONTROLLER_LM64,         
    NVAPI_THERMAL_CONTROLLER_ADT7473,
    NVAPI_THERMAL_CONTROLLER_SBMAX6649,
    NVAPI_THERMAL_CONTROLLER_VBIOSEVT,  
    NVAPI_THERMAL_CONTROLLER_OS,    
    NVAPI_THERMAL_CONTROLLER_UNKNOWN = -1,
} NV_THERMAL_CONTROLLER;

typedef struct
{
    NvU32   version;                //structure version 
    NvU32   count;                  //number of associated thermal sensors with the selected GPU
    struct 
    {
        NV_THERMAL_CONTROLLER       controller;         //internal, ADM1032, MAX6649...
        NvU32                       defaultMinTemp;     //the min default temperature value of the thermal sensor in degrees centigrade 
        NvU32                       defaultMaxTemp;    //the max default temperature value of the thermal sensor in degrees centigrade 
        NvU32                       currentTemp;       //the current temperature value of the thermal sensor in degrees centigrade 
        NV_THERMAL_TARGET           target;             //thermal senor targetted @ GPU, memory, chipset, powersupply, canoas...
    } sensor[NVAPI_MAX_THERMAL_SENSORS_PER_GPU];

} NV_GPU_THERMAL_SETTINGS;

#define NV_GPU_THERMAL_SETTINGS_VER  MAKE_NVAPI_VERSION(NV_GPU_THERMAL_SETTINGS,1)

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME:   NvAPI_GPU_GetThermalSettings
//
// DESCRIPTION:     Retrieves the thermal information of all thermal sensors or specific thermal sensor associated with the selected GPU.
//                  Thermal sensors are indexed 0 to NVAPI_MAX_THERMAL_SENSORS_PER_GPU-1.
//                  To retrieve specific thermal sensor info set the sensorIndex to the required thermal sensor index. 
//                  To retrieve info for all sensors set sensorIndex to NVAPI_THERMAL_TARGET_ALL. 
//
// PARAMETERS :     hPhysicalGPU(IN) - GPU selection.
//                  sensorIndex(IN)  - Explict thermal sensor index selection. 
//                  pThermalSettings(OUT) - Array of thermal settings.
//
// RETURN STATUS: 
//    NVAPI_OK - completed request
//    NVAPI_ERROR - miscellaneous error occurred
//    NVAPI_INVALID_ARGUMENT - pThermalInfo is NULL
//    NVAPI_HANDLE_INVALIDATED - handle passed has been invalidated (see user guide)
//    NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE - handle passed is not a physical GPU handle
//    NVAPI_INCOMPATIBLE_STRUCT_VERSION - the version of the INFO struct is not supported
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_GetThermalSettings(NvPhysicalGpuHandle hPhysicalGpu, NvU32 sensorIndex, NV_GPU_THERMAL_SETTINGS *pThermalSettings);

typedef enum _NV_DISPLAY_TV_FORMAT
{
    NV_DISPLAY_TV_FORMAT_NONE         = 0,
    NV_DISPLAY_TV_FORMAT_SD_NTSCM     = 0x00000001,
    NV_DISPLAY_TV_FORMAT_SD_NTSCJ     = 0x00000002,
    NV_DISPLAY_TV_FORMAT_SD_PALM      = 0x00000004,
    NV_DISPLAY_TV_FORMAT_SD_PALBDGH   = 0x00000008,
    NV_DISPLAY_TV_FORMAT_SD_PALN      = 0x00000010,
    NV_DISPLAY_TV_FORMAT_SD_PALNC     = 0x00000020,
    NV_DISPLAY_TV_FORMAT_SD_576i      = 0x00000100,
    NV_DISPLAY_TV_FORMAT_SD_480i      = 0x00000200,
    NV_DISPLAY_TV_FORMAT_ED_480p      = 0x00000400,
    NV_DISPLAY_TV_FORMAT_ED_576p      = 0x00000800,
    NV_DISPLAY_TV_FORMAT_HD_720p      = 0x00001000,
    NV_DISPLAY_TV_FORMAT_HD_1080i     = 0x00002000,
    NV_DISPLAY_TV_FORMAT_HD_1080p     = 0x00004000,
    NV_DISPLAY_TV_FORMAT_HD_720p50    = 0x00008000,
    NV_DISPLAY_TV_FORMAT_HD_1080p24   = 0x00010000,
    NV_DISPLAY_TV_FORMAT_HD_1080i50   = 0x00020000,

} NV_DISPLAY_TV_FORMAT;


typedef struct
{
    NvU32                   version;                            //[IN]       Structure version.
    NvU32                   supportedFormats;                   //[OUT only] One or more TV formats defined in NV_DISPLAY_TV_FORMAT matching, encoder supported formats for analog TVs or EDID exposed modes for digital TVs.
    NV_DISPLAY_TV_FORMAT    currentFormat;                      //[IN/OUT]   One of the selected TV output format from supportedFormats defined in NV_DISPLAY_TV_FORMAT.
    NV_GPU_CONNECTOR_TYPE   currentConnector;                   //[IN/OUT]   For Analog TV, valid TV output connector is one of the NVAPI_GPU_CONNECTOR_TV types.
                                                                //           For Digital TV, valid TV output connector is one of the NVAPI_GPU_CONNECTOR_DVI types.
} NV_DISPLAY_TV_OUTPUT_INFO;    

#define NV_DISPLAY_TV_OUTPUT_INFO_VER MAKE_NVAPI_VERSION(NV_DISPLAY_TV_OUTPUT_INFO, 1)

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_GetTVOutputInfo
//
//   DESCRIPTION: Retrieves the TV display output information of the selected display.
//
//    PARAMETERS: hNvDisplay(IN)    - NVIDIA Display selection. It can be NVAPI_DEFAULT_HANDLE or a handle enumerated from NvAPI_EnumNVidiaDisplayHandle().
//                outputId(IN)      - One of the selected outputId retrieved from NvAPI_GPU_GetActiveOutputs or NvAPI_GetView, to identify the targetted TV.
//                                    Can be NULL to auto pick the TV output associated with hNvDisplay.
//                                    The outputId has to be of type NVAPI_GPU_OUTPUT_TV or NVAPI_GPU_OUTPUT_DFP in case of digital HDTV.
//                pTVOutInfo(OUT)  -  The returned TV output information.
//                                    With digital HDTV, the supportedFormats are limited to the available EIA-861B modes in the EDID 
//                                    or the custom 861B modes if added by the user.
//
// RETURN STATUS: NVAPI_OK: call successful.
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found.
//                NVAPI_EXPECTED_DISPLAY_HANDLE: hNvDisplay is not a valid display handle.
//                NVAPI_INVALID_ARGUMENT: pTVOutInfo is NULL.
//                NVAPI_INCOMPATIBLE_STRUCT_VERSION: the version of the NV_DISPLAY_TV_OUTPUT_INFO_VER struct is not supported.
//                NVAPI_EXPECTED_TV_DISPLAY: expected TV output display in outputId.
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GetTVOutputInfo(NvDisplayHandle hNvDisplay, NvU32 outputId, NV_DISPLAY_TV_OUTPUT_INFO *pTVOutInfo);


///////////////////////////////////////////////////////////////////////////////////
//  GPU topology reconfiguration APIs.
//  Provides ability to define one or more SLI devices and standalone GPU topology.
//
//  NV_GPU_TOPOLOGY - this structure defines a set of all GPUs present in a system.  All GPUs with
//  the same parentNdx value describe a single logical GPU.  GPUs that have a unique parentNdx
//  represent standalone GPUs. 
//
//  The values returned in parentNdx are arbitrary.  They are only used to determine which
//  physical GPUs will belong to the same logical-GPU.
//   

typedef enum
{ 
    NV_GPU_TOPOLOGY_STATUS_OK                       = 0x00000000,//SLI is capable, topology "status" field indicates this state.
    NV_GPU_TOPOLOGY_STATUS_INVALID_GPU_COUNT        = 0x00000001,//SLI is NOT capable, "pStatus" param in NvAPI_GetValidGpuTopologies indicates these states.
    NV_GPU_TOPOLOGY_STATUS_OS_NOT_SUPPORTED         = 0x00000002,
    NV_GPU_TOPOLOGY_STATUS_OS_ERROR                 = 0x00000004,
    NV_GPU_TOPOLOGY_STATUS_NO_VIDLINK               = 0x00000008,
    NV_GPU_TOPOLOGY_STATUS_INSUFFICIENT_LINK_WIDTH  = 0x00000010,
    NV_GPU_TOPOLOGY_STATUS_CPU_NOT_SUPPORTED        = 0x00000020,
    NV_GPU_TOPOLOGY_STATUS_GPU_NOT_SUPPORTED        = 0x00000040,
    NV_GPU_TOPOLOGY_STATUS_BUS_NOT_SUPPORTED        = 0x00000080,
    NV_GPU_TOPOLOGY_STATUS_NON_APPROVED_CHIPSET     = 0x00000100,
    NV_GPU_TOPOLOGY_STATUS_VBIOS_NOT_SUPPORTED      = 0x00000200,
    NV_GPU_TOPOLOGY_STATUS_GPU_MISMATCH             = 0x00000400,
    NV_GPU_TOPOLOGY_STATUS_ARCH_MISMATCH            = 0x00000800,
    NV_GPU_TOPOLOGY_STATUS_IMPL_MISMATCH            = 0x00001000,
    NV_GPU_TOPOLOGY_STATUS_REV_MISMATCH             = 0x00002000,
    NV_GPU_TOPOLOGY_STATUS_NON_PCIE_BUS             = 0x00004000,
    NV_GPU_TOPOLOGY_STATUS_FB_MISMATCH              = 0x00008000,
    NV_GPU_TOPOLOGY_STATUS_VBIOS_MISMATCH           = 0x00010000,
    NV_GPU_TOPOLOGY_STATUS_QUADRO_MISMATCH          = 0x00020000,
    NV_GPU_TOPOLOGY_STATUS_BUS_TOPOLOGY_ERROR       = 0x00040000,
} NV_GPU_TOPOLOGY_STATUS_FLAGS;


typedef enum
{ 
    NV_SET_GPU_TOPOLOGY_DEFER_APPLY                 = 0x00000001,//calling application controls the reload of the display driver
    NV_SET_GPU_TOPOLOGY_DEFER_3D_APP_SHUTDOWN       = 0x00000002,//calling application will control the shutdown of 3d applications holding hw resources
    NV_SET_GPU_TOPOLOGY_DEFER_DISPLAY_RECONFIG      = 0x00000004,//calling application will control the display configuration required for the settopology to work
} NV_SET_GPU_TOPOLOGY_FLAGS;

typedef enum
{
    NV_GPU_TOPOLOGY_ACTIVE                          = 0x00000001,//topology "flags" field indicate if SLI is ACTIVE on this topology of gpus. Read only.
    NV_GPU_TOPOLOGY_VIDLINK_PRESENT                 = 0x00000002,//topology "flags" field indicate if Video link is present.
    NV_GPU_TOPOLOGY_MULTIGPU                        = 0x00000004,//topology "flags" field indicate that this is a Multi-GPU configuration
    NV_GPU_TOPOLOGY_COMPUTE                         = 0x00010000,//reserve the gpus in this topology for compute. 
    NV_GPU_TOPOLOGY_SLIMULTIMON                     = 0x00020000,//this topology allows multi display SLI output. 
} NV_GPU_TOPOLOGY_FLAGS;

typedef struct
{
    NvU32                   version;                                    //structure version
    NvU32                   gpuCount;                                   //count of GPUs in this topology
    NvPhysicalGpuHandle     hPhysicalGpu[NVAPI_MAX_GPU_PER_TOPOLOGY];   //array of GPU handles
    NvU32                   displayGpuIndex;                            //index of the display GPU owner in the gpu array
    NvU32                   displayOutputTargetMask;                    //target device mask 
    NvU32                   flags;                                      //one or more topology flags from NV_GPU_TOPOLOGY_FLAGS
    NvU32                   status;                                     //indicates one of the flags in NV_GPU_TOPOLOGY_STATUS_FLAGS
} NV_GPU_TOPOLOGY;

#define NV_GPU_TOPOLOGY_VER  MAKE_NVAPI_VERSION(NV_GPU_TOPOLOGY,1)

typedef struct 
{
    NvU32                   version;                                    //structure version
    NvU32                   gpuTopoCount;                               //count of valid topologies
    NV_GPU_TOPOLOGY         gpuTopo[NVAPI_MAX_AVAILABLE_GPU_TOPOLOGIES];//max gputopologies
} NV_GPU_VALID_GPU_TOPOLOGIES;
#define NV_GPU_VALID_GPU_TOPOLOGIES_VER  MAKE_NVAPI_VERSION(NV_GPU_VALID_GPU_TOPOLOGIES,1)


///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME:   NvAPI_GetValidGpuTopologies
//
// DESCRIPTION:     This API returns all valid GPU topologies that can be used to configure the physical GPUs 
//                  using the NvAPI_SetGpuTopologies API. Also returns the current active topologies.
// 
//                  This call returns an array of NV_GPU_TOPOLOGY structs; one for each valid configuration 
//                  of GPUs present in the system.  Note that this list is constant while GPUs remain in the 
//                  same slots in the system.  It is not affected by which GPUs are presently in use.
//                  NV_GPU_TOPOLOGY.displayGpuIndex returned will match the boot GPU if it exists as an active topology.  
//                  If it not an active topology, it points to the "first" GPU that has a display monitor connected.
//
// PARAMETERS:      ptopology(OUT):  An array of *pCount topology structures. Use NvAPI_SetGpuTopology() to set up
//                  one of these GPU topologies.
//                  pStatus(OUT): Any system status returned in case zero topology is retrieved. 
//                  System status is one or more flags in NV_GPU_TOPOLOGY_STATUS_FLAGS when SLI is NOT capable.
//
// RETURN STATUS    NVAPI_OK: Call succeeded; 1 or more GPU topologies were returned
//                  NVAPI_INVALID_ARGUMENT: one or more args are invalid
//                  NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GetValidGpuTopologies(NV_GPU_VALID_GPU_TOPOLOGIES *pTopology, NvU32 *pStatus);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME:   NvAPI_SetGpuTopologies
//
// DESCRIPTION:     This API lets the caller configure the physical GPUs in the system into one or more logical devices
//                  defined by the NV_GPU_TOPOLOGY structure.
//                  Recommended that the caller application:
//                  - save the current GPU topology retrieved from NvAPI_EnumLogicalGPUs and NvAPI_GetPhysicalGPUsFromLogicalGPU APIs.
//                  - save the current view state for associated displays on these GPUs using the GetView and GetDisplayTargets APIs.
//                  - set NV_GPU_TOPOLOGY.displayGpuIndex to the GPU index in the topology with an active display connection.
//                  - if DEFER_3D_APP_SHUTDOWN is not set notify the user that all 3D application will be forced to close.
//                  - itself does not create 3D handles or objects that can block the topology transition.
//
// PARAMETERS:      pTopology(IN) - a pointer to a array of structure definining the desired GPU topology retrieved 
//                  from NvAPI_GetValidGpuTopologies
//                  flags(IN) - See NV_SET_GPU_TOPOLOGY_FLAGS
//
// RETURN STATUS:   NVAPI_OK: Call succeeded. pTopology.gputTopo[].flags indicates the new status. Reenum all GPU handles after this call.
//                  NVAPI_INVALID_ARGUMENT: one or more args are invalid.
//                  NVAPI_NVIDIA_DEVICE_NOT_FOUND: no NVidia GPU driving a display was found.
//                  NVAPI_ERROR:check the status returned in pTopology->status.
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_SetGpuTopologies(NV_GPU_VALID_GPU_TOPOLOGIES *pTopology, NvU32 flags);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME:  NvAPI_GPU_GetPerGpuTopologyStatus
//
// DESCRIPTION:    Returns per GPU topology state flags from NV_GPU_TOPOLOGY_STATUS_FLAGS for the queried GPU handle.
//
// PARAMETERS:     hPhysicalGPU(IN) - GPU selection.
//                 pStatus(OUT)     - Indicates one or more flags from NV_GPU_TOPOLOGY_STATUS_FLAGS which are the subset of the 
//                                    same flags retrieved from NV_GPU_TOPOLOGY.status or pStatus in NvAPI_GetValidGpuTopologies API.
//                 Note: The per GPU topology status can be queried independent of the whether the queried GPU is part of a topology or not.
//
// RETURN STATUS: 
//    NVAPI_OK - completed request
//    NVAPI_ERROR - miscellaneous error occurred
//    NVAPI_HANDLE_INVALIDATED - handle passed has been invalidated (see user guide)
//    NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE - handle passed is not a physical GPU handle
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_GetPerGpuTopologyStatus(NvPhysicalGpuHandle hPhysicalGpu, NvU32 *pStatus);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME:  NvAPI_GPU_GetAllGpusOnSameBoard
//
// DESCRIPTION:    Returns a set of GPUs handles that exists on the same board as the queried GPU handle.
//
// PARAMETERS:     hPhysicalGPU(IN) - GPU selection.
//                 nvGPUHandle      - The associated GPUs on the same board as the queried GPU. This array includes the queried GPU handle too.
//                 pGpuCount        - The count of GPUs that exists on the same board. This count includes the queried GPU handle too.
//
// RETURN STATUS: 
//    NVAPI_OK - completed request
//    NVAPI_ERROR - miscellaneous error occurred
//    NVAPI_HANDLE_INVALIDATED - handle passed has been invalidated (see user guide)
//    NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE - handle passed is not a physical GPU handle
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_GetAllGpusOnSameBoard(NvPhysicalGpuHandle hPhysicalGpu, NvPhysicalGpuHandle nvGPUHandle[NVAPI_MAX_PHYSICAL_GPUS], NvU32 *pGpuCount);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_SYS_GetChipSetTopologyStatus
//
// DESCRIPTION:   Returns topology state flags from NV_GPU_TOPOLOGY_STATUS_FLAGS possible with the System ChipSet.
//
// PARAMETERS:    pStatus(OUT)     - Indicates one or more flags from NV_GPU_TOPOLOGY_STATUS_FLAGS which are the subset of the 
//                                   same flags retrieved from NV_GPU_TOPOLOGY.status or pStatus in NvAPI_GetValidGpuTopologies API.
// RETURN STATUS: 
//    NVAPI_OK    - completed request
//    NVAPI_ERROR - miscellaneous error occurred
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_SYS_GetChipSetTopologyStatus(NvU32 *pStatus);

// End of GPU topology configuration APIs

///////////////////////////////////////////////////////////////////////////////////
//  I2C API
//  Provides ability to read or wirte data using I2C protocol.

#define NVAPI_MAX_SIZEOF_I2C_DATA_BUFFER 256
#define NVAPI_NO_PORTID_FOUND 5
#define NVAPI_DISPLAY_DEVICE_MASK_MAX 24

typedef struct 
{
    NvU32                   version;        //structure version 
    NvU32                   displayMask;    //the Display Mask of the concerned display
    NvU8                    bIsDDCPort;     //Flag indicating DDC port or a communication port
    NvU8                    i2cDevAddress;  //the I2C target device address
    NvU8*                   pbI2cRegAddress;//the I2C target register address  
    NvU32                   regAddrSize;    //the size in bytes of target register address  
    NvU8*                   pbData;         //The buffer of data which is to be read/written
    NvU32                   cbSize;         //The size of Data buffer to be read.
    NvU32                   i2cSpeed;       //The speed at which the transaction is be made(between 28kbps to 40kbps)
} NV_I2C_INFO;

#define NV_I2C_INFO_VER  MAKE_NVAPI_VERSION(NV_I2C_INFO,1)
/***********************************************************************************/

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME:  NvAPI_I2CRead
//
// DESCRIPTION:    Read data buffer from I2C port
//
// PARAMETERS:     hPhysicalGPU(IN) - GPU selection.
//                 NV_I2C_INFO *pI2cInfo -The I2c data input stucture
//
// RETURN STATUS: 
//    NVAPI_OK - completed request
//    NVAPI_ERROR - miscellaneous error occurred
//    NVAPI_HANDLE_INVALIDATED - handle passed has been invalidated (see user guide)
//    NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE - handle passed is not a physical GPU handle
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_I2CRead(NvPhysicalGpuHandle hPhysicalGpu, NV_I2C_INFO *pI2cInfo);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME:  NvAPI_I2CWrite
//
// DESCRIPTION:    Writes data buffer to I2C port
//
// PARAMETERS:     hPhysicalGPU(IN) - GPU selection.
//                 NV_I2C_INFO *pI2cInfo -The I2c data input stucture
//
// RETURN STATUS: 
//    NVAPI_OK - completed request
//    NVAPI_ERROR - miscellaneous error occurred
//    NVAPI_HANDLE_INVALIDATED - handle passed has been invalidated (see user guide)
//    NVAPI_EXPECTED_PHYSICAL_GPU_HANDLE - handle passed is not a physical GPU handle
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_I2CWrite(NvPhysicalGpuHandle hPhysicalGpu, NV_I2C_INFO *pI2cInfo);

// END OF I2C API


///////////////////////////////////////////////////////////////////////////////
// POWERMIZER APIs
//
// Provides the ability to Limit PowerMizer's Maximum Performance.
// Grants access on Adaptive Clocking turn on and off.
// PowerMizer can be either Soft Limited or Hard Limited.
// Soft Limit can be exceeded by the adaptive systems in the GPU if there is a need.
// Hard Limit cannot be exceeded even if there is a need to exceed this limit.
//
//////////////////////////////////////////////////////////////////////////////
typedef enum _NV_LEVEL_INFO
{
    NVAPI_PWR_MZR_HARD_LIMIT_MAX                        = 0x00000001,       // Power Mizer Maximum Performance for Hard limit
    NVAPI_PWR_MZR_HARD_LIMIT_BAL                        = 0x00000002,       // Power Mizer Balanced Performance for Hard limit
    NVAPI_PWR_MZR_MAX_BATT                              = 0x00000003,       // Power Mizer Maximum Battery Performance
    NVAPI_PWR_MZR_SOFT_LIMIT_MAX                        = 0x00000004,       // Power mizer Maximum performance for Soft limit
    NVAPI_PWR_MZR_SOFT_LIMIT_BAL                        = 0x00000005,       // Power mizer Balanced performance for Soft limit
    NVAPI_ADC_OFF                                       = 0x00000006,       // Adaptive Clocking Disable
    NVAPI_ADC_ON                                        = 0x00000007,       // Adaptive Clocking Enable
} NV_LEVEL_INFO;


typedef enum _NV_PWR_SOURCE_INFO
{
    NVAPI_PWR_SOURCE_AC                                 = 0x00000001,       // Power source AC
    NVAPI_PWR_SOURCE_BATT                               = 0x00000002,       // Power source Battery
} NV_PWR_SOURCE_INFO;

typedef enum _NV_SELECT_INFO
{
    NVAPI_INDEX_PWR_MZR_HARD                            = 0x00000001,       // To set/get PowerMizer Hard limits. Hard limits modifies the hardware limits. 
    NVAPI_INDEX_PWR_MZR_SOFT                            = 0x00000002,       // To set/get PowerMizer Soft limits. Soft limits sets the application preference and could be exceeded upto Hard limits if required by the system.
    NVAPI_INDEX_ADC                                     = 0x00000003,       // To set/get Adaptive Clocking parameters where the driver automatically selects the limits. 
} NV_SELECT_INFO;


///////////////////////////////////////////////////////////////////////////////
// FUNCTION NAME:    NvAPI_GPU_GetPowerMizerInfo
//
// DESCRIPTION:      Gets the PowerMizer Maximum Limit for Battery or AC 
//                   Gets the Adaptive Clocking status for Battery or AC 
//
// PARAMETERS:       hPhysicalGPU(IN)         GPU selection.
//                   powerSource              Power source selection with one of the values from NV_PWR_SOURCE_INFO.
//                   select                   PowerMizer type selection with one of the values from NV_SELECT_INFO.
//                   pLevel                   Pointer to return value 
//
// RETURN STATUS:  
//                   NVAPI_OK                 completed request
//                   NVAPI_NOT_SUPPORTED      Power Mizer Not supported
//                   NVAPI_ERROR              Invalid return to API
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_GetPowerMizerInfo(NvPhysicalGpuHandle hPhysicalGpu, NV_PWR_SOURCE_INFO powerSourceInfo,
                                          NV_SELECT_INFO select, NV_LEVEL_INFO *pLevelInfo);


///////////////////////////////////////////////////////////////////////////////
// FUNCTION NAME:    NvAPI_GPU_SetPowerMizerInfo
//
// DESCRIPTION:      Sets the PowerMizer Maximum Limit for both Battery and/or AC 
//
// PARAMETERS:       hPhysicalGPU(IN)          GPU selection
//                   powerSource               Power source selection with one of the values from NV_PWR_SOURCE_INFO.
//                   select                    PowerMizer type selection with one of the values from NV_SELECT_INFO.
//                   level                     Level that has to be set on PwrMzr /Adaptive clocking
//
// RETURN STATUS:    NVAPI_OK                  completed request
//                   NVAPI_NOT_SUPPORTED       PowerMizer Not supported
//                   NVAPI_INVALID_ARGUMENT    Invalid arguments
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GPU_SetPowerMizerInfo(NvPhysicalGpuHandle hPhysicalGpu, NV_PWR_SOURCE_INFO powerSourceInfo,
                                         NV_SELECT_INFO select, NV_LEVEL_INFO levelInfo);

 

typedef struct
{
    NvU32               version;        //structure version
    NvU32               vendorId;       //vendorId
    NvU32               deviceId;       //deviceId
    NvAPI_ShortString   szVendorName;   //vendor Name
    NvAPI_ShortString   szChipsetName;  //device Name
} NV_CHIPSET_INFO;

#define NV_CHIPSET_INFO_VER  MAKE_NVAPI_VERSION(NV_CHIPSET_INFO,1)

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_SYS_GetChipSetInfo
//
//   DESCRIPTION: Returns information about the System's ChipSet
//
// RETURN STATUS: NVAPI_INVALID_ARGUMENT: pChipSetInfo is NULL
//                NVAPI_OK: *pChipSetInfo is now set
//                NVAPI_INCOMPATIBLE_STRUCT_VERSION - NV_CHIPSET_INFO version not compatible with driver
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_SYS_GetChipSetInfo(NV_CHIPSET_INFO *pChipSetInfo);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_OGL_ExpertModeSet[Get]
//
//   DESCRIPTION: Configure OpenGL Expert Mode, an API usage feedback and
//                advice reporting mechanism. The effects of this call are
//                applied only to the current context, and are reset to the
//                defaults when the context is destroyed.
//
//                Note: These functions are valid only for the current OpenGL
//                      context. Calling these functions prior to creating a
//                      context and calling MakeCurrent with it will result
//                      in errors and undefined behavior.
//
//    PARAMETERS: expertDetailLevel Value which specifies the detail level in
//                                  the feedback stream. Set the detail level
//                                  to anything greater than zero to enable
//                                  output. A value of zero disables output.
//                                  Meaningful values range by 10s from 0-30.
//
//                expertReportMask  Mask made up of NVAPI_OGLEXPERT_REPORT bits,
//                                  this parameter specifies the areas of
//                                  fuctional interest.
//
//                expertOutputMask  Mask made up of NVAPI_OGLEXPERT_OUTPUT bits,
//                                  this parameter specifies the feedback output
//                                  location.
//
//                expertCallback    Used in conjunction with OUTPUT_TO_CALLBACK,
//                                  this is a simple callback function the user
//                                  may use to obtain the feedback stream. The
//                                  function will be called once per fully
//                                  qualified feedback stream extry.
//
// RETURN STATUS: NVAPI_API_NOT_INTIALIZED         : NVAPI not initialized
//                NVAPI_NVIDIA_DEVICE_NOT_FOUND    : no NVIDIA GPU found
//                NVAPI_OPENGL_CONTEXT_NOT_CURRENT : no NVIDIA OpenGL context
//                                                   which supports GLExpert
//                                                   has been made current
//                NVAPI_ERROR : OpenGL driver failed to load properly
//                NVAPI_OK    : success
//
///////////////////////////////////////////////////////////////////////////////
#define NVAPI_OGLEXPERT_REPORT_NONE          0x00000000
#define NVAPI_OGLEXPERT_REPORT_ERROR         0x00000001
#define NVAPI_OGLEXPERT_REPORT_SWFALLBACK    0x00000002
#define NVAPI_OGLEXPERT_REPORT_PROGRAM       0x00000004
#define NVAPI_OGLEXPERT_REPORT_VBO           0x00000008
#define NVAPI_OGLEXPERT_REPORT_FBO           0x00000010
#define NVAPI_OGLEXPERT_REPORT_ALL           0xFFFFFFFF

#define NVAPI_OGLEXPERT_OUTPUT_TO_NONE       0x00000000
#define NVAPI_OGLEXPERT_OUTPUT_TO_CONSOLE    0x00000001
#define NVAPI_OGLEXPERT_OUTPUT_TO_DEBUGGER   0x00000004
#define NVAPI_OGLEXPERT_OUTPUT_TO_CALLBACK   0x00000008
#define NVAPI_OGLEXPERT_OUTPUT_TO_ALL        0xFFFFFFFF

typedef void (* NVAPI_OGLEXPERT_CALLBACK)(unsigned int, unsigned int, const char *);

NVAPI_INTERFACE NvAPI_OGL_ExpertModeSet(NvU32 expertDetailLevel,
                                        NvU32 expertReportMask,
                                        NvU32 expertOutputMask,
                     NVAPI_OGLEXPERT_CALLBACK expertCallback);

NVAPI_INTERFACE NvAPI_OGL_ExpertModeGet(NvU32 *pExpertDetailLevel,
                                        NvU32 *pExpertReportMask,
                                        NvU32 *pExpertOutputMask,
                     NVAPI_OGLEXPERT_CALLBACK *pExpertCallback);

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME: NvAPI_OGL_ExpertModeDefaultsSet[Get]
//
//   DESCRIPTION: Configure OpenGL Expert Mode global defaults. These settings
//                apply to any OpenGL application which starts up after these
//                values are applied (i.e. these settings *do not* apply to
//                currently running applications).
//
//    PARAMETERS: expertDetailLevel Value which specifies the detail level in
//                                  the feedback stream. Set the detail level
//                                  to anything greater than zero to enable
//                                  output. A value of zero disables output.
//                                  Meaningful values range by 10s from 0-30.
//
//                expertReportMask  Mask made up of NVAPI_OGLEXPERT_REPORT bits,
//                                  this parameter specifies the areas of
//                                  fuctional interest.
//
//                expertOutputMask  Mask made up of NVAPI_OGLEXPERT_OUTPUT bits,
//                                  this parameter specifies the feedback output
//                                  location. Note that using OUTPUT_TO_CALLBACK
//                                  here is meaningless and has no effect, but
//                                  using it will not cause an error.
//
// RETURN STATUS: NVAPI_API_NOT_INTIALIZED or NVAPI_ERROR or NVAPI_OK
//
///////////////////////////////////////////////////////////////////////////////

NVAPI_INTERFACE NvAPI_OGL_ExpertModeDefaultsSet(NvU32 expertDetailLevel,
                                                NvU32 expertReportMask,
                                                NvU32 expertOutputMask);

NVAPI_INTERFACE NvAPI_OGL_ExpertModeDefaultsGet(NvU32 *pExpertDetailLevel,
                                                NvU32 *pExpertReportMask,
                                                NvU32 *pExpertOutputMask);

#define NVAPI_MAX_VIEW_TARGET  2

typedef enum _NV_TARGET_VIEW_MODE
{
    NV_VIEW_MODE_STANDARD  = 0,
    NV_VIEW_MODE_CLONE     = 1,
    NV_VIEW_MODE_HSPAN     = 2,
    NV_VIEW_MODE_VSPAN     = 3,
    NV_VIEW_MODE_DUALVIEW  = 4,
    NV_VIEW_MODE_MULTIVIEW = 5,
} NV_TARGET_VIEW_MODE;


// Following definitions are used in NvAPI_SetViewEx.
// Scaling modes
typedef enum _NV_SCALING
{
    NV_SCALING_DEFAULT          = 0,        // No change
    NV_SCALING_MONITOR_SCALING  = 1,
    NV_SCALING_ADAPTER_SCALING  = 2,
    NV_SCALING_CENTERED         = 3,
    NV_SCALING_ASPECT_SCALING   = 5,
    NV_SCALING_CUSTOMIZED       = 255       // For future use
} NV_SCALING;

// Rotate modes
typedef enum _NV_ROTATE
{
    NV_ROTATE_0           = 0,
    NV_ROTATE_90          = 1,
    NV_ROTATE_180         = 2,
    NV_ROTATE_270         = 3
} NV_ROTATE;

// Color formats
typedef enum _NV_FORMAT
{
    NV_FORMAT_UNKNOWN       =  0,       // unknown. Driver will choose one as following value.
    NV_FORMAT_P8            = 41,       // for 8bpp mode
    NV_FORMAT_R5G6B5        = 23,       // for 16bpp mode
    NV_FORMAT_A8R8G8B8      = 21,       // for 32bpp mode
    NV_FORMAT_A16B16G16R16F = 113       // for 64bpp(floating point) mode.
} NV_FORMAT;

// TV standard



///////////////////////////////////////////////////////////////////////////////
// FUNCTION NAME:   NvAPI_SetView
//
// DESCRIPTION:     This API lets caller to modify target display arrangement for selected source display handle in any of the nview modes.
//                  It also allows to modify or extend the source display in dualview mode.
//
// PARAMETERS:      hNvDisplay(IN) - NVIDIA Display selection. NVAPI_DEFAULT_HANDLE not allowed, it has to be a handle enumerated with NvAPI_EnumNVidiaDisplayHandle().
//                  pTargetInfo(IN) - Pointer to array of NV_VIEW_TARGET_INFO, specifying device properties in this view.
//                                    The first device entry in the array is the physical primary.
//                                    The device entry with the lowest source id is the desktop primary.
//                  targetCount(IN) - Count of target devices specified in pTargetInfo.
//                  targetView(IN) - Target view selected from NV_TARGET_VIEW_MODE.
//
// RETURN STATUS: 
//                  NVAPI_OK - completed request
//                  NVAPI_ERROR - miscellaneous error occurred
//                  NVAPI_INVALID_ARGUMENT: Invalid input parameter.
//
///////////////////////////////////////////////////////////////////////////////

typedef struct
{
    NvU32 version;     // (IN) structure version
    NvU32 count;       // (IN) target count
    struct 
    {
        NvU32 deviceMask;   // (IN/OUT) device mask
        NvU32 sourceId;     // (IN/OUT) source id
        NvU32 bPrimary:1;   // (OUT) Indicates if this is the GPU's primary view target. This is not the desktop GDI primary.
                            // NvAPI_SetView automatically selects the first target in NV_VIEW_TARGET_INFO index 0 as the GPU's primary view.
        NvU32 bInterlaced:1;// (IN/OUT) Indicates if the timing being used on this monitor is interlaced
        NvU32 bGDIPrimary:1;// (IN/OUT) Indicates if this is the desktop GDI primary.
    } target[NVAPI_MAX_VIEW_TARGET];
} NV_VIEW_TARGET_INFO; 

#define NV_VIEW_TARGET_INFO_VER  MAKE_NVAPI_VERSION(NV_VIEW_TARGET_INFO,2)

NVAPI_INTERFACE NvAPI_SetView(NvDisplayHandle hNvDisplay, NV_VIEW_TARGET_INFO *pTargetInfo, NV_TARGET_VIEW_MODE targetView);

///////////////////////////////////////////////////////////////////////////////
// FUNCTION NAME:   NvAPI_GetView
//
// DESCRIPTION:     This API lets caller retrieve the target display arrangement for selected source display handle.
//
// PARAMETERS:      hNvDisplay(IN) - NVIDIA Display selection. NVAPI_DEFAULT_HANDLE not allowed, it has to be a handle enumerated with NvAPI_EnumNVidiaDisplayHandle().
//                  pTargetInfo(OUT) - User allocated storage to retrieve an array of  NV_VIEW_TARGET_INFO. Can be NULL to retrieve the targetCount.
//                  targetMaskCount(IN/OUT) - Count of target device mask specified in pTargetMask.
//                  targetView(OUT) - Target view selected from NV_TARGET_VIEW_MODE.
//
// RETURN STATUS: 
//                  NVAPI_OK - completed request
//                  NVAPI_ERROR - miscellaneous error occurred
//                  NVAPI_INVALID_ARGUMENT: Invalid input parameter.
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GetView(NvDisplayHandle hNvDisplay, NV_VIEW_TARGET_INFO *pTargets, NvU32 *pTargetMaskCount, NV_TARGET_VIEW_MODE *pTargetView);

///////////////////////////////////////////////////////////////////////////////
// FUNCTION NAME:   NvAPI_SetViewEx
//
// DESCRIPTION:     This API lets caller to modify the display arrangement for selected source display handle in any of the nview modes.
//                  It also allows to modify or extend the source display in dualview mode.
//
// PARAMETERS:      hNvDisplay(IN) - NVIDIA Display selection. NVAPI_DEFAULT_HANDLE not allowed, it has to be a handle enumerated with NvAPI_EnumNVidiaDisplayHandle().
//                  pPathInfo(IN)  - Pointer to array of NV_VIEW_PATH_INFO, specifying device properties in this view.
//                                    The first device entry in the array is the physical primary.
//                                    The device entry with the lowest source id is the desktop primary.
//                  pathCount(IN)  - Count of paths specified in pPathInfo.
//                  displayView(IN)- Display view selected from NV_TARGET_VIEW_MODE.
//
// RETURN STATUS: 
//                  NVAPI_OK - completed request
//                  NVAPI_ERROR - miscellaneous error occurred
//                  NVAPI_INVALID_ARGUMENT: Invalid input parameter.
//
///////////////////////////////////////////////////////////////////////////////

#define NVAPI_MAX_DISPLAY_PATH  NVAPI_MAX_VIEW_TARGET

typedef struct
{
    NvU32 version;     // (IN) structure version
    NvU32 count;       // (IN) path count
    struct 
    {
        NvU32                   deviceMask;     // (IN) device mask
        NvU32                   sourceId;       // (IN) source id
        NvU32                   bPrimary:1;     // (IN/OUT) Indicates if this is the GPU's primary view target. This is not the desktop GDI primary.
                                                // NvAPI_SetViewEx automatically selects the first target in NV_DISPLAY_PATH_INFO index 0 as the GPU's primary view.
        NV_GPU_CONNECTOR_TYPE   connector;      // (IN) Specify connector type. For TV only.
        
        // source mode information
        NvU32                   width;          // (IN) width of the mode
        NvU32                   height;         // (IN) height of the mode
        NvU32                   depth;          // (IN) depth of the mode
        NV_FORMAT               colorFormat;    //      color format if needs to specify. Not used now.
        
        //rotation setting of the mode
        NV_ROTATE               rotation;       // (IN) rotation setting.
      
        // the scaling mode
        NV_SCALING              scaling;        // (IN) scaling setting

        // Timing info
        NvU32                   refreshRate;    // (IN) refresh rate of the mode
        NvU32                   interlaced:1;   // (IN) interlaced mode flag

        NV_DISPLAY_TV_FORMAT    tvFormat;       // (IN) to choose the last TV format set this value to NV_DISPLAY_TV_FORMAT_NONE

        // Windows desktop position
        NvU32                   posx;           // (IN/OUT) x offset of this display on the Windows desktop
        NvU32                   posy;           // (IN/OUT) y offset of this display on the Windows desktop
        NvU32                   bGDIPrimary:1;  // (IN/OUT) Indicates if this is the desktop GDI primary.

    } path[NVAPI_MAX_DISPLAY_PATH];
} NV_DISPLAY_PATH_INFO; 

#define NV_DISPLAY_PATH_INFO_VER  MAKE_NVAPI_VERSION(NV_DISPLAY_PATH_INFO,2)

NVAPI_INTERFACE NvAPI_SetViewEx(NvDisplayHandle hNvDisplay, NV_DISPLAY_PATH_INFO *pPathInfo, NV_TARGET_VIEW_MODE displayView);

///////////////////////////////////////////////////////////////////////////////
// FUNCTION NAME:   NvAPI_GetViewEx
//
// DESCRIPTION:     This API lets caller retrieve the target display arrangement for selected source display handle.
//
// PARAMETERS:      hNvDisplay(IN) - NVIDIA Display selection. NVAPI_DEFAULT_HANDLE not allowed, it has to be a handle enumerated with NvAPI_EnumNVidiaDisplayHandle().
//                  pPathInfo(IN/OUT) - count field should be set to NVAPI_MAX_DISPLAY_PATH. Can be NULL to retrieve just the pathCount.
//                  pPathCount(IN/OUT) - Number of elements in array pPathInfo->path.
//                  pTargetViewMode(OUT)- Display view selected from NV_TARGET_VIEW_MODE.
//
// RETURN STATUS: 
//                  NVAPI_OK - completed request
//                  NVAPI_API_NOT_INTIALIZED - NVAPI not initialized
//                  NVAPI_ERROR - miscellaneous error occurred
//                  NVAPI_INVALID_ARGUMENT - Invalid input parameter.
//                  NVAPI_EXPECTED_DISPLAY_HANDLE - hNvDisplay is not a valid display handle.
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GetViewEx(NvDisplayHandle hNvDisplay, NV_DISPLAY_PATH_INFO *pPathInfo, NvU32 *pPathCount, NV_TARGET_VIEW_MODE *pTargetViewMode);

///////////////////////////////////////////////////////////////////////////////
// FUNCTION NAME:   NvAPI_GetSupportedViews
//
// DESCRIPTION:     This API lets caller enumerate all the supported NVIDIA display views - nview and dualview modes.
//
// PARAMETERS:      hNvDisplay(IN) - NVIDIA Display selection. It can be NVAPI_DEFAULT_HANDLE or a handle enumerated from NvAPI_EnumNVidiaDisplayHandle().
//                  pTargetViews(OUT) - Array of supported views. Can be NULL to retrieve the pViewCount first.
//                  pViewCount(IN/OUT) - Count of supported views.
//
// RETURN STATUS: 
//                  NVAPI_OK - completed request
//                  NVAPI_ERROR - miscellaneous error occurred
//                  NVAPI_INVALID_ARGUMENT: Invalid input parameter.
//
///////////////////////////////////////////////////////////////////////////////
NVAPI_INTERFACE NvAPI_GetSupportedViews(NvDisplayHandle hNvDisplay, NV_TARGET_VIEW_MODE *pTargetViews, NvU32 *pViewCount);


#ifdef __cplusplus
}; //extern "C" {
#endif

#pragma pack(pop)

#endif // _NVAPI_H
