#include <stdlib.h>

#include "utils.h"
#include "actionmeta.h"

ActionMeta* newActionMeta(void) {
    ActionMeta* toRet = malloc(sizeof(ActionMeta));
    if (toRet)
    {
        toRet->filePath = null;
        toRet->type = binary_mode;
        toRet->seperator = ' ';
        toRet->tokensToNewLine = 0;
        toRet->bufferSize = 8192;
        toRet->first = 0;
        toRet->last = 0;
        toRet->unbounded = true;
        toRet->base = 63;
        toRet->max = 'z';
    }
    return toRet;
}
