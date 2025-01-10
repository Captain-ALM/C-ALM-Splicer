#include <stdlib.h>

#include "actionmeta.h"

ActionMeta* newActionMeta(char* filePath) {
    ActionMeta* toRet = malloc(sizeof(ActionMeta));
    toRet->filePath = filePath;
    toRet->type = binary_mode;
    toRet->seperator = ' ';
    toRet->tokensToNewLine = 0;
    toRet->bufferSize = 8192;
    toRet->first = 0;
    toRet->last = 0;
    toRet->unbounded = true;
    return toRet;
}
