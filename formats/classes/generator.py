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
	["TYPE_CHAR","char",1],
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
	#["TYPE_POINTER","unsigned long",8],
	["TYPE_CSTRING","char*",8],
	["TYPE_HALF","half",2],
	["TYPE_REAL","float",4],
	["TYPE_VOID","void*",8],
	["TYPE_QSTRANSFORM","qstransform",64],
	["TYPE_QUATERNION","quaternion",16],
	["TYPE_TRANSFORM","transform",64],
	["TYPE_MATRIX2","mat2",8],
	["TYPE_MATRIX3","mat3",12],
	["TYPE_MATRIX4","mat4",16],
]

tag = '/* This file was automatically generated from XML. */'

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
		['TYPE_SIMPLEARRAY','vsubtype'],
		['TYPE_FLAGS','vsubtype'],
		['TYPE_CSTRING','vsubtype'],
		['TYPE_POINTER','ctype']
	]

	temp = vtype

	#stupid simple method to derive the type
	if temp == 'TYPE_ARRAY':
		temp = member.attrib.get('vsubtype')
		if temp == 'TYPE_PONTER':
			temp = member.attrib.get('ctype')
		elif temp == 'TYPE_STRUCT':
			temp = member.attrib.get('ctype')
	elif temp == 'TYPE_ENUM':
		temp = member.attrib.get('etype')
		if temp == None:
			temp = member.attrib.get('vsubtype')
	elif temp == 'TYPE_STRUCT':
		temp = member.attrib.get('ctype')
	elif temp == 'TYPE_SIMPLEARRAY':
		temp = member.attrib.get('vsubtype')
	elif temp == 'TYPE_FLAGS':
		temp = member.attrib.get('vsubtype')
	elif temp == 'TYPE_POINTER':
		temp = member.attrib.get('vsubtype')
		if temp == 'TYPE_STRUCT':
			temp = member.attrib.get('ctype')

	if temp == None:
		print(member.attrib.get('name'))

	info = getTypeInfo(temp)

	return info

class hkEnum:
	def __init__(self, enum):
		self.name = enum.attrib.get('name')
		self.data = []

		for i in enum:
			name = i.attrib.get('name')
			value = int(i.attrib.get('value'))
			self.data.append([name,value])

	def insert(self, key, value):
		self.data.append([key,value])

	def define(self):
		ret = "enum " + self.name + "\n{\n"

		for d in self.data:
			ret += "\t" + str(d[0]) + " = " + str(d[1]) + ",\n"

		ret += "};\n"

		return ret

class hkMem:
	def __init__(self, hkmember, offset):
		#rename to prevent name collision with C/C++
		self.name = "m_" + hkmember.attrib.get('name')

		self.vtype = str(hkmember.attrib.get('vtype'))
		self.vsubtype = str(hkmember.attrib.get('vsubtype'))
		self.arrsize = int(hkmember.attrib.get('arrsize'))

		self.m_size = int(offset)
		
		info = parseType(hkmember)
		self.m_type = info[1]
		self.t_size = info[2] #this CAN be a string!
		#self.m_offset = hkmember.attrib.get('offset')

		if hkmember.attrib.get('etype') is not None:
			self.is_enum = True
		else:
			self.is_enum = False

	def define(self):
		ret = self.m_type

		#only use pointer when array AND size is unkown
		if self.vtype == "TYPE_ARRAY" or self.vtype == "TYPE_SIMPLEARRAY":
			if self.arrsize == 0:
				ret += "* "
		else:
			ret += " "

		ret += self.name
		
		if self.arrsize > 0:
			ret += "[" + str(self.arrsize) + "]"

		ret += ";\n"
		return ret

	def implementRead(self):
		ret = "mread(&x->" + self.name + "," + str(self.t_size) + ",1,src);\n"

		if isinstance(self.t_size, int):
			if self.m_size > self.t_size:
				ret += "\tmseek(src," + str(self.m_size - self.t_size) + ",SEEK_CUR);\n"
		
		return ret

	def implementWrite(self):
		ret = "sum += " + str(self.t_size) + " - mwrite((char*)target->"  
		ret += self.name + "," + str(self.t_size) + ",1,dst);\n"

		if isinstance(self.t_size, int):
			if self.m_size > self.t_size:
				ret += "\tmseek(dst," + str(self.m_size - self.t_size) + ",SEEK_CUR);\n"

		return ret

