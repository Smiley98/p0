#include "MechAnimation.h"
#include "Shaders.h"

void LoadMechAnimation()
{
    gMechAnimation.clips = LoadModelAnimations("./assets/meshes/mech.glb", &gMechAnimation.count);
    gMechAnimation.model = LoadModel("./assets/meshes/mech.glb");

    gMechAnimation.model.materials[1].shader = gShaders.skinning;
    gMechAnimation.model.materials[1].maps[MATERIAL_MAP_DIFFUSE].color = DARKGRAY;
}

void UnloadMechAnimation()
{
    UnloadModelAnimations(gMechAnimation.clips, gMechAnimation.count);
    UnloadModel(gMechAnimation.model);
}

void UpdateMechAnimation(MechAnimation& ma)
{
    int index = ma.index;

    if (IsKeyPressed(KEY_T))
        ++ma.index %= ma.count;

    else if (IsKeyPressed(KEY_G))
        ma.index = (ma.index + ma.count - 1) % ma.count;

    if (index != ma.index)
        TraceLog(LOG_INFO, TextFormat("Playing animation %i - %s\n", ma.index, ma.clips[ma.index].name));

    ModelAnimation clip = ma.clips[ma.index];
    UpdateModelAnimationBones(ma.model, clip, ma.frame++);
}

void DrawMechAnimation(const MechAnimation& ma, Matrix transform)
{
    DrawMesh(ma.model.meshes[0], ma.model.materials[1], transform);
}
