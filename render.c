#include <assert.h>
#include "render.h"
#include <assert.h>
#include "window.h"

static int
render_init_color_pairs()
{
    for (int i = COLOR_PAIR_START; i < COLOR_PAIR_MAX; ++i)
    {
        switch (i)
        {
            case BLACK_WHITE:
                {
                    init_pair(i, COLOR_BLACK, COLOR_WHITE);
                    break;
                }
            case WHITE_BLACK:
                {
                    init_pair(i, COLOR_WHITE, COLOR_BLACK);
                    break;
                }
            default:
                assert(0 == 1);
        }
    }

    return 0;
}

static int
render_init_colors()
{
    start_color();

    render_init_color_pairs();

    return 0;
}

static int
render_window_top_bar(render_t r)
{
    WINDOW *win = R_GET_NCURSE_WIN(r, WINDOW_TOP_BAR);

    werase(win);
    wbkgd(win, COLOR_PAIR(BLACK_WHITE));
    wprintw(win, "GNU nano clone");
    wrefresh(win);
    return 0;
}

static int
render_window_text_area(render_t r)
{
    WINDOW *win = R_GET_NCURSE_WIN(r, WINDOW_TEXT_AREA);
    wmove(win, R_GET_WIN(r, WINDOW_TEXT_AREA).cursor_pos_y,
            R_GET_WIN(r, WINDOW_TEXT_AREA).cursor_pos_x);

    return 0;
}

static int
render_window_bottom_bar(render_t r)
{
    WINDOW *win = R_GET_NCURSE_WIN(r, WINDOW_BOTTOM_BAR);
    R_DEFAULT_RENDER(r, WINDOW_BOTTOM_BAR);
    return 0;
}

static int
render_window(render_t r, window_t window)
{
    int ret = 0;

    if (window.valid == FALSE)
    {
        ret = -1;
        return ret;
    }

    switch (window.type)
    {
        case WINDOW_TOP_BAR:
            {
                render_window_top_bar(r);
                break;
            }
        case WINDOW_TEXT_AREA:
            {
                render_window_text_area(r);
                break;
            }
        case WINDOW_BOTTOM_BAR:
            {
                render_window_bottom_bar(r);
                break;
            }
        default:
            {
                debug("Unknown window type to render: %d", window.type);
                ret = -1;
            }
    }

    return ret;
}

inline int
render_explicit(render_t r, int wtype)
{
    return render_window(r, r.p_editor->windows[wtype]);
}

int
render_editor(render_t r)
{
    int ret = 0;
    window_t *windows = r.p_editor->windows;

    for (int i = 0; i < WINDOW_MAX; ++i)
    {
        ret = render_explicit(r, i);
        if (ret < 0)
        {
            return ret;
        }
    }

    return ret;
}


render_t render_init(editor_t *const p_editor)
{
    render_t renderer;

    renderer.p_editor = p_editor;

    render_init_colors();

    return (renderer.valid = TRUE, renderer);
}
