#ifndef STD_PCH
#include "std.h"
#endif
#pragma hdrstop ( "build/std.h.pch" )

#define GLOBAL_STATE __declspec(dllexport)

#include "gui.h"
#include "io.h"
#include "input.h"
#include "gfx.h"
#include "timers.h"

enum ui_active_mode {
    ACTIVE_MODE_NONE,
    ACTIVE_MODE_SCROLL_X,
    ACTIVE_MODE_SCROLL_Y,
    ACTIVE_MODE_MOVE_MAYBE,  // when the left mouse is down, but the cursor did not move enough yet
    ACTIVE_MODE_MOVE,
    ACTIVE_MODE_RESIZE,
};

struct active_item_infos {
    // panel
    vec2f last_drag_pos = vec2f();
    ui_active_mode mode;

    // textfield
    i32 cursor_pos;  // where we type
    i32 selection_start;  // -1 if no selection
    bool show_cursor;
    f64 cursor_time_before_blink;
    bool wait_for_released = false;

    void reset_textfield_stuff() {
        cursor_pos = -1;
        selection_start = -1;
        show_cursor = false;
        cursor_time_before_blink = 0.f;
    }
};

struct ui_panel;

active_item_infos active_item;
GUI_ID_TYPE active_item_id;
GUI_ID_TYPE active_item_id_before_panel;
i32 max_panel_zindex = 0;  // TODO handle overflow
std::map<GUI_ID_TYPE, ui_panel> ui_panels;
std::stack<ui_panel*> panel_stack;
std::vector<u64> ui_context_stack;
u64 current_ui_context = 0;
bool focus_next_widget = false;
GUI_ID_TYPE last_focussed_widget_id = -1;  // tab selection - to make sure a widget does not respond to the tab key press if it just took the focus
GUI_ID_TYPE previous_drawn_textfield_id = -1;  // to select the previous textfield / slider
GUI_ID_TYPE widget_to_focus = -1;  // shift tab selection to select the previous textfield / slider
bool focus_given_widget = false;  // to reset widget_to_focus if it wasnt found

// hints about where to create next panels
vec2f panel_spawn = {-1+PANEL_MARGIN, -1+PANEL_MARGIN};
f32 panel_spawn_next_x = panel_spawn.x;
f32 panel_no_space_left = false;

u32 group_offset = 0;

void focus_next_textfield() {
    focus_next_widget = true;
}

u64 prng(u64 n) {  // xorshift*
    n ^= n >> 12;
    n ^= n << 25;
    n ^= n >> 27;
    return n * 0x2545F4914F6CDD1D;
}

u64 ui_context() {
    u64 h0 = 0;
    for (u64 h : ui_context_stack) {
        h0 = prng(h0 ^ prng(h));
    }
    return h0;
}

void ui_push_ctx(u64 c) {
    ui_context_stack.push_back(c);
    current_ui_context = ui_context();
}

void ui_push_ctx(void* c) {
    ui_push_ctx((u64)c);
}

void ui_pop_ctx() {
    ui_context_stack.pop_back();
    current_ui_context = ui_context();
}


// generic container to store stuff for each widgets
// creates one static map for each stored type
// and one cleaner for each type
// TODO cleaner implementation

std::set<GUI_ID_TYPE> alive_containers;  // keep track of alive items with a container to cleanup dead ones
std::set<GUI_ID_TYPE> dead_containers;
std::vector<std::function<void(std::set<GUI_ID_TYPE>)>> container_cleaner_container;

void container_is_alive(GUI_ID_TYPE id) {
    alive_containers.insert(id);
    dead_containers.erase(id);
}

template<typename T>
struct static_container_container {
    std::map<GUI_ID_TYPE, T> map;
    static_container_container() {
        std::map<GUI_ID_TYPE, T>& m = map;
        auto cleaner = [&m](std::set<GUI_ID_TYPE> dead_ids) {
            for(GUI_ID_TYPE id: dead_ids)
                m.erase(id);
        };
        container_cleaner_container.push_back(cleaner);
    }
};

template<typename T>
std::map<GUI_ID_TYPE, T>& get_static_container() {
    static static_container_container<T> container;  // =)
    return container.map;
}

template<typename T>
bool load_static_data(GUI_ID_TYPE id, T* data) {
    container_is_alive(id);

    auto& map = get_static_container<T>();
    auto pair = map.find(id);
    if(pair == map.end()) {  // not found => init the panel
        return false;
    } else {
        *data = pair->second;
        return true;
    }
}

template<typename T>
void store_static_data(GUI_ID_TYPE id, T data) {
    container_is_alive(id);

    get_static_container<T>()[id] = data;
}

template<typename T>
bool remove_static_data(GUI_ID_TYPE id) {
    container_is_alive(id);

    auto& map = get_static_container<T>();
    auto pair = map.find(id);
    if(pair == map.end()) {  // not found => init the panel
        return false;
    } else {
        map.erase(pair);
        return true;
    }
}


vec2f text_size(std::string str, f32 font_size){
    return TEXT_WIDTH(str.data(), str.size(), font_size);
}




enum ui_shape_type {
    UI_RECTANGLE,
    UI_TEXT,
};

struct ui_shape {
    ui_shape_type type;

    // shared
    vec4f color = white;
    i32 zindex = 0;
    bool clipped_to_panel;
    aabb2i scissor;
    aabb2f rect = aabb2f();

    // text
    std::string text;
    f32 font_size = 0;

    // rect
    vec4f border_color = white;
    f32 border_radius = 0;

    ui_shape() {}

    // rect
    ui_shape(
            aabb2f rect,
            vec4f color=white,
            i32 zindex=0,
            vec4f bcolor=white,
            f32 bradius=0,
            bool clipped_to_panel=true,
            aabb2i scissor=aabb2i())
             :
                 type(UI_RECTANGLE),
                 color(color),
                 zindex(zindex),
                 clipped_to_panel(clipped_to_panel),
                 scissor(scissor),
                 rect(rect),
                 border_color(bcolor),
                 border_radius(bradius)
                 {}

    // text
    ui_shape(
            std::string text,
            vec2f pos,
            f32 font_size=30,
            vec4f color=black,
            i32 zindex=0,
            bool clipped_to_panel=true,
            aabb2i scissor=aabb2i())
             :
                 type(UI_TEXT),
                 color(color),
                 zindex(zindex),
                 clipped_to_panel(clipped_to_panel),
                 scissor(scissor),
                 rect(aabb2f(pos, text_size(text, font_size))),
                 text(text),
                 font_size(font_size)
                 {}

    /* std::string to_string() { */
    /*     std::ostringstream stream; */
    /*     stream << "Shape(" << rect.to_string() << ") @ " << zindex; */
    /*     return stream.str(); */
    /* } */
};

struct dock_area;

struct ui_panel {
    bool hidden;
    aabb2f rect;
    vec2f item_spawn;
    aabb2f current_row_bb;
    i32 zindex;
    vec2f packed_dim;
    bool pushed_this_frame = false;
    bool pushed_last_frame = false;

    std::vector<ui_shape> shapes;

    vec2f scroll;
    vec2f component_dim;  // usually == packed_dim, except when some components are inserted / removed while the panel is open
    vec2f scrollbar_size;

    dock_area* dock = NULL;
    vec2f undocked_dim;  // remember old pos/dim

    vec2f padding = vec2f{ PANEL_PADDING, PANEL_PADDING };
    bool draw_border = true;

    ui_panel():
        hidden(true),
        rect(aabb2f()),
        zindex(0),
        packed_dim(vec2f()),
        shapes(std::vector<ui_shape>()),
        scroll(vec2f()),
        component_dim(vec2f()),
        scrollbar_size(vec2f()) {}

    // box to components
    aabb2f inside_box() const {
        return aabb2f(rect.corner + padding, rect.dim);
    }

    // box to border - before the scrollbars
    aabb2f border_box() const {
        return noscrollbar_box().with_margin(vec2f{ PANEL_BORDER_RADIUS, PANEL_BORDER_RADIUS });
    }

    aabb2f noscrollbar_box() const {
        aabb2f nsb = bounding_box();
        nsb.dim -= scrollbar_size;
        return nsb;
    }

    // box to exterior
     aabb2f bounding_box() const {
         return aabb2f(rect.corner, rect.dim + 2*padding);
    }

     // 'resize' corner bounding box
     aabb2f resize_box() const {
         return aabb2f(rect.opposite_corner() + 2*padding - PANEL_RESIZE_CORNER_DIM,
                       PANEL_RESIZE_CORNER_DIM + scrollbar_size);
     }

    /* std::string to_string() { */
    /*     std::ostringstream stream; */
    /*     stream << "Panel(" << rect.to_string() << ") @ " << zindex; */
    /*     return stream.str(); */
    /* } */
};

struct dock_area_panel_info {
    ui_panel* panel = NULL;
    f32 size_ratio = 0.f;
};

struct dock_area {
    f32 width;
    std::vector<dock_area_panel_info> panels;
};

dock_area left_dock = {260.f, {}};
dock_area right_dock = {300.f, {}};

i32 find_panel_dock_index(ui_panel* panel) {
    if(!panel->dock)
        return -1;

    u32 panel_idx = 0;
    for(auto& p: panel->dock->panels) {
        if(p.panel == panel)
            return panel_idx;
        panel_idx++;
    }

    return -1;
}

void detach_panel(ui_panel* panel) {
    if(!panel->dock)
        return;

    auto dpanels = &panel->dock->panels;

    i32 panel_idx = find_panel_dock_index(panel);
    check(panel_idx != -1);

    // recompute every size ratio to keep total = 1
    if(dpanels->size() > 1) {
        f32 ratio = 1.f/(1.f-(*dpanels)[panel_idx].size_ratio);
        dpanels->erase(dpanels->begin()+panel_idx);
        for(u32 i=0 ; i<dpanels->size() ; i++)
            (*dpanels)[i].size_ratio *= ratio;  // TODO handle precision errors
    } else {
        dpanels->clear();
    }

    panel->rect.dim = panel->undocked_dim;
    panel->dock = NULL;

    panel->zindex = max_panel_zindex++;
}

u32 panel_insert_pos(dock_area* dock, f32 y) {
    // find where it should be inserted
    u32 insert_idx = dock->panels.size();
    f32 current_y = 0;
    if(y >= 0)
        for(u32 i=0 ; i<dock->panels.size() ; i++) {
            current_y += dock->panels[i].size_ratio*WINDOW_HEIGHT;
            if(current_y > y) {
                insert_idx = i;
                break;
            }
        }
    return insert_idx;
}

