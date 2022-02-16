#include "hkx.h"

struct HKX_Header_Start* initHKX_Header_Start()
{

};

struct HKX_Header_Ext* initHKX_Header_Ext()
{

};

struct HKX_Header* initHKX_Header()
{

};

struct HKX_Section_Header* initHKX_Section_Header()
{

};

struct HKX_Section_LocalFixup* initHKX_Section_LocalFixup()
{

};

struct HKX_Section_GlobalFixup* initHKX_Section_GlobalFixup()
{

};

struct HKX_Section_VirtualFixup* initHKX_Section_VirtualFixup()
{

};

struct HKX_Section* initHKX_Section()
{

};

struct HKX* initHKX(MEM* mem)
{
	
};

HKX* openHKX()
{
	// Later versions of Havok have an extended header
	/*if(version >= 0x0B)
	{
		

		if(sectionOffset > 0)
		{
			int unk40;
			int unk44;
			int unk48;
			int unk4C;
		}
		
		FSeek(Ftell() + 0x40 + sectionOffset);
	}
	else
	{
		int padding; Assert(padding == 0xFFFFFFFF);
	}*/
    
};