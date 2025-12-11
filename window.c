#include <assert.h>
#include "common.h"
#include "window.h"

static window_t
create_window(window_params_t params)
{
    return (window_t){
        .type = params.type,

        .width = params.width,
        .height = params.height,

        .startx = params.startx,
        .starty = params.starty,

        .cursor_pos_x = params.cursor_pos_x,
        .cursor_pos_y = params.cursor_pos_y,

        .win = newwin(params.height, params.width, params.starty, params.startx),
        .valid = params.valid 
    };
}

static int
ensure_windows_created(const window_t *windows)
{
    int ret = 0;

    for (int i = 0; i < WINDOW_MAX; ++i)
    {
        if (windows[i].valid == FALSE)
        {
            fprintf(stderr, "Failed to create %d window\n", i);
            ret = -1;
        }
    }

    return ret;
}

static window_t
window_top_bar_init()
{
    int type = WINDOW_TOP_BAR;

    int h = WINDOW_TOP_BAR_HEIGHT;
    int w = get_n_by_perc(WINDOW_BASE_WIDTH, 100);

    int startx = 0;
    int starty = 0;

    window_params_t window_params = {
        .type = type,
        .height = h,
        .width = w,
        .startx = startx,
        .starty = starty,
        .cursor_pos_x = 0,
        .cursor_pos_y = 0
    };

    window_t window = (window_params.valid = TRUE, create_window(window_params));
    WINDOW* curs_win = window.win;

    return window;
}

static window_t
window_text_area_init()
{
    int type = WINDOW_TEXT_AREA;

    int h = WINDOW_TEXT_AREA_HEIGHT;
    int w = get_n_by_perc(WINDOW_BASE_WIDTH, 100);

    int startx = 0;
    int starty = 0 + WINDOW_TOP_BAR_HEIGHT;

    window_params_t window_params = {
        .type = type,
        .height = h,
        .width = w,
        .startx = startx,
        .starty = starty,
        .cursor_pos_x = 0,
        .cursor_pos_y = 0
    };

    window_t window = (window_params.valid = TRUE, create_window(window_params));
    WINDOW* curs_win = window.win;
    keypad(curs_win, TRUE);

    return window;
}

static window_t
window_bottom_bar_init()
{
    int type = WINDOW_BOTTOM_BAR;

    int h = WINDOW_BOTTOM_BAR_HEIGHT;
    int w = get_n_by_perc(WINDOW_BASE_WIDTH, 100);

    int startx = 0;
    int starty = WINDOW_BASE_HEIGHT - h - 1;

    window_params_t window_params = {
        .type = type,
        .height = h,
        .width = w,
        .startx = startx,
        .starty = starty,
        .cursor_pos_x = 0,
        .cursor_pos_y = 0
    };

    window_t window = (window_params.valid = TRUE, create_window(window_params));
    WINDOW* curs_win = window.win;

    return window;
}

static window_t
window_init(int type)
{
    window_t window;

    switch (type)
    {
        case WINDOW_TOP_BAR:
            {
                window = window_top_bar_init();
                break;
            }
        case WINDOW_TEXT_AREA:
            {
                window = window_text_area_init();
                break;
            }
        case WINDOW_BOTTOM_BAR:
            {
                window = window_bottom_bar_init();
                break;
            }
        default:
            {
                fprintf(stderr, "Unknwon window type %d\n", type);
                window.valid = FALSE;
            }
    }

    return window;
}

int
window_editor_init(window_t *windows)
{
    int ret = 0;

    assert(WINDOW_TEXT_AREA_HEIGHT >= 0);

    for (int i = 0; i < WINDOW_MAX; ++i)
    {
        windows[i] = window_init(i);
    }

    ret = ensure_windows_created(windows);

    return ret;
}

void
window_set_cursor(window_t *const window, int x, int y)
{
    window->cursor_pos_x = x;
    window->cursor_pos_y = y;
}
