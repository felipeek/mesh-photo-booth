#ifndef MESH_PHOTO_BOOTH_CORE_H
#define MESH_PHOTO_BOOTH_CORE_H

#include "common.h"
#include "graphics.h"

extern int coreParseArguments(s32 argc, char** argv);
extern int coreInit(const char* meshPath);
extern void coreDestroy();
extern void coreUpdate(r32 deltaTime);
extern void coreRender();
extern void coreInputProcess(boolean* keyState, r32 deltaTime);
extern void coreMouseChangeProcess(boolean reset, r64 xPos, r64 yPos);
extern void coreMouseClickProcess(s32 button, s32 action, r64 xPos, r64 yPos);
extern void coreScrollChangeProcess(r64 xOffset, r64 yOffset);
extern void coreWindowResizeProcess(s32 width, s32 height);

extern void coreTakePhoto(const char* outputImgPath, Vec4 cameraPosition, Vec4 cameraView, Vec3 meshRotation);
#endif