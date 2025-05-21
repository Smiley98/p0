#include "GearScene.h"

static bool showMessageBox = false;

void GearScene::OnLoad()
{
}

void GearScene::OnUnload()
{
}

void GearScene::OnStart()
{
}

void GearScene::OnStop()
{
}

void GearScene::OnUpdate()
{
}

void GearScene::OnDraw()
{
}

void GearScene::OnDrawDebug()
{
}

void GearScene::OnDrawGui()
{
    if (GuiButton( { 24, 24, 120, 30 }, "#191#Show Message"))
        showMessageBox = true;

    if (showMessageBox)
    {
        int result = GuiMessageBox( { 85, 70, 250, 100 },
            "#191#Message Box", "Hi! This is a message!", "Nice;Cool");

        if (result >= 0) showMessageBox = false;
    }
}