void attach_panel(ui_panel* panel, dock_area* dock, f32 y_pos=-1.f) {
    if(panel->dock == dock)
        return;

    if(panel->dock != NULL)
        detach_panel(panel);

    panel->dock = dock;

    // recompute every size ratio to keep total = 1
    f32 ratio = 1.f - 1.f/(1+dock->panels.size());
    for(auto& p: dock->panels)
        p.size_ratio *= ratio;  // TODO handle precision errors

    u32 insert_idx = panel_insert_pos(dock, y_pos);
    dock->panels.insert(dock->panels.begin()+insert_idx, {panel, 1.f/(1+dock->panels.size())});  // (size ratio, index)
    panel->undocked_dim = panel->rect.dim;

    panel->zindex = 0;
}

void set_current_panel_size_ratio(f32 s) {
    ui_panel* panel = panel_stack.top();
    dock_area* dock = panel->dock;
    if(dock == NULL)
        return;

    f32 old_size = -1.f;
    for(auto& pi: dock->panels) {
        if(pi.panel == panel) {
            old_size = pi.size_ratio;
            break;
        }
    }
    assert(old_size >= 0.f);

    f32 mult = (1.f - s) / (1.f - old_size);
    for(auto& pi: dock->panels) {
        if(pi.panel == panel)
            pi.size_ratio = s;
        else
            pi.size_ratio *= mult;
    }
}

void reorder_docked_panel(ui_panel* panel, f32 y_pos) {
    if(!panel->dock)
        return;

    i32 panel_idx = find_panel_dock_index(panel);
    check(panel_idx != -1);
    dock_area_panel_info panel_info = panel->dock->panels[panel_idx];
    panel->dock->panels.erase(panel->dock->panels.begin()+panel_idx);

    u32 insert_idx = panel_insert_pos(panel->dock, y_pos);
    panel->dock->panels.insert(panel->dock->panels.begin()+insert_idx, panel_info);
}


static f64 last_frame_duration[FPS_WIDGET_FRAME_COUNT];
u8 ui_current_frame_idx;

f64 ui_dt;
void begin_ui(f64 dt) {
    assert(group_offset == 0);

    ui_dt = dt;

    // fps stuff
    ui_current_frame_idx = (ui_current_frame_idx+1)%FPS_WIDGET_FRAME_COUNT;
    last_frame_duration[ui_current_frame_idx] = dt;


    // reset panel stuff

    std::vector<GUI_ID_TYPE> toremove;
    for(auto& panel: ui_panels) {
        panel.second.shapes.clear();
        panel.second.item_spawn = vec2f();
        panel.second.current_row_bb = aabb2f();
        panel.second.pushed_last_frame = panel.second.pushed_this_frame;
        panel.second.pushed_this_frame = false;

        if(!panel.second.pushed_last_frame
                && panel.second.dock) {  // TODO =( - to handle docked panel hidding etc
            detach_panel(&panel.second);
            toremove.push_back(panel.first);
        }
    }

    for(GUI_ID_TYPE _id: toremove)  // TODO =( - to handle docked panel hidding etc
        ui_panels.erase(_id);

    focus_next_widget = false;
    last_focussed_widget_id = -1;

    // cleanup
    for(auto cleaner: container_cleaner_container)
        cleaner(dead_containers);

    std::swap(alive_containers, dead_containers);
    alive_containers.clear();

    if(!focus_given_widget)
        widget_to_focus = -1;

    focus_given_widget = false;
}

bool draw_ui = true;

void render_ui_status();
void render_ui() {
    if(!draw_ui) return;

    SCOPE_TIMER(TIMER_GPU | TIMER_CPU);

    // sort panels
    std::vector<ui_panel> sorted_panels;
    for(const auto& panel: ui_panels) {
        sorted_panels.push_back(panel.second);
    }

    std::stable_sort(sorted_panels.begin(), sorted_panels.end(),
            [](const ui_panel& p1, const ui_panel& p2) {
                return p1.zindex < p2.zindex;
            }
    );

    // draw all initialized none empty panels
    for(ui_panel panel: sorted_panels)
        if(!panel.shapes.empty() && !panel.hidden){

            // sort shapes
            std::stable_sort(panel.shapes.begin(), panel.shapes.end(),
                    [](const ui_shape& s1, const ui_shape& s2) {
                        return s1.zindex < s2.zindex;
                    }
            );

            // draw panel rect
            if(panel.draw_border)
            DRAW_RECT(panel.bounding_box(),
                      PANEL_COLOR,
                      PANEL_BORDER_COLOR,
                      panel.draw_border ? PANEL_BORDER_RADIUS: 0.f);

            // clip rectangles to their panel
            for(ui_shape& shape: panel.shapes) {

                // intersection of panel's scissor & shape's one
                if(shape.scissor.dim.x == 0) shape.scissor.dim.x = WINDOW_WIDTH;
                if(shape.scissor.dim.y == 0) shape.scissor.dim.y = WINDOW_HEIGHT;

                if(shape.clipped_to_panel)
                    // padding restricted to border for clipping
                    shape.scissor &= panel.border_box();

                switch(shape.type) {
                    case UI_RECTANGLE:
                        DRAW_RECT(shape.rect,
                                  shape.color,
                                  shape.border_color,
                                  shape.border_radius,
                                  shape.scissor);
                        break;
                    case UI_TEXT:
                        if(!kb_state.keys[GLFW_KEY_F11].down)
                        DRAW_TEXT(
                                shape.rect.corner,
                                shape.font_size,
                                shape.text,
                                shape.color,
                                shape.scissor);
                        break;
                }
            }
        }

    render_ui_status();
}


ui_panel* first_panel_at(vec2f pos, bool include_borders=false) {  // handles focus
    i32 max_zindex = -1;
    ui_panel* max_panel = 0;
    for(auto& panel: ui_panels) {
        if(!panel.second.pushed_last_frame) continue;
        aabb2f bb = include_borders ? panel.second.bounding_box() : panel.second.border_box();
        if(!panel.second.hidden
                && bb.contains(pos)
                && panel.second.zindex > max_zindex) {
            max_zindex = panel.second.zindex;
            max_panel = &panel.second;
        }
    }
    /* if(pos == gui_mouse_state.pos && gui_mouse_state.left().pressed) */
    /*     printf("panel: %lu\n", (u64)max_panel); */
    return max_panel;
}


f32 absolute_or_zero(f32 in, f32 _default) {
    if(in > 0)
        return in;
    else if(in == 0)
        return 0;
    else if(in < 0)
        return -in * _default;
    unreachable();
}

f32 absolute_or_not(f32 in, f32 _default) {
    if(in > 0)
        return in;
    else if(in == 0)
        return _default;
    else if(in < 0)
        return -in * _default;
    unreachable();
}

vec2f absolute_or_zero(vec2f in, vec2f _default) {
    return vec2f{ absolute_or_zero(in.x, _default.x),
                  absolute_or_zero(in.y, _default.y) };
}

vec2f absolute_or_not(vec2f in, vec2f _default) {
    return vec2f{ absolute_or_not(in.x, _default.x),
                  absolute_or_not(in.y, _default.y) };
}

std::string get_clipboard() {
    const char* res = glfwGetClipboardString(WINDOW_HANDLE);
    if(res == NULL)
        return std::string();
    return std::string(res);
}

void set_clipboard(std::string str) {
    glfwSetClipboardString(WINDOW_HANDLE, str.data());
}

inline bool mouse_inside(const aabb2f rect, const ui_panel* panel=NULL) {
    if(panel == NULL)
        panel = panel_stack.top();

    return rect.contains(gui_mouse_state.pos)
        && panel == first_panel_at(gui_mouse_state.pos);
}

inline bool mouse_inside(const ui_shape& shape, const ui_panel* panel=NULL) {
    return mouse_inside(shape.rect, panel);
}

void eat_kb_state() {
    for (u32 i = 0; i < array_size(mutable_gui_kb_state.keys); i++) {
        mutable_gui_kb_state.keys[i].pressed = false;
        mutable_gui_kb_state.keys[i].released = false;
        mutable_gui_kb_state.keys[i].down = false;
    }
}

void eat_mouse_state() {
    if(!draw_ui) return;
    mutable_gui_mouse_state.scroll = vec2f();
    for(i32 i=0 ; i<=GLFW_MOUSE_BUTTON_LAST ; i++) {
        mutable_gui_mouse_state.buttons[i].pressed = false;
        mutable_gui_mouse_state.buttons[i].released = false;
        mutable_gui_mouse_state.buttons[i].down = false;
    }
}

