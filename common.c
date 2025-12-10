#include "common.h"
#include <assert.h>

inline size_t
get_n_by_perc(size_t n, uint perc)
{
    assert(perc >= 0 && perc <= 100);
    return ((n * perc) / 100);
}
