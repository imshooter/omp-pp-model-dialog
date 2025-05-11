# Installation

```pwn
#include <pp-model-dialog>
```

# Menu Skins
```pwn
void:ShowSkinsModelDialog(playerid) {
    yield 1;

    new
        List:list = list_new(),
        response[E_MODEL_DIALOG_DATA]
    ;

    for (new i; i <= 311; i++) {
        AddModelDialogMenuItem(list, i);
    }

    await_arr(response) ShowModelDialogMenuAsync(playerid, list, "SKINS", "SELECT", "CANCEL");

    if (response[E_MODEL_DIALOG_RESPONSE] == MODEL_DIALOG_RESPONSE_SELECT) {
        SetPlayerSkin(playerid, response[E_MODEL_DIALOG_MODEL_ID]);
    }
}

public OnPlayerCommandText(playerid, cmdtext[]) {
    if (strequal(cmdtext, "/skins")) {
        ShowSkinsModelDialog(playerid);

        return 1;
    }

    return 0;
}
```
![Skins](https://github.com/user-attachments/assets/ac655772-0aac-4d53-b708-4b5d5d46eef2)

# Menu Items
```pwn
void:ShowItemsModelDialog(playerid) {
    yield 1;

    new
        List:list = list_new(),
        response[E_MODEL_DIALOG_DATA]
    ;

    new const
        models[] = {2891, 1271, 19054, 19055, 19056, 19057, 19058, 355, 356}
    ;

    new const Float:arr[][] = {
        {53.0, 53.0, -20.0, 00.0, -45.0, 1.0, 01.0, 0.0},
        {53.0, 53.0, -20.0, 00.0, 045.0, 1.0, 00.0, 0.0},
        {53.0, 53.0, -20.0, 00.0, 045.0, 1.0, 00.0, 0.0},
        {53.0, 53.0, -20.0, 00.0, 045.0, 1.0, 00.0, 0.0},
        {53.0, 53.0, -20.0, 00.0, 045.0, 1.0, 00.0, 0.0},
        {53.0, 53.0, -20.0, 00.0, 045.0, 1.0, 00.0, 0.0},
        {53.0, 53.0, -20.0, 00.0, 045.0, 1.0, 00.0, 0.0},
        {53.0, 53.0, 000.0, 35.0, 180.0, 2.0, -5.0, 6.0},
        {53.0, 53.0, 000.0, 35.0, 180.0, 2.0, -5.0, 6.0}
    };

    for (new i, size = sizeof (models); i < size; i++) {
        AddModelDialogMenuItem(list, models[i],
            .sizeX = arr[i][0],
            .sizeY = arr[i][1],
            .rotationX = arr[i][2],
            .rotationY = arr[i][3],
            .rotationZ = arr[i][4],
            .zoom = arr[i][5],
            .offsetX = arr[i][6],
            .offsetY = arr[i][7]
        );
    }

    await_arr(response) ShowModelDialogMenuAsync(playerid, list, "ITEMS", "SELECT", "CANCEL");

    // ...
}

public OnPlayerCommandText(playerid, cmdtext[]) {
    if (strequal(cmdtext, "/items")) {
        ShowItemsModelDialog(playerid);

        return 1;
    }

    return 0;
}
```
![Items](https://github.com/user-attachments/assets/4036a789-b86e-46fd-865e-541c0cf57a34)
