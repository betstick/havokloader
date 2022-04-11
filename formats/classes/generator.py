# the code this generates is not memory friendly, but should be safe on 
# more than just x86_64. maybe. it'll use double the memory and more ops
# but coding it in a "smarter" way like the FROM formats is very complex
# and would also make this code very likely to segfault or corrupt data.

import xml.etree.ElementTree as ET
from enum import Enum
from os import listdir
from os.path import isfile, join

#might need to convert this to an array :(
types = [
	["TYPE_VECTOR4","vec4",16],
	["TYPE_BOOL","bool",1],
	["TYPE_INT8","signed char",1],
	["TYPE_UINT8","unsigned char",1],
	["TYPE_INT16","short",2],
	["TYPE_UINT16","unsigned short",2],
	["TYPE_INT32","int",4],
	["TYPE_UINT32","unsigned int",4],
	["TYPE_INT64","long",8],
	["TYPE_UINT64","unsigned long",8],
	["TYPE_LONG","long",8],
	["TYPE_ULONG","unsigned long",8],
	["TYPE_STRINGPTR","unsigned long",8],
	["TYPE_POINTER","unsigned long",8],
	["TYPE_REAL","float",4],
	["TYPE_VOID","void",8],
	["TYPE_QSTRANSFORM","qstransform",48],
	#["hkbEvent","hkbEvent",24]
]

#there's certainly a cleaner way to do this :/
def getTypeInfo(type_in):
	for t in types:
		if t[0] == type_in:
			return t
		
	else:
		return [type_in,type_in,"sizeof("+type_in+")"]

def parseType(member):
	info = []
	vtype = member.attrib.get('vtype')

	typeFixes = [
		['TYPE_ENUM','vsubtype'],
		['TYPE_STRUCT','ctype'],
		['TYPE_ARRAY','vsubtype'],
	]

	i = True
	temp = vtype

	while i == True:
		m = False
		for t in typeFixes:
			if temp == t[0]:
				m = True
				temp = member.attrib.get(t[1])
		if m == False:
			info = getTypeInfo(temp)
			i = False

	#if vtype == 'TYPE_ENUM':
	#	info = getTypeInfo(member.attrib.get('vsubtype'))
	#elif vtype == 'TYPE_STRUCT':
	#	info = getTypeInfo(member.attrib.get('ctype'))
	#elif vtype == 'TYPE_ARRAY':
	#	info = getTypeInfo(member.attrib.get('vsubtype'))
	#else:
	#	info = getTypeInfo(vtype)

	return info

class hkEnum:
	def __init__(self, name):
		self.name = name
		self.data = []

	def insert2(self, key, value):
		self.data.append([key,value])

	def define(self):
		ret = "enum " + self.name + "\n{\n"

		for d in self.data:
			ret += "\t" + d[0] + " = " + d[1] + ",\n"

		ret += "};"

		return ret

class hkMem:
	name = "none"
	vtype = "none"
	vsubtype = "none"
	arrsize = 0 #how many members in the array

	m_size = 0 #full size of the member
	t_size = 0 #size that the type should be

	def __init__(self, hkmember, offset):
		self.name = hkmember.attrib.get('name')

		self.vtype = hkmember.attrib.get('vtype')
		self.vsubtype = hkmember.attrib.get('vsubtype')
		self.arrsize = hkmember.attrib.get('arrsize')

		self.m_size = int(offset)
		
		info = parseType(hkmember)
		self.m_type = info[1]
		self.t_size = info[2]
		#self.m_offset = hkmember.attrib.get('offset')

	def define(self):
		ret = str(self.m_type)

		if self.vtype == "TYPE_ARRAY":
			ret += "* "
		else:
			ret += " "

		ret += self.name + ";\n"
		return ret

	def implementRead(self):
		ret = "mread(&x->" + self.name + "," + str(self.t_size) + ",1,src);\n"

		if isinstance(self.t_size,int):
			if self.m_size > self.t_size:
				ret += "\tmseek(src," + str(self.m_size - self.t_size) + ",SEEK_CUR);\n"
		
		return ret

	#def implementWrite(self):
	#	ret = "mwrite((char*)" + self.name + "," + str(self.t_size) + ",1,dst);\n"
	#	if isinstance(self.t_size,int):
	#		if self.m_size > self.t_size:
	#			ret += "\tmseek(dst," + str(self.m_size - self.t_size) + ",SEEK_CUR);\n"
	#	return ret