void _push_panel(GUI_ID_TYPE id,
      dock_id dock,
      ui_resize_policy resize_policy,
      u8 scroll_flags,
      vec2f padding,
      bool draw_border) {

    auto panel_pair = ui_panels.find(id);

    // not found => init the panel
    if(panel_pair == ui_panels.end()) {
        ui_panel panel;
        panel.zindex = max_panel_zindex++;
        panel.padding = padding;
        panel.draw_border = draw_border;
        ui_panels[id] = panel;
        panel_stack.push(&ui_panels[id]);
        return;
    }

    ui_panel* panel = &panel_pair->second;
    panel->pushed_this_frame = true;

    // init panel
    if(panel->hidden) {
        panel->hidden = false;  // is now initialized

        // pack panel
        panel->packed_dim = panel->component_dim;
        panel->rect.dim = absolute_or_not(resize_policy.dim, panel->packed_dim);

        f32 final_width = panel->rect.dim.x + 2*panel->padding.x;
        f32 final_height = panel->rect.dim.y + 2*panel->padding.y;

        // if we see it for the first time, decide where to make it spawn
        if(panel_no_space_left) {
            panel_spawn.x += PANEL_MARGIN;
            panel_spawn.y += PANEL_MARGIN;

        // no space in the current column, create a new one
        } else if(panel_spawn.y + final_height > WINDOW_HEIGHT) {
            panel_spawn.x = panel_spawn_next_x + PANEL_MARGIN;

            if(panel_spawn.x + final_width > WINDOW_WIDTH) {
                panel_no_space_left = true;
                panel_spawn.x = -1 + 2*PANEL_MARGIN;
                panel_spawn.y = -1 + 2*PANEL_MARGIN;
            }

            panel_spawn.y = -1 + PANEL_MARGIN;
        }

        // space left
        panel->rect.corner.x = panel_spawn.x;
        panel->rect.corner.y = panel_spawn.y;

        if(!panel_no_space_left) {
            panel_spawn.y += final_height + PANEL_MARGIN;
            panel_spawn_next_x = std::max(panel_spawn_next_x, panel_spawn.x + final_width);
        }

        if(dock != dock_id::none)
            attach_panel(panel, dock == dock_id::left ? &left_dock : &right_dock, INFINITY);
    }

    if(dock == dock_id::none && !resize_policy.enabled) {
        if(resize_policy.pos != vec2f::O)
            panel->rect.corner = resize_policy.pos;
        if(resize_policy.dim != vec2f::O)
            panel->rect.dim = absolute_or_not(resize_policy.dim, panel->packed_dim);
    }

    if(panel->dock != NULL) {
        assert(!panel->hidden);

        f32 offset = 0.f;
        f32 height = -1.f;
        i32 i=0;

        u32 valid_panels = 0;
        f32 height_to_distrib = 0.f;
        for(dock_area_panel_info& pi: panel->dock->panels) {
            if(!pi.panel->hidden && pi.panel->pushed_last_frame)
                valid_panels++;
            else
                height_to_distrib += pi.size_ratio;
        }
        f32 total_height = 1-height_to_distrib;
        f32 height_mult = 1/total_height;

        for(dock_area_panel_info& pi: panel->dock->panels) {
            if(pi.panel == panel) {
                height = pi.size_ratio * height_mult;
                break;
            }
            if(!pi.panel->hidden && pi.panel->pushed_last_frame)
                offset += pi.size_ratio * height_mult;
            i++;
        }
        assert(height != -1.f);
        f32 dock_height = WINDOW_HEIGHT - status_bar_locked_height();
        if(panel->dock == &left_dock)
            panel->rect.corner = {0.f, offset*dock_height};
        else  // right dock
            panel->rect.corner = {WINDOW_WIDTH-panel->dock->width-2*panel->padding.x, offset*dock_height};
        assert(panel->dock->panels.size() > 0);
        bool last = (i == (i32)panel->dock->panels.size()-1);
        panel->rect.dim = {panel->dock->width,
                           height*dock_height - 2*panel->padding.y
                               + (last ? 0 : PANEL_BORDER_RADIUS)};
    }

    // check handle inputs
    if(active_item_id == id && gui_mouse_state.left().released) {
        eat_mouse_state();
        active_item_id = 0;
    }

    // cf explanation in the scrollbar shape below
    vec2f unresized_dim = panel->rect.dim;

    if(active_item_id == id && gui_mouse_state.left().down) {
        eat_mouse_state();

        switch(active_item.mode) {
            case ACTIVE_MODE_MOVE_MAYBE:
                if(norm(active_item.last_drag_pos - gui_mouse_state.pos) < 30.f)
                    break;
                active_item.mode = ACTIVE_MODE_MOVE;

            case ACTIVE_MODE_MOVE:
                if(panel->dock) {
                    if(        panel->dock == &left_dock  && gui_mouse_state.pos.x < panel->dock->width
                            || panel->dock == &right_dock && gui_mouse_state.pos.x > WINDOW_WIDTH-panel->dock->width)
                        reorder_docked_panel(panel, gui_mouse_state.pos.y);
                    else
                        detach_panel(panel);
                } else {
                    if(gui_mouse_state.pos.x < 50.f)
                        attach_panel(panel, &left_dock, gui_mouse_state.pos.y);
                    else if(gui_mouse_state.pos.x > WINDOW_WIDTH-50.f)
                        attach_panel(panel, &right_dock, gui_mouse_state.pos.y);
                    else
                        panel->rect.corner = panel->rect.corner + (gui_mouse_state.pos - active_item.last_drag_pos);
                }
                active_item.last_drag_pos = gui_mouse_state.pos;
                break;

            case ACTIVE_MODE_RESIZE:
            {
                if(panel->dock) {
                    vec2f d = (gui_mouse_state.pos - active_item.last_drag_pos);
                    active_item.last_drag_pos = gui_mouse_state.pos;

                    panel->dock->width += d.x;

                    if(d.y != 0) {
                        i32 idx = find_panel_dock_index(panel);
                        check(idx != -1);
                        if(idx == (i32)panel->dock->panels.size()-1)  // last panel: cannot vertically resize
                            break;
                        f32 size_diff = d.y/WINDOW_HEIGHT;
                        if(        panel->dock->panels[idx].size_ratio   + size_diff > 0.05
                                && panel->dock->panels[idx+1].size_ratio - size_diff > 0.05) {
                            panel->dock->panels[idx].size_ratio   += size_diff;
                            panel->dock->panels[idx+1].size_ratio -= size_diff;
                        }
                    }

                    break;
                }

                vec2f new_dim = panel->rect.dim + (gui_mouse_state.pos - active_item.last_drag_pos);

                active_item.last_drag_pos = gui_mouse_state.pos;

                // if a min is defined the panel dim cant exceed it.
                // also sets the drag pos to the edge in this case

                vec2f min_dim = absolute_or_zero(resize_policy.min, panel->packed_dim);
                vec2f max_dim = absolute_or_zero(resize_policy.max, panel->packed_dim);

                if(new_dim.x < min_dim.x){
                    active_item.last_drag_pos.x -= new_dim.x - min_dim.x;
                    new_dim.x = min_dim.x;
                } else if(max_dim.x != 0 && new_dim.x > max_dim.x){
                    active_item.last_drag_pos.x -= new_dim.x - max_dim.x;
                    new_dim.x = max_dim.x;
                }

                if(new_dim.y < min_dim.y){
                    active_item.last_drag_pos.y -= new_dim.y - min_dim.y;
                    new_dim.y = min_dim.y;
                } else if(max_dim.y != 0 && new_dim.y > max_dim.y){
                    active_item.last_drag_pos.y -= new_dim.y - max_dim.y;
                    new_dim.y = max_dim.y;
                }

                if(resize_policy.ratio != 0){
                    if(new_dim.x/new_dim.y > resize_policy.ratio)
                        new_dim.y = new_dim.x/resize_policy.ratio;
                    else
                        new_dim.x = new_dim.y*resize_policy.ratio;
                }

                panel->rect.dim = new_dim;
                break;
            }

            default:
                break;
        }

    // hover, can either click to drag / resize, or scroll
    } else if(first_panel_at(gui_mouse_state.pos, true) == panel
              && panel->draw_border) {
        // if clicked on this panel, and either he had focus or the previous focused panel wasnt there
        if(gui_mouse_state.left().pressed) {
            eat_mouse_state();

            if(!panel->dock) {
                // focus
                max_panel_zindex++;
                panel->zindex = max_panel_zindex;
            }

            active_item_id_before_panel = active_item_id;
            active_item_id = id;

            active_item.last_drag_pos = gui_mouse_state.pos;

            if(resize_policy.can_move)
                active_item.mode = ACTIVE_MODE_MOVE_MAYBE;
            else
                active_item.mode = ACTIVE_MODE_NONE;

            if(resize_policy.enabled) {
                if(panel->resize_box().contains(gui_mouse_state.pos)) {
                    active_item.mode = ACTIVE_MODE_RESIZE;
                } else if(gui_mouse_state.right().down) {
                    active_item.mode = ACTIVE_MODE_RESIZE;
                    active_item.last_drag_pos = panel->resize_box().center();
                }
            }

        // hover - can scroll
        } else if(gui_mouse_state.scroll != vec2f::O) {
            eat_mouse_state();

            if(scroll_flags & HORIZONTAL_SCROLL)
                panel->scroll.x -= gui_mouse_state.scroll.x * PANEL_SCROLL_SPEED;

            if(scroll_flags & VERTICAL_SCROLL)
                panel->scroll.y -= gui_mouse_state.scroll.y * PANEL_SCROLL_SPEED;
        }
    }


    // round scroll stuff - in case we're resizing the window while scrolled for example
    // uses the unresized dim, as we want to be one frame late on the resize stuff since the components
    // dimension is currently to one from one frame late too
    // avoid case where a component takes the whole width
    // and you resize on the x axis
    // you dont want the scrollbar to appear for 1 frame - the time it takes for the component dim to be
    // up to date
    // TODO put that in the resize stuff?
    vec2f max_scroll = std::max(vec2f(), panel->component_dim - unresized_dim + panel->scrollbar_size);
    panel->scroll.x = clamp(panel->scroll.x, 0.f, max_scroll.x);
    panel->scroll.y = clamp(panel->scroll.y, 0.f, max_scroll.y);


    // scrollbar shapes
    if(max_scroll.y > 0
            && scroll_flags & VERTICAL_SCROLL) {

        aabb2f bb = panel->noscrollbar_box();
        vec2f zone_pos {
                bb.right() - PANEL_BORDER_RADIUS,
                bb.corner.y + PANEL_BORDER_RADIUS
        };

        vec2f zone_dim {
                SCROLLBAR_ZONE_WIDTH,
                bb.bottom()-PANEL_BORDER_RADIUS-zone_pos.y
        };

        ui_shape vertical_scrollbar_zone_shape = ui_shape(
                aabb2f(zone_pos, zone_dim),
                SCROLLBAR_ZONE_COLOR,
                4,  // zindex
                PANEL_BORDER_COLOR,
                0,  // border radius
                false // disable clipping
        );
        panel->shapes.push_back(vertical_scrollbar_zone_shape);


        // scrolling part

        vec2f pos = zone_pos;
        vec2f dim = zone_dim;

        pos.x += SCROLLBAR_ZONE_PADDING;
        dim.x -= 2*SCROLLBAR_ZONE_PADDING;

        dim.y *= (unresized_dim.y-panel->scrollbar_size.y) / panel->component_dim.y;
        pos.y += (panel->scroll.y/max_scroll.y)*(zone_dim.y-dim.y);

        ui_shape vertical_scrollbar_shape = ui_shape(
                aabb2f(pos, dim),
                SCROLLBAR_COLOR,
                5,  // zindex
                PANEL_BORDER_COLOR,
                0,  // border radius
                false // disable clipping
        );

        if(first_panel_at(gui_mouse_state.pos, true) == panel) {

            if(vertical_scrollbar_shape.rect.contains(gui_mouse_state.pos)) {

                // as its also a click on the panel, the active_item_id is already set
                // && no need to re-eat the mouse inputs
                if(gui_mouse_state.left().pressed){
                    active_item.mode = ACTIVE_MODE_SCROLL_Y;
                    active_item.last_drag_pos = gui_mouse_state.pos;  // for clarity (pretty sure its not needed as a side effect of the click-on-panel stuff)
                }

                // hover
                else if(!gui_mouse_state.left().down)
                    vertical_scrollbar_shape.color = SCROLLBAR_HOVER_COLOR;

            // clicks on the scrollbar
            } else if(gui_mouse_state.left().pressed
                        && vertical_scrollbar_zone_shape.rect.contains(gui_mouse_state.pos)) {

                active_item.mode = ACTIVE_MODE_NONE;

                // clicked before
                if(gui_mouse_state.pos.y < vertical_scrollbar_shape.rect.corner.y){
                    panel->scroll.y -= SCROLLBAR_CLICK_SCROLL_SIZE_RATIO * panel->rect.dim.y;
                    if(panel->scroll.y < 0)
                        panel->scroll.y = 0;

                // clicked after
                } else {
                    panel->scroll.y += SCROLLBAR_CLICK_SCROLL_SIZE_RATIO * panel->rect.dim.y;
                    if(panel->scroll.y > max_scroll.y)
                        panel->scroll.y = max_scroll.y;

                }
            }
        }

        if(active_item_id == id && active_item.mode == ACTIVE_MODE_SCROLL_Y) {
            eat_mouse_state();

            if(gui_mouse_state.left().released) {
                active_item_id = -1;

            } else {  // scroll
                vertical_scrollbar_shape.color = SCROLLBAR_CLICKED_COLOR;

                f32 drag = gui_mouse_state.pos.y - active_item.last_drag_pos.y;
                panel->scroll.y += (drag / (zone_dim.y-dim.y)) * max_scroll.y;

                active_item.last_drag_pos = gui_mouse_state.pos;

                if(panel->scroll.y < 0) {
                    active_item.last_drag_pos.y -= panel->scroll.y
                                                   * (zone_dim.y-dim.y) / max_scroll.y;
                    panel->scroll.y = 0;

                } else if(panel->scroll.y > max_scroll.y) {
                    active_item.last_drag_pos.y += (max_scroll.y - panel->scroll.y)
                                                   * (zone_dim.y-dim.y) / max_scroll.y;
                    panel->scroll.y = max_scroll.y;
                }

            }
        }

        panel->shapes.push_back(vertical_scrollbar_shape);

        panel->scrollbar_size.x = SCROLLBAR_ZONE_WIDTH;

    } else {
        panel->scrollbar_size.x = 0;
    }

    if(max_scroll.x > 0
            && scroll_flags & HORIZONTAL_SCROLL) {

        aabb2f bb = panel->noscrollbar_box();
        vec2f zone_pos {
                bb.corner.x + PANEL_BORDER_RADIUS,
                bb.bottom() - PANEL_BORDER_RADIUS
        };

        vec2f zone_dim {
                bb.right()-PANEL_BORDER_RADIUS-zone_pos.x,
                SCROLLBAR_ZONE_WIDTH
        };

        ui_shape horizontal_scrollbar_zone_shape = ui_shape(
                aabb2f(zone_pos, zone_dim),
                SCROLLBAR_ZONE_COLOR,
                4,  // zindex
                PANEL_BORDER_COLOR,
                0,  // border radius
                false // disable clipping
                );
        panel->shapes.push_back(horizontal_scrollbar_zone_shape);


        // scrolling part

        vec2f pos = zone_pos;
        vec2f dim = zone_dim;

        pos.y += SCROLLBAR_ZONE_PADDING;
        dim.y -= 2*SCROLLBAR_ZONE_PADDING;

        dim.x *= (unresized_dim.x-panel->scrollbar_size.x) / panel->component_dim.x;
        pos.x += (panel->scroll.x/max_scroll.x)*(zone_dim.x-dim.x);

        ui_shape horizontal_scrollbar_shape = ui_shape(
                aabb2f(pos, dim),
                SCROLLBAR_COLOR,
                5,  // zindex
                PANEL_BORDER_COLOR,
                0,  // border radius
                false // disable clipping
                );

        if(first_panel_at(gui_mouse_state.pos, true) == panel) {

            if(horizontal_scrollbar_shape.rect.contains(gui_mouse_state.pos)) {

                // as its also a click on the panel, the active_item_id is already set
                // && no need to re-eat the mouse inputs
                if(gui_mouse_state.left().pressed){
                    active_item.mode = ACTIVE_MODE_SCROLL_X;
                    active_item.last_drag_pos = gui_mouse_state.pos;  // for clarity (pretty sure its not needed as a side effect of the click-on-panel stuff)
                }

                // hover
                else if(!gui_mouse_state.left().down)
                    horizontal_scrollbar_shape.color = SCROLLBAR_HOVER_COLOR;

            // clicks on the scrollbar
            } else if(gui_mouse_state.left().pressed
                    && horizontal_scrollbar_zone_shape.rect.contains(gui_mouse_state.pos)) {

                active_item.mode = ACTIVE_MODE_NONE;

                // clicked before
                if(gui_mouse_state.pos.x < horizontal_scrollbar_shape.rect.corner.x){
                    panel->scroll.x -= SCROLLBAR_CLICK_SCROLL_SIZE_RATIO * panel->rect.dim.x;
                    if(panel->scroll.x < 0)
                        panel->scroll.x = 0;

                // clicked after
                } else {
                    panel->scroll.x += SCROLLBAR_CLICK_SCROLL_SIZE_RATIO * panel->rect.dim.x;
                    if(panel->scroll.x > max_scroll.x)
                        panel->scroll.x = max_scroll.x;

                }
            }
        }

        if(active_item_id == id && active_item.mode == ACTIVE_MODE_SCROLL_X) {
            eat_mouse_state();

            if(gui_mouse_state.left().released) {
                active_item_id = -1;

            } else {  // scroll
                horizontal_scrollbar_shape.color = SCROLLBAR_CLICKED_COLOR;

                f32 drag = gui_mouse_state.pos.x - active_item.last_drag_pos.x;
                panel->scroll.x += (drag / (zone_dim.x-dim.x)) * max_scroll.x;

                active_item.last_drag_pos = gui_mouse_state.pos;

                if(panel->scroll.x < 0) {
                    active_item.last_drag_pos.x -= panel->scroll.x
                                                   * (zone_dim.x-dim.x) / max_scroll.x;
                    panel->scroll.x = 0;

                } else if(panel->scroll.x > max_scroll.x) {
                    active_item.last_drag_pos.x += (max_scroll.x - panel->scroll.x)
                                                   * (zone_dim.x-dim.x) / max_scroll.x;
                    panel->scroll.x = max_scroll.x;
                }

            }
        }

        panel->shapes.push_back(horizontal_scrollbar_shape);

        panel->scrollbar_size.y = SCROLLBAR_ZONE_WIDTH;

    } else {
        panel->scrollbar_size.y = 0;
    }


    // reset the component bounding box for it to be updated on this next push_panel call
    panel->component_dim = vec2f();

    panel_stack.push(panel);
}

