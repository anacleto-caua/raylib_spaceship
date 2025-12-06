#include "debugger.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const Color DEBUG_FONT_COLOR = RED;
const int DEBUG_FONT_SIZE = 20;

#define MAX_DEBUG_MESSAGES 100
struct DebugMsg* MessagesToDebug[MAX_DEBUG_MESSAGES];
int debugMsgCount = 0;

// helpers

void OutInt(char* label, int intToFormat, float textPosX)
{
    char formatedInt[10] = "";
    char finalString[100] = "";
    snprintf(formatedInt, sizeof(intToFormat), "%d", intToFormat);
    strcpy(finalString, label);
    strcat(finalString, formatedInt);

    DrawText(finalString, GetScreenWidth() - strlen(finalString) * (DEBUG_FONT_SIZE / 2), textPosX, DEBUG_FONT_SIZE, DEBUG_FONT_COLOR);
}

void OutFloat(char* label, float floatToFormat, float textPosX)
{
    char formatedFloat[10] = "";
    char finalString[100] = "";
    snprintf(formatedFloat, sizeof(formatedFloat), "%.2f", floatToFormat);
    strcpy(finalString, label);
    strcat(finalString, formatedFloat);

    DrawText(finalString, GetScreenWidth() - strlen(finalString) * (DEBUG_FONT_SIZE / 2), textPosX, DEBUG_FONT_SIZE, DEBUG_FONT_COLOR);
}

// public

DebugMsg* AddDebugMsg(char* label, DataType dataType, void* dataPtr)
{
    if (debugMsgCount >= MAX_DEBUG_MESSAGES)
    {
        return NULL;
    }

    DebugMsg* newMsg = (DebugMsg*)malloc(sizeof(DebugMsg));
    if (newMsg == NULL){
        return NULL;
    }

    newMsg->label = label;
    newMsg->dataType = dataType;
    newMsg->dataPtr = dataPtr;
    
    MessagesToDebug[debugMsgCount] = newMsg;
    debugMsgCount++;

    return newMsg;
}

void RemoeveDebugMsg(int index)
{
    DebugMsg* MsgToRemove = MessagesToDebug[index];

    free(MsgToRemove);

    int elementsToShift = debugMsgCount - index - 1;
    if (elementsToShift > 0) {
        memmove(&MsgToRemove, &MessagesToDebug[index + 1], elementsToShift * sizeof(DebugMsg*));
    }

    debugMsgCount--;
}

void DrawAllDebugMessages()
{
    int textPosX = 0;
    DebugMsg* msg;
    for(int i = 0; i < debugMsgCount; i++){
        msg = MessagesToDebug[i];
        switch (msg->dataType) {
            case TYPE_FLOAT:
                // Cast to float* and dereference
                OutFloat(msg->label, *(float*)msg->dataPtr, textPosX);
            break;
            case TYPE_INT:
                // Cast to float* and dereference
                OutFloat(msg->label, *(int*)msg->dataPtr, textPosX);
            break;
            default:
                printf("[WARNING] NOT IMPLEMENTED DEBUG TYPE\n");
        }
        textPosX += DEBUG_FONT_SIZE;
    }
}