class hkClass:
	def __init__(self,root):
		self.name = root.attrib.get('name')
		self.parent = root.attrib.get('parent')
		self.enums = []
		self.members = []
		self.size = root.attrib.get('size')

		offset = 0

		for child in root:
			if child.tag == 'enums':
				for cenum in child:
					e = hkEnum(cenum.attrib.get('name'))
					e.data = [] #idk, this is needed??!?!?!

					for item in cenum:
						e.insert2(item.attrib.get('name'),item.attrib.get('value'))

					self.enums.append(e)

			if child.tag == 'members':
				for i, member in enumerate(child,start=0):
					os = int(member.attrib.get('offset'))

					if i + 1 < len(child):
						no = int(child[i+1].attrib.get('offset'))
						offset = no - os
					else:
						offset = int(root.attrib.get('size')) - os
					
					m = hkMem(member,offset)
					self.members.append(m)

	def define(self):
		ret = '#pragma once\n' + '#include <stdio.h>\n#include "cmem.h"\n'
		
		if self.parent is not None:
			ret += '#include "' + self.parent + '.h"\n'

		if len(self.enums) > 0:
			for e in self.enums:
				ret += e.define() + "\n"

		ret += "\nclass " + self.name + "\n" + "{\n\tpublic:\n"
		
		if self.parent is not None:
			ret += "\t" + self.parent + " base;\n"

		for m in self.members:
			ret += "\t" + m.define()

		#ret += "\n\t" + self.name + "();\n"

		ret += "\n\tstatic " + self.name + "* " + self.name + "Read(MEM* src);\n"

		#write is currently not functional
		#ret += "\n\tvoid write(MEM* src);\n"
	
		ret += "};\n"

		return ret

	def implmenet(self):
		#headers
		ret = '#pragma once\n#include "' + self.name + '.h"\n\n'

		#reader
		ret += self.name + "* " + self.name + "::" + self.name + "Read(MEM* src)\n{\n"
		ret += "\t" + self.name + "* x = new " + self.name + ";\n\n"
		
		if self.parent is not None:
			ret += "\tx->base = *" + self.parent + "::" + self.parent + "Read(src);\n"

		size = 0

		for m in self.members:
			ret += "\t" + m.implementRead()
			size += m.m_size

		if size < int(self.size):
			ret += "\tmseek(src," + str(int(self.size) - size) + ",SEEK_CUR);\n"

		ret += "\n\treturn x;\n"

		ret += "};\n"

		#writer disabled for now cause it's really bad
		#ret += "\nvoid " + self.name + "::write(MEM* dst)\n{\n"
		#for m in self.members:
		#	ret += "\t" + m.implementWrite()
		#ret += "};\n"

		return ret

def parseFile(file):
	tree = ET.parse(file)
	root = tree.getroot()

	hkc = hkClass(root)

	return [(hkc.define()),(hkc.implmenet())]

#parseFile("reference/classxmlds3/CustomBoneFixModifier.xml")
#parseFile("reference/classxmlds3/CustomClipGenerator.xml")

paths = ["reference/classxmlds3/"]

stdafx = open("autogen/stdafx.h","w")

for path in paths:
	for xml in listdir(path):
		if isfile(join(path,xml)):
			print("Generating class for: " + xml + "...")

			hfile = open(join("autogen/",xml).replace(".xml",".h"),"w")
			cfile = open(join("autogen/",xml).replace(".xml",".cpp"),"w")
			
			hfile.write(parseFile(join(path,xml))[0])
			cfile.write(parseFile(join(path,xml))[1])

			hfile.close()
			cfile.close()

			stdafx.write('#include "' + xml.replace(".xml",".h") + '"\n')

stdafx.close()