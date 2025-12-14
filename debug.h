#ifndef __DEBUG__
#define __DEBUG__

#define LOG_FILE "./text_editor.log"

int debug_init();
void debug(const char *fmt, ...);
void debug_close();

#endif // __DEBUG__