void pop_panel() {
    panel_stack.pop();
}


f32 ui_panel_width() {
    ui_panel* panel = panel_stack.top();
    return panel->rect.dim.x - panel->scrollbar_size.x;
}

bool force_new_row_once = false;
bool force_same_row_once = false;
bool force_new_row = false;
bool force_same_row = false;
bool new_row_start = false;
void ui_new_row() {
    force_new_row_once = true;
}
void ui_same_row() {
    force_same_row_once = true;
}
void begin_new_row() {
    force_new_row = true;
}
void begin_same_row() {
    force_same_row = true;
}
void end_new_row() {
    force_new_row = false;
}
void end_same_row() {
    force_same_row = false;
}
void begin_row() {
    force_same_row = true;
    new_row_start = true;  // special case for the first item, as it should still break line
}
void end_row() {
    force_same_row = false;
}

void get_real_new_row_value(bool* new_row, bool update_global_status=false) {
    if(force_same_row_once) {
        *new_row = false;
        if(update_global_status)
            force_same_row_once = false;
        return;
    }

    if(force_new_row_once) {
        *new_row = true;
        if(update_global_status)
            force_new_row_once = false;
        return;
    }

    if(force_same_row) {
        if(new_row_start) {
            if(update_global_status)
                new_row_start = false;
        } else {
            *new_row = false;
        }
    }
    if(force_new_row)
        *new_row = true;
}

bool fit_shape(ui_panel* panel, ui_shape& shape, bool new_row=true, f32 row_spacing=PANEL_ROW_MARGIN_Y) {
    get_real_new_row_value(&new_row, true);  // and update global 'new row'-status

    if(new_row) {
        panel->item_spawn.x = (f32)group_offset;
        panel->item_spawn.y = row_spacing + panel->current_row_bb.bottom();
    } else {
        panel->item_spawn.x += PANEL_ROW_MARGIN_X;
    }

    shape.rect.corner += panel->item_spawn;

    if(shape.rect.dim.x == 0) {  // take the whole row
        shape.rect.dim.x =   panel->rect.dim.x
                           - panel->scrollbar_size.x
                           - panel->current_row_bb.right()
                           - PANEL_ROW_MARGIN_X;
    }

    if(new_row)
        panel->current_row_bb = shape.rect;
    else
        panel->current_row_bb |= shape.rect;

    panel->item_spawn.x = panel->current_row_bb.right();

    panel->component_dim = std::max(panel->component_dim, shape.rect.opposite_corner());

    shape.rect.corner += panel->padding + panel->rect.corner - panel->scroll;

    if(!panel->hidden && (
                   shape.rect.bottom() < panel->rect.corner.y
                || shape.rect.corner.y > panel->bounding_box().bottom()))
        return false;

    return true;
}


void ui_label(std::string text, vec2f size, vec2f offset, f32 font_size, vec4f color) {
    ui_panel* panel = panel_stack.top();

    // default size
    vec2f text_dim = text_size(text, font_size);
    if(size.x == 0) size.x = text_dim.x;
    if(size.y == 0) size.y = text_dim.y;

    if(color.w < 0) color = BUTTON_TEXT_COLOR;
    ui_shape text_shape(text, offset, font_size, color, 0);

    if(!fit_shape(panel, text_shape))
        return;

    panel->shapes.push_back(text_shape);
}

void ui_labelf(ccstring format, ...) {
    va_list va;
    va_start(va, format);
    u32 n = vsnprintf(NULL, 0, format, va);
    va_end(va);
    std::string b(n+1, '\0');
    va_start(va, format);
    vsnprintf(&b[0], n+1, format, va);
    b.resize(n);
    va_end(va);
    ui_label(b);
}