class hkClass:
	def __init__(self,root):
		self.name = root.attrib.get('name')
		self.parent = root.attrib.get('parent')
		self.enums = []
		self.members = []
		self.deps = []
		self.size = int(root.attrib.get('size'))

		offset = 0

		#setup enums and member data
		for child in root:
			if child.tag == 'enums':
				for enum in child:
					self.enums.append(hkEnum(enum))

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

		#setup deps
		self.deps.append("<stdio.h>")
		self.deps.append('"cmem.h"')
		self.deps.append('"../../../lib/half/half.hpp"')
		self.deps.append('"../manual/include.h"')
		#self.deps.append('"stdafx.h"') //gonna leave this out for now

		#add deps for types not in the types list
		for m in self.members:
			match = False
			
			for t in types:
				if t[1] == m.m_type:
					match = True

			for e in self.enums:
				if str(e.name) == str(m.m_type):
					match = True

			if match == False:
				if m.is_enum == False:
					self.deps.append('"' + m.m_type + '.h"')
				else:
					header = '"hkcd' + str(m.m_type).replace('Enum','') + '.h"'
					self.deps.append(header)

		if self.parent is not None:
			self.deps.append('"' + self.parent + '.h"')

	def define(self):
		ret = tag + "\n"

		ret += '#pragma once\n'
		
		for d in self.deps:
			ret += '#include ' + d + '\n'

		ret += "using half_float::half;\n\n"

		if len(self.enums) > 0:
			for e in self.enums:
				ret += e.define() + "\n"

		ret += "class " + self.name + "\n" + "{\n\tpublic:\n"
		
		if self.parent is not None:
			ret += "\t" + self.parent + " base;\n"

		for m in self.members:
			ret += "\t" + m.define()

		ret += "\n\tstatic " + self.name + "* " + self.name + "Read(MEM* src);\n"

		ret += "\n\tstatic int " + self.name + "Write(MEM* src, "

		ret += self.name + "* target);\n"

		ret += "};\n"

		return ret

	def implmenet(self):
		ret = tag + "\n"

		#headers
		ret += '#pragma once\n#include "' + self.name + '.h"\n'

		ret += "using half_float::half;\n\n"

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

		ret += "\n"

		#writer
		ret += "int " + self.name + "::" + self.name + "Write(MEM* dst, "
		
		ret+= self.name + "* target)\n{\n"

		ret += "\tint sum = 0;\n\n"

		if self.parent is not None:
			ret += "\tsum += " + self.parent + "::" + self.parent
			ret += "Write(dst, &target->base);\n"

		for m in self.members:
			ret += "\t" + m.implementWrite()

		ret += "\n\treturn sum;\n"

		ret += "};\n"

		return ret

def parseFile(file):
	tree = ET.parse(file)
	root = tree.getroot()

	hkc = hkClass(root)

	return [(hkc.define()),(hkc.implmenet())]

#parseFile("reference/classxmlds3/CustomBoneFixModifier.xml")
#parseFile("reference/classxmlds3/CustomClipGenerator.xml")

paths = ["reference/classxmlds3/"]

inc = open("autogen/autogen.h","w")

for path in paths:
	for xml in listdir(path):
		if isfile(join(path,xml)):
			#print("Generating class for: " + xml + "...")

			hfile = open(join("autogen/",xml).replace(".xml",".h"),"w")
			cfile = open(join("autogen/",xml).replace(".xml",".cpp"),"w")
			
			#calc once, don't run parse file twice
			res = parseFile(join(path,xml))

			hfile.write(res[0])
			cfile.write(res[1])

			hfile.close()
			cfile.close()

			inc.write('#include "' + xml.replace(".xml",".h") + '"\n')

inc.close()