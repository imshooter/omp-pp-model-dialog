#include <PawnPlus>
#include <YSI_Coding\y_hooks>

enum {
    MODEL_DIALOG_RESPONSE_SELECT,
    MODEL_DIALOG_RESPONSE_CANCEL
};

enum _:E_MODEL_DIALOG_DATA {
    E_MODEL_DIALOG_RESPONSE,
    E_MODEL_DIALOG_LISTITEM,
    E_MODEL_DIALOG_MODEL_ID
};

const static
    MODEL_DIALOG_ROWS = 2,
    MODEL_DIALOG_COLUMNS = 6,
    MODEL_DIALOG_PAGE_ITEMS = (MODEL_DIALOG_ROWS * MODEL_DIALOG_COLUMNS)
;

static enum E_TEXT_DRAW_DATA {
    Text:E_TEXT_DRAW_BACKGROUND[9],
    Text:E_TEXT_DRAW_BOX_BUTTON[MODEL_DIALOG_PAGE_ITEMS],
    Text:E_TEXT_DRAW_BOX_TOP[MODEL_DIALOG_PAGE_ITEMS],
    Text:E_TEXT_DRAW_MODEL_PREVIEW[MODEL_DIALOG_PAGE_ITEMS],
    Text:E_TEXT_DRAW_UP_BUTTON,
    Text:E_TEXT_DRAW_DOWN_BUTTON,
    Text:E_TEXT_DRAW_SELECT_BUTTON,
    Text:E_TEXT_DRAW_CANCEL_BUTTON
};

static enum E_PLAYER_TEXT_DRAW_DATA {
    PlayerText:E_PLAYER_TEXT_DRAW_SCROLL_BAR,
    PlayerText:E_PLAYER_TEXT_DRAW_CAPTION,
    PlayerText:E_PLAYER_TEXT_DRAW_PAGE,
    PlayerText:E_PLAYER_TEXT_DRAW_FIRST_BUTTON,
    PlayerText:E_PLAYER_TEXT_DRAW_SECOND_BUTTON
};

static enum _:E_DIALOG_PREVIEW_DATA {
    E_DIALOG_PREVIEW_MODEL_ID,
    Float:E_DIALOG_PREVIEW_SIZE_X,
    Float:E_DIALOG_PREVIEW_SIZE_Y,
    Float:E_DIALOG_PREVIEW_ROTATION_X,
    Float:E_DIALOG_PREVIEW_ROTATION_Y,
    Float:E_DIALOG_PREVIEW_ROTATION_Z,
    Float:E_DIALOG_PREVIEW_ZOOM,
    Float:E_DIALOG_PREVIEW_OFFSET_X,
    Float:E_DIALOG_PREVIEW_OFFSET_Y
};

static enum E_PLAYER_MENU_DATA {
    List:E_PLAYER_MENU_LIST,
    Task:E_PLAYER_MENU_TASK,
    E_PLAYER_MENU_PAGE,
    E_PLAYER_MENU_SIZE,
    E_PLAYER_MENU_ITEM_CHOSEN
};

static
    gTextDrawData[E_TEXT_DRAW_DATA],
    gPlayerTextDrawData[MAX_PLAYERS][E_PLAYER_TEXT_DRAW_DATA],
    gPlayerMenuData[MAX_PLAYERS][E_PLAYER_MENU_DATA],
    gTextDrawBoxListItem[MAX_TEXT_DRAWS] = { -1, ... }
;

forward AddModelDialogMenuItem(List:list, modelid, Float:sizeX = 53.000, Float:sizeY = 53.000, Float:rotationX = 0.000, Float:rotationY = 0.000, Float:rotationZ = 0.000, Float:zoom = 1.000, Float:offsetX = 0.000, Float:offsetY = 0.000);
forward bool:ShowModelDialogMenu(playerid, List:list, const caption[], const button1[], const button2[], start = 0);
forward Task:ShowModelDialogMenuAsync(playerid, List:list, const caption[], const button1[], const button2[], start = 0);
forward bool:HideModelDialogMenu(playerid);

/**
 * # Internal
 */

