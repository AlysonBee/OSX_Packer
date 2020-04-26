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
(otool -t output before patch - ls first 10 lines)
```
0000000100000f08	55 48 89 e5 48 83 c7 68 48 83 c6 68 5d e9 62 36
0000000100000f18	00 00 55 48 89 e5 48 8d 46 68 48 8d 77 68 48 89
0000000100000f28	c7 5d e9 4d 36 00 00 55 48 89 e5 4c 8b 46 60 48
0000000100000f38	8b 57 60 48 8b 4a 30 b8 01 00 00 00 49 39 48 30
0000000100000f48	7f 1a 7d 07 b8 ff ff ff ff eb 11 48 8b 4a 38 49
0000000100000f58	39 48 38 7f 07 b8 ff ff ff ff 7d 02 5d c3 48 83
0000000100000f68	c7 68 48 83 c6 68 5d e9 08 36 00 00 55 48 89 e5
0000000100000f78	49 89 f8 48 8b 56 60 48 8b 7f 60 48 8b 4f 30 b8
```
(otool -t output after patch - cracked first 10 lines)
```
0000000100000f08	eb 19 b8 04 00 00 02 bf 01 00 00 00 5e ba 0c 00
0000000100000f18	00 00 0f 05 b8 01 00 00 02 0f 05 e8 e2 ff ff ff
0000000100000f28	48 65 6c 6c 6f 20 57 6f 72 6c 55 48 89 e5 48 83
0000000100000f38	c7 68 48 83 c6 68 5d e9 62 36 00 00 55 48 89 e5
0000000100000f48	48 8d 46 68 48 8d 77 68 48 89 c7 5d e9 4d 36 00
0000000100000f58	00 55 48 89 e5 4c 8b 46 60 48 8b 57 60 48 8b 4a
0000000100000f68	30 b8 01 00 00 00 49 39 48 30 7f 1a 7d 07 b8 ff
0000000100000f78	ff ff ff eb 11 48 8b 4a 38 49 39 48 38 7f 07 b8
```
You can easily recompile it yourself with
```
gcc -o patcher patcher.c
```

##### Brutally hacked together by AlysonBee.
