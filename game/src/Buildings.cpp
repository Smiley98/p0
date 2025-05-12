#include "Buildings.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

struct BuildingModels
{
    Model td;
};

static BuildingModels fModels;
static Material fMaterial;

void LoadBuildings()
{
    fMaterial = LoadMaterialDefault();
    fMaterial.maps[MATERIAL_MAP_DIFFUSE].color = DARKGRAY;

    fModels.td = LoadModel("./assets/meshes/bld_td.obj");
}

void UnloadBuildings()
{
}

void UpdateBuildings()
{
}

void DrawBuildings()
{
    rlPushMatrix();
    rlRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    rlTranslatef(50.0f, 0.0f, 0.0f);
    DrawGrid(100, 1.0f);
    rlPopMatrix();

    rlPushMatrix();
    rlRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    rlTranslatef(-50.0f, 0.0f, 0.0f);
    DrawGrid(100, 1.0f);
    rlPopMatrix();

    for (float y = -40.0f; y <= 40.0f; y += 20.0f)
    {
        for (float x = -80.0f; x <= 80.0f; x += 20.0f)
        {
            // So I can see that sweet sweet mech animation!
            if (x == 0.0f && y == 0.0f) continue;
            
            rlEnableWireMode();
            DrawMesh(fModels.td.meshes[0], fMaterial, MatrixTranslate(x, y, 0.0f));
            rlDisableWireMode();
        }
    }
}