static ShowModelDialogMenuTextDraws(playerid, const caption[], const button1[], const button2[]) {
    for (new i; i != 9; ++i) {
        TextDrawShowForPlayer(playerid, gTextDrawData[E_TEXT_DRAW_BACKGROUND][i]);
    }

    TextDrawShowForPlayer(playerid, gTextDrawData[E_TEXT_DRAW_UP_BUTTON]);
    TextDrawShowForPlayer(playerid, gTextDrawData[E_TEXT_DRAW_DOWN_BUTTON]);
    
    TextDrawShowForPlayer(playerid, gTextDrawData[E_TEXT_DRAW_SELECT_BUTTON]);
    TextDrawShowForPlayer(playerid, gTextDrawData[E_TEXT_DRAW_CANCEL_BUTTON]);

    new
        PlayerText:pt
    ;

    pt = gPlayerTextDrawData[playerid][E_PLAYER_TEXT_DRAW_SCROLL_BAR] = CreatePlayerTextDraw(playerid, 0.0, 0.0, "LD_DUAL:WHITE");
    PlayerTextDrawColour(playerid, pt, 0xFF0000FF);
    PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT_SPRITE_DRAW);

    pt = gPlayerTextDrawData[playerid][E_PLAYER_TEXT_DRAW_CAPTION] = CreatePlayerTextDraw(playerid, 142.000, 132.000, caption);
    PlayerTextDrawLetterSize(playerid, pt, 0.180, 1.000);
    PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawSetShadow(playerid, pt, 0);
    PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT_2);
    PlayerTextDrawSetProportional(playerid, pt, true);
    PlayerTextDrawShow(playerid, pt);

    pt = gPlayerTextDrawData[playerid][E_PLAYER_TEXT_DRAW_PAGE] = CreatePlayerTextDraw(playerid, 498.500, 132.000, "-/-");
    PlayerTextDrawLetterSize(playerid, pt, 0.180, 1.000);
    PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN_RIGHT);
    PlayerTextDrawSetShadow(playerid, pt, 0);
    PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT_2);
    PlayerTextDrawSetProportional(playerid, pt, true);
    PlayerTextDrawShow(playerid, pt);

    pt = gPlayerTextDrawData[playerid][E_PLAYER_TEXT_DRAW_FIRST_BUTTON] = CreatePlayerTextDraw(playerid, 291.500, 295.000, button1);
    PlayerTextDrawLetterSize(playerid, pt, 0.180, 1.000);
    PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawSetShadow(playerid, pt, 0);
    PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT_2);
    PlayerTextDrawSetProportional(playerid, pt, true);
    PlayerTextDrawShow(playerid, pt);

    pt = gPlayerTextDrawData[playerid][E_PLAYER_TEXT_DRAW_SECOND_BUTTON] = CreatePlayerTextDraw(playerid, 347.500, 295.000, button2);
    PlayerTextDrawLetterSize(playerid, pt, 0.180, 1.000);
    PlayerTextDrawAlignment(playerid, pt, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawSetShadow(playerid, pt, 0);
    PlayerTextDrawFont(playerid, pt, TEXT_DRAW_FONT_2);
    PlayerTextDrawSetProportional(playerid, pt, true);
    PlayerTextDrawShow(playerid, pt);
}

static UpdateModelDialogMenuScrollBar(playerid, page, totalItems, totalPages) {
    new const
        Float:h = 112.000 * (float(MODEL_DIALOG_PAGE_ITEMS) / float(totalItems)),
        Float:y = 161.500 + (112.000 - h) * (float(page) / float(totalPages - 1))
    ;

    PlayerTextDrawSetString(playerid, gPlayerTextDrawData[playerid][E_PLAYER_TEXT_DRAW_PAGE], "%i/%i", page + 1, totalPages);
    PlayerTextDrawSetPos(playerid, gPlayerTextDrawData[playerid][E_PLAYER_TEXT_DRAW_SCROLL_BAR], 485.000, y);
    PlayerTextDrawTextSize(playerid, gPlayerTextDrawData[playerid][E_PLAYER_TEXT_DRAW_SCROLL_BAR], 8.000, h);
    PlayerTextDrawShow(playerid, gPlayerTextDrawData[playerid][E_PLAYER_TEXT_DRAW_SCROLL_BAR]);
}