bool _ui_button(GUI_ID_TYPE id, std::string text, vec2f size, f32 font_size) {
    ui_panel* panel = panel_stack.top();

    // default size
    vec2f text_dim = text_size(text, font_size);
    if(size.x == 0)
        size.x = text_dim.x + 2*BUTTON_TEXT_MARGIN.x;
    if(size.y == 0)
        size.y = text_dim.y + 2*BUTTON_TEXT_MARGIN.y;

    ui_shape rect_shape = ui_shape(
            aabb2f(
                vec2f(),
                size
            ),
            BUTTON_COLOR,
            0,  // zindex
            BUTTON_BORDER_COLOR,
            BUTTON_BORDER_RADIUS  // border radius
    );

    if(!fit_shape(panel, rect_shape))
        return false;

    bool clicked = false;
    if(!panel->hidden){
        bool is_active = active_item_id == id;

        if(active_item_id == id && gui_mouse_state.left().released){
            eat_mouse_state();
            active_item_id = 0;
        }

        if(mouse_inside(rect_shape)) {

            if(gui_mouse_state.left().pressed) {
                eat_mouse_state();
                active_item_id = id;

            } else if(is_active && gui_mouse_state.left().released) {
                eat_mouse_state();
                clicked = true;

            } else if(!gui_mouse_state.left().down) {
                rect_shape.color = BUTTON_HOVER_COLOR;
            }

            if(is_active){
                eat_mouse_state();
                rect_shape.color = BUTTON_CLICKED_COLOR;
                rect_shape.border_color = BUTTON_CLICKED_BORDER_COLOR;
            }

        } else if(is_active) {  // active but curosr not inside
            eat_mouse_state();
            rect_shape.color = BUTTON_HOVER_COLOR;
        }
    }


    panel->shapes.push_back(rect_shape);

    ui_shape text_shape(
            text, rect_shape.rect.corner + vec2f {
                BUTTON_TEXT_MARGIN.x,
                BUTTON_TEXT_MARGIN.y },
            font_size, BUTTON_TEXT_COLOR, 0
    );
    panel->shapes.push_back(text_shape);

    return clicked;
}

void _ui_toggle(GUI_ID_TYPE id, bool* value, std::string text, std::string text_pushed, vec2f size, vec2f offset, f32 font_size) {
    ui_panel* panel = panel_stack.top();

    if(!text_pushed.empty() && (*value))
        text = text_pushed;

    // default size
    vec2f text_dim = text_size(text, font_size);
    if(size.x == 0)
        size.x = text_dim.x + 2*BUTTON_TEXT_MARGIN.x;
    if(size.y == 0)
        size.y = text_dim.y + 2*BUTTON_TEXT_MARGIN.y;

    ui_shape rect_shape = ui_shape(
            aabb2f(
                offset,
                size
                ),
            BUTTON_COLOR,
            0,  // zindex
            BUTTON_BORDER_COLOR,
            BUTTON_BORDER_RADIUS  // border radius
            );

    if(!fit_shape(panel, rect_shape))
        return;

    bool clicked = false;
    if(!panel->hidden){
        bool is_active = active_item_id == id;

        if(active_item_id == id && gui_mouse_state.left().released){
            eat_mouse_state();
            active_item_id = 0;
        }

        if(mouse_inside(rect_shape)) {

            if(gui_mouse_state.left().pressed) {
                eat_mouse_state();
                active_item_id = id;

            } else if(is_active && gui_mouse_state.left().released) {
                eat_mouse_state();
                clicked = true;

            } else if(!gui_mouse_state.left().down) {
                rect_shape.color = BUTTON_HOVER_COLOR;
            }

            if(is_active){
                eat_mouse_state();
                rect_shape.color = BUTTON_CLICKED_COLOR;
                rect_shape.border_color = BUTTON_CLICKED_BORDER_COLOR;
            }

        } else if(is_active) {  // active but curosr not inside
            eat_mouse_state();
            rect_shape.color = BUTTON_HOVER_COLOR;
        }
    }

    if(clicked)
        *value = !(*value);

    // only change color on push if no pushed_text was defined
    vec4f text_color = BUTTON_TEXT_COLOR;
    if((*value) && text_pushed.empty())
        text_color = TOGGLE_TEXT_COLOR;

    if((*value))
        rect_shape.color = TOGGLE_FILL_COLOR;

    panel->shapes.push_back(rect_shape);

    ui_shape text_shape(
            text, rect_shape.rect.corner + vec2f {
                BUTTON_TEXT_MARGIN.x,
                BUTTON_TEXT_MARGIN.y },
            font_size, text_color, 0
            );
    panel->shapes.push_back(text_shape);
}

void hide_current_panel() {
    ui_panel* panel = panel_stack.top();
    if(panel)
        panel->hidden = true;
}

void unhide_current_panel() {
    ui_panel* panel = panel_stack.top();
    if(panel)
        panel->hidden = false;
}

// takes a value instead of a pointer for convenience
// the caller should take care of updating his state when the button is pushed
bool _ui_toggle(GUI_ID_TYPE id, bool value, std::string text, std::string text_pushed, vec2f size, vec2f offset, f32 font_size) {
    _ui_toggle(id, &value, text, text_pushed, size, offset, font_size);
    return value;
}

bool _static_toggle(GUI_ID_TYPE id, std::string text, std::string text_pushed, bool init, vec2f size, vec2f offset, f32 font_size) {
    bool b;
    if(!load_static_data(id, &b))
        b = init;
    _ui_toggle(id, &b, text, text_pushed, size, offset, font_size);
    store_static_data(id, b);
    return b;
}

// TODO make better ui groups

GUI_ID_TYPE last_opened_group_id = 0;
bool _begin_group(GUI_ID_TYPE id, std::string text, bool init, vec2f size, vec2f offset, f32 font_size) {
    bool b;
    if(!load_static_data(id, &b))
        b = false;
    _ui_toggle(id, &b, "> " + text, "< " + text, size, offset, font_size);
    if(b) {
        group_offset += UI_GROUP_OFFSET_X;
        last_opened_group_id = id;
    }
    store_static_data(id, b);
    return b;
}

void close_group() {
    // TODO only works if no close_group was called since the last begin_group
    // TODO doesnt 'exit' the client if clause
    check(last_opened_group_id != 0);
    store_static_data(last_opened_group_id, false);
    last_opened_group_id = 0;
}

void end_group() {
    assert(group_offset >= UI_GROUP_OFFSET_X);
    last_opened_group_id = 0;
    group_offset -= UI_GROUP_OFFSET_X;

    ui_panel* panel = panel_stack.top();
    panel->current_row_bb.dim.y += UI_GROUP_OFFSET_BELOW;  // TODO a bit hacky
}

template<typename T>
T ratio_to_value (slider_mode mode, f32 ratio, T min, T max) {
    switch(mode) {
        case slider_mode::linear:
            return min+(max-min)*ratio;
        case slider_mode::log:
            // asserts max > min
            if(ratio == 0)
                return min;
            if(min == 0)
                min = (T)SLIDER_MODE_LOG_EPSILON;  // TODO wont work on integers
            check(min > 0);
            return min*exp(ratio*log(max/min));
        default:
            unreachable();
    }
}

template<typename T>
f32 value_to_ratio (slider_mode mode, T value, T min, T max) {
    switch(mode) {
        case slider_mode::linear:
            return (value-min)/(f32)(max-min);
        case slider_mode::log:
            // asserts max > min
            if(min == 0)
                min = (T)SLIDER_MODE_LOG_EPSILON;  // TODO wont work on integers
            check(min > 0);
            return log(value/(f32)min)/log(max/(f32)min);
        default:
            unreachable();
    }
}



template<typename T, typename T2, typename T3>
bool _ui_slider(GUI_ID_TYPE id, T* f, T2 _min, T3 _max, std::string name, slider_mode mode, vec2f size) {
    ui_panel* panel = panel_stack.top();

    if(name.size() > 0)
        ui_label(name + "=", vec2f());

    T min = T(_min);
    T max = T(_max);

    // default size
    // if size.x == 0, it will take the whole row (handled by fit_shape
    if(size.y == 0)
        size.y = 20;
    bool validated = false;
    bool is_textfield;
    if(focus_next_widget || widget_to_focus == id) {

        is_textfield = true;
        store_static_data<bool>(id, true);

        last_focussed_widget_id = id;
        focus_next_widget = false;

        active_item_id = id;
        active_item.reset_textfield_stuff();

    } else if(!load_static_data<bool>(id, &is_textfield)) {
        is_textfield = false;
    }

    if(is_textfield) {
        std::string str;
        if(!load_static_data<std::string>(id, &str))
            str = std::to_string(*f);

        // try to format the string - display in red if invalid
        vec4f color = SLIDER_TEXTFIELD_TEXT_COLOR;
        T val;
        if(!from_string(str, &val)) {
            color = SLIDER_TEXTFIELD_INVALID_COLOR;
        }

        if(_ui_textfield(
                id,
                &str,
                vec2f(),  // size
                vec2f(),  // offset
                UI_FONT_SIZE,  // font size
                false,    // new row
                color     // font color
                )) {

            if(from_string(str, &val)) {
                *f = val;
                validated = true;
            } else {
                printf("format error %s\n", str.c_str());
            }
        }

        store_static_data<std::string>(id, str);

        // unselected textfield => is not a textfield anymore
        if(active_item_id != id) {
            /* printf("unselection handled\n"); */
            store_static_data<bool>(id, false);
            remove_static_data<std::string>(id);  // also cleanup old textfield value
        }
    }

    previous_drawn_textfield_id = id;

    if(!is_textfield) {

        ui_shape container = ui_shape(
                aabb2f(
                    vec2f{ 0.f, (panel->current_row_bb.dim.y-size.y)/2.f },
                    size
                    ),
                SLIDER_FILL_COLOR,
                0,  // zindex
                SLIDER_BORDER_COLOR,
                SLIDER_BORDER_SIZE  // border radius
        );

        if(!fit_shape(panel, container, name.size() == 0))  // only as a new row if no label was drawn
            return false;

        f32 slider_size = container.rect.dim.y*.6;
        f32 slider_padding = container.rect.dim.y*.2;
        f32 sliding_range = container.rect.dim.x - 2*slider_padding - slider_size;
        vec4f slider_color = SLIDER_HANDLE_COLOR;
        if(*f < min || *f > max)  // invalid value
            slider_color = SLIDER_HANDLE_INVALID_COLOR;

        if(!panel->hidden){

            if(mouse_inside(container)) {

                if(gui_mouse_state.left().pressed) {
                    eat_mouse_state();
                    active_item_id = id;
                } else if(gui_mouse_state.right().pressed) {
                    eat_mouse_state();
                    active_item_id = id;
                    active_item.reset_textfield_stuff();
                    store_static_data<bool>(id, true); // active textfield mode
                    return false;

                } else if(!gui_mouse_state.left().down) {
                    /* eat_mouse_state();  // avoid eating right+drag */
                    slider_color = SLIDER_HANDLE_HOVER_COLOR;  // hover
                }
            }

            if(active_item_id == id) {
                eat_mouse_state();
                slider_color = SLIDER_HANDLE_ACTIVE_COLOR;
                f32 value_ratio = (gui_mouse_state.pos.x-container.rect.corner.x-slider_padding)/sliding_range;
                if(value_ratio <= 0) {
                    *f = min;
                } else if(value_ratio >= 1 ) {
                    *f = max;
                } else {
                    T new_value = ratio_to_value(mode, value_ratio, min, max);
                    *f = new_value;
                }
            }
        }

        if(active_item_id == id && gui_mouse_state.left().released){
            active_item_id = 0;
            eat_mouse_state();
            validated = true;
        }

        panel->shapes.push_back(container);

        vec2f slider_pos = container.rect.corner
                           + vec2f{ slider_padding, slider_padding };

        slider_pos.x += sliding_range*clamp(value_to_ratio(mode, *f, min, max), 0.f, 1.f);

        ui_shape slider = ui_shape(
                aabb2f(
                    slider_pos,
                    vec2f{ slider_size, slider_size }
                ),
                slider_color,
                0,  // zindex
                SLIDER_HANDLE_BORDER_COLOR,
                SLIDER_HANDLE_BORDER_SIZE  // border radius
        );
        panel->shapes.push_back(slider);

        std::string value_as_str = std::to_string(*f);
        vec2f inside_label_size = text_size(value_as_str, SLIDER_INSIDE_TEXT_SIZE);
        ui_shape text_shape(
                value_as_str,
                container.rect.center() - inside_label_size/2.f,
                SLIDER_INSIDE_TEXT_SIZE,
                SLIDER_INSIDE_TEXT_COLOR,
                0  // zindex
        );
        panel->shapes.push_back(text_shape);
    }
    return validated;
}

