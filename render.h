#ifndef __RENDER__
#define __RENDER__

#include <ncurses.h>
#include "editor.h"

#define R_GET_WIN(r, type) (r.p_editor->windows[type])
#define R_GET_NCURSE_WIN(r, type) (R_GET_WIN(r, type).win)
#define R_DEFAULT_RENDER_WIN(win) do { \
        box(win, 0, 0); wrefresh(win); \
    } while (0)

#define R_DEFAULT_RENDER(r, type) R_DEFAULT_RENDER_WIN(R_GET_NCURSE_WIN(r, type))

typedef struct render_s
{

    editor_t *p_editor;
    bool valid;
} render_t;

#define COLOR_PAIR_START 1
typedef enum color_pairs_e
{
    WHITE_BLACK = COLOR_PAIR_START,
    BLACK_WHITE,
    COLOR_PAIR_MAX
} color_paris_t;
_Static_assert(COLOR_PAIR_MAX < 255, "ncurses use short for color pairs, check extend version");

render_t render_init(editor_t *const p_editor);
int render_editor(render_t renderer);
int render_explicit(render_t, int wtype);

#endif // __RENDER__
