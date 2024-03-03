#ifndef MESH_PHOTO_BOOTH_OBJ_H
#define MESH_PHOTO_BOOTH_OBJ_H

#include "common.h"
#include "graphics.h"

extern int objParse(const char* objPath, Vertex** vertices, u32** indexes, Vec4 defaultColor);

#endif