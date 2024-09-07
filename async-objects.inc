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

enum _:e_OBJECT_INFO
{
    bool:E_OBJECT_PLAYEROBJECT,
    _:E_OBJECT_OBJECTID,
    EDIT_RESPONSE:E_OBJECT_RESPONSE,
    Float:E_OBJECT_X,
    Float:E_OBJECT_Y,
    Float:E_OBJECT_Z,
    Float:E_OBJECT_RX,
    Float:E_OBJECT_RY,
    Float:E_OBJECT_RZ
};

enum _:e_ATTACHED_OBJECT_INFO
{
    EDIT_RESPONSE:E_ATTACHED_OBJECT_RESPONSE,
    E_ATTACHED_OBJECT_INDEX,
    E_ATTACHED_OBJECT_MODELID,
    E_ATTACHED_OBJECT_BONEID,
    Float:E_ATTACHED_OBJECT_X,
    Float:E_ATTACHED_OBJECT_Y,
    Float:E_ATTACHED_OBJECT_Z,
    Float:E_ATTACHED_OBJECT_RX,
    Float:E_ATTACHED_OBJECT_RY,
    Float:E_ATTACHED_OBJECT_RZ,
    Float:E_ATTACHED_OBJECT_SCALEX,
    Float:E_ATTACHED_OBJECT_SCALEY,
    Float:E_ATTACHED_OBJECT_SCALEZ
};

enum _:I_MOVE_OBJECT_INFO{
    Task:I_MOVE_OBJECT_TASK,
    I_MOVE_OBJECT_ID,
    I_MOVE_OBJECT_CUSTOM_ID,
    Float:I_MOVE_OBJECT_X,
    Float:I_MOVE_OBJECT_Y,
    Float:I_MOVE_OBJECT_Z,
    Float:I_MOVE_OBJECT_RX,
    Float:I_MOVE_OBJECT_RY,
    Float:I_MOVE_OBJECT_RZ
};

enum _:E_MOVE_OBJECT_INFO{
    _:E_MOVE_OBJECT_OBJECTID,
    Float:E_MOVE_OBJECT_FROM_X,
    Float:E_MOVE_OBJECT_FROM_Y,
    Float:E_MOVE_OBJECT_FROM_Z,
    Float:E_MOVE_OBJECT_FROM_RX,
    Float:E_MOVE_OBJECT_FROM_RY,
    Float:E_MOVE_OBJECT_FROM_RZ,
    Float:E_MOVE_OBJECT_MOVED_X,
    Float:E_MOVE_OBJECT_MOVED_Y,
    Float:E_MOVE_OBJECT_MOVED_Z,
    Float:E_MOVE_OBJECT_MOVED_RX,
    Float:E_MOVE_OBJECT_MOVED_RY,
    Float:E_MOVE_OBJECT_MOVED_RZ,
    E_MOVE_OBJECT_CUSTOM_ID
};

static 
    CallbackHandler: AsyncObject_Handler,
    CallbackHandler: AsyncDynamicObject_Handler,
    CallbackHandler: AsyncAttachedObject_Handler,
    CallbackHandler: AsyncMoveObject_Handler
;

static 
    Task:AsyncObject_PlayerTask[MAX_PLAYERS] = {Task:0, ...},
    MoveObjectAsyncData[MAX_OBJECTS][I_MOVE_OBJECT_INFO] = {{Task:0, INVALID_OBJECT_ID, -1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, ...}
;

forward _pp@on_init@PPAsync_Objects();
public _pp@on_init@PPAsync_Objects() {
    wait_ticks(1);

    AsyncObject_Handler = pawn_register_callback("OnPlayerEditObject", "AsyncObjectHandler"); 
    AsyncDynamicObject_Handler = pawn_register_callback("OnPlayerEditDynamicObject", "AsyncDynamicObjectHandler"); 
    AsyncAttachedObject_Handler = pawn_register_callback("OnPlayerEditAttachedObject", "AsyncAttachedObjectHandler"); 
    AsyncMoveObject_Handler = pawn_register_callback("OnObjectMoved", "AsyncOnObjectMovedHandler"); 
}

forward _pp@on_exit@PPAsync_Objects();
public _pp@on_exit@PPAsync_Objects() {
    pawn_unregister_callback(AsyncObject_Handler);
    pawn_unregister_callback(AsyncDynamicObject_Handler);
    pawn_unregister_callback(AsyncAttachedObject_Handler);
    pawn_unregister_callback(AsyncMoveObject_Handler);
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

stock Task:MoveObjectAsync(objectid, Float:targetX, Float:targetY, Float:targetZ, Float:speed, Float:rotationX = -1000.0, Float:rotationY = -1000.0, Float:rotationZ = -1000.0, custom_id = -1){
    MoveObjectAsyncData[objectid][I_MOVE_OBJECT_ID] = objectid;
    MoveObjectAsyncData[objectid][I_MOVE_OBJECT_CUSTOM_ID] = custom_id;
    GetObjectPos(objectid, MoveObjectAsyncData[objectid][I_MOVE_OBJECT_X], MoveObjectAsyncData[objectid][I_MOVE_OBJECT_Y], MoveObjectAsyncData[objectid][I_MOVE_OBJECT_Z]);
    GetObjectRot(objectid, MoveObjectAsyncData[objectid][I_MOVE_OBJECT_RX], MoveObjectAsyncData[objectid][I_MOVE_OBJECT_RY], MoveObjectAsyncData[objectid][I_MOVE_OBJECT_RZ]);

    pawn_call_native("MoveObject", "dfffffff", objectid, targetX, targetY, targetZ, speed, rotationX, rotationY, rotationZ);

    if(MoveObjectAsyncData[objectid][I_MOVE_OBJECT_TASK]) {
        task_delete(MoveObjectAsyncData[objectid][I_MOVE_OBJECT_TASK]);
    }
    return MoveObjectAsyncData[objectid][I_MOVE_OBJECT_TASK] = task_new();
}
stock Task:MoveDynamicObjectAsync(STREAMER_TAG_OBJECT:objectid, Float:x, Float:y, Float:z, Float:speed, Float:rx = -1000.0, Float:ry = -1000.0, Float:rz = -1000.0, custom_id = -1){
    new Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz;
    GetDynamicObjectPos(objectid, x, y, z);
    GetDynamicObjectRot(objectid, rx, ry, rz);

    Streamer_SetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_CUSTOM(1), x);
    Streamer_SetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_CUSTOM(2), y);
    Streamer_SetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_CUSTOM(3), z);
    Streamer_SetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_CUSTOM(4), rx);
    Streamer_SetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_CUSTOM(5), ry);
    Streamer_SetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_CUSTOM(6), rz);

    new 
        data[2];
    data[0] = objectid;
    data[1] = custom_id;

    Streamer_SetArrayData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_CUSTOM(7), data, 2);

    pawn_call_native("MoveObject", "dfffffff", objectid, x, y, z, speed, rx, ry, rz);
}

