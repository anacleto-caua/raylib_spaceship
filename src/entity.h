#pragma once

typedef void (*InitFunc)();
typedef void (*UpdateFunc)();
typedef void (*EndFunc)();

// TODO: Consider breaking it into Update() and Draw()
typedef struct Entity {
    InitFunc Init;
    UpdateFunc Update;
    EndFunc End;
    int index;
} Entity;

Entity* AddEntity(InitFunc InitFunc, UpdateFunc UpdateFunc, EndFunc EndFunc);

void RemoveEntity(int index);

void UpdateAllEntities();

void EndAllEntities();