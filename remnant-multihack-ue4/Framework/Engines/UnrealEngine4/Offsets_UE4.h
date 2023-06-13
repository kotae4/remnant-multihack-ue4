#pragma once

namespace UnrealEngine4
{
#ifdef _WIN64
    typedef unsigned __int64  uintptr_t;
#else
    typedef unsigned int uintptr_t;
#endif
	const uintptr_t GNAMES_OFFSET = 0x34AE908;
    const int NAME_BUCKET_SIZE = 16384;
    const int NAME_MAX_BUCKETS = 256;
    const int NAME_SIZE = 1024;
    const int NAME_WIDE_MASK = 0x1;
    const int NAME_INDEX_SHIFT = 1;

    const uintptr_t GOBJECTS_OFFSET = 0x34B2C08;
    const int OBJECT_BUCKET_SIZE = 65536;

    const uintptr_t GENGINE_OFFSET = 0x35AF758;

    const uintptr_t GWORLD_OFFSET = 0x35B2120;

    const int POSTRENDER_VTABLEINDEX = 210;
    const int AHUD_DRAWTEXT_OFFSET = 0x18DC460;
    const int AHUD_PROJECT_OFFSET = 0x18E9FF0;
    const int UOBJECT_GETFULLNAME_OFFSET = 0xB7E8E0;

    const int UWORLD_LINETRACETESTBYCHANNEL_OFFSET = 0x178C180;

    const int USKINNEDMESHCPNT_GETBONENAME_OFFSET = 0x17E0220;
    const int USKINNEDMESHCPNT_GETBONEMATRIX_OFFSET = 0x17DFE70;

    const int ULINEBATCHCPNT_DRAWLINE_OFFSET = 0x17B5300;
    const int ULINEBATCHCPNT_DRAWLINE_VTABLEINDEX = 266;

    const int APLAYERCONTROLLER_SETCONTROLROTATION_VTABLEINDEX = 201;
    const int APLAYERCONTROLLER_GETPLAYERVIEWPOINT_VTABLEINDEX = 219;

    // TODO review if these are necessary, i think i was just using them as a quick shortcut
    const uintptr_t GDEFAULTCOLLISIONRESPONSEPARAMS_OFFSET = 0x358C300;
    const uintptr_t GLINEOFSIGHT_FNAME_OFFSET = 0x35C4670;
}