static ShowModelDialogMenuPage(playerid, page) {
    new
        Iter:iterator = list_iter(gPlayerMenuData[playerid][E_PLAYER_MENU_LIST], page * MODEL_DIALOG_PAGE_ITEMS),
        listItem,
        data[E_DIALOG_PREVIEW_DATA]
    ;

    for (; iter_inside(iterator) && listItem != MODEL_DIALOG_PAGE_ITEMS; iter_move_next(iterator), ++listItem) {
        iter_get_arr(iterator, data);

        TextDrawSetPos(gTextDrawData[E_TEXT_DRAW_MODEL_PREVIEW][listItem], 
            148.000 + listItem % MODEL_DIALOG_COLUMNS * 56.000 + data[E_DIALOG_PREVIEW_OFFSET_X],
            158.000 + listItem / MODEL_DIALOG_COLUMNS * 66.000 + data[E_DIALOG_PREVIEW_OFFSET_Y]
        );
        TextDrawTextSize(gTextDrawData[E_TEXT_DRAW_MODEL_PREVIEW][listItem],
            data[E_DIALOG_PREVIEW_SIZE_X],
            data[E_DIALOG_PREVIEW_SIZE_Y]
        );
        TextDrawSetPreviewRot(gTextDrawData[E_TEXT_DRAW_MODEL_PREVIEW][listItem],
            data[E_DIALOG_PREVIEW_ROTATION_X],
            data[E_DIALOG_PREVIEW_ROTATION_Y],
            data[E_DIALOG_PREVIEW_ROTATION_Z],
            data[E_DIALOG_PREVIEW_ZOOM]
        );
        TextDrawSetPreviewModel(gTextDrawData[E_TEXT_DRAW_MODEL_PREVIEW][listItem], data[E_DIALOG_PREVIEW_MODEL_ID]);
        TextDrawColour(gTextDrawData[E_TEXT_DRAW_BOX_BUTTON][listItem], 0x424242FF);

        TextDrawShowForPlayer(playerid, gTextDrawData[E_TEXT_DRAW_BOX_BUTTON][listItem]);
        TextDrawShowForPlayer(playerid, gTextDrawData[E_TEXT_DRAW_BOX_TOP][listItem]);
        TextDrawShowForPlayer(playerid, gTextDrawData[E_TEXT_DRAW_MODEL_PREVIEW][listItem]);
    }

    for (; listItem != MODEL_DIALOG_PAGE_ITEMS; ++listItem) {
        TextDrawHideForPlayer(playerid, gTextDrawData[E_TEXT_DRAW_BOX_BUTTON][listItem]);
        TextDrawHideForPlayer(playerid, gTextDrawData[E_TEXT_DRAW_BOX_TOP][listItem]);
        TextDrawHideForPlayer(playerid, gTextDrawData[E_TEXT_DRAW_MODEL_PREVIEW][listItem]);
    }

    new const
        totalItems = gPlayerMenuData[playerid][E_PLAYER_MENU_SIZE],
        totalPages = (totalItems + MODEL_DIALOG_PAGE_ITEMS - 1) / MODEL_DIALOG_PAGE_ITEMS
    ;

    UpdateModelDialogMenuScrollBar(playerid, page, totalItems, totalPages);
    gPlayerMenuData[playerid][E_PLAYER_MENU_PAGE] = page;
    gPlayerMenuData[playerid][E_PLAYER_MENU_ITEM_CHOSEN] = -1;
}

static SelectModelDialogMenuItem(playerid, listItem) {
    if (gPlayerMenuData[playerid][E_PLAYER_MENU_ITEM_CHOSEN] >= 0) {
        new const
            lastItem = gPlayerMenuData[playerid][E_PLAYER_MENU_ITEM_CHOSEN]
        ;

        TextDrawColour(gTextDrawData[E_TEXT_DRAW_BOX_BUTTON][lastItem], 0x424242FF);
        TextDrawShowForPlayer(playerid, gTextDrawData[E_TEXT_DRAW_BOX_BUTTON][lastItem]);
    }

    TextDrawColour(gTextDrawData[E_TEXT_DRAW_BOX_BUTTON][listItem], 0xFF0000FF);
    TextDrawShowForPlayer(playerid, gTextDrawData[E_TEXT_DRAW_BOX_BUTTON][listItem]);
    gPlayerMenuData[playerid][E_PLAYER_MENU_ITEM_CHOSEN] = listItem;
}

