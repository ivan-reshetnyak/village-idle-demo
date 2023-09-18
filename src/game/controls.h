#pragma once

#include <map>
#include <memory>
#include <set>

#include "win/window.h"
#include "math/common.h"


class controls {
public:
    enum class input_event {
        MOUSE_MOVE,
        MOUSE_L_DOWN, MOUSE_L_UP,
        MOUSE_R_DOWN, MOUSE_R_UP,
        MOUSE_M_DOWN, MOUSE_M_UP,
        KEYBOARD_KEY_DOWN, KEYBOARD_KEY_UP
    };

    class listener {
    public:
        friend controls;
        using ptr = std::weak_ptr<listener>;
        using storage_type = std::shared_ptr<listener>;


        /// @brief Process incoming event.
        /// @return true if event is to be consumed, false otherwise.
        virtual bool ProcessMouseEvent(controls*, input_event) { return false; }
        virtual bool ProcessKeyboardEvent(controls*, input_event, unsigned keyCode) { return false; }
        virtual ~listener() = default;

        int GetPriority() const { return priority; }

    protected:
        listener() = default;

    private:
        /// @brief listeners with HIGHER priority are processed first
        /// DO NOT set manually, controls::Add() handles this.
        /// @sa controls::Add()
        int priority = 0;
    };

private:
    /// @brief Mouse data class
    class mouse {
    public:
        enum class button_state {
            UP, DOWN
        };

        void Init(window::info);

        void MoveTo(const point& newPos);
        point GetPos() const { return curPos; }
        coord_winapi GetPosRelative() const { return winInfo.ClientToRelative(curPos); }
        point GetDelta() const { return curPos - prevPos; }


        coord_winapi GetDeltaWINAPI() const
        {
            point delta = GetDelta();
            point clientSize = winInfo.GetClientSize();
            return coord_winapi((float)delta.x / clientSize.x, (float)delta.y / clientSize.y);
        }


        coord_opengl GetDeltaOpenGL() const
        {
            point delta = GetDelta();
            point clientSize = winInfo.GetClientSize();
            return coord_opengl(2.f * delta.x / clientSize.x, -2.f * delta.y / clientSize.y);
        }


        void SetLeft(button_state state) { left = state; }
        void SetRight(button_state state) { right = state; }
        void SetMiddle(button_state state) { middle = state; }
        button_state GetLeft() const { return left; }
        button_state GetRight() const { return right; }
        button_state GetMiddle() const { return middle; }

    private:
        window::info winInfo;
        button_state
            left = button_state::UP,
            middle = button_state::UP,
            right = button_state::UP;
        point curPos{0, 0}, prevPos{0, 0};
    };

public:
    void Init(window::info);

    const mouse& GetMouse() const { return mMouse; }
    void OnMouseMove(int x, int y);
    void OnMouseLDown();
    void OnMouseLUp();
    void OnMouseRDown();
    void OnMouseRUp();
    void OnMouseMDown();
    void OnMouseMUp();

    void OnKeyDown(unsigned key) { SendEvent(input_event::KEYBOARD_KEY_DOWN, key); }
    void OnKeyUp(unsigned key) { SendEvent(input_event::KEYBOARD_KEY_UP, key); }

    controls& Add(int priority, listener::storage_type);
    controls& Remove(listener::ptr);

private:
    mouse mMouse;
    std::map<int, std::set<listener::storage_type>> listeners;

    void SendEvent(input_event);
    void SendEvent(input_event, unsigned key);
};