template<typename T, typename T2, typename T3>
T _static_slider(GUI_ID_TYPE id, T init, T2 _min, T3 _max, std::string name, slider_mode mode, vec2f size) {
    T min = T(_min);
    T max = T(_max);

    T f;
    if(!load_static_data(id, &f))
        f = init;
    _ui_slider(id, &f, min, max, name, mode, size);
    store_static_data(id, f);
    return f;
}


template i32 _static_slider<i32,i32,i32>(GUI_ID_TYPE,
                                         i32,i32,i32,
                                         std::string, slider_mode, vec2f);

template u32 _static_slider<u32,u32,u32>(GUI_ID_TYPE,
                                         u32,u32,u32,
                                         std::string, slider_mode, vec2f);


i32 _textfield_get_string_index_at_pos(f32 pos, f32 view_offset, const ui_shape& container, const std::string* str, f32 font_size) {

    // get the mouse position relative to the text
    f32 mouse_x = pos - container.rect.corner.x - TEXTFIELD_TEXT_OFFSET_X;  // in textfield's referential
    i32 cursor_pos = 0;
    /* f32 offset_to_cursor = text_size(str->substr(0, cursor_pos), font_size).x; */

    // we keep track of the last offset
    // if we go past a character, we 'round' to the closest char position
    // by checking for the closest offset
    f32 previous_offset = -1000;
    while(cursor_pos < (i32)str->size()){
        f32 offset = text_size(str->substr(0, cursor_pos), font_size).x + view_offset - mouse_x;
        if(offset > 0) {
            if(-previous_offset < offset)
                cursor_pos--;
            break;
        }
        previous_offset = offset;
        cursor_pos++;
    }

    return cursor_pos;
}

vec2i _textfield_get_selection() {
    i32 start = active_item.selection_start;
    i32 end = active_item.cursor_pos;
    if(end < start)
        std::swap(start, end);
    return vec2i{ start, end };
}

bool _textfield_erase_selection(std::string* str) {
    if(active_item.selection_start == -1)
        return false;

    vec2i sel = _textfield_get_selection();

    str->erase(sel.x, sel.y-sel.x);

    active_item.cursor_pos = sel.x;
    active_item.selection_start = -1;

    return true;
}

bool is_current_panel_hidden() {
    ui_panel* panel = panel_stack.top();
    return panel->hidden;
}