static HandleModelDialogMenuSelection(playerid, response, listItem) {
    new
        data[E_MODEL_DIALOG_DATA]
    ;

    data[E_MODEL_DIALOG_RESPONSE] = response;
    data[E_MODEL_DIALOG_LISTITEM] = listItem;
    data[E_MODEL_DIALOG_MODEL_ID] = listItem >= 0 ? list_get(gPlayerMenuData[playerid][E_PLAYER_MENU_LIST], listItem) : -1;

    new const
        Task:t = gPlayerMenuData[playerid][E_PLAYER_MENU_TASK]
    ;

    HideModelDialogMenu(playerid);

    gPlayerMenuData[playerid][E_PLAYER_MENU_TASK] = INVALID_TASK;
    task_set_result_arr(t, data);
}

static ResetModelDialogMenuData(playerid) {
    gPlayerTextDrawData[playerid][E_PLAYER_TEXT_DRAW_SCROLL_BAR] = INVALID_PLAYER_TEXT_DRAW;
    gPlayerTextDrawData[playerid][E_PLAYER_TEXT_DRAW_CAPTION] = INVALID_PLAYER_TEXT_DRAW;
    gPlayerTextDrawData[playerid][E_PLAYER_TEXT_DRAW_PAGE] = INVALID_PLAYER_TEXT_DRAW;
    gPlayerTextDrawData[playerid][E_PLAYER_TEXT_DRAW_FIRST_BUTTON] = INVALID_PLAYER_TEXT_DRAW;
    gPlayerTextDrawData[playerid][E_PLAYER_TEXT_DRAW_SECOND_BUTTON] = INVALID_PLAYER_TEXT_DRAW;

    if (gPlayerMenuData[playerid][E_PLAYER_MENU_LIST]) {
        list_delete(gPlayerMenuData[playerid][E_PLAYER_MENU_LIST]);
    }

    gPlayerMenuData[playerid][E_PLAYER_MENU_LIST] = INVALID_LIST;
    gPlayerMenuData[playerid][E_PLAYER_MENU_PAGE] = 0;
    gPlayerMenuData[playerid][E_PLAYER_MENU_SIZE] = 0;
    gPlayerMenuData[playerid][E_PLAYER_MENU_ITEM_CHOSEN] = -1;
}

static ResetModelDialogMenuDataAndTask(playerid) {
    if (gPlayerMenuData[playerid][E_PLAYER_MENU_TASK]) {
        task_delete(gPlayerMenuData[playerid][E_PLAYER_MENU_TASK]);
    }

    gPlayerMenuData[playerid][E_PLAYER_MENU_TASK] = INVALID_TASK;
    ResetModelDialogMenuData(playerid);
}

/**
 * # External
 */

stock AddModelDialogMenuItem(List:list, modelid, Float:sizeX = 53.000, Float:sizeY = 53.000, Float:rotationX = 0.000, Float:rotationY = 0.000, Float:rotationZ = 0.000, Float:zoom = 1.000, Float:offsetX = 0.000, Float:offsetY = 0.000) {
    new
        data[E_DIALOG_PREVIEW_DATA]
    ;

    data[E_DIALOG_PREVIEW_MODEL_ID] = modelid;
    data[E_DIALOG_PREVIEW_SIZE_X] = sizeX;
    data[E_DIALOG_PREVIEW_SIZE_Y] = sizeY;
    data[E_DIALOG_PREVIEW_ROTATION_X] = rotationX;
    data[E_DIALOG_PREVIEW_ROTATION_Y] = rotationY;
    data[E_DIALOG_PREVIEW_ROTATION_Z] = rotationZ;
    data[E_DIALOG_PREVIEW_ZOOM] = zoom;
    data[E_DIALOG_PREVIEW_OFFSET_X] = offsetX;
    data[E_DIALOG_PREVIEW_OFFSET_Y] = offsetY;

    list_add_arr(list, data);
}

