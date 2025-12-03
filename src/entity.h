#pragma once

typedef void (*InitFunc)();
typedef void (*UpdateFunc)();
typedef void (*EndFunc)();

typedef struct Entity {
    InitFunc Init;
    UpdateFunc Update;
    EndFunc End;
    int index;
} Entity;

#define MAX_ENTITIES 100
extern Entity* Entities[MAX_ENTITIES];
extern int entityCount;

Entity* AddEntity(InitFunc InitFunc, UpdateFunc UpdateFunc, EndFunc EndFunc);

void RemoveEntity(int index);