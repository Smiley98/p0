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
GBuffer gBuffer = { 0 };

//this is from ray lib example, I think I need something like this ???
//gBuffer.framebuffer = rlLoadFramebuffer();
void GroundScene::OnLoad()
{
   
    camera.position = { 5.0f, 4.0f, 5.0f };
    camera.target = { 0.0f, 1.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

  
    hexShader = LoadShader(0, TextFormat("assets/shaders/hexagon_grid_raymarch.frag"));

    
    timeLoc = GetShaderLocation(hexShader, "u_time");
    resLoc = GetShaderLocation(hexShader, "u_resolution");
    fgColLoc = GetShaderLocation(hexShader, "u_fg_col");
    bgColLoc = GetShaderLocation(hexShader, "u_bg_col");
    hexResLoc = GetShaderLocation(hexShader, "u_hex_res");
    hexThicknessLoc = GetShaderLocation(hexShader, "u_hex_thickness");

   
    fgColor = { 1.0f, 0.10f, 1.0f };
    bgColor = { 0.0f, 1.0f, 0.0f };
    hexRes = 10.0f;
    hexThickness = 0.1f;

    
    hexGridTarget = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
}

void GroundScene::OnUnload()
{
    UnloadShader(hexShader);
    UnloadRenderTexture(hexGridTarget);
}

void GroundScene::OnStart()
{
    
}

void GroundScene::OnStop()
{
   
}

void GroundScene::OnUpdate()
{
    time = GetTime(); // Store for use in drawing

}

void GroundScene::OnDraw()
{
    BeginTextureMode(hexGridTarget);
    BeginShaderMode(hexShader);
        Vector2 resolution = { hexGridTarget.texture.width, hexGridTarget.texture.height };
        ClearBackground(BLACK);
        SetShaderValue(hexShader, timeLoc, &time, SHADER_UNIFORM_FLOAT);
        SetShaderValue(hexShader, resLoc, &resolution, SHADER_UNIFORM_VEC2);
        SetShaderValue(hexShader, fgColLoc, &fgColor, SHADER_UNIFORM_VEC3);
        SetShaderValue(hexShader, bgColLoc, &bgColor, SHADER_UNIFORM_VEC3);
        SetShaderValue(hexShader, hexResLoc, &hexRes, SHADER_UNIFORM_FLOAT);
        SetShaderValue(hexShader, hexThicknessLoc, &hexThickness, SHADER_UNIFORM_FLOAT);
        DrawRectangle(0, 0, resolution.x, resolution.y, WHITE);
    EndShaderMode();
    EndTextureMode();

    DrawTextureRec(hexGridTarget.texture, { 0, 0, resolution.x, -resolution.y }, { 0, 0 }, WHITE);
}

void GroundScene::OnDrawDebug()
{
}

void GroundScene::OnDrawGui()
{
}
