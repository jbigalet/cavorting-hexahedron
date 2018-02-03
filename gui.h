#pragma once

// from https://color.adobe.com/Neutral-Blue-color-theme-22361/
#define PALETTE_1 vec(color::from_srgb888(0xfcfff5), 1.f)
#define PALETTE_2 vec(color::from_srgb888(0xd1dbbd), 1.f)
#define PALETTE_3 vec(color::from_srgb888(0x91aa9d), 1.f)
#define PALETTE_4 vec(color::from_srgb888(0x3e606f), 1.f)
#define PALETTE_5 vec(color::from_srgb888(0x193441), 1.f)

#define DRAW_RECT draw_rect
#define DRAW_TEXT draw_text
#define TEXT_WIDTH text_dim
#define WINDOW_WIDTH gfx_state.window_width
#define WINDOW_HEIGHT gfx_state.window_height
#define WINDOW_HANDLE gfx_state.window

#define SLIDER_MODE_LOG_EPSILON 0.000001
#define SLIDER_HANDLE_COLOR PALETTE_3
#define SLIDER_HANDLE_HOVER_COLOR PALETTE_2
#define SLIDER_HANDLE_ACTIVE_COLOR PALETTE_1
#define SLIDER_HANDLE_INVALID_COLOR red
#define SLIDER_HANDLE_BORDER_COLOR PALETTE_1
#define SLIDER_HANDLE_BORDER_SIZE 1.f
#define SLIDER_FILL_COLOR PALETTE_5
#define SLIDER_BORDER_COLOR PALETTE_4
#define SLIDER_BORDER_SIZE 1.f
#define SLIDER_INSIDE_TEXT_COLOR PALETTE_1
#define SLIDER_INSIDE_TEXT_SIZE 20.f
#define SLIDER_TEXTFIELD_TEXT_COLOR PALETTE_5
#define SLIDER_TEXTFIELD_INVALID_COLOR red

#define PANEL_MARGIN 5
#define PANEL_PADDING 10
#define PANEL_ROW_MARGIN_X 5
#define PANEL_ROW_MARGIN_Y 3
#define PANEL_RESIZE_CORNER_DIM vec2f{ 30, 30 }
#define PANEL_COLOR PALETTE_5
#define PANEL_BORDER_COLOR vec4f{ 0.2f, 0.2f, 0.2f, 1 }
#define PANEL_BORDER_RADIUS 3.f

#define PANEL_SCROLL_SPEED 10.f  // pixel by scroll tick

#define UI_GROUP_OFFSET_X 15
#define UI_GROUP_OFFSET_BELOW 8

#define SCROLLBAR_ZONE_WIDTH 10.f
#define SCROLLBAR_ZONE_PADDING 1.f
#define SCROLLBAR_ZONE_COLOR PALETTE_2
#define SCROLLBAR_COLOR PALETTE_3
#define SCROLLBAR_HOVER_COLOR PALETTE_4
#define SCROLLBAR_CLICKED_COLOR PALETTE_5
#define SCROLLBAR_CLICK_SCROLL_SIZE_RATIO 0.5f  // ratio of the current panel dim

#define UI_FONT_SIZE 20

#define BUTTON_COLOR PALETTE_4
#define BUTTON_BORDER_COLOR PALETTE_2
#define BUTTON_BORDER_RADIUS 1
#define BUTTON_HOVER_COLOR PALETTE_2
#define BUTTON_CLICKED_COLOR PALETTE_3
#define BUTTON_CLICKED_BORDER_COLOR PALETTE_1
#define BUTTON_TEXT_COLOR PALETTE_1
#define BUTTON_TEXT_MARGIN (vec2f {(BUTTON_BORDER_RADIUS + 3),(BUTTON_BORDER_RADIUS)})
#define TOGGLE_TEXT_COLOR BUTTON_TEXT_COLOR
#define TOGGLE_FILL_COLOR PALETTE_3

