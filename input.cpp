// actual state of the input at the begining of the frame
Mouse gui_mouse_state;
Keyboard gui_kb_state;
// copy of gui_mouse_state that is mutated in place by the gui code
// along the way of the frame
Mouse mutable_gui_mouse_state;
Keyboard mutable_gui_kb_state;
// state of the input at the end of the last frame, after gui mutation
// this is what's used by the rest of the code with a 1 frame lag
Mouse mouse_state;
Keyboard kb_state;

vec2f mouse_move_to_ignore;
bool need_to_ignore_mouse_move = false;

void reset_mouse_state() {
    gui_mouse_state.scroll = vec2f::O;
    gui_mouse_state.dpos = vec2f::O;
    for(int i=0 ; i<=GLFW_MOUSE_BUTTON_LAST ; i++) {
        gui_mouse_state.buttons[i].pressed = false;
        gui_mouse_state.buttons[i].released = false;
        gui_mouse_state.buttons[i].mods = 0;
    }
}

void swap_mouse_states() {
    mouse_state = mutable_gui_mouse_state;
    mutable_gui_mouse_state = gui_mouse_state;
}

void swap_keyboard_states() {
    kb_state = mutable_gui_kb_state;
    mutable_gui_kb_state = gui_kb_state;
}

void mouse_button_callback(int button, int action, int mods){
    InputButton* button_infos = &gui_mouse_state.buttons[button];

    if(action == GLFW_PRESS) {
        button_infos->pressed = true;
        button_infos->down = true;
        button_infos->mods = mods;
    } else {  // == GLFW_RELEASE
        button_infos->released = true;
        button_infos->down = false;
        button_infos->mods = mods;
    }}

void mouse_position_callback(double xpos, double ypos){
    vec2f next = vec2f{ (f32)xpos, (f32)ypos };
    if(need_to_ignore_mouse_move && next == mouse_move_to_ignore) {
        need_to_ignore_mouse_move = false;
        gui_mouse_state.dpos = vec2f::O;
    } else {
        gui_mouse_state.dpos = next - gui_mouse_state.pos;
    }
    gui_mouse_state.pos = next;
}

void mouse_scroll_callback(double xoffset, double yoffset){
    gui_mouse_state.scroll.x += xoffset;
    gui_mouse_state.scroll.y += yoffset;
}

void reset_keyboard_state() {
    std::queue<u32> empty;
    std::swap(gui_kb_state.chars, empty);

    for(int i=0 ; i<=GLFW_KEY_LAST; i++) {
        gui_kb_state.keys[i].pressed = false;
        gui_kb_state.keys[i].released = false;
        gui_kb_state.keys[i].repeat = false;
        gui_kb_state.keys[i].mods = 0;
    }
}

void keyboard_callback(int key, int scancode, int action, int mods) {
    if(key != GLFW_KEY_UNKNOWN){
        InputButton* key_infos = &gui_kb_state.keys[key];

        if(action == GLFW_PRESS) {
            key_infos->pressed = true;
            key_infos->down = true;
        } else if(action == GLFW_RELEASE) {
            key_infos->released = true;
            key_infos->down = false;
        } else {  // repeat
            key_infos->repeat = true;
        }

        key_infos->mods = mods;
    }
}

void char_callback(u32 code) {
    gui_kb_state.chars.push(code);
}

void ignore_mouse_move(vec2f pos) {
    mouse_move_to_ignore = pos;
    need_to_ignore_mouse_move = true;
}

void setup_input_callbacks(GLFWwindow* window) {
    glfwSetKeyCallback(window, keyboard_callback);
    glfwSetCharCallback(window, char_callback);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_position_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);

    // poll mouse pos once, otherwise a move needs to be made
    f64 x, y;
    glfwGetCursorPos(window, &x, &y);
    gui_mouse_state.pos = vec2f { (f32)x, (f32)y };
}

void update_inputs() {
    reset_keyboard_state();
    reset_mouse_state();
    glfwPollEvents();
    swap_mouse_states();
    swap_keyboard_states();
}
