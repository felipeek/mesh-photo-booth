#ifndef MESH_PHOTO_BOOTH_PHOTO_H
#define MESH_PHOTO_BOOTH_PHOTO_H
#include "camera.h"
#include "graphics.h"

extern void photoTake(const char* outputImgPath, PerspectiveCamera* camera, Entity* e, Entity* e2, s32 photoWidth, s32 photoHeight, s32 msaaSamples,
		Vec4 cameraPosition, Vec4 cameraView, Vec3 meshRotation);

#endif