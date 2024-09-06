# samp-async-objects

[![Mergevos](https://img.shields.io/badge/Mergevos-samp--async--objects-2f2f2f.svg?style=for-the-badge)](https://github.com/Mergevos/samp-async-objects)

## Installation

Simply install to your project:

```bash
sampctl package install Mergevos/samp-async-objects
```

Include in your code and begin using the library:

```pawn
#include <async-objects>
```

## Usage 

Let's look at the following example:

```c
CMD:test(playerid, params[])
{ 

    SelectObject(playerid);

    return 1;
}

public OnPlayerSelectObject(playerid, type, objectid, modelid, Float:fX, Float:fY, Float:fZ)
{
    new obj_res[e_OBJECT_INFO];
    await_arr(obj_res) EditObjectAsync(playerid, objectid);
    print("Response awaited");

    if(obj_res[E_OBJECT_Response] == EDIT_RESPONSE_FINAL) {
        print("Response final, player clicked on save icon.");
    } else if(obj_res[E_OBJECT_Response] == EDIT_RESPONSE_CANCEL) {
        print("Canceled");
    }

    return 1;
}
```

## Streamer Objects (Dynamic Object)

```c
CMD:test(playerid, params[]){
    new STREAMER_TAG_OBJECT:objectid = CreateDynamicObject(3632, 0.0, 0.0, 0.0);

    new obj_res[e_OBJECT_INFO];
    await_arr(obj_res) EditDynamicObjectAsync(playerid, objectid);

    // bool:E_OBJECT_Playerobject,
    // E_OBJECT_Objectid,
    // EDIT_RESPONSE:E_OBJECT_Response,
    // Float:E_OBJECT_X,
    // Float:E_OBJECT_Y,
    // Float:E_OBJECT_Z,
    // Float:E_OBJECT_RX,
    // Float:E_OBJECT_RY,
    // Float:E_OBJECT_RZ

    printf("EditDynamicObjectAsync objectid %d", obj_res[E_OBJECT_Objectid]);

    if(obj_res[E_OBJECT_Response] == EDIT_RESPONSE_FINAL){
        SetDynamicObjectPos(objectid, obj_res[E_OBJECT_X], obj_res[E_OBJECT_Y], obj_res[E_OBJECT_Z], obj_res[E_OBJECT_RX], obj_res[E_OBJECT_RY], obj_res[E_OBJECT_RZ]);
    }else if(obj_res[E_OBJECT_Response] == EDIT_RESPONSE_CANCEL){
        // Cancel 
    }
}
```

## Player Attached objects

```c
CMD:test(playerid, params[]){
    new obj_res[e_ATTACHED_OBJECT_INFO];
    await_arr(obj_res) EditAttachedObjectAsync(playerid, INDEX_HERE);

    // EDIT_RESPONSE:E_OBJECT_Response,
    // E_OBJECT_Index,
    // E_OBJECT_Modelid,
    // E_OBJECT_Boneid,
    // Float:E_OBJECT_X,
    // Float:E_OBJECT_Y,
    // Float:E_OBJECT_Z,
    // Float:E_OBJECT_RX,
    // Float:E_OBJECT_RY,
    // Float:E_OBJECT_RZ,
    // Float:E_OBJECT_ScaleX,
    // Float:E_OBJECT_ScaleY,
    // Float:E_OBJECT_ScaleZ

    printf("EditAttachedObjectAsync modelid %d, boneid %d", obj_res[E_OBJECT_Modelid], obj_res[E_OBJECT_Boneid]);

    if(obj_res[E_OBJECT_Response] == EDIT_RESPONSE_FINAL){
        // save
    }else if(obj_res[E_OBJECT_Response] == EDIT_RESPONSE_CANCEL){
        // Cancel 
    }
}
```

```c
Task:EditObjectAsync(playerid, objectid) > *required callback: OnPlayerEditObject*
Task:EditDynamicObjectAsync(playerid, STREAMER_TAG_OBJECT:objectid)  > *required callback: OnPlayerEditDynamicObject*
Task:EditAttachedObjectAsync(playerid, index)  > *required callback: OnPlayerEditAttachedObject*
```