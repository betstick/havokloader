## Basics
HKX files are binary blobs of data. They usually contain stuff like skeletons and some other weird things. There are tools to convert the HKX packfiles back into XML format. This makes them much easier to read and edit.

## Structure
The header of an HKX file specifies how many sections are in the file and some other info. Check the 010 template for more detailed info. Each section is of a specific type, either defining characteristics of the data, or being the data itself. 

For example, PTDE HKX files usually have a `__classnames__ ` type section as the first section. It defines all the different classes that will be present. I think. The next section is usually `__types__` to specify something. The last section is `__data__` and stores the important stuff.

Sections can have "fixups" in them. Fixups are essentially a memory/pointer map thing. From Katalash:
```
Fixups are just things like patching in pointers after the hkx file is loaded. Packfiles are designed to be read directly into memory and cast into c++ objects, but pointers need to be patched in to be relative to the memory they are loaded in
The virtual fixups are to copy in the address of the class’s vtable into the c++ classes, but I use it to determine which class a given chunk of memory is

Pack files are actually pretty straightforward to read once you understand them
Read all the bytes, patch the pointers to each other, copy paste the vtables
```

On the subject of tag files and their diffs from packfiles, from Katalash:
```
Tag files basically store all the fields that aren’t 0 and use some hyperpacked format
So deserializing is more work
```