stock bool:ShowModelDialogMenu(playerid, List:list, const caption[], const button1[], const button2[], start = 0) {
    if (!IsPlayerConnected(playerid)) {
        return false;
    }

    if (gPlayerMenuData[playerid][E_PLAYER_MENU_LIST]) {
        return false;
    }

    gPlayerMenuData[playerid][E_PLAYER_MENU_LIST] = list;
    gPlayerMenuData[playerid][E_PLAYER_MENU_SIZE] = list_size(list);
    gPlayerMenuData[playerid][E_PLAYER_MENU_ITEM_CHOSEN] = -1;

    ShowModelDialogMenuTextDraws(playerid, caption, button1, button2);
    ShowModelDialogMenuPage(playerid, start);
    SelectTextDraw(playerid, 0xFF0000FF);

    return true;
}

stock Task:ShowModelDialogMenuAsync(playerid, List:list, const caption[], const button1[], const button2[], start = 0) {
    if (!IsPlayerConnected(playerid)) {
        return INVALID_TASK;
    }

    if (gPlayerMenuData[playerid][E_PLAYER_MENU_TASK]) {
        task_delete(gPlayerMenuData[playerid][E_PLAYER_MENU_TASK]);
    }

    new const
        Task:t = task_new()
    ;

    gPlayerMenuData[playerid][E_PLAYER_MENU_TASK] = t;
    ShowModelDialogMenu(playerid, list, caption, button1, button2, start);

    return t;
}

stock bool:HideModelDialogMenu(playerid) {
    if (!IsPlayerConnected(playerid)) {
        return false;
    }

    if (!gPlayerMenuData[playerid][E_PLAYER_MENU_LIST]) {
        return false;
    }

    for (new i; i != 9; ++i) {
        TextDrawHideForPlayer(playerid, gTextDrawData[E_TEXT_DRAW_BACKGROUND][i]);
    }

    for (new i; i != MODEL_DIALOG_PAGE_ITEMS; ++i) {
        TextDrawHideForPlayer(playerid, gTextDrawData[E_TEXT_DRAW_BOX_BUTTON][i]);
        TextDrawHideForPlayer(playerid, gTextDrawData[E_TEXT_DRAW_BOX_TOP][i]);
        TextDrawHideForPlayer(playerid, gTextDrawData[E_TEXT_DRAW_MODEL_PREVIEW][i]);
    }

    TextDrawHideForPlayer(playerid, gTextDrawData[E_TEXT_DRAW_UP_BUTTON]);
    TextDrawHideForPlayer(playerid, gTextDrawData[E_TEXT_DRAW_DOWN_BUTTON]);

    TextDrawHideForPlayer(playerid, gTextDrawData[E_TEXT_DRAW_SELECT_BUTTON]);
    TextDrawHideForPlayer(playerid, gTextDrawData[E_TEXT_DRAW_CANCEL_BUTTON]);

    PlayerTextDrawDestroy(playerid, gPlayerTextDrawData[playerid][E_PLAYER_TEXT_DRAW_SCROLL_BAR]);
    PlayerTextDrawDestroy(playerid, gPlayerTextDrawData[playerid][E_PLAYER_TEXT_DRAW_CAPTION]);
    PlayerTextDrawDestroy(playerid, gPlayerTextDrawData[playerid][E_PLAYER_TEXT_DRAW_PAGE]);
    PlayerTextDrawDestroy(playerid, gPlayerTextDrawData[playerid][E_PLAYER_TEXT_DRAW_FIRST_BUTTON]);
    PlayerTextDrawDestroy(playerid, gPlayerTextDrawData[playerid][E_PLAYER_TEXT_DRAW_SECOND_BUTTON]);

    ResetModelDialogMenuData(playerid);
    CancelSelectTextDraw(playerid);

    return true;
}

/**
 * # Calls
 */

