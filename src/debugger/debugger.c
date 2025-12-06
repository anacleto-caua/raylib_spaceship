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

float textPosX = 0;

// helpers
void DrawOut(char* outString)
{
    DrawText(outString, GetScreenWidth() - strlen(outString) * (DEBUG_FONT_SIZE / 2), textPosX, DEBUG_FONT_SIZE, DEBUG_FONT_COLOR);
}

void OutInt(char* label, int intToFormat)
{
    char formatedInt[10] = "";
    char finalString[100] = "";
    snprintf(formatedInt, sizeof(intToFormat), "%d", intToFormat);
    strcpy(finalString, label);
    strcat(finalString, formatedInt);

    DrawOut(finalString);
}

void OutFloat(char* label, float floatToFormat)
{
    char formatedFloat[10] = "";
    char finalString[100] = "";
    snprintf(formatedFloat, sizeof(formatedFloat), "%.2f", floatToFormat);
    strcpy(finalString, label);
    strcat(finalString, formatedFloat);

    DrawOut(finalString);
}

void OutVector2(char* label, Vector2 vec)
{
    char formated[10] = "";
    char finalString[100] = "";

    strcpy(finalString, label);

    strcat(finalString, "{ ");

    snprintf(formated, sizeof(formated), "%.2f", vec.x);
    strcat(finalString, formated);

    strcat(finalString, " , ");

    snprintf(formated, sizeof(formated), "%.2f", vec.y);
    strcat(finalString, formated);

    strcat(finalString, " }");

    DrawOut(finalString);
}

void OutVector3(char* label, Vector3 vec)
{
    char formated[10] = "";
    char finalString[100] = "";

    strcpy(finalString, label);

    strcat(finalString, "{ ");

    snprintf(formated, sizeof(formated), "%.2f", vec.x);
    strcat(finalString, formated);

    strcat(finalString, " , ");

    snprintf(formated, sizeof(formated), "%.2f", vec.y);
    strcat(finalString, formated);

    strcat(finalString, " , ");

    snprintf(formated, sizeof(formated), "%.2f", vec.z);
    strcat(finalString, formated);

    strcat(finalString, " }");

    DrawOut(finalString);
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
    textPosX = 0;
    DebugMsg* msg;
    for(int i = 0; i < debugMsgCount; i++){
        msg = MessagesToDebug[i];
        switch (msg->dataType) {
            case TYPE_FLOAT:
                // Cast to float* and dereference
                OutFloat(msg->label, *(float*)msg->dataPtr);
            break;
            case TYPE_INT:
                // Cast to float* and dereference
                OutFloat(msg->label, *(int*)msg->dataPtr);
            break;
            case TYPE_VECTOR2:
                OutVector2(msg->label, *(Vector2*)msg->dataPtr);
            break;
            case TYPE_VECTOR3:
                // Cast to float* and dereference
                OutVector3(msg->label, *(Vector3*)msg->dataPtr);
            break;
            default:
                printf("[WARNING] NOT IMPLEMENTED DEBUG TYPE\n");
        }
        textPosX += DEBUG_FONT_SIZE;
    }
}
