/**********************************************************************
 *
 * Copyright(c) 2008 Imagination Technologies Ltd. All rights reserved.
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope it will be useful but, except 
 * as otherwise stated in writing, without any warranty; without even the 
 * implied warranty of merchantability or fitness for a particular purpose. 
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 * 
 * The full GNU General Public License is included in this distribution in
 * the file called "COPYING".
 *
 * Contact Information:
 * Imagination Technologies Ltd. <gpl-support@imgtec.com>
 * Home Park Estate, Kings Langley, Herts, WD4 8LZ, UK 
 *
 ******************************************************************************/

#ifndef __SGXAPI_KM_H__
#define __SGXAPI_KM_H__

#if defined (__cplusplus)
extern "C" {
#endif

#include "sgxdefs.h"

#if defined(__linux__) && !defined(USE_CODE)
	#if defined(__KERNEL__)
		#include <asm/unistd.h>
	#else
		#include <unistd.h>
	#endif
#endif

#define SGX_UNDEFINED_HEAP_ID					(~0LU)
#define SGX_GENERAL_HEAP_ID						0
#define SGX_TADATA_HEAP_ID						1
#define SGX_KERNEL_CODE_HEAP_ID					2
#define SGX_KERNEL_DATA_HEAP_ID					3
#define SGX_PIXELSHADER_HEAP_ID					4
#define SGX_VERTEXSHADER_HEAP_ID				5
#define SGX_PDSPIXEL_CODEDATA_HEAP_ID			6
#define SGX_PDSVERTEX_CODEDATA_HEAP_ID			7
#define SGX_SYNCINFO_HEAP_ID					8
#define SGX_3DPARAMETERS_HEAP_ID				9
#if defined(SUPPORT_SGX_GENERAL_MAPPING_HEAP)
#define SGX_GENERAL_MAPPING_HEAP_ID				10
#endif
#if defined(SGX_FEATURE_2D_HARDWARE)
#define SGX_2D_HEAP_ID							11
#else
#if defined(FIX_HW_BRN_26915)
#define SGX_CGBUFFER_HEAP_ID					12
#endif
#endif
#define SGX_MAX_HEAP_ID							13


#define SGX_MAX_TA_STATUS_VALS	32
#define SGX_MAX_3D_STATUS_VALS	3

#if defined(SUPPORT_SGX_GENERALISED_SYNCOBJECTS)
#define SGX_MAX_TA_DST_SYNCS			1
#define SGX_MAX_TA_SRC_SYNCS			1
#define SGX_MAX_3D_SRC_SYNCS			4
#else
#if defined(ANDROID)
#define SGX_MAX_SRC_SYNCS				8
#else
#define SGX_MAX_SRC_SYNCS				4
#endif
#endif

#ifdef SUPPORT_SGX_HWPERF

#define	PVRSRV_SGX_HWPERF_NUM_COUNTERS	9

#define PVRSRV_SGX_HWPERF_INVALID					0x1

#define PVRSRV_SGX_HWPERF_TRANSFER					0x2
#define PVRSRV_SGX_HWPERF_TA						0x3
#define PVRSRV_SGX_HWPERF_3D						0x4
#define PVRSRV_SGX_HWPERF_2D						0x5

#define PVRSRV_SGX_HWPERF_MK_EVENT					0x101
#define PVRSRV_SGX_HWPERF_MK_TA						0x102
#define PVRSRV_SGX_HWPERF_MK_3D						0x103
#define PVRSRV_SGX_HWPERF_MK_2D						0x104

#define PVRSRV_SGX_HWPERF_TYPE_STARTEND_BIT			28
#define PVRSRV_SGX_HWPERF_TYPE_OP_MASK				((1UL << PVRSRV_SGX_HWPERF_TYPE_STARTEND_BIT) - 1)
#define PVRSRV_SGX_HWPERF_TYPE_OP_START				(0UL << PVRSRV_SGX_HWPERF_TYPE_STARTEND_BIT)
#define PVRSRV_SGX_HWPERF_TYPE_OP_END				(1Ul << PVRSRV_SGX_HWPERF_TYPE_STARTEND_BIT)

#define PVRSRV_SGX_HWPERF_TYPE_TRANSFER_START		(PVRSRV_SGX_HWPERF_TRANSFER | PVRSRV_SGX_HWPERF_TYPE_OP_START)
#define PVRSRV_SGX_HWPERF_TYPE_TRANSFER_END			(PVRSRV_SGX_HWPERF_TRANSFER | PVRSRV_SGX_HWPERF_TYPE_OP_END)
#define PVRSRV_SGX_HWPERF_TYPE_TA_START				(PVRSRV_SGX_HWPERF_TA | PVRSRV_SGX_HWPERF_TYPE_OP_START)
#define PVRSRV_SGX_HWPERF_TYPE_TA_END				(PVRSRV_SGX_HWPERF_TA | PVRSRV_SGX_HWPERF_TYPE_OP_END)
#define PVRSRV_SGX_HWPERF_TYPE_3D_START				(PVRSRV_SGX_HWPERF_3D | PVRSRV_SGX_HWPERF_TYPE_OP_START)
#define PVRSRV_SGX_HWPERF_TYPE_3D_END				(PVRSRV_SGX_HWPERF_3D | PVRSRV_SGX_HWPERF_TYPE_OP_END)
#define PVRSRV_SGX_HWPERF_TYPE_2D_START				(PVRSRV_SGX_HWPERF_2D | PVRSRV_SGX_HWPERF_TYPE_OP_START)
#define PVRSRV_SGX_HWPERF_TYPE_2D_END				(PVRSRV_SGX_HWPERF_2D | PVRSRV_SGX_HWPERF_TYPE_OP_END)

#define PVRSRV_SGX_HWPERF_TYPE_MK_EVENT_START		(PVRSRV_SGX_HWPERF_MK_EVENT | PVRSRV_SGX_HWPERF_TYPE_OP_START)
#define PVRSRV_SGX_HWPERF_TYPE_MK_EVENT_END			(PVRSRV_SGX_HWPERF_MK_EVENT | PVRSRV_SGX_HWPERF_TYPE_OP_END)
#define PVRSRV_SGX_HWPERF_TYPE_MK_TA_START			(PVRSRV_SGX_HWPERF_MK_TA | PVRSRV_SGX_HWPERF_TYPE_OP_START)
#define PVRSRV_SGX_HWPERF_TYPE_MK_TA_END			(PVRSRV_SGX_HWPERF_MK_TA | PVRSRV_SGX_HWPERF_TYPE_OP_END)
#define PVRSRV_SGX_HWPERF_TYPE_MK_3D_START			(PVRSRV_SGX_HWPERF_MK_3D | PVRSRV_SGX_HWPERF_TYPE_OP_START)
#define PVRSRV_SGX_HWPERF_TYPE_MK_3D_END			(PVRSRV_SGX_HWPERF_MK_3D | PVRSRV_SGX_HWPERF_TYPE_OP_END)
#define PVRSRV_SGX_HWPERF_TYPE_MK_2D_START			(PVRSRV_SGX_HWPERF_MK_2D | PVRSRV_SGX_HWPERF_TYPE_OP_START)
#define PVRSRV_SGX_HWPERF_TYPE_MK_2D_END			(PVRSRV_SGX_HWPERF_MK_2D | PVRSRV_SGX_HWPERF_TYPE_OP_END)

#define PVRSRV_SGX_HWPERF_OFF						(0x0)
#define PVRSRV_SGX_HWPERF_GRAPHICS_ON				(1UL << 0)
#define PVRSRV_SGX_HWPERF_MK_EXECUTION_ON			(1UL << 1)


typedef struct _PVRSRV_SGX_HWPERF_CB_ENTRY_
{
	IMG_UINT32	ui32FrameNo;
	IMG_UINT32	ui32Type;
	IMG_UINT32	ui32Ordinal;
	IMG_UINT32	ui32Clocksx16;
	IMG_UINT32	ui32Counters[PVRSRV_SGX_HWPERF_NUM_COUNTERS];
} PVRSRV_SGX_HWPERF_CB_ENTRY;


typedef struct _PVRSRV_SGX_HWPERF_CBDATA_
{
	IMG_UINT32	ui32FrameNo;
	IMG_UINT32	ui32Type;
	IMG_UINT32	ui32StartTimeWraps;
	IMG_UINT32	ui32StartTime;
	IMG_UINT32	ui32EndTimeWraps;
	IMG_UINT32	ui32EndTime;
	IMG_UINT32	ui32ClockSpeed;
	IMG_UINT32	ui32TimeMax;
} PVRSRV_SGX_HWPERF_CBDATA;


typedef struct _SGX_MISC_INFO_HWPERF_RETRIEVE_CB
{
	PVRSRV_SGX_HWPERF_CBDATA*	psHWPerfData;	
	IMG_UINT32					ui32ArraySize;	
	IMG_UINT32					ui32DataCount;	
	IMG_UINT32					ui32Time;		
} SGX_MISC_INFO_HWPERF_RETRIEVE_CB;
#endif 


typedef struct _CTL_STATUS_
{
	IMG_DEV_VIRTADDR	sStatusDevAddr;
	IMG_UINT32			ui32StatusValue;
} CTL_STATUS;


typedef enum _SGX_MISC_INFO_REQUEST_
{
	SGX_MISC_INFO_REQUEST_CLOCKSPEED = 0,
	SGX_MISC_INFO_REQUEST_SGXREV,
	SGX_MISC_INFO_REQUEST_DRIVER_SGXREV,
#if defined(SUPPORT_SGX_EDM_MEMORY_DEBUG)
	SGX_MISC_INFO_REQUEST_MEMREAD,
#endif
#if defined(SUPPORT_SGX_HWPERF)
	SGX_MISC_INFO_REQUEST_SET_HWPERF_STATUS,
	SGX_MISC_INFO_REQUEST_HWPERF_CB_ON, 
	SGX_MISC_INFO_REQUEST_HWPERF_CB_OFF, 
	SGX_MISC_INFO_REQUEST_HWPERF_RETRIEVE_CB, 
#endif 
#if defined(SGX_FEATURE_DATA_BREAKPOINTS)
	SGX_MISC_INFO_REQUEST_SET_BREAKPOINT,
#endif 
#if defined(SUPPORT_CPU_CACHED_BUFFERS)
	SGX_MISC_INFO_FLUSH_CPUCACHE_NOW,
	SGX_MISC_INFO_FLUSH_CPUCACHE_QUEUE,
#endif 
	SGX_MISC_INFO_DUMP_DEBUG_INFO,
	SGX_MISC_INFO_PANIC,
	SGX_MISC_INFO_REQUEST_FORCE_I16 				=  0x7fff
} SGX_MISC_INFO_REQUEST;


typedef struct _PVRSRV_SGX_MISCINFO_FEATURES
{
	IMG_UINT32			ui32CoreRev;	
	IMG_UINT32			ui32CoreID;		
	IMG_UINT32			ui32DDKVersion;	
	IMG_UINT32			ui32DDKBuild;	
	IMG_UINT32			ui32CoreIdSW;	
	IMG_UINT32			ui32CoreRevSW;	
	IMG_UINT32			ui32BuildOptions;	
#if defined(SUPPORT_SGX_EDM_MEMORY_DEBUG)
	IMG_UINT32			ui32DeviceMemValue;		
#endif
} PVRSRV_SGX_MISCINFO_FEATURES;


#if defined(SGX_FEATURE_DATA_BREAKPOINTS)
typedef struct _SGX_BREAKPOINT_INFO
{
	
	IMG_BOOL					bBPEnable;
	
	
	
	IMG_UINT32					ui32BPIndex;
	
	IMG_DEV_VIRTADDR			sBPDevVAddr;
} SGX_BREAKPOINT_INFO;
#endif 

typedef struct _SGX_MISC_INFO_
{
	SGX_MISC_INFO_REQUEST	eRequest;	
#if defined(SUPPORT_SGX_EDM_MEMORY_DEBUG)
	IMG_DEV_VIRTADDR			sDevVAddr;			
	IMG_HANDLE					hDevMemContext;		
#endif
	union
	{
		IMG_UINT32	reserved;	
		PVRSRV_SGX_MISCINFO_FEATURES						sSGXFeatures;
		IMG_UINT32											ui32SGXClockSpeed;
#if defined(SGX_FEATURE_DATA_BREAKPOINTS)
		SGX_BREAKPOINT_INFO									sSGXBreakpointInfo;
#endif
#ifdef SUPPORT_SGX_HWPERF
		IMG_UINT32											ui32NewHWPerfStatus;
		SGX_MISC_INFO_HWPERF_RETRIEVE_CB					sRetrieveCB;
#endif 
	} uData;
} SGX_MISC_INFO;

#if defined(SGX_FEATURE_2D_HARDWARE)
#define PVRSRV_MAX_BLT_SRC_SYNCS		3
#endif


#define SGX_KICKTA_DUMPBITMAP_MAX_NAME_LENGTH		256

typedef struct _SGX_KICKTA_DUMPBITMAP_
{
	IMG_DEV_VIRTADDR	sDevBaseAddr;
	IMG_UINT32			ui32Flags;
	IMG_UINT32			ui32Width;
	IMG_UINT32			ui32Height;
	IMG_UINT32			ui32Stride;
	IMG_UINT32			ui32PDUMPFormat;
	IMG_UINT32			ui32BytesPP;
	IMG_CHAR			pszName[SGX_KICKTA_DUMPBITMAP_MAX_NAME_LENGTH];
} SGX_KICKTA_DUMPBITMAP, *PSGX_KICKTA_DUMPBITMAP;

#define PVRSRV_SGX_PDUMP_CONTEXT_MAX_BITMAP_ARRAY_SIZE	(16)

typedef struct _PVRSRV_SGX_PDUMP_CONTEXT_
{
	
	IMG_UINT32						ui32CacheControl;

} PVRSRV_SGX_PDUMP_CONTEXT;


typedef struct _SGX_KICKTA_DUMP_ROFF_
{
	IMG_HANDLE			hKernelMemInfo;						
	IMG_UINT32			uiAllocIndex;						
	IMG_UINT32			ui32Offset;							
	IMG_UINT32			ui32Value;							
	IMG_PCHAR			pszName;							
} SGX_KICKTA_DUMP_ROFF, *PSGX_KICKTA_DUMP_ROFF;

typedef struct _SGX_KICKTA_DUMP_BUFFER_
{
	IMG_UINT32			ui32SpaceUsed;
	IMG_UINT32			ui32Start;							
	IMG_UINT32			ui32End;							
	IMG_UINT32			ui32BufferSize;						
	IMG_UINT32			ui32BackEndLength;					
	IMG_UINT32			uiAllocIndex;
	IMG_HANDLE			hKernelMemInfo;						
	IMG_PVOID			pvLinAddr;
#if defined(SUPPORT_SGX_NEW_STATUS_VALS)
	IMG_HANDLE			hCtrlKernelMemInfo;					
	IMG_DEV_VIRTADDR	sCtrlDevVAddr;						
#endif
	IMG_PCHAR			pszName;							
} SGX_KICKTA_DUMP_BUFFER, *PSGX_KICKTA_DUMP_BUFFER;

#ifdef PDUMP
typedef struct _SGX_KICKTA_PDUMP_
{
	
	PSGX_KICKTA_DUMPBITMAP		psPDumpBitmapArray;
	IMG_UINT32						ui32PDumpBitmapSize;

	
	PSGX_KICKTA_DUMP_BUFFER	psBufferArray;
	IMG_UINT32						ui32BufferArraySize;

	
	PSGX_KICKTA_DUMP_ROFF		psROffArray;
	IMG_UINT32						ui32ROffArraySize;
} SGX_KICKTA_PDUMP, *PSGX_KICKTA_PDUMP;
#endif	

#if defined(TRANSFER_QUEUE)
#if defined(SGX_FEATURE_2D_HARDWARE)
#define SGX_MAX_2D_BLIT_CMD_SIZE 		26
#define SGX_MAX_2D_SRC_SYNC_OPS			3
#endif
#define SGX_MAX_TRANSFER_STATUS_VALS	2	
#define SGX_MAX_TRANSFER_SYNC_OPS	5
#endif

#if defined (__cplusplus)
}
#endif

#endif 