hook OnGameModeInit() {
    new
        Text:t
    ;

    t = gTextDrawData[E_TEXT_DRAW_BACKGROUND][0] = TextDrawCreate(139.000, 131.000, "LD_DUAL:BLACK");
    TextDrawTextSize(t, 361.500, 12.000);
    TextDrawFont(t, TEXT_DRAW_FONT_SPRITE_DRAW);

    t = gTextDrawData[E_TEXT_DRAW_BACKGROUND][1] = TextDrawCreate(139.000, 143.000, "LD_DUAL:BLACK");
    TextDrawTextSize(t, 361.500, 172.500);
    TextDrawColour(t, 200);
    TextDrawFont(t, TEXT_DRAW_FONT_SPRITE_DRAW);

    t = gTextDrawData[E_TEXT_DRAW_BACKGROUND][2] = TextDrawCreate(145.000, 150.000, "LD_DUAL:WHITE");
    TextDrawTextSize(t, 349.500, 135.000);
    TextDrawFont(t, TEXT_DRAW_FONT_SPRITE_DRAW);

    t = gTextDrawData[E_TEXT_DRAW_BACKGROUND][3] = TextDrawCreate(145.500, 150.500, "LD_DUAL:BLACK");
    TextDrawTextSize(t, 338.000, 134.000);
    TextDrawFont(t, TEXT_DRAW_FONT_SPRITE_DRAW);

    t = gTextDrawData[E_TEXT_DRAW_BACKGROUND][4] = TextDrawCreate(484.000, 150.500, "LD_DUAL:BLACK");
    TextDrawTextSize(t, 10.000, 134.000);
    TextDrawFont(t, TEXT_DRAW_FONT_SPRITE_DRAW);

    // SCROLL BAR
    t = gTextDrawData[E_TEXT_DRAW_BACKGROUND][5] = TextDrawCreate(482.000, 148.000, "LD_BEAT:CHIT");
    TextDrawTextSize(t, 14.000, 14.000);
    TextDrawFont(t, TEXT_DRAW_FONT_SPRITE_DRAW);

    t = gTextDrawData[E_TEXT_DRAW_BACKGROUND][6] = TextDrawCreate(482.000, 273.000, "LD_BEAT:CHIT");
    TextDrawTextSize(t, 14.000, 14.000);
    TextDrawFont(t, TEXT_DRAW_FONT_SPRITE_DRAW);

    t = gTextDrawData[E_TEXT_DRAW_UP_BUTTON] = TextDrawCreate(485.000, 151.000, "LD_BEAT:UP");
    TextDrawTextSize(t, 8.000, 8.000);
    TextDrawColour(t, 255);
    TextDrawFont(t, TEXT_DRAW_FONT_SPRITE_DRAW);
    TextDrawSetSelectable(t, true);

    t = gTextDrawData[E_TEXT_DRAW_DOWN_BUTTON] = TextDrawCreate(485.000, 276.500, "LD_BEAT:DOWN");
    TextDrawTextSize(t, 8.000, 8.000);
    TextDrawColour(t, 255);
    TextDrawFont(t, TEXT_DRAW_FONT_SPRITE_DRAW);
    TextDrawSetSelectable(t, true);

    // BUTTON
    t = gTextDrawData[E_TEXT_DRAW_SELECT_BUTTON] = TextDrawCreate(264.500, 292.500, "LD_DUAL:WHITE");
    TextDrawTextSize(t, 54.000, 15.000);
    TextDrawFont(t, TEXT_DRAW_FONT_SPRITE_DRAW);
    TextDrawSetSelectable(t, true);

    t = gTextDrawData[E_TEXT_DRAW_CANCEL_BUTTON] = TextDrawCreate(320.500, 292.500, "LD_DUAL:WHITE");
    TextDrawTextSize(t, 54.000, 15.000);
    TextDrawFont(t, TEXT_DRAW_FONT_SPRITE_DRAW);
    TextDrawSetSelectable(t, true);

    t = gTextDrawData[E_TEXT_DRAW_BACKGROUND][7] = TextDrawCreate(265.000, 293.000, "LD_DUAL:BLACK");
    TextDrawTextSize(t, 53.000, 14.000);
    TextDrawFont(t, TEXT_DRAW_FONT_SPRITE_DRAW);

    t = gTextDrawData[E_TEXT_DRAW_BACKGROUND][8] = TextDrawCreate(321.000, 293.000, "LD_DUAL:BLACK");
    TextDrawTextSize(t, 53.000, 14.000);
    TextDrawFont(t, TEXT_DRAW_FONT_SPRITE_DRAW);

    // BOX SELECTABLE
    for (new i; i != MODEL_DIALOG_PAGE_ITEMS; i++) {
        t = gTextDrawData[E_TEXT_DRAW_BOX_BUTTON][i] = TextDrawCreate(
            147.500 + i % MODEL_DIALOG_COLUMNS * 56.000,
            152.500 + i / MODEL_DIALOG_COLUMNS * 66.000, "LD_DUAL:WHITE"
        );
        TextDrawTextSize(t, 54.000, 64.000);
        TextDrawFont(t, TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetSelectable(t, true);
        gTextDrawBoxListItem[t] = i;

        t = gTextDrawData[E_TEXT_DRAW_BOX_TOP][i] = TextDrawCreate(
            148.000 + i % MODEL_DIALOG_COLUMNS * 56.000,
            153.000 + i / MODEL_DIALOG_COLUMNS * 66.000, "LD_DUAL:WHITE"
        );
        TextDrawTextSize(t, 53.000, 63.000);
        TextDrawColour(t, 0x323232FF);
        TextDrawFont(t, TEXT_DRAW_FONT_SPRITE_DRAW);

        t = gTextDrawData[E_TEXT_DRAW_MODEL_PREVIEW][i] = TextDrawCreate(0.000, 0.000, "_");
        TextDrawBackgroundColour(t, 0);
        TextDrawFont(t, TEXT_DRAW_FONT_MODEL_PREVIEW);
    }

    return 1;
}

hook OnPlayerConnect(playerid) {
    ResetModelDialogMenuDataAndTask(playerid);

    return 1;
}

hook OnPlayerDisconnect(playerid, reason) {
    ResetModelDialogMenuDataAndTask(playerid);

    return 1;
}

hook OnPlayerClickTextDraw(playerid, Text:clickedid) {
    if (gPlayerMenuData[playerid][E_PLAYER_MENU_LIST]) {
        if (clickedid != INVALID_TEXT_DRAW) {
            new const
                listItemClicked = gTextDrawBoxListItem[clickedid]
            ;

            if (listItemClicked >= 0) {
                SelectModelDialogMenuItem(playerid, listItemClicked);

                return 1;
            }
        }

        new const
            p = gPlayerMenuData[playerid][E_PLAYER_MENU_PAGE]
        ;

        if (clickedid == gTextDrawData[E_TEXT_DRAW_UP_BUTTON] && p > 0) {
            ShowModelDialogMenuPage(playerid, p - 1);

            return 1;
        }

        if (clickedid == gTextDrawData[E_TEXT_DRAW_DOWN_BUTTON] && p < (gPlayerMenuData[playerid][E_PLAYER_MENU_SIZE] + MODEL_DIALOG_PAGE_ITEMS - 1) / MODEL_DIALOG_PAGE_ITEMS - 1) {
            ShowModelDialogMenuPage(playerid, p + 1);

            return 1;
        }

        new const
            listItemChosen = gPlayerMenuData[playerid][E_PLAYER_MENU_ITEM_CHOSEN],
            listItem = listItemChosen + p * MODEL_DIALOG_PAGE_ITEMS
        ;

        if (clickedid == gTextDrawData[E_TEXT_DRAW_SELECT_BUTTON] && listItemChosen >= 0) {
            HandleModelDialogMenuSelection(playerid, MODEL_DIALOG_RESPONSE_SELECT, listItem);

            return 1;
        }

        if (clickedid == INVALID_TEXT_DRAW || clickedid == gTextDrawData[E_TEXT_DRAW_CANCEL_BUTTON]) {
            HandleModelDialogMenuSelection(playerid, MODEL_DIALOG_RESPONSE_CANCEL, (listItemChosen >= 0) ? listItem : -1);

            return 1;
        }
    }

    return 1;
}