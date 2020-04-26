

#include "packer.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <mach-o/loader.h>
#include <mach-o/loader.h>
#include <sys/mman.h>
#include <sys/stat.h>


void	map_file(char *filename, unsigned char **content, size_t *size)
{
	struct stat	info;
	int		fd;

	fd = open(filename, O_RDONLY);
	fstat(fd, &info);
	*size = info.st_size;
	*content = mmap(0, *size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (*content == MAP_FAILED)
	{
		printf("Error mapping file\nExiting\n");
		exit(1);
	}
}

size_t g_usage_start;

size_t  find_usage_length(unsigned char *file_content, size_t size,
     struct section_64 *cstring_section)
{
    unsigned char *cstring;
    size_t counter = 0;
    int flag = -1;
    size_t  usage_length = 0;

    cstring = &file_content[cstring_section->offset];
    while (counter < cstring_section->size)
    {
        if (strncmp(&file_content[cstring_section->offset + counter], "usage", 5) == 0)
        {
            g_usage_start = cstring_section->offset + counter;
            flag = 1;
        }
        if (flag == 1)
            usage_length++;
        if (flag == 1 && file_content[cstring_section->offset + counter] == '\0')
        {
            flag = -1;
            break;
        }
        //printf("%c", file_content[cstring_section->offset + counter]);
        counter++;
    }
    printf("Usage string length == %zu\n",  usage_length);
    printf("g_start_ position : %zu\n", g_usage_start);
    g_usage_start -= 2;
    return usage_length;
}


void    edit_cstring_section(struct load_command *loader, unsigned char *content, size_t size)
{
    struct segment_command_64 *segment;
   
    segment = (struct segment_command_64 *)loader;
    if (strcmp(segment->segname, "__TEXT") != 0)
        return;
    int fd = open("cracked", O_RDWR | O_CREAT | O_TRUNC, 0777);
    struct section_64 *section;
    int counter;
    section = (struct section_64 *)&segment[1];
    size_t text_offset;
    size_t text_size;
    size_t cstring_offset;
    size_t usage_string;
    size_t cstring_size;
    counter = 0;
    while (counter < segment->nsects)
    {
        if (strcmp(section->sectname, "__text") == 0)
        {
            printf("section isze : %lu\n", section->size);
            text_offset = section->offset;
            text_size = section->size;
            section->size += 42;
        }
        if (strcmp(section->sectname, "__stub_helper") == 0 ||
            strcmp(section->sectname, "__stubs") ==0  ||
            strcmp(section->sectname, "__const") == 0)
        {
            section->offset += 42;
        }
        if (strcmp(section->sectname, "__cstring") == 0)
        {
            cstring_offset = section->offset;
            cstring_size = section->size;
            usage_string = find_usage_length(content, size, section) - 2;
            section->offset += 42;
            section->size -= 42;
        }
        section = (struct section_64 *)((void *)section + sizeof(struct section_64));
        counter++;
    }
    write(fd, content, text_offset);
  //  write(fd, (void *)"\x41\x50", 2);
    write(fd, (void *)"\xeb\x19\xb8\x04\x00\x00\x02\xbf\x01\x00\x00\x00\x5e\xba\x0c\x00\x00\x00\x0f\x05\xb8\x01\x00\x00\x02\x0f\x05\xe8\xe2\xff\xff\xff\x48\x65\x6c\x6c\x6f\x20\x57\x6f\x72\x6c\x64", 42);
  //  write(fd, (void *)"\x48\x31\xf6\x56\x48\xbf\x2f\x2f\x62\x69\x6e\x2f\x73\x68\x57\x48\x89\xe7\x48\x31\xd2\x48\x31\xc0\xb0\x02\x48\xc1\xc8\x28\xb0\x3b\x0f\x05", 35); 
    write(fd, &content[text_offset], text_size);
    size_t file_offset = text_offset + text_size;
    size_t how_many = cstring_offset - file_offset;
    write(fd, &content[file_offset], how_many);
    file_offset += how_many; 
    how_many = cstring_size - (42 + 2);
    write(fd, &content[file_offset], how_many);
    file_offset += how_many + (42 + 2);
    how_many = size - file_offset;
    write(fd, (void *)"\xa\x00",2);
    write(fd, &content[file_offset], how_many);
    close(fd);
    exit(1);
}

void    insert_code(unsigned char *content, size_t size)
{
    struct mach_header_64 *header;
    struct load_command *loader;
    int counter;

    header = (struct mach_header_64 *)content;
    loader = (struct load_command *)&header[1];
    counter = 0;
    while (counter < header->ncmds)
    {
        if (loader->cmd == LC_SEGMENT_64)
            edit_cstring_section(loader, content, size);
        loader = (struct load_command *)((void *)loader + loader->cmdsize);
        counter++;
    }

}

int     main(int argc, char **argv)
{
    size_t  size;
    unsigned char *content;

    if (argc != 2)
    {
        printf("Usage : %s [file]\n", argv[0]);
        return (-1);
    }
    map_file(argv[1], &content, &size);
    insert_code(content, size);
    return (0);
}