//Handlers
forward AsyncObjectHandler(playerid, playerobject, objectid, response, Float:fX, Float:fY, Float:fZ, Float:fRotX, Float:fRotY, Float:fRotZ );
public AsyncObjectHandler(playerid, playerobject, objectid, response, Float:fX, Float:fY, Float:fZ, Float:fRotX, Float:fRotY, Float:fRotZ ) {
    new 
        obj_response[e_OBJECT_INFO];

    obj_response[E_OBJECT_PLAYEROBJECT] = playerobject;
    obj_response[E_OBJECT_OBJECTID] = objectid;
    obj_response[E_OBJECT_RESPONSE] = response;
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

    obj_response[E_OBJECT_PLAYEROBJECT] = false;
    obj_response[E_OBJECT_OBJECTID] = _:objectid;
    obj_response[E_OBJECT_RESPONSE] = _:response;
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

    obj_response[E_ATTACHED_OBJECT_RESPONSE] = response;
    obj_response[E_ATTACHED_OBJECT_INDEX] = index;
    obj_response[E_ATTACHED_OBJECT_MODELID] = modelid;
    obj_response[E_ATTACHED_OBJECT_BONEID] = boneid;
    obj_response[E_ATTACHED_OBJECT_X] = fOffsetX;
    obj_response[E_ATTACHED_OBJECT_Y] = fOffsetY;
    obj_response[E_ATTACHED_OBJECT_Z] = fOffsetZ;
    obj_response[E_ATTACHED_OBJECT_RX] = rotationX;
    obj_response[E_ATTACHED_OBJECT_RY] = rotationY;
    obj_response[E_ATTACHED_OBJECT_RZ] = rotationZ;
    obj_response[E_ATTACHED_OBJECT_SCALEX] = scaleX;
    obj_response[E_ATTACHED_OBJECT_SCALEY] = scaleY;
    obj_response[E_ATTACHED_OBJECT_SCALEZ] = scaleZ;

    if (response >= 0) {
        new 
            const Task:task = AsyncObject_PlayerTask[playerid];

        AsyncObject_PlayerTask[playerid] = Task:0;
        task_set_result_arr(task, obj_response);
    }

    return 1;
}

forward AsyncOnObjectMovedHandler(objectid);
public AsyncOnObjectMovedHandler(objectid){
    if(MoveObjectAsyncData[objectid][I_MOVE_OBJECT_ID] == objectid){

        new 
            const Task:task = MoveObjectAsyncData[objectid][I_MOVE_OBJECT_TASK];

        MoveObjectAsyncData[objectid][I_MOVE_OBJECT_TASK] = Task:0;

        new move_response[E_MOVE_OBJECT_INFO];

        move_response[E_MOVE_OBJECT_OBJECTID] = objectid;
        move_response[E_MOVE_OBJECT_CUSTOM_ID] = MoveObjectAsyncData[objectid][I_MOVE_OBJECT_CUSTOM_ID];
        move_response[E_MOVE_OBJECT_FROM_X] = MoveObjectAsyncData[objectid][I_MOVE_OBJECT_X];
        move_response[E_MOVE_OBJECT_FROM_Y] = MoveObjectAsyncData[objectid][I_MOVE_OBJECT_Y];
        move_response[E_MOVE_OBJECT_FROM_Z] = MoveObjectAsyncData[objectid][I_MOVE_OBJECT_Z];
        move_response[E_MOVE_OBJECT_FROM_RX] = MoveObjectAsyncData[objectid][I_MOVE_OBJECT_RX];
        move_response[E_MOVE_OBJECT_FROM_RY] = MoveObjectAsyncData[objectid][I_MOVE_OBJECT_RY];
        move_response[E_MOVE_OBJECT_FROM_RZ] = MoveObjectAsyncData[objectid][I_MOVE_OBJECT_RZ];

        GetObjectPos(objectid, move_response[E_MOVE_OBJECT_MOVED_X], move_response[E_MOVE_OBJECT_MOVED_Y], move_response[E_MOVE_OBJECT_MOVED_Z]);
        GetObjectRot(objectid, move_response[E_MOVE_OBJECT_MOVED_RX], move_response[E_MOVE_OBJECT_MOVED_RY], move_response[E_MOVE_OBJECT_MOVED_RZ]);

        task_set_result_arr(task, move_response);
    }
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

// public OnObjectMoved(objectid)
// {

//     return 1;
// }