bool _ui_textfield(GUI_ID_TYPE id, std::string* str, vec2f size, vec2f offset, f32 font_size, bool new_row, vec4f color) {
    ui_panel* panel = panel_stack.top();

    get_real_new_row_value(&new_row);

    // default size
    if(size.x == 0) {
            size.x = panel->rect.dim.x - panel->scrollbar_size.x - offset.x;
        if(!new_row)
            size.x -= panel->item_spawn.x + PANEL_ROW_MARGIN_X;
    }
    if(size.y == 0)
        size.y = TEXTFIELD_HEIGHT;

    ui_shape container = ui_shape(
            aabb2f(
                offset,
                size
                ),
            TEXTFIELD_FILL_COLOR,
            0,  // zindex
            TEXTFIELD_BORDER_COLOR,
            TEXTFIELD_BORDER_SIZE  // border radius
    );

    if(!fit_shape(panel, container, new_row))
        return false;

    f32 view_offset = 0.f;
    load_static_data(id, &view_offset);
    bool force_enter = false;

    if(!panel->hidden){

        if(focus_next_widget || widget_to_focus == id) {
            focus_next_widget = false;
            last_focussed_widget_id = id;
            active_item_id = id;
            active_item.reset_textfield_stuff();

            load_static_data(id, &view_offset);
        }

        bool force_show_cursor = false;
        bool selecting = gui_kb_state.shift_down() && gui_kb_state.chars.empty();
        bool just_clicked = false;

        if(mouse_inside(container)) {
            if(gui_mouse_state.left().pressed) {
                eat_mouse_state();

                if(id != active_item_id && id != active_item_id_before_panel)
                    just_clicked = true;

                active_item_id = id;

                if(!selecting)
                    active_item.selection_start = _textfield_get_string_index_at_pos(
                            gui_mouse_state.pos.x, view_offset, container, str, font_size );

            } else if(!gui_mouse_state.left().down) {
                eat_mouse_state();
                container.border_color = TEXTFIELD_HOVER_BORDER_COLOR;
            }

        } else if(active_item_id == id && gui_mouse_state.left().pressed) {
            active_item_id = 0;
        }

        if(active_item_id == id) {

            if(just_clicked || active_item.wait_for_released) {
                just_clicked = true;
                active_item.wait_for_released = true;
            }

            // select all text if it was activated
            // by something else than clicking on it (ex: slider textfield mode)
            if(just_clicked || active_item.cursor_pos == -1) {
                active_item.selection_start = 0;
                active_item.cursor_pos = str->size();
            }

            container.border_color = TEXTFIELD_ACTIVE_BORDER_COLOR;
            eat_kb_state();

            if(selecting && active_item.selection_start == -1)
                active_item.selection_start = active_item.cursor_pos;

            // mouse selection
            if(!just_clicked && gui_mouse_state.left().down) {
                eat_mouse_state();

                active_item.cursor_pos = _textfield_get_string_index_at_pos(
                        gui_mouse_state.pos.x, view_offset, container, str, font_size );

            // on release, check if an actual selection was done
            } else if(gui_mouse_state.left().released) {
                eat_mouse_state();

                if(just_clicked) {
                    just_clicked = false;
                    active_item.wait_for_released = false;

                } else if(active_item.cursor_pos == active_item.selection_start){
                    active_item.selection_start = -1;
                    force_show_cursor = true;
                }
            }

            // movements
            if(    gui_kb_state.keys[GLFW_KEY_LEFT].pressed
                || gui_kb_state.keys[GLFW_KEY_LEFT].repeat) {
                if(!selecting) active_item.selection_start = -1;

                force_show_cursor = true;
                if(active_item.cursor_pos > 0) {
                    // control not pressed => go to previous char
                    if(!(gui_kb_state.keys[GLFW_KEY_LEFT].mods & GLFW_MOD_CONTROL)) {
                        active_item.cursor_pos--;

                    // control is pressed => go to the start of the previous word
                    } else {
                        u64 previous_char = str->find_last_not_of(" ", active_item.cursor_pos-1);
                        if(previous_char == std::string::npos) {
                            active_item.cursor_pos = 0;
                        } else {
                            u64 space_pos = str->rfind(" ", previous_char);
                            if(space_pos == std::string::npos)
                                active_item.cursor_pos = 0;
                            else
                                active_item.cursor_pos = space_pos + 1;  // put the cursor right after the space
                        }
                    }
                }
            }

            if(    gui_kb_state.keys[GLFW_KEY_RIGHT].pressed
                || gui_kb_state.keys[GLFW_KEY_RIGHT].repeat) {

                if(!selecting) active_item.selection_start = -1;

                force_show_cursor = true;
                if(active_item.cursor_pos < (i32)str->size()) {
                    // control not pressed => go to next char
                    if(!(gui_kb_state.keys[GLFW_KEY_RIGHT].mods & GLFW_MOD_CONTROL)) {
                        active_item.cursor_pos++;

                    // control is pressed => go to the end of the next word
                    } else {
                        u64 next_char = str->find_first_not_of(" ", active_item.cursor_pos);
                        if(next_char == std::string::npos) {
                            active_item.cursor_pos = str->size();
                        } else {
                            u64 space_pos = str->find(" ", next_char);
                            if(space_pos == std::string::npos)
                                active_item.cursor_pos = str->size();
                            else
                                active_item.cursor_pos = space_pos;  // put the cursor right after the space
                        }
                    }
                }
            }

            if(gui_kb_state.keys[GLFW_KEY_HOME].pressed) {
                if(!selecting) active_item.selection_start = -1;
                active_item.cursor_pos = 0;
            }
            if(gui_kb_state.keys[GLFW_KEY_END].pressed) {
                if(!selecting) active_item.selection_start = -1;
                active_item.cursor_pos = str->size();
            }

            // Ctrl-A to select all
            if(gui_kb_state.pressed_with_mod(GLFW_KEY_Q, GLFW_MOD_CONTROL)) {
                active_item.selection_start = 0;
                active_item.cursor_pos = str->size();
            }

            // backspace
            if(    gui_kb_state.keys[GLFW_KEY_BACKSPACE].pressed
                || gui_kb_state.keys[GLFW_KEY_BACKSPACE].repeat) {
                eat_kb_state();
                if(!_textfield_erase_selection(str) && active_item.cursor_pos > 0) {
                    str->erase(active_item.cursor_pos-1, 1);
                    active_item.cursor_pos--;
                }
                force_show_cursor = true;
            }

            // regular chars or paste
            std::string newchars;
            while(!mutable_gui_kb_state.chars.empty()) {
                u32 k = mutable_gui_kb_state.chars.front();
                mutable_gui_kb_state.chars.pop();
                newchars += (char)k;
            }
            if(gui_kb_state.pressed_with_mod(GLFW_KEY_V, GLFW_MOD_CONTROL)){
                std::string clipboard = get_clipboard();
                /* if(clipboard != NULL) */
                    newchars += clipboard;
            }
            if(!newchars.empty()) {
                _textfield_erase_selection(str);
                str->insert(active_item.cursor_pos, newchars);
                active_item.cursor_pos += newchars.size();
                force_show_cursor = true;
            }


            if(selecting && active_item.selection_start == active_item.cursor_pos)
                active_item.selection_start = -1;


            // copy / cut
            if(active_item.selection_start != -1){
                bool copy = gui_kb_state.pressed_with_mod(GLFW_KEY_C, GLFW_MOD_CONTROL);
                bool cut = gui_kb_state.pressed_with_mod(GLFW_KEY_X, GLFW_MOD_CONTROL);
                if(copy || cut) {
                    vec2i sel = _textfield_get_selection();
                    set_clipboard(str->substr(sel.x, sel.y-sel.x));
                }
                if(cut)
                    _textfield_erase_selection(str);
            }

            // minimum move of the view for the cursor to be displayed
            // TODO make the view jump for the cursor to be in the middle if we overflow (cf view)
            f32 offset_to_cursor = text_size(str->substr(0, active_item.cursor_pos), font_size).x;
            f32 text_max_width = container.rect.dim.x - 2.f*TEXTFIELD_TEXT_OFFSET_X;
            if(view_offset + offset_to_cursor > text_max_width)
                view_offset = text_max_width - offset_to_cursor;
            else if(view_offset + offset_to_cursor < 0)
                view_offset = -offset_to_cursor;


            if(active_item.selection_start == -1){

                // cursor blink status
                if(force_show_cursor) {
                    active_item.show_cursor = true;
                    active_item.cursor_time_before_blink = TEXTFIELD_CURSOR_TIMER;
                } else {
                    active_item.cursor_time_before_blink -= ui_dt;
                    if(active_item.cursor_time_before_blink < 0){
                        active_item.show_cursor ^= 1;
                        active_item.cursor_time_before_blink = TEXTFIELD_CURSOR_TIMER;
                    }
                }

                // cursor draw
                if(active_item.show_cursor) {
                    ui_shape cursor = ui_shape(
                            aabb2f(
                                container.rect.corner + vec2f{ TEXTFIELD_TEXT_OFFSET_X + offset_to_cursor + view_offset, 0 },
                                vec2f{ TEXTFIELD_CURSOR_WIDTH, container.rect.dim.y }
                            ).with_margin(vec2f{ 0, TEXTFIELD_CURSOR_MARGIN_Y }),
                            TEXTFIELD_CURSOR_COLOR,
                            2,  // zindex
                            TEXTFIELD_CURSOR_BORDER_COLOR,
                            TEXTFIELD_CURSOR_BORDER_SIZE  // border radius
                    );
                    panel->shapes.push_back(cursor);
                }

            } else {  // selection active
                f32 offset_to_selection_start = text_size(str->substr(0, active_item.selection_start), font_size).x;
                if(offset_to_selection_start > offset_to_cursor)
                    std::swap(offset_to_selection_start, offset_to_cursor);

                // clip
                if(offset_to_cursor + view_offset > text_max_width)
                    offset_to_cursor = text_max_width - view_offset;
                if(offset_to_selection_start + view_offset < 0)
                    offset_to_selection_start = -view_offset;
                vec2f selection_dim = vec2f{ offset_to_cursor-offset_to_selection_start, container.rect.dim.y };

                vec2f selection_pos = container.rect.corner;
                selection_pos.x += TEXTFIELD_TEXT_OFFSET_X + offset_to_selection_start + view_offset;

                ui_shape selection = ui_shape(
                        aabb2f(selection_pos, selection_dim)
                            .with_margin(vec2f{ 0, TEXTFIELD_CURSOR_MARGIN_Y }),
                        TEXTFIELD_SELECTION_COLOR,
                        1,  // zindex
                        TEXTFIELD_SELECTION_BORDER_COLOR,
                        TEXTFIELD_SELECTION_BORDER_SIZE  // border radius
                );
                panel->shapes.push_back(selection);
            }

            // escape to clear the textfield state
            if(gui_kb_state.keys[GLFW_KEY_ESCAPE].down) {
                active_item_id = 0;
            }

            // tab to go to the next widget
            // TODO more generic way to handle focus change
            if(gui_kb_state.keys[GLFW_KEY_TAB].pressed && last_focussed_widget_id != id) {
                /* printf("tab pressed - handled %d != %d\n", id, last_focussed_widget_id); */
                force_enter = true;
                active_item_id = 0;
                last_focussed_widget_id = -1;

                if(!gui_kb_state.shift_down()) {
                    focus_next_widget = true;
                } else {
                    widget_to_focus = previous_drawn_textfield_id;
                    focus_given_widget = true;
                }
            }
        }

        /* println(*str); */
    }

    previous_drawn_textfield_id = id;

    if(all(container.rect.dim > vec2f::O))
        panel->shapes.push_back(container);

    store_static_data(id, view_offset);

    vec2f text_pos = container.rect.corner;
    text_pos.x += TEXTFIELD_TEXT_OFFSET_X + view_offset;
    aabb2i text_scissor = aabb2i(container.rect.with_margin(vec2f{ TEXTFIELD_TEXT_OFFSET_X, 0 }));
    if(all(text_scissor.dim > vec2i::O)) {
        ui_shape text_shape(
                *str, text_pos,
                font_size, color, 1,  // zindex
                true,  // clipping
                text_scissor  // scissor
        );
        panel->shapes.push_back(text_shape);
    }

    return force_enter || (active_item_id == id && gui_kb_state.keys[GLFW_KEY_ENTER].pressed);
}

std::string _static_textfield(GUI_ID_TYPE id, std::string init, vec2f size, vec2f offset, f32 font_size) {
    std::string str;
    if(!load_static_data(id, &str))
        str = init;
    _ui_textfield(id, &str, size, offset, font_size);
    store_static_data(id, str);
    return str;
}


static GUI_ID_TYPE current_ui_list = 0;
static f32 current_ui_list_font_size;
static f32 current_ui_list_top;
static bool current_ui_list_item_is_first;
static f32 current_ui_list_max_x;
static f32 current_ui_list_last_item_bottom;

void _begin_list(GUI_ID_TYPE id) {
    assert(current_ui_list == 0);
    current_ui_list = id;

    current_ui_list_font_size = UI_FONT_SIZE;  // TODO as arg?
    current_ui_list_item_is_first = true;
    current_ui_list_max_x = 0;

    if(active_item_id == id && gui_mouse_state.left().released) {
        active_item_id = 0;
        eat_mouse_state();
    }
}

bool ui_list_item(std::string label, bool selected, u8 *pmods) {
    ui_panel* panel = panel_stack.top();
    u8 mods = 0;
    ui_shape text_shape;

    text_shape = ui_shape(label, vec2f{ LIST_PADDING, 0.f }, current_ui_list_font_size, LIST_ITEM_TEXT_COLOR, 2);
    vec2f text_dim = text_size(label, current_ui_list_font_size);
    if(text_dim.x > current_ui_list_max_x)
        current_ui_list_max_x = text_dim.x;

    if(current_ui_list_item_is_first) {
        current_ui_list_item_is_first = false;
        fit_shape(panel, text_shape, true, LIST_PADDING + PANEL_ROW_MARGIN_Y);
        current_ui_list_top = text_shape.rect.corner.y;
    } else {
        fit_shape(panel, text_shape, true, LIST_ITEM_SPACING);
    }

    current_ui_list_last_item_bottom = text_shape.rect.bottom();

    panel->shapes.push_back(text_shape);

    aabb2f selection_rect = text_shape.rect;

    f32 current_ui_previous_max_x;
    if(load_static_data<f32>(current_ui_list, &current_ui_previous_max_x))
        selection_rect.dim.x = current_ui_previous_max_x;

    selection_rect = selection_rect.with_margin({-LIST_PADDING-LIST_ITEM_SPACING, -LIST_ITEM_SPACING-2.f});

    bool draw_hover = false;
    bool was_clicked = false;
    if(mouse_inside(selection_rect)) {
        if(!gui_mouse_state.left().pressed) {
            draw_hover = true;
        } else {
            active_item_id = current_ui_list;
            was_clicked = true;
            mods = gui_mouse_state.left().mods;
            eat_mouse_state();
        }
    }

    if(selected || draw_hover) {
        ui_shape hover_shape = ui_shape(
                selection_rect,
                selected ? LIST_ITEM_FILL_COLOR_SELECTED : LIST_ITEM_FILL_COLOR,
                1,  // zindex
                LIST_ITEM_BORDER_COLOR,
                LIST_ITEM_BORDER_SIZE  // border radius
        );

        panel->shapes.push_back(hover_shape);
    }
    if (was_clicked && pmods) {
        *pmods = mods;
    }
    return was_clicked;
}

bool ui_list_item(std::string label, bool selected) {
    return ui_list_item(label, selected, NULL);
}

void end_list() {
    assert(current_ui_list != 0);
    store_static_data<f32>(current_ui_list, current_ui_list_max_x);

    current_ui_list = 0;

    ui_panel* panel = panel_stack.top();

    f32 list_height = current_ui_list_last_item_bottom - current_ui_list_top;

    ui_shape rect_shape = ui_shape(
            aabb2f(
                vec2f{ 0.f, -LIST_PADDING-list_height },
                vec2f{ current_ui_list_max_x+2*LIST_PADDING, list_height+2*LIST_PADDING }
            ),
            LIST_FILL_COLOR,
            0,  // zindex
            LIST_BORDER_COLOR,
            LIST_BORDER_SIZE  // border radius
    );
    fit_shape(panel, rect_shape);
    panel->shapes.push_back(rect_shape);
}


