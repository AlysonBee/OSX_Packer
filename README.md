# OSX_Packer
A binary packer for the Mach-O  file format.

This is a small project to demonstrate binary packing design for the Mach-O file format done in pure C.
I decided to attempt this project out of of a curiosity I have for the Mach-O file format and how it differs from ELF.

## How to execute
I've provided everything that the code outputs directly in this repo.
patcher.c - the source code of the program.
patcher - the binary it produces.
ls - a copy of OSX's ls command
cracked - the patched version of the ls command.

If you run
```
nm cracked
```
You'll see a list of all the labels in cracked completely in tact.
and
```
otool -t cracked
```
You'll notice that the first 10+ bytes of t`ls` and `cracked` differ. `cracked` has the bytes appended to it:
```
\xeb\x19\xb8\x04\x00\x00\x02\xbf\x01\x00\x00\x00\x5e\xba\x0c\x00\x00\x00\x0f\x05\xb8\x01\x00\x00\x02\x0f\x05\xe8\xe2\xff\xff\xff\x48\x65\x6c\x6c\x6f\x20\x57\x6f\x72\x6c\x64
```
You can easily recompile it yourself with
```
gcc -o patcher patcher.c
```

### Brutally hacked together by me.
