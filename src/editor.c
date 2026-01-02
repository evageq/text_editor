#include "debug.h"
#include "definitions.h"
#include "prototypes.h"
#include <ctype.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

WINDOW *g_top_win;
WINDOW *g_text_win;
WINDOW *g_bottom_win;

int g_top_win_lines;
int g_text_win_lines;
int g_bottom_win_lines;

str_line_t *g_line_head;
str_line_t *g_line_print_start;

cursor_t g_curs;

char g_line_buffer[LINE_BUFFER_LEN];
char g_file_name_arg[FILE_NAME_LEN];

bool g_running = FALSE;

str_line_t *
get_first_print_line()
{
    assert(g_line_print_start != NULL);
    return g_line_print_start;
}

str_line_t *
get_last_print_line()
{
    str_line_t *start_line = get_first_print_line();
    str_line_t *tmp_line = start_line;

    for (size_t i = 0; i < g_text_win_lines; ++i)
    {
        if (tmp_line->next != NULL)
        {
            tmp_line = tmp_line->next;
        }
        else
        {
            break;
        }
    }

    assert(tmp_line != NULL);
    return tmp_line;
}

void
load_file(const char *file)
{

    FILE *f = fopen(file, "r");

    str_line_t *tmp_line;
    str_line_t *prev_line = NULL;
    while (fgets(g_line_buffer, sizeof(g_line_buffer), f))
    {
        g_line_buffer[strlen(g_line_buffer)-1] = 0;
        tmp_line = malloc(sizeof(str_line_t));
        *tmp_line = (str_line_t){ .next = NULL,
                                  .prev = prev_line,
                                  .p_str = string_new(g_line_buffer) };
        if (prev_line != NULL)
        {
            prev_line->next = tmp_line;
        }

        prev_line = tmp_line;

        if (g_line_head == NULL)
        {
            g_line_head = tmp_line;
        }
    }

    fclose(f);
}

void
draw_text_curs()
{
    wmove(g_text_win, g_curs.y, g_curs.x);
}

void
draw()
{
    draw_top_win();
    draw_text_win();
    draw_bottom_win();
    draw_text_curs();
    doupdate();
}

void
draw_top_win()
{
    WINDOW *win = g_top_win;

    const char editor_banner[] = "GNU nano clone";
    werase(win);
    wbkgd(win, COLOR_PAIR(TITLE_BAR));
    wprintw(win, editor_banner);
    wnoutrefresh(win);
}

void
draw_text_line(str_line_t *line)
{
    wprintw(g_text_win, "%s\n", line->p_str->buf);
}

void
draw_text_win()
{
    WINDOW *win = g_text_win;
    werase(win);

    str_line_t *line = get_first_print_line();
    str_line_t *end_line = get_last_print_line();
    while (line != NULL && line != end_line)
    {
        draw_text_line(line);
        line = line->next;
    }
    draw_text_line(end_line);

    wnoutrefresh(win);
}

void
draw_bottom_win()
{
    WINDOW *win = g_bottom_win;
    box(win, 0, 0);
    wnoutrefresh(win);
}

void
setup()
{
    debug_init();

    initscr();
    start_color();
    cbreak();
    noecho();
    if (has_colors() == FALSE)
    {
        debug("Terminal doesn't support colors");
    }
    else
    {
        use_default_colors();
    }
    init_pair(TITLE_BAR, COLOR_BLACK, COLOR_WHITE);
    window_init();
    keypad(g_text_win, TRUE);

    load_file(g_file_name_arg);
    g_line_print_start = g_line_head;

    g_curs = (cursor_t){ .x = 0, .y = 0, .line = g_line_print_start };

    g_running = TRUE;
}

void
window_init()
{

    if (LINES >= 3)
    {
        g_top_win_lines = 1;
        g_bottom_win_lines = 3;
        assert(g_top_win_lines >= 0);
        assert(g_bottom_win_lines >= 0);

        g_text_win_lines = LINES - g_top_win_lines - g_bottom_win_lines;
        assert(g_text_win_lines >= 0);

        g_top_win = newwin(g_top_win_lines, COLS, 0, 0);
        g_text_win = newwin(g_text_win_lines, COLS, 0 + g_top_win_lines, 0);
        g_bottom_win = newwin(g_bottom_win_lines, COLS, g_text_win_lines, 0);
    }
    else
    {
        g_top_win = newwin(0, 0, 0, 0);
        g_text_win = newwin(LINES, COLS, 0, 0);
        g_bottom_win = newwin(0, 0, 0, 0);
    }
}

void
run()
{

    while (g_running == TRUE)
    {
        draw();
        int ch;
        ch = wgetch(g_text_win);
        process_key(ch);
    }
}

// static inline bool
// is_curs_out_bound(int x, int y)
// {
//     str_line_t *tmp= get_first_print_line();
//     if (x < 0)
//     {
//         return true;
//     }
//
//
// }

void
move_curs(int key)
{
    assert(g_curs.line != NULL);
    assert(g_curs.x >= 0);
    assert(g_curs.y < g_text_win_lines);

    switch (key)
    {
        case E_KEY_UP:
        {
            if (g_curs.line->prev != NULL)
            {
                if (g_curs.y <= 0)
                {
                    g_line_print_start = g_curs.line->prev;
                }
                g_curs.line = g_curs.line->prev;

                if (g_curs.y > 0)
                {
                    g_curs.y--;
                }
                g_curs.x = MIN(g_curs.x, g_curs.line->p_str->len);
            }
            break;
        }
        case E_KEY_DOWN:
        {
            if (g_curs.line->next != NULL)
            {
                if (g_curs.y + 1 < g_text_win_lines)
                {
                    g_curs.y++;
                }
                else
                {
                    g_line_print_start = g_line_print_start->next;
                }
                g_curs.line = g_curs.line->next;
                // debug("%d %d\n", g_curs.x, g_curs.line->p_str->len);
                g_curs.x = MIN(g_curs.x, g_curs.line->p_str->len);
                
            }
            break;
        }
        case E_KEY_LEFT:
        {
            if (g_curs.x > 0)
            {
                g_curs.x--;
            }
            break;
        }
        case E_KEY_RIGHT:
        {
            if (g_curs.x + 1 <= g_curs.line->p_str->len)
            {
                g_curs.x++;
            }
            break;
        }
    }
}

void
process_key(int key)
{
    switch (key)
    {
        case E_KEY_UP:
        case E_KEY_DOWN:
        case E_KEY_LEFT:
        case E_KEY_RIGHT:
        {
            move_curs(key);
            break;
        }
        case E_PASTE:
        {
            break;
        }
        case E_COPY:
        {
            break;
        }
        case E_NEXT_WORD:
        {
            break;
        }
        case E_PREV_WORD:
        {
            break;
        }
        case E_SAVE:
        {
            break;
        }
        default: // this branch mean we're probably typing
        {
            if (isprint(key))
            {
            }
        }
    }
}

void
cleanup()
{
    debug_close();
    endwin();
}

int
main(int argc, char *argv[])
{

    const char help_str[] = "Usage:\
                         ./a.out <file_name>";

    if (argc < 1)
    {
        error("Failed to run\n%s", help_str);
        exit(-1);
    }
    else
    {
        strncpy(g_file_name_arg, argv[1], sizeof(g_file_name_arg));
    }

    setup();
    run();
    cleanup();

    return 0;
}
