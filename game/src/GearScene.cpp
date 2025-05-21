#include "GearScene.h"

void RayguiExample();

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
    // Decided against raygui for gear UI in the short-term since it can't draw textures
    //RayguiExample();
}

// raygui can't draw textures. If we reuse original assets, we'll need to draw the gear stat pannels as textured rectangles.
// Is it less long-term effort to design gear stat pannels in-engine, or continue to represent stat pannels as textures and author them externally??
// (Don't worry about this for now. We need a system that selects loadouts before worrying about aesthetics and ease of use).
void RayguiExample()
{
    static bool showMessageBox = false;

    // May optionally include icons. I doubt we'll use heavily stylized GUI
    //int clicked = GuiButton({ 24, 24, 120, 30 }, GuiIconText(ICON_INFO, "Show Message"));
    //int clicked = GuiButton({ 24, 24, 120, 30 }, "#191#Show Message"); // Can also wrap icon id in #
    int clicked = GuiButton({ 24, 24, 120, 30 }, "Show Message");
    if (clicked)
        showMessageBox = true;

    if (showMessageBox)
    {
        int result = GuiMessageBox({ 85, 70, 250, 100 },
            "Message Box Title", "Do you like this message box?", "Yes;No");

        if (result >= 0)
        {
            showMessageBox = false;
            switch (result)
            {
            case 0:
                TraceLog(LOG_INFO, "X was clicked");
                break;

            case 1:
                TraceLog(LOG_INFO, "Yes was clicked");
                break;

            case 2:
                TraceLog(LOG_INFO, "No was clicked");
                break;
            }
        }
    }
}
