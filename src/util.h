#ifndef MESH_PHOTO_BOOTH_UTIL_H
#define MESH_PHOTO_BOOTH_UTIL_H
#include "common.h"

extern s8* utilReadFile(const s8* path, s32* fileLength);
extern void utilFreeFile(s8* file);
extern r32 utilRandomFloat(r32 min, r32 max);
extern u8* utilLoadFaceColorMap(const s8* path);

#endif