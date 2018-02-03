#pragma once

struct InputButton {
    bool pressed = false;
    bool released = false;
    bool down = false;
    bool repeat = false;
    i32 mods = 0;
};


struct Keyboard {
    std::queue<u32> chars;
    InputButton keys[GLFW_KEY_LAST+1];

    bool shift_down() {
        return keys[GLFW_KEY_LEFT_SHIFT].down
            || keys[GLFW_KEY_RIGHT_SHIFT].down;
    }

    bool control_down() {
        return keys[GLFW_KEY_LEFT_CONTROL].down
            || keys[GLFW_KEY_RIGHT_CONTROL].down;
    }

    bool alt_down() {
        return keys[GLFW_KEY_LEFT_ALT].down
            || keys[GLFW_KEY_RIGHT_ALT].down;
    }

    bool super_down() {
        return keys[GLFW_KEY_LEFT_SUPER].down
            || keys[GLFW_KEY_RIGHT_SUPER].down;
    }

    bool pressed_with_mod(i32 key, i32 mod) {
        return keys[key].pressed && (keys[key].mods & mod);
    }
};

struct Mouse {
    vec2f pos = vec2f::O;
    vec2f dpos = vec2f::O;
    vec2f scroll; // delta scrolled at this frame
    InputButton buttons[GLFW_MOUSE_BUTTON_LAST+1];

    InputButton left() { return buttons[GLFW_MOUSE_BUTTON_LEFT]; }
    InputButton right() { return buttons[GLFW_MOUSE_BUTTON_RIGHT]; }
};

// actual state of the input at the begining of the frame
extern "C" GLOBAL_STATE Mouse gui_mouse_state;
extern "C" GLOBAL_STATE Keyboard gui_kb_state;
// copy of gui_mouse_state that is mutated in place by the gui code
// along the way of the frame
extern "C" GLOBAL_STATE Mouse mutable_gui_mouse_state;
extern "C" GLOBAL_STATE Keyboard mutable_gui_kb_state;
// state of the input at the end of the last frame, after gui mutation
// this is what's used by the rest of the code with a 1 frame lag
extern "C" GLOBAL_STATE Mouse mouse_state;
extern "C" GLOBAL_STATE Keyboard kb_state;

