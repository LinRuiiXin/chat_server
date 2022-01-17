#include <sys/timeb.h>
#include "headers/timestamp.h"

uint_64 current_timestamp() {
    struct timeb now{};
    ftime(&now);
    return (now.time * 1000) + now.millitm;
}
