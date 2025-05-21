#pragma once
#include "raylib.h"
#include "raymathext.h"

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

void UpdateButton(Button& button, Vector2 point);
void DrawButton(const Button& button);

inline Rectangle RecFromCenter(int center_x, int center_y, int width, int height)
{
    Rectangle rec;
    rec.x = center_x - width / 2;
    rec.y = center_y - height / 2;
    rec.width = width;
    rec.height = height;
    return rec;
}