#define TEXTFIELD_TEXT_OFFSET_X 5.0f
#define TEXTFIELD_TEXT_COLOR PALETTE_5
#define TEXTFIELD_CURSOR_MARGIN_Y 5.f
#define TEXTFIELD_CURSOR_WIDTH 2.f
#define TEXTFIELD_CURSOR_TIMER 0.5f  // in seconds
#define TEXTFIELD_CURSOR_COLOR PALETTE_5
#define TEXTFIELD_CURSOR_BORDER_COLOR PALETTE_5
#define TEXTFIELD_CURSOR_BORDER_SIZE 0.f
#define TEXTFIELD_SELECTION_COLOR vec4f{ 0.276f, 0.443f, 0.941f, 1 }
#define TEXTFIELD_SELECTION_BORDER_COLOR PALETTE_5
#define TEXTFIELD_SELECTION_BORDER_SIZE 0.f
#define TEXTFIELD_HEIGHT UI_FONT_SIZE
#define TEXTFIELD_FILL_COLOR PALETTE_1
#define TEXTFIELD_BORDER_COLOR PALETTE_5
#define TEXTFIELD_ACTIVE_BORDER_COLOR PALETTE_4
#define TEXTFIELD_HOVER_BORDER_COLOR PALETTE_3
#define TEXTFIELD_BORDER_SIZE 2.f

#define LIST_PADDING 10.f
#define LIST_FILL_COLOR PALETTE_4
#define LIST_BORDER_COLOR PALETTE_1
#define LIST_BORDER_SIZE 1
#define LIST_ITEM_SPACING -5.f
#define LIST_ITEM_TEXT_COLOR PALETTE_1
#define LIST_ITEM_FILL_COLOR PALETTE_3
#define LIST_ITEM_FILL_COLOR_SELECTED PALETTE_5
#define LIST_ITEM_BORDER_COLOR PALETTE_5
#define LIST_ITEM_BORDER_SIZE 0.f

#define FPS_WIDGET_FRAME_COUNT 50
#define FPS_WIDGET_FONT_SIZE 40.f
#define FPS_WIDGET_TEXT_COLOR black
#define FPS_WIDGET_BAR_COLOR PALETTE_4
#define FPS_WIDGET_BAR_BORDER_COLOR PALETTE_3
#define FPS_WIDGET_BAR_BORDER_RADIUS 0.f
#define FPS_WIDGET_INFO_FONT_SIZE 20.f
#define FPS_WIDGET_INFO_TEXT_COLOR PALETTE_1
#define FPS_WIDGET_FILL_COLOR PALETTE_2
#define FPS_WIDGET_BORDER_COLOR PALETTE_1
#define FPS_WIDGET_BORDER_SIZE 2.f

#define STATUS_BAR_MAX_LINES 2
#define STATUS_BAR_LINE_HEIGHT 21.f
#define STATUS_BAR_COLOR PANEL_COLOR
#define STATUS_BAR_BORDER_COLOR PANEL_BORDER_COLOR
#define STATUS_BAR_BORDER_SIZE 1.f
#define STATUS_BAR_FONT_SIZE 18.f
#define STATUS_BAR_TEXT_X_OFFSET 5.f
#define STATUS_BAR_TEXT_Y_OFFSET 2.f

static u32 __local_unique;
extern u64 current_ui_context;
extern std::vector<u64> ui_context_stack;  // TODO remove - only for debug purposes
#define UI_NEW_ID (((((i64)&__local_unique)<<16) | (__COUNTER__+1)) ^ current_ui_context)
#define GUI_ID_TYPE i64

#define push_panel() _push_panel(UI_NEW_ID)
#define push_custom_panel(...) _push_panel(UI_NEW_ID, __VA_ARGS__)
#define ui_button(...) _ui_button(UI_NEW_ID, __VA_ARGS__)
#define ui_toggle(...) _ui_toggle(UI_NEW_ID, __VA_ARGS__)
#define static_toggle(...) _static_toggle(UI_NEW_ID, __VA_ARGS__)
#define ui_slider(...) _ui_slider(UI_NEW_ID, __VA_ARGS__)
#define static_slider(...) _static_slider(UI_NEW_ID, __VA_ARGS__)
#define ui_textfield(...) _ui_textfield(UI_NEW_ID, __VA_ARGS__)
#define static_textfield(...) _static_textfield(UI_NEW_ID, __VA_ARGS__)
#define ui_list(...) _ui_list(UI_NEW_ID, __VA_ARGS__)
#define static_list(...) _static_list(UI_NEW_ID, __VA_ARGS__)
#define begin_list() _begin_list(UI_NEW_ID)
#define begin_group(...) _begin_group(UI_NEW_ID, __VA_ARGS__)

enum struct dock_id {
    left,
    right,
    none
};

void ui_label(std::string text,
              vec2f size=vec2f(),
              vec2f offset=vec2f(),
              f32 font_size=UI_FONT_SIZE,
              vec4f color=vec(vec3f::O, -1.f));
void ui_labelf(ccstring format, ...);

bool _ui_button(GUI_ID_TYPE id, std::string text, vec2f size=vec2f(), f32 font_size=UI_FONT_SIZE);

void _ui_toggle(GUI_ID_TYPE id,
                bool* value,
                std::string text,
                std::string text_pushed=std::string(),
                vec2f size=vec2f(),
                vec2f offset=vec2f(),
                f32 font_size=UI_FONT_SIZE);

bool _ui_toggle(GUI_ID_TYPE id,
                bool value,
                std::string text,
                std::string text_pushed=std::string(),
                vec2f size=vec2f(),
                vec2f offset=vec2f(),
                f32 font_size=UI_FONT_SIZE);

bool _static_toggle(GUI_ID_TYPE id,
                    std::string text,
                    std::string text_pushed=std::string(),
                    bool init=false,
                    vec2f size=vec2f(),
                    vec2f offset=vec2f(),
                    f32 font_size=UI_FONT_SIZE);

bool _begin_group(GUI_ID_TYPE id,
                  std::string text,
                  bool init=false,
                  vec2f size=vec2f(),
                  vec2f offset=vec2f(),
                  f32 font_size=UI_FONT_SIZE);
void close_group();
void end_group();

bool _ui_textfield(GUI_ID_TYPE id,
                   std::string* str,
                   vec2f size    = vec2f(),
                   vec2f offset  = vec2f(),
                   f32 font_size = UI_FONT_SIZE,
                   bool new_row  = true,
                   vec4f color   = TEXTFIELD_TEXT_COLOR);

std::string _static_textfield(GUI_ID_TYPE id,
        std::string init="",
        vec2f size=vec2f(),
        vec2f offset=vec2f(),
        f32 font_size=UI_FONT_SIZE);

enum struct slider_mode {
    linear,
    log
};

template<typename T, typename T2, typename T3>
bool _ui_slider(GUI_ID_TYPE id,
                T* f, T2 _min=0, T3 _max=1,
                std::string name="",
                slider_mode mode=slider_mode::linear,
                vec2f size=vec2f());

template<typename T, typename T2, typename T3>
T _static_slider(GUI_ID_TYPE id,
                 T init, T2 _min=0, T3 _max=1,
                 std::string name="",
                 slider_mode mode=slider_mode::linear,
                 vec2f size=vec2f());

void _begin_list(GUI_ID_TYPE id);
void end_list();
bool ui_list_item(std::string label, bool selected);
bool ui_list_item(std::string label, bool selected, u8 *mods);


void ui_push_ctx(u64);
void ui_push_ctx(void*);
void ui_pop_ctx();

struct ui_resize_policy {
    bool enabled;
    bool can_move;
    vec2f dim;  // starting dim. if not set, will start packed, if negative will be a ratio of the packed dim
    vec2f min;  // if negative, will be a ratio of the packed dim
    vec2f max;  // if negative, will be a ratio of the packed dim
    f32 ratio;  // width / height
    vec2f pos;  // sets position
    ui_resize_policy(): enabled(true), can_move(true), dim(vec2f()), min(vec2f()), max(vec2f()), ratio(0), pos(vec2f::O) {}
};

enum scroll_flags {
    VERTICAL_SCROLL = 1,
    HORIZONTAL_SCROLL = 2
};

void _push_panel(GUI_ID_TYPE id,
                 dock_id dock=dock_id::none,
                 ui_resize_policy resize_policy=ui_resize_policy(),
                 u8 scroll_flags=VERTICAL_SCROLL | HORIZONTAL_SCROLL,
                 vec2f padding={PANEL_PADDING,PANEL_PADDING},
                 bool draw_border=true);
void pop_panel();
void hide_current_panel();
void unhide_current_panel();
void set_current_panel_size_ratio(f32);

void begin_ui(f64 dt);
void render_ui();

void fps_widget();
void ui_example();

void ui_new_row();
void ui_same_row();
void begin_new_row();
void begin_same_row();
void end_new_row();
void end_same_row();
void begin_row();
void end_row();

f32 status_bar_locked_height();
void ui_add_status(std::string);
aabb2f no_gui_window_bb();

bool is_over_panel(vec2f);
void focus_next_textfield();
bool is_current_panel_hidden();

extern bool draw_ui;
