#include "pch.h"

#include "controls.h"


void controls::Init(window::info winInfo)
{
    mMouse.Init(winInfo);
}


void controls::SendEvent(input_event inpEvent)
{
    for (auto& priorityIt : listeners) {
        for (auto& listenerIt : priorityIt.second) {
            if (listenerIt->ProcessMouseEvent(this, inpEvent)) {
                // Event consumed
                return;
            }
        }
    }
}


void controls::SendEvent(input_event inpEvent, unsigned key)
{
    for (auto& priorityIt : listeners) {
        for (auto& listenerIt : priorityIt.second) {
            if (listenerIt->ProcessKeyboardEvent(this, inpEvent, key)) {
                // Event consumed
                return;
            }
        }
    }
}



controls& controls::Add(int priority, listener::storage_type newListener)
{
    priority = -priority;  // Negate for reverse sorting
    newListener->priority = priority;
    auto priorityIt = listeners.find(priority);
    if (priorityIt == listeners.end()) {
        priorityIt = listeners.insert({priority, std::set<listener::storage_type>()}).first;
    }

    priorityIt->second.insert(newListener);
    return *this;
}


controls& controls::Remove(listener::ptr toDelete)
{
    auto pListener = toDelete.lock();
    if (!pListener) {
        // Listener was never added into manager or was already deleted.
        return *this;
    }

    auto priorityIt = listeners.find(pListener->priority);
    if (priorityIt == listeners.end()) {
        // Something wrong with priority.
        return *this;
    }

    auto listenerIt = priorityIt->second.find(pListener);
    if (listenerIt == priorityIt->second.end()) {
        // Listener was never added into manager or was already deleted.
        return *this;
    }
    priorityIt->second.erase(listenerIt);
    return *this;
}


void controls::mouse::Init(window::info winInfo)
{
    this->winInfo = winInfo;
}


void controls::mouse::MoveTo(const point& newPos)
{
    prevPos = curPos;
    curPos = newPos;
}


void controls::OnMouseMove(int x, int y)
{
    mMouse.MoveTo(point(x, y));
    SendEvent(input_event::MOUSE_MOVE);
}


void controls::OnMouseLDown()
{
    mMouse.SetLeft(mouse::button_state::DOWN);
    SendEvent(input_event::MOUSE_L_DOWN);
}


void controls::OnMouseLUp()
{
    mMouse.SetLeft(mouse::button_state::UP);
    SendEvent(input_event::MOUSE_L_UP);
}


void controls::OnMouseRDown()
{
    mMouse.SetRight(mouse::button_state::DOWN);
    SendEvent(input_event::MOUSE_R_DOWN);
}


void controls::OnMouseRUp()
{
    mMouse.SetRight(mouse::button_state::UP);
    SendEvent(input_event::MOUSE_R_UP);
}


void controls::OnMouseMDown()
{
    mMouse.SetMiddle(mouse::button_state::DOWN);
    SendEvent(input_event::MOUSE_M_DOWN);
}


void controls::OnMouseMUp()
{
    mMouse.SetMiddle(mouse::button_state::UP);
    SendEvent(input_event::MOUSE_M_UP);
}
