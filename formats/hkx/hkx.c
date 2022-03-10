#include "hkx.h"
#include <stdio.h>

Hkx_Header* initHkx_Header(MEM* src, int offset)
{
	Hkx_Header* header = malloc(sizeof(Hkx_Header));
	header->start = (Hkx_Header_Start*)mtellptr(src);
	mseek(src,sizeof(Hkx_Header_Start),SEEK_CUR);
	
	if(header->start->version >= 0x0B)
	{
		header->ext->unk3C = (short)mtellptr(src);
		mseek(src,sizeof(short),SEEK_CUR);

		header->ext->sectionOffset = (short)mtellptr(src);
		mseek(src,sizeof(short),SEEK_CUR);

		if(header->ext->sectionOffset > 0)
		{
			header->ext->unk40 = (int)mtellptr(src);
			mseek(src,sizeof(int),SEEK_CUR);

			header->ext->unk44 = (int)mtellptr(src);
			mseek(src,sizeof(int),SEEK_CUR);

			header->ext->unk48 = (int)mtellptr(src);
			mseek(src,sizeof(int),SEEK_CUR);

			header->ext->unk4C = (int)mtellptr(src);
			mseek(src,sizeof(int),SEEK_CUR);
		}

		mseek(src,0x40+header->ext->sectionOffset+offset,SEEK_CUR);
	}
	else
	{
		mseek(src,sizeof(int),SEEK_CUR); //account for padding
	}

	return header;
};

Hkx_Section_LocalFixup* initHkx_Section_LocalFixup(MEM* src, int offset)
{
	Hkx_Section_LocalFixup* fixup = mtellptr(src);
	mseek(src,sizeof(Hkx_Section_LocalFixup),SEEK_CUR);
	return fixup;
};

Hkx_Section_GlobalFixup* initHkx_Section_GlobalFixup(MEM* src, int offset)
{
	Hkx_Section_GlobalFixup* fixup = mtellptr(src);
	mseek(src,sizeof(Hkx_Section_GlobalFixup),SEEK_CUR);
	return fixup;
};

Hkx_Section_VirtualFixup* initHkx_Section_VirtualFixup(MEM* src, int offset)
{
	Hkx_Section_VirtualFixup* fixup = mtellptr(src);
	mseek(src,sizeof(Hkx_Section_VirtualFixup),SEEK_CUR);
	return fixup;
};

Hkx_Section_ClassName* initHkx_Section_ClassName(MEM* src, int offset)
{
	Hkx_Section_ClassName* className = mtellptr(src);
	mseek(src,sizeof(Hkx_Section_ClassName),SEEK_CUR);
	
	char temp = 'a';

	//get to the end of the null terminated string
	while(temp != '\0')
		mread(temp,1,1,src);

	return className;
};

Hkx_Section* initHkx_Section(MEM* src, int offset, int version)
{
	Hkx_Section* section = malloc(sizeof(Hkx_Section));

	section->header = (Hkx_Section_Header*)mtellptr(src);
	mseek(src,sizeof(Hkx_Section_Header),SEEK_CUR);

	int pos = mtell(src);
	int dataSize = section->header->localFixupsOffset;

	if(dataSize != 0)
	{
		mseek(src,offset+section->header->absoluteDataStart,SEEK_SET);
		
		if(strncmp(section->header->sectionTag,"__classnames__",14) == 0)
		{
			int i = 0;
			short temp = 0x0000;
			while(temp != 0xFFFF)
			{
				mseek(src,-2,SEEK_CUR);
				section->classNames[i] = *initHkx_Section_ClassName(src,offset);
				i++;
				mread(temp,2,1,src);
			}
		}

		//pass classes into a function and do something with it?

		section->data = (char*)mtellptr(src);
	}

	dataSize = section->header->globalFixupsOffset - section->header->localFixupsOffset;
	if(dataSize != 0)
	{
		mseek(src,section->header->absoluteDataStart + section->header->localFixupsOffset + offset,SEEK_SET);
		
		for(int i = 0; i < dataSize / 8; i++)
			section->localFixups[i] = *initHkx_Section_LocalFixup(src,offset);
	};

	dataSize = section->header->virtualFixupsOffset - section->header->globalFixupsOffset;
	if(dataSize != 0)
	{
		mseek(src,section->header->absoluteDataStart + section->header->globalFixupsOffset + offset,SEEK_SET);
		
		for(int i = 0; i < dataSize / 0xC; i++)
			section->globalFixups[i] = *initHkx_Section_GlobalFixup(src,offset);
	};

	dataSize = section->header->exportsOffset - section->header->virtualFixupsOffset;
	if(dataSize != 0)
	{
		mseek(src,section->header->absoluteDataStart + section->header->virtualFixupsOffset + offset,SEEK_SET);
		
		for(int i = 0; i < dataSize / 0xC; i++)
			section->virtualFixups[i] = *initHkx_Section_VirtualFixup(src,offset);
	};

	dataSize = section->header->importsOffset - section->header->exportsOffset;
	if(dataSize != 0)
	{
		mseek(src,section->header->absoluteDataStart + section->header->importsOffset + offset,SEEK_SET);
		section->exports = mtellptr(src);
	};

	dataSize = section->header->endOffset - section->header->importsOffset;
	if(dataSize != 0)
	{
		mseek(src,section->header->absoluteDataStart - section->header->importsOffset + offset,SEEK_SET);
		section->imports = mtellptr(src);
	};

	mseek(src,pos,SEEK_SET);

	//if variant is BB or DS3, assert 16 bytes of 0xFF
};

Hkx* initHkx(MEM* src)
{
	int offset = mtell(src);
	Hkx* hkx = malloc(sizeof(Hkx));
	hkx->header = initHkx_Header(src,offset);

	initHkx_Section(src,offset,hkx->header->start->version);
};

void readClassArray(MEM* src)
{

};

void readClassPointer(MEM* src)
{
	
};

void readClassPointerArray(MEM* src)
{
	
};

Hkx* openHkx(MEM* src)
{
    
};