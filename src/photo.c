#include "photo.h"
#include "core.h"
#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>

// TODO(fek): Clean GL resources - for now, this is a nitpick, since process exits right after photo is taken.
static GLuint createMsaaFramebuffer(s32 width, s32 height, s32 samples) {
	GLuint msaaFBO, msaaTexture;

	// Generate the MSAA framebuffer
	glGenFramebuffers(1, &msaaFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, msaaFBO);

	// Create a multisampled color attachment texture
	glGenTextures(1, &msaaTexture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msaaTexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA, width, height, GL_TRUE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, msaaTexture, 0);

	GLuint depthStencilRBO;
	glGenRenderbuffers(1, &depthStencilRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, depthStencilRBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Attach the depth and stencil buffer to the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, msaaFBO);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilRBO);

	// Check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		printf("ERROR::FRAMEBUFFER:: Multisampled Framebuffer is not complete!\n");
		return 0;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return msaaFBO;
}

// TODO(fek): Clean GL resources - for now, this is a nitpick, since process exits right after photo is taken.
static GLuint createIntermediateFramebuffer(s32 width, s32 height) {
	GLuint intermediateFBO, screenTexture;

	// Generate the intermediate framebuffer
	glGenFramebuffers(1, &intermediateFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);

	// Create a color attachment texture
	glGenTextures(1, &screenTexture);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);

	// Check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		printf("ERROR::FRAMEBUFFER:: Intermediate Framebuffer is not complete!\n");
		return 0;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return intermediateFBO;
}

extern void photoTake(const char* outputImgPath, PerspectiveCamera* camera, Entity* e, s32 photoWidth, s32 photoHeight, s32 msaaSamples,
		Vec4 cameraPosition, Vec4 cameraView, Vec3 meshRotation)
{
	GLuint msaaFBO = createMsaaFramebuffer(photoWidth, photoHeight, msaaSamples);
	if (!msaaFBO) {
		return;
	}

	GLuint intermediateFBO = createIntermediateFramebuffer(photoWidth, photoHeight);
	if (!intermediateFBO) {
		return;
	}

	// Bind the multisampled framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, msaaFBO);

	// Render
	glEnable(GL_CULL_FACE);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cameraSetPosition(camera, cameraPosition);
	cameraSetView(camera, cameraView);
	graphicsEntitySetRotation(e, meshRotation);

	coreUpdate(0.1);
	coreRender();

	// Bind the MSAA framebuffer as the read framebuffer and the intermediate one as the draw framebuffer then blit
	glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
	glBlitFramebuffer(0, 0, photoWidth, photoHeight, 0, 0, photoWidth, photoHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);

	char* color_buffer = calloc(4, photoWidth * photoHeight);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glReadPixels(0, 0, photoWidth, photoHeight, GL_RGBA, GL_UNSIGNED_BYTE, color_buffer);

	ImageData img;
	img.channels = 4;
	img.data = color_buffer;
	img.height = photoHeight;
	img.width = photoWidth;
	graphicsImageSave(outputImgPath, &img);

	free(color_buffer);
}