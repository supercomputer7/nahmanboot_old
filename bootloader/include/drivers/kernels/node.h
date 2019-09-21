#ifndef __NODE_H
#define __NODE_H

#include <stdint.h>

typedef struct DataNode {
    uint32_t type;
    uint32_t size;
} DataNode;

typedef struct ExitNode {

    void (*startNode)(void *ptr);

} ExitNode;

#endif