#include "Widgets.h"

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
