#include "GearScene.h"

using ButtonOnClick = void(*)(void*);

struct Button
{
    Rectangle rec;
    Color color_out = DARKGRAY;
    Color color_in = GRAY;
    Color color_click = LIGHTGRAY;
    Color color;

    ButtonOnClick on_click = nullptr;
    void* on_click_data = nullptr;
};

void UpdateButton(Button& button, Vector2 point)
{
    bool collision = CheckCollisionPointRec(point, button.rec);
    if (collision)
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && button.on_click != nullptr)
        {
            button.on_click(button.on_click_data);
        }

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            button.color = button.color_click;
        }
        else
        {
            button.color = button.color_in;
        }
    }
    else
    {
        button.color = button.color_out;
    }
}

void DrawButton(const Button& button)
{
    DrawRectangleRec(button.rec, button.color);
}

inline Rectangle RecFromCenter(int center_x, int center_y, int width, int height)
{
    Rectangle rec;
    rec.x = center_x - width / 2;
    rec.y = center_y - height / 2;
    rec.width = width;
    rec.height = height;
    return rec;
}

struct ClickTestData
{
    int number;
};

void OnClickTest(void* data)
{
    ClickTestData& click_data = *(ClickTestData*)data;
    TraceLog(LOG_INFO, "Automatic button pressed. Data: %i", click_data.number);
}

static Button button;

void RayguiExample();

void GearScene::OnLoad()
{
    button.rec = RecFromCenter(GetScreenWidth() / 2, GetScreenHeight() / 2, 80, 40);
    button.color_out = RED;
    button.color_in = ORANGE;
    button.color_click = GOLD;
    
    static ClickTestData click_test_data;
    click_test_data.number = 420;

    button.on_click = OnClickTest;
    button.on_click_data = &click_test_data;
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
    UpdateButton(button, GetMousePosition());
}

void GearScene::OnDraw()
{
}

void GearScene::OnDrawDebug()
{
}

void GearScene::OnDrawGui()
{
    // Manual button:
    Rectangle btnTest = RecFromCenter(GetScreenWidth() / 2, GetScreenHeight() / 4, 80, 40);
    bool collision = CheckCollisionPointRec(GetMousePosition(), btnTest);
    DrawRectangleRec(btnTest, collision ? RED : GREEN);
    if (collision && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        TraceLog(LOG_INFO, "Manual button pressed. Data: %i", 69);
    }

    // Automatic button:
    DrawButton(button);

    // The manual button has less setup, and its nice to not have to cast a void*, but its technically incorrect since it updates within draw
    // Let's try and use automatic buttons where possible for clear separation between update vs draw

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
