#include "raylib.h"
#include "raymath.h"

struct Gear {
    //Model gModel;
    const char* gName;
};
Gear Rifle = {
    "Rifle"
};
Gear Shotgun = {
    "Shotgun"
};
Gear MachineGun = {
    "Machine Gun"
};
struct GearList {
    Gear* items;
    int count;
    int currentIndex = 0;

    Gear* GetCurrent() {
        return &items[currentIndex];
    }

    void Next() {
        currentIndex = (currentIndex + 1) % count;
    }

    void Prev() {
        currentIndex = (currentIndex - 1 + count) % count;
    }
};
Gear gearItems[] = { Rifle, Shotgun, MachineGun };
GearList allGear = { gearItems, 3};

struct Player {
    int player_id = 1;
    Gear* gearSlots[4] = { nullptr };
    int gearCount = 0;
};
Player player1;
//UI loadout //errmmmm ui for testing >///<
Rectangle selectButton = { 100, 600, 200, 45 };
Rectangle gearBG = { 100, 100, 200, 450 };
Rectangle leftArrow = { gearBG.x - 20, gearBG.y+ gearBG.height/2, 20, 45 };
Rectangle rightArrow = { gearBG.x + gearBG.width , gearBG.y+ gearBG.height /2, 20, 45 };
bool IsHovered(Rectangle button, Vector2 mouse) { return CheckCollisionPointRec(mouse, button);}
bool IsPressed(Rectangle button, Vector2 mouse) {return CheckCollisionPointRec(mouse, button) && IsMouseButtonDown(MOUSE_LEFT_BUTTON);}
bool DrawButton(Rectangle rect, const char* Text, int fontSize, Color baseColour) {
    Vector2 mouse = GetMousePosition();
    Color drawColour = baseColour;
    bool isClicked = IsHovered(rect, mouse) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
    if (IsHovered(rect, mouse))
    {
        float tint = IsPressed(rect,mouse) ? 0.6f : 0.8f;
        drawColour = Color{
            (unsigned char)(baseColour.r * tint),
            (unsigned char)(baseColour.g * tint),
            (unsigned char)(baseColour.b * tint),
            baseColour.a
        };
    }
    DrawRectangleRec(rect, drawColour);
    DrawText(Text, selectButton.x + (selectButton.width - MeasureText(Text, fontSize)) / 2, selectButton.y + 15, fontSize, WHITE);

    return isClicked;
}
void UpdateUI() {
    Vector2 mouse = GetMousePosition();
    const char* gearName = allGear.GetCurrent()->gName;

    //Draw select button
    if (DrawButton(selectButton, "Select", 20, GREEN))
    { 
        //if player has enough room and doesnt have it in its slots yet
        Gear* current = allGear.GetCurrent();
        bool alreadyAdded = false;
        for (int i = 0; i < player1.gearCount; i++) {
            if (player1.gearSlots[i] == current) {
                alreadyAdded = true;
                break;
            }
        }
        if (!alreadyAdded && player1.gearCount < 4) {
            player1.gearSlots[player1.gearCount++] = current;
        }
        //debug tracelog
        TraceLog(LOG_INFO, TextFormat("Player%d current gear:", player1.player_id));

        for (int i = 0; i < player1.gearCount; i++) {
            if (player1.gearSlots[i]) {
                TraceLog(LOG_INFO, TextFormat("  Slot %d: %s", i, player1.gearSlots[i]->gName));
            }
            else {
                TraceLog(LOG_INFO, TextFormat("  Slot %d: Empty", i));
            }
        }
    }
    if (DrawButton(rightArrow, NULL, 0, ORANGE))
        allGear.Next();
    if (DrawButton(leftArrow, NULL, 0, ORANGE))
        allGear.Prev();
    DrawRectangleRec(gearBG,LIGHTGRAY);
    DrawText(gearName, gearBG.x + (gearBG.width - MeasureText(gearName, 30)) / 2, gearBG.y + 15, 30, WHITE);
}


void GameInit()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 800, "Game");
    SetTargetFPS(144);
}

void GameCleanup()
{
    CloseWindow();
}

void GameUpdate(float dt)
{
    UpdateUI();
}

void GameDraw()
{
    BeginDrawing();
    ClearBackground(DARKGRAY);
    DrawText("Hello World!", 10, 10, 20, RED);
    EndDrawing();
}

int main()
{
    GameInit();

    while (!WindowShouldClose())
    {
        GameUpdate(GetFrameTime());
        GameDraw();
    }

    GameCleanup();
    return 0;
}