template<typename T>
void _ui_list(GUI_ID_TYPE id, T* value, std::vector<T> values, vec2f size=vec2f(), f32 font_size=UI_FONT_SIZE) {
    // TODO use font_size & size once they're implemented in begin_list
    begin_list();
    for(const T& val: values) {
        std::ostringstream stream;
        stream << val;
        if(ui_list_item(stream.str(), *value == val))  // TODO replace by custom to_string
            *value = val;
    }
    end_list();
}

template<typename T>
T _static_list(GUI_ID_TYPE id, T init, std::vector<T> values, vec2f size=vec2f(), f32 font_size=UI_FONT_SIZE) {
    T str;
    if(!load_static_data(id, &str))
        str = init;
    _ui_list(id, &str, values, size, font_size);
    store_static_data(id, str);
    return str;
}

void fps_widget() {
    ui_panel* panel = panel_stack.top();

    vec2f size = { 5.f * FPS_WIDGET_FRAME_COUNT + 4.f, 50.f };

    ui_shape rect_shape = ui_shape(
            aabb2f(
                vec2f::O,
                size
            ),
            FPS_WIDGET_FILL_COLOR,
            0,  // zindex
            FPS_WIDGET_BORDER_COLOR,
            FPS_WIDGET_BORDER_SIZE  // border radius
    );
    fit_shape(panel, rect_shape);
    panel->shapes.push_back(rect_shape);

    f64 sum = 0;
    f64 max_cost = 0;
    for(u8 i=0 ; i<FPS_WIDGET_FRAME_COUNT ; i++) {
        f64 d = last_frame_duration[(i + ui_current_frame_idx+1)%FPS_WIDGET_FRAME_COUNT];
        sum += d;
        max_cost = std::max(max_cost, d);
        f32 height = 40.f*((f32)d)/0.04f;

        ui_shape hist_bar_shape = ui_shape(
            aabb2f(
                rect_shape.rect.corner + vec2f{ 2.f + 5.f*i, 5.f+40.f-height },
                vec2f{ 4.f, height }
            ),
            /* vec(vec3f{ 28.f, 96.f, 4.f }/255.f, 1.f),  // color */
            FPS_WIDGET_BAR_COLOR,
            0,  // zindex
            FPS_WIDGET_BAR_BORDER_COLOR,
            FPS_WIDGET_BAR_BORDER_RADIUS  // border radius
        );

        panel->shapes.push_back(hist_bar_shape);
    }

    f64 cost = 1000.f*sum/FPS_WIDGET_FRAME_COUNT;

    std::ostringstream text_stream;
    text_stream << std::setprecision(3) << cost << " ms";
    std::string text = text_stream.str();

    vec2f text_dim = text_size(text, FPS_WIDGET_FONT_SIZE);

    ui_shape text_shape(
            text,
            vec2f { rect_shape.rect.right()-text_dim.x-5.f, rect_shape.rect.corner.y },
            FPS_WIDGET_FONT_SIZE,
            FPS_WIDGET_TEXT_COLOR,
            0);
    panel->shapes.push_back(text_shape);

    std::ostringstream info_stream;
    info_stream << "last: " << std::setprecision(3) << (1000.f*last_frame_duration[ui_current_frame_idx]);
    std::string info = info_stream.str();

    ui_shape info_shape(
            info,
            {},
            FPS_WIDGET_INFO_FONT_SIZE,
            FPS_WIDGET_INFO_TEXT_COLOR,
            0);
    info_shape.rect.dim.x = text_size("last: 999.9", FPS_WIDGET_INFO_FONT_SIZE).x;  // static size to avoid wiggle
    fit_shape(panel, info_shape);
    panel->shapes.push_back(info_shape);


    std::ostringstream max_stream;
    max_stream << " ; max: " << std::setprecision(3) << (1000.f*max_cost);
    std::string max_str = max_stream.str();

    ui_shape max_shape(
            max_str,
            {},
            FPS_WIDGET_INFO_FONT_SIZE,
            FPS_WIDGET_INFO_TEXT_COLOR,
            0);
    max_shape.rect.dim.x = text_size(" ; max: 999.9", FPS_WIDGET_INFO_FONT_SIZE).x;  // static size to avoid wiggle
    fit_shape(panel, max_shape, false);
    panel->shapes.push_back(max_shape);
}


struct {
    std::vector<std::string> lines;
    bool next_on_same_line = false;
} status_bar;

void ui_add_status(std::string str) {
    std::string time = get_formatted_time();

    bool first_line = true;
    u32 current_pos = 0;
    for(bool _break=false ; !_break && current_pos < str.size() ; ) {
        u64 linebreak = str.find('\n', current_pos);

        std::string substr;
        if(linebreak == std::string::npos) {  // no more linebreak
            substr = str.substr(current_pos);
            _break = true;
        } else {
            substr = str.substr(current_pos, linebreak-current_pos);
            current_pos = linebreak+1;
        }

        if(first_line && !status_bar.lines.empty() && status_bar.next_on_same_line) {
            status_bar.lines[status_bar.lines.size()-1] += substr;
        } else {
            std::string line = "[" + time + "] " + substr;
            status_bar.lines.push_back(line);
        }

        first_line = false;
    }

    status_bar.next_on_same_line = (current_pos != str.size());  // ended with linebreak
}

// TODO scrollable panel while hovering
void render_ui_status() {  // autocalled on render_ui (needs to be called to draw the status bar)
    SCOPE_TIMER(TIMER_GPU | TIMER_CPU);
    i32 max_lines = std::min((i32)status_bar.lines.size(), STATUS_BAR_MAX_LINES);
    f32 bar_height = max_lines*STATUS_BAR_LINE_HEIGHT + STATUS_BAR_TEXT_Y_OFFSET;

    aabb2f bar(
        vec2f {0, (f32)WINDOW_HEIGHT-bar_height},
        vec2f {(f32)WINDOW_WIDTH, bar_height}
    );

    if(first_panel_at(gui_mouse_state.pos) == NULL
            && bar.contains(gui_mouse_state.pos)) {  // no panel in front of the status bar: draw all the log

        max_lines = std::max(max_lines, (i32)status_bar.lines.size());
        bar_height = max_lines*STATUS_BAR_LINE_HEIGHT + STATUS_BAR_TEXT_Y_OFFSET;
        bar.corner.y = (f32)WINDOW_HEIGHT-bar_height;
        bar.dim.y = bar_height;
    }

    // bar
    DRAW_RECT(bar,
              STATUS_BAR_COLOR,
              STATUS_BAR_BORDER_COLOR,
              STATUS_BAR_BORDER_SIZE);

    // log
    for(i32 i=0 ; i < max_lines ; i++) {
        i32 idx = status_bar.lines.size()-i-1;
        if(idx < 0) break;  // no more lines to draw
        DRAW_TEXT(
                bar.corner
                    + vec2f{STATUS_BAR_TEXT_X_OFFSET, STATUS_BAR_TEXT_Y_OFFSET}
                    + (max_lines-i-1)*vec2f{0.f, STATUS_BAR_LINE_HEIGHT},
                STATUS_BAR_FONT_SIZE,
                status_bar.lines[idx],
                white);
    }
}

f32 status_bar_locked_height() {
    i32 status_lines = std::min((i32)status_bar.lines.size(), STATUS_BAR_MAX_LINES);
    f32 status_height = status_lines*STATUS_BAR_LINE_HEIGHT + STATUS_BAR_TEXT_Y_OFFSET;
    return status_height;
}

bool is_panel_empty(dock_area* d) {
    if(d->panels.empty())
        return true;

    for(dock_area_panel_info pi: d->panels)
        if(pi.panel->pushed_last_frame && !pi.panel->hidden)
            return false;

    return true;
}

aabb2f no_gui_window_bb() {
    aabb2f bb({0,0}, {(f32)WINDOW_WIDTH,(f32)WINDOW_HEIGHT});

    if(!draw_ui)
        return bb;

    if(!is_panel_empty(&left_dock)) {
        assert(left_dock.panels[0].panel);
        f32 offset = left_dock.panels[0].panel->bounding_box().dim.x - 5.f;
        bb.corner.x += offset;
        bb.dim.x -= offset;
    }

    if(!is_panel_empty(&right_dock)) {
        assert(right_dock.panels[0].panel);
        bb.dim.x -= right_dock.panels[0].panel->bounding_box().dim.x;
    }

    f32 status_height = status_bar_locked_height() - 5.f;
    bb.corner.y += status_height;
    bb.dim.y -= status_height;

    return bb;
}


bool is_over_panel(vec2f p)
{
    ui_panel *panel = first_panel_at(p, true);
    if (!panel) return false;
    // TODO :(
    if (!panel->draw_border) return false;
    return true;
}

void ui_example() {
    push_panel();
    ui_label("hello!");
    if(!static_toggle("hide slider", "show slider"))
        static_slider(1.2f, -5, 5);
    static_slider(1.2f, -5, 5, "plop");
    ui_button("plop", vec2f{ ui_panel_width(), 0 });

    static std::string uis;
    if(ui_textfield(&uis))
        std::cout << uis << std::endl;

    if(static_toggle(">", "<")) {
        static_slider(0.8f, 0.f, 1.f, "a");
        static_textfield("plouf");
        if(static_toggle(">", "<")) {
            static_slider(0.1f, 0.f, 1.f, "a2");
            static_slider(0.6f, 0.f, 1.f, "a3");
        }
        static_slider(0.2f, 0.f, 1.f, "c");
    }

    ui_button("plouf");
    ui_same_row();
    ui_button("plop");
    ui_same_row();
    ui_label("hey!");

    begin_row();
    ui_button("plouf");
    ui_button("plop");
    ui_label("hey!");
    end_row();

    static_list("ab", { "jmoidzaj", "ab", "jmida", "joidmza joiz" });

    static f32 val = 12.f;
    begin_list();
    for(f32 vx = 8.f ; vx < 15.f ; vx += 1.f) {
        if(ui_list_item("10x" + std::to_string(0.1f * vx), val == vx))
            val = vx;
        /* ui_same_row(); */
        /* _static_toggle(UI_NEW_ID + (i32)vx + 10000, "toggle!"); */
    }
    end_list();

    ui_button("plop post list");
    pop_panel();
}
