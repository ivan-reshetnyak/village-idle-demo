#include "pch.h"

#include "ui_clickable.h"


namespace ui {
clickable::clickable(const std::function<bool(controls*)>& checkBounds,
                     const std::function<void()>& callbackLeft,
                     const std::function<void()>& callbackRight)
    : checkBounds(checkBounds)
    , callbackLeft(callbackLeft), callbackRight(callbackRight)
{
}


bool clickable::ProcessMouseEvent(controls* ctrls, controls::input_event evt)
{
    if (isActive
        && (evt == controls::input_event::MOUSE_L_DOWN || evt == controls::input_event::MOUSE_R_DOWN)
        && checkBounds(ctrls)) {
        switch (evt) {
        case controls::input_event::MOUSE_L_DOWN:
            callbackLeft();
            return true;
        case controls::input_event::MOUSE_R_DOWN:
            callbackRight();
            return true;
        }
    }

    return false;
}
}
