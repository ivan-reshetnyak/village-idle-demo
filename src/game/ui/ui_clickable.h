#pragma once

#include <functional>

#include "game/controls.h"


namespace ui {
class clickable : public controls::listener {
public:
    using ptr = std::weak_ptr<clickable>;


    clickable(const std::function<bool(controls*)>& checkBounds,
              const std::function<void()>& callbackLeft,
              const std::function<void()>& callbackRight = [](){});
    virtual ~clickable() = default;

    void SetActive(bool newActive) { isActive = newActive; }

    virtual bool ProcessMouseEvent(controls*, controls::input_event) override;

private:
    bool isActive = true;
    std::function<void()> callbackLeft, callbackRight;
    std::function<bool(controls*)> checkBounds;
};
}
