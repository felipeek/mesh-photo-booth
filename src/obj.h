#ifndef BASIC_ENGINE_OBJ_H
#define BASIC_ENGINE_OBJ_H

#include "common.h"
#include "graphics.h"

int objParse(const char* objPath, Vertex** vertices, u32** indexes, Vec4 defaultColor);

#endif