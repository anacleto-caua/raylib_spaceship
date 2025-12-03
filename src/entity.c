#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entity.h"

#define MAX_ENTITIES 100
Entity* Entities[MAX_ENTITIES];
int entityCount = 0;

Entity* AddEntity(InitFunc InitFunc, UpdateFunc UpdateFunc, EndFunc EndFunc)
{
    if (entityCount >= MAX_ENTITIES)
    {
        return NULL;
    }

    Entity* newEntity = (Entity*)malloc(sizeof(Entity));
    if (newEntity == NULL){
        return NULL;
    }

    newEntity->Init = InitFunc;
    newEntity->Update = UpdateFunc;
    newEntity->End = EndFunc;

    Entities[entityCount] = newEntity;
    entityCount++;

    newEntity->Init();
    
    return newEntity;
}

void RemoveEntity(int index)
{
    Entity* EntityToRemove = Entities[index];
    EntityToRemove->End();

    free(EntityToRemove);

    int elementsToShift = entityCount - index - 1;
    if (elementsToShift > 0) {
        memmove(&EntityToRemove, &Entities[index + 1], elementsToShift * sizeof(Entity*));
    }

    entityCount--;
}