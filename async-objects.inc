/*
	* Library: Samp Async Objects (async-objects.inc)
	* Author: Mergevos
	* Date: 30. Novembre 2020.
	* Copyright: (C) 2020
	* Credits: 
        IllidanS4 - PawnPlus
        Graber - Snippets from async-dialogs.inc. 
        BLU - Streamer objects and Player attached Objects 
*/

#if defined _inc_async_objects
    #undef _inc_async_objects
#endif

#if defined _Async_Objects_included_
    #endinput
#endif
#define _Async_Objects_included_

#include <open.mp>

#include <PawnPlus>

#define _ASYNC_OBJECT_VERSION "1.0.1"

/* 
 * Enum * 
 */

enum e_OBJECT_INFO
{
    bool:E_OBJECT_Playerobject,
    _:E_OBJECT_Objectid,
	EDIT_RESPONSE:E_OBJECT_Response,
    Float:E_OBJECT_X,
    Float:E_OBJECT_Y,
    Float:E_OBJECT_Z,
    Float:E_OBJECT_RX,
    Float:E_OBJECT_RY,
    Float:E_OBJECT_RZ
};

enum e_ATTACHED_OBJECT_INFO
{
    EDIT_RESPONSE:E_OBJECT_Response,
    E_OBJECT_Index,
    E_OBJECT_Modelid,
    E_OBJECT_Boneid,
    Float:E_OBJECT_X,
    Float:E_OBJECT_Y,
    Float:E_OBJECT_Z,
    Float:E_OBJECT_RX,
    Float:E_OBJECT_RY,
    Float:E_OBJECT_RZ,
    Float:E_OBJECT_ScaleX,
    Float:E_OBJECT_ScaleY,
    Float:E_OBJECT_ScaleZ
};

static 
    CallbackHandler: AsyncObject_Handler,
    CallbackHandler: AsyncDynamicObject_Handler
    CallbackHandler: AsyncAttachedObject_Handler
;
static Task:AsyncObject_PlayerTask[MAX_PLAYERS] = {Task:0, ...};

forward _pp@on_init@PPAsync_Objects();
public _pp@on_init@PPAsync_Objects() {
	wait_ticks(1);

	AsyncObject_Handler = pawn_register_callback("OnPlayerEditObject", "AsyncObjectHandler"); 
    AsyncDynamicObject_Handler = pawn_register_callback("OnPlayerEditDynamicObject", "AsyncDynamicObjectHandler"); 
    AsyncAttachedObject_Handler = pawn_register_callback("OnPlayerEditAttachedObject", "AsyncAttachedObjectHandler"); 
}

forward _pp@on_exit@PPAsync_Objects();
public _pp@on_exit@PPAsync_Objects() {
	pawn_unregister_callback(AsyncObject_Handler);
    pawn_unregister_callback(AsyncDynamicObject_Handler);
    pawn_unregister_callback(AsyncAttachedObject_Handler);
}

stock Task:EditObjectAsync(playerid, objectid) {
    pawn_call_native("EditObject", "dd", playerid, objectid);
    if(AsyncObject_PlayerTask[playerid]) {
        task_delete(AsyncObject_PlayerTask[playerid]);
    }
    return AsyncObject_PlayerTask[playerid] = task_new();
}

stock Task:EditDynamicObjectAsync(playerid, STREAMER_TAG_OBJECT:objectid) {
    pawn_call_native("EditDynamicObject", "dd", playerid, objectid);
    if(AsyncObject_PlayerTask[playerid]) {
        task_delete(AsyncObject_PlayerTask[playerid]);
    }
    return AsyncObject_PlayerTask[playerid] = task_new();
}

stock Task:EditAttachedObjectAsync(playerid, index) {
    pawn_call_native("EditAttachedObject", "dd", playerid, index);
    if(AsyncObject_PlayerTask[playerid]) {
        task_delete(AsyncObject_PlayerTask[playerid]);
    }
    return AsyncObject_PlayerTask[playerid] = task_new();
}

//Handlers
forward AsyncObjectHandler(playerid, playerobject, objectid, response, Float:fX, Float:fY, Float:fZ, Float:fRotX, Float:fRotY, Float:fRotZ );
public AsyncObjectHandler(playerid, playerobject, objectid, response, Float:fX, Float:fY, Float:fZ, Float:fRotX, Float:fRotY, Float:fRotZ ) {
    new 
        obj_response[e_OBJECT_INFO];

    obj_response[E_OBJECT_Playerobject] = playerobject;
    obj_response[E_OBJECT_Objectid] = objectid;
    obj_response[E_OBJECT_Response] = response;
    obj_response[E_OBJECT_X] = fX;
    obj_response[E_OBJECT_Y] = fY;
    obj_response[E_OBJECT_Z] = fZ;
    obj_response[E_OBJECT_RX] = fRotX;
    obj_response[E_OBJECT_RY] = fRotY;
    obj_response[E_OBJECT_RZ] = fRotZ;

    if (response == EDIT_RESPONSE_CANCEL || response == EDIT_RESPONSE_FINAL) {
        new 
            const Task:task = AsyncObject_PlayerTask[playerid];

        AsyncObject_PlayerTask[playerid] = Task:0;
        task_set_result_arr(task, obj_response);
    }

    return 1;
}

forward AsyncDynamicObjectHandler(playerid, STREAMER_TAG_OBJECT:objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz);
public AsyncDynamicObjectHandler(playerid, STREAMER_TAG_OBJECT:objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz) {
    new 
        obj_response[e_OBJECT_INFO];

    obj_response[E_OBJECT_Playerobject] = false;
    obj_response[E_OBJECT_Objectid] = _:objectid;
    obj_response[E_OBJECT_Response] = _:response;
    obj_response[E_OBJECT_X] = x;
    obj_response[E_OBJECT_Y] = y;
    obj_response[E_OBJECT_Z] = z;
    obj_response[E_OBJECT_RX] = rx;
    obj_response[E_OBJECT_RY] = ry;
    obj_response[E_OBJECT_RZ] = rz;

    if (response == EDIT_RESPONSE_CANCEL || response == EDIT_RESPONSE_FINAL) {
        new 
            const Task:task = AsyncObject_PlayerTask[playerid];

        AsyncObject_PlayerTask[playerid] = Task:0;
        task_set_result_arr(task, obj_response);
    }

    return 1;
}

forward AsyncAttachedObjectHandler(playerid, EDIT_RESPONSE:response, index, modelid, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:rotationX, Float:rotationY, Float:rotationZ, Float:scaleX, Float:scaleY, Float:scaleZ);
public AsyncAttachedObjectHandler(playerid, EDIT_RESPONSE:response, index, modelid, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:rotationX, Float:rotationY, Float:rotationZ, Float:scaleX, Float:scaleY, Float:scaleZ) {
    new 
        obj_response[e_ATTACHED_OBJECT_INFO];

    obj_response[E_OBJECT_Response] = response;
    obj_response[E_OBJECT_Index] = index;
    obj_response[E_OBJECT_Modelid] = modelid;
    obj_response[E_OBJECT_Boneid] = boneid;
    obj_response[E_OBJECT_X] = fOffsetX;
    obj_response[E_OBJECT_Y] = fOffsetY;
    obj_response[E_OBJECT_Z] = fOffsetZ;
    obj_response[E_OBJECT_RX] = rotationX;
    obj_response[E_OBJECT_RY] = rotationY;
    obj_response[E_OBJECT_RZ] = rotationZ;
    obj_response[E_OBJECT_ScaleX] = scaleX;
    obj_response[E_OBJECT_ScaleY] = scaleY;
    obj_response[E_OBJECT_ScaleZ] = scaleZ;

    if (response >= 0) {
        new 
            const Task:task = AsyncObject_PlayerTask[playerid];

        AsyncObject_PlayerTask[playerid] = Task:0;
        task_set_result_arr(task, obj_response);
    }

    return 1;
}

// public OnPlayerEditObject(playerid, playerobject, objectid, EDIT_RESPONSE:response, Float:fX, Float:fY, Float:fZ, Float:fRotX, Float:fRotY, Float:fRotZ)
// {
//     return 1;
// }

// public OnPlayerEditDynamicObject(playerid, STREAMER_TAG_OBJECT:objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz){
    
//     return 1;   
// }
// public OnPlayerEditAttachedObject(playerid, EDIT_RESPONSE:response, index, modelid, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fScaleX, Float:fScaleY, Float:fScaleZ){

//     return 1;
// }