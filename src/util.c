#include "util.h"
#include <dynamic_array.h>
#include <stdio.h>
#include <stdlib.h>

extern r32 utilRandomFloat(r32 min, r32 max)
{
    r32 scale = rand() / (r32)RAND_MAX;
    return min + scale * (max - min);
}

extern s8* utilReadFile(const s8* path, s32* _fileLength)
{
	FILE* file;
	s8* buffer;
	s32 fileLength;

	file = fopen(path, "rb");
	fseek(file, 0, SEEK_END);
	fileLength = ftell(file);
	rewind(file);

	buffer = (s8*)malloc((fileLength + 1) * sizeof(s8));
	fread(buffer, fileLength, 1, file);
	fclose(file);

	buffer[fileLength] = '\0';

	if (_fileLength)
		*_fileLength = fileLength;

	return buffer;
}

extern void utilFreeFile(s8* file)
{
	free(file);
}

extern u8* utilLoadFaceColorMap(const s8* path) {
	u8* faceColorMapPath = array_create(u8, 1024);
	s32 file_length;
	s8* file_content = utilReadFile(path, &file_length);

	for (s32 i = 0; i < file_length; ++i) {
		u8 val = file_content[i] == '1';
		array_push(faceColorMapPath, &val);
	}

	return faceColorMapPath;
}