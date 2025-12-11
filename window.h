#ifndef __WINDOW__
#define __WINDOW__

#include <ncurses.h>

#define WINDOW_BASE_LINES LINES
#define WINDOW_BASE_COLS COLS
#define WINDOW_BASE_HEIGHT WINDOW_BASE_LINES
#define WINDOW_BASE_WIDTH WINDOW_BASE_COLS

#define WINDOW_TOP_BAR_HEIGHT 1
#define WINDOW_BOTTOM_BAR_HEIGHT 3
#define WINDOW_TEXT_AREA_HEIGHT (WINDOW_BASE_HEIGHT-(WINDOW_TOP_BAR_HEIGHT+WINDOW_BOTTOM_BAR_HEIGHT))

typedef enum window_type_e
{
    WINDOW_TOP_BAR = 0,
    WINDOW_TEXT_AREA,
    WINDOW_BOTTOM_BAR,
    WINDOW_MAX
} window_type_t;

typedef struct window_s
{
    window_type_t type;

    int height;
    int width;

    int startx;
    int starty;

    int cursor_pos_x;
    int cursor_pos_y;

    WINDOW* win;

    bool valid;

} window_t;

typedef window_t window_params_t;

int window_editor_init(window_t *windows);
void window_set_cursor(window_t *const window, int x, int y);

#endif // __WINDOW__
