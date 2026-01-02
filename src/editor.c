#include <ctype.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definitions.h"
#include "prototypes.h"
#include "debug.h"

WINDOW *top_win;
WINDOW *text_win;
WINDOW *bottom_win;

char file_name_arg[FILE_NAME_LEN_MAX];

bool running = FALSE;

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
        strncpy(file_name_arg, argv[1], sizeof(file_name_arg));
    }

    setup();
    run();
    cleanup();

    return 0;
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

    keypad(text_win, TRUE);
    // nodelay() for text area

    running = TRUE;
}

void
run()
{

    while (running == TRUE)
    {
        draw();
        int ch;
        ch = wgetch(text_win);
        process_key(ch);
    }
}

void
cleanup()
{
    debug_close();
    endwin();
}

void
process_key(int ch)
{
    switch (ch)
    {
        case E_KEY_UP:
        {
            break;
        }
        case E_KEY_DOWN:
        {
            break;
        }
        case E_KEY_LEFT:
        {
            break;
        }
        case E_KEY_RIGHT:
        {
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
            if (isprint(ch))
            {
            }
        }
    }
}

void
window_init()
{
    int top_win_lines;
    int text_win_lines;
    int bottom_win_lines;

    if (LINES >= 3)
    {
        top_win_lines = 1;
        bottom_win_lines = 3;
        assert(top_win_lines >= 0);
        assert(bottom_win_lines >= 0);

        text_win_lines = LINES - top_win_lines - bottom_win_lines;
        assert(text_win_lines >= 0);

        top_win = newwin(top_win_lines, COLS, 0, 0);
        text_win = newwin(text_win_lines, COLS, 0+top_win_lines, 0);
        bottom_win = newwin(bottom_win_lines, COLS, text_win_lines, 0);

    }
    else
    {
        top_win = newwin(0, 0, 0, 0);
        text_win = newwin(LINES, COLS, 0, 0);
        bottom_win = newwin(0, 0, 0, 0);
    }


}

void draw_top_win()
{
    WINDOW *win = top_win;

    const char editor_banner[] = "GNU nano clone";
    werase(win);
    wbkgd(win, COLOR_PAIR(TITLE_BAR));
    wprintw(win, editor_banner);
    wrefresh(win);
}

void
draw_text_win()
{
    WINDOW *win = text_win;
    wrefresh(win);
}

void
draw_bottom_win()
{
    WINDOW *win = bottom_win;
    box(win, 0, 0);
    wrefresh(win);
}

void
draw()
{
    draw_top_win();
    draw_text_win();
    draw_bottom_win();
}
