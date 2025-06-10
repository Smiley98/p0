#include "GroundScene.h"
#include "Camera.h"
#include "rlgl.h"

// Im sure this stuff exists else where, but for now,
// GBuffer data
typedef struct GBuffer {
    unsigned int framebuffer;

    unsigned int positionTexture;
    unsigned int normalTexture;
    unsigned int albedoSpecTexture;

    unsigned int depthRenderbuffer;
} GBuffer;

// Deferred mode passes
typedef enum {
    DEFERRED_POSITION,
    DEFERRED_NORMAL,
    DEFERRED_ALBEDO,
    DEFERRED_SHADING
} DeferredMode;
void GroundScene::OnLoad()
{
}

void GroundScene::OnUnload()
{
}

void GroundScene::OnStart()
{
}

void GroundScene::OnStop()
{
}

void GroundScene::OnUpdate()
{
}

void GroundScene::OnDraw()
{
}

void GroundScene::OnDrawDebug()
{
}

void GroundScene::OnDrawGui()
{
}
