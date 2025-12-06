#pragma once

#include "debugger.h"

typedef enum {
    TYPE_INT,
    TYPE_FLOAT
    // TODO: Add more types
} DataType;

typedef struct DebugMsg {
    char* label;
    DataType dataType;
    void* dataPtr;
}DebugMsg;


#define MAX_DEBUG_MESSAGES 100
extern struct DebugMsg* MessagesToDebug[MAX_DEBUG_MESSAGES];
extern int debugMsgCount;

// TODO: Decide if this should or not be a full on entity
// TODO: Decide if I'll need thread safety for handling multiple messages 
void DebuggerInit();
void DebuggerUpdate();
void DebuggerEnd();

void DrawAllDebugMessages();

// TODO: Make it possible to attach more messages without overdrawing
DebugMsg* AddDebugMsg(char* label, DataType dataType, void* dataPtr);
void RemoeveDebugMsg(int index);