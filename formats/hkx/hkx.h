#include <stdint.h>
#include "cmem.h"

typedef struct
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
} Hkx_Header_Start;

typedef struct
{
	short unk3C;
	short sectionOffset;

	//only if section offset greater than 0
	int unk40;
	int unk44;
	int unk48;
	int unk4C;
} Hkx_Header_Ext;

typedef struct 
{
	Hkx_Header_Start* start;
	Hkx_Header_Ext* ext;
} Hkx_Header;

typedef struct
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
} Hkx_Section_Header;

typedef struct
{
	unsigned int signature;
	char padding; //always 0x09
	char name[]; //null terminated
} Hkx_Section_ClassName;

typedef struct
{
	unsigned int src;
	unsigned int dst;
} Hkx_Section_LocalFixup;

typedef struct
{
	unsigned int src;
	unsigned int sec; //section index of destination
	unsigned int dst;
} Hkx_Section_GlobalFixup;

typedef struct
{
	unsigned int src;
	unsigned int sec;
	unsigned int nof; //name offset
} Hkx_Section_VirtualFixup;

typedef struct
{
	Hkx_Section_Header*              header;
	Hkx_Section_ClassName*       classNames; //only if tag is classnames
	Hkx_Section_LocalFixup*     localFixups;
	Hkx_Section_GlobalFixup*   globalFixups;
	Hkx_Section_VirtualFixup* virtualFixups;

	char* data; //only if datasize != 0
	char* exports;
	char* imports;
} Hkx_Section;

typedef struct
{
	Hkx_Header* header;
	Hkx_Section* sections;
} Hkx;

Hkx* openHkx(MEM* src);