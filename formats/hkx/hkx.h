#include <stdint.h>
#include "cmem.h"

struct HKX_Header_Start
{
	char magic[8];
	int userTag;
	int version;
	unsigned char pointerSize;
	unsigned char endian;
	unsigned char paddingOption;
	unsigned char baseClass;
	int sectionCount; //usually 3
	int contentsSectionIndex;
	int contentsSectionOffset;
	int contentsClassNameSectionIndex;
	int contentsClassNameSectionOffset;
	char contentsVersion[16];
	int flags;
};

struct HKX_Header_Ext
{
	short unk3C;
	short sectionOffset;

	//only if section offset greater than 0
	int unk40;
	int unk44;
	int unk48;
	int unk4C;
};

struct HKX_Header
{
	struct HKX_Header_Start* start;
	struct HKX_Header_Ext* ext;
};

struct HKX_Section_Header
{
	char sectionTag[19];
	char pad;
	int absoluteDataStart;
	int localFixupsOffset;
	int globalFixupsOffset;
	int virtualFixupsOffset;
	int exportsOffset;
	int importsOffset;
	int endOffset;
};

struct HKX_Section_ClassName
{
	unsigned int signature;
	char strnStart; //assert 0x09
};

struct HKX_Section_LocalFixup
{
	int src;
	int dst;
};

struct HKX_Section_GlobalFixup
{
	int src;
	int sec; //section index of destination
	int dst;
};

struct HKX_Section_VirtualFixup
{
	int src;
	int sec;
	int nof; //name offset
};

struct HKX_Section
{
	struct HKX_Section_Header*              header;
	struct HKX_Section_ClassName*       classNames; //only if tag is classnames
	struct HKX_Section_LocalFixup*     localFixups;
	struct HKX_Section_GlobalFixup*   globalFixups;
	struct HKX_Section_VirtualFixup* virtualFixups;

	char* data; //only if datasize != 0
};

typedef struct
{
	struct HKX_Header* header;
	struct HKX_Section* sections;
} HKX;

HKX* openHKX();