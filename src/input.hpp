#pragma once

#include <map>
#include <queue>
#include "utils/GLCommon.hpp"

class Input {
public:

    void setFirstCursorPos(double x, double y) {
        m_lastCursorPos = m_cursorPos = {x, y};
        m_cursorOffset = glm::zero<glm::dvec2>();
        auto lol = glm::dvec2(0);
    }

    void setKeyPressed(int keyCode) {
        auto entry = keys.find(keyCode);

        if (entry == keys.end()) {
            keys.emplace(keyCode, Input::DownState);
        }

        entry->second = Input::DownState;
    }

    void setKeyReleased(int keyCode) {
        auto entry = keys.find(keyCode);

        if (entry == keys.end()) {
            keys.emplace(keyCode, Input::UpState);
        }

        entry->second = Input::UpState;
    }

    bool getKey(int keyCode) const {
        auto entry = keys.find(keyCode);

        if (entry == keys.end())
            return false;

        return entry->second.isPressed;
    }

    bool getKeyDown(int keyCode) const {
        auto entry = keys.find(keyCode);

        if (entry == keys.end())
            return false;

        return entry->second.isDown;
    }

    bool getKeyUp(int keyCode) {
        auto entry = keys.find(keyCode);

        if (entry == keys.end())
            return false;

        return entry->second.isUp;
    }

    const glm::vec2 &getScroll() const {
        return m_scrollOffset;
    }

    const glm::dvec2 &getCursorOffset() const {
        return m_cursorOffset;
    }

    const glm::dvec2 &getCursorPos() const {
        return m_cursorPos;
    }

    void handleKeyEvent(int keyCode, int action) {
        unhandled_keys.emplace(keyCode, action);
    }

    void handleScroll(float x, float y) {
        m_scroll.x += x;
        m_scroll.y += y;
    }

    void handleCursorPosition(double x, double y) {
        m_cursorPos.x = x;
        m_cursorPos.y = y;
    }

    void update() {
        for (auto &entry: keys) {
            entry.second.isUp = false;
            entry.second.isDown = false;
        }

        m_scrollOffset = m_scroll - m_lastScroll;
        m_lastScroll = m_scroll;

        m_cursorOffset = m_cursorPos - m_lastCursorPos;
        m_lastCursorPos = m_cursorPos;

        while (!unhandled_keys.empty()) {
            auto entry = unhandled_keys.front();
            unhandled_keys.pop();
            auto keyCode = std::get<0>(entry);
            auto action = std::get<1>(entry);

            if (action == GLFW_PRESS) {
                setKeyPressed(keyCode);
            } else if (action == GLFW_RELEASE) {
                setKeyReleased(keyCode);
            }
        }
    }

private:

    typedef struct {
        bool isPressed;
        bool isDown;
        bool isUp;
    } State;

    constexpr const static State DownState = State{true, true, false};
    constexpr const static State UpState = State{false, false, true};

private:
    std::map<int, State> keys;

    glm::vec2 m_scroll{};
    glm::vec2 m_lastScroll{};
    glm::vec2 m_scrollOffset{};

    glm::dvec2 m_lastCursorPos{};
    glm::dvec2 m_cursorPos{};
    glm::dvec2 m_cursorOffset{};


    std::queue<std::tuple<int, int>> unhandled_keys;
};