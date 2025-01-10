#include <stdlib.h>

#include "utils.h"
#include "actionmeta.h"

ActionMeta* newActionMeta(void) {
    ActionMeta* toRet = malloc(sizeof(ActionMeta));
    toRet->filePath = null;
    toRet->type = binary_mode;
    toRet->seperator = ' ';
    toRet->tokensToNewLine = 0;
    toRet->bufferSize = 8192;
    toRet->first = 0;
    toRet->last = 0;
    toRet->unbounded = true;
    return toRet;
}
