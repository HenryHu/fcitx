/***************************************************************************
 *   Copyright (C) 2010~2010 by CSSlayer                                   *
 *   wengxt@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/**
 * @file   ime.h
 * @author Yuking yuking_net@sohu.com
 * @date   2008-1-16
 *
 * @brief  Public Header for Input Method Develop
 *
 */
#ifndef _FCITX_IME_H_
#define _FCITX_IME_H_

#include "fcitx-utils/utf8.h"

#define MAX_IM_NAME    (8 * 6)

#define MAX_CAND_LEN    127
#define MAX_TIPS_LEN    9

#define MAX_CAND_WORD    10
#define MAX_USER_INPUT    300

#define HOT_KEY_COUNT   2
#include "fcitx/fcitx.h"
#include "fcitx-config/hotkey.h"

struct FcitxInstance;
struct FcitxAddon;
typedef enum _SEARCH_MODE {
    SM_FIRST,
    SM_NEXT,
    SM_PREV
} SEARCH_MODE;

typedef enum _KEY_RELEASED {
    KR_OTHER = 0,
    KR_CTRL,
    KR_2ND_SELECTKEY,
    KR_3RD_SELECTKEY,
} KEY_RELEASED;

typedef enum _INPUT_RETURN_VALUE {
    //IRV_UNKNOWN = -1,
    IRV_DO_NOTHING = 0, /* do nothing */
    IRV_DONOT_PROCESS, /* key will be forward */
    IRV_DONOT_PROCESS_CLEAN, /* key will be forward and process as IRV_CLEAN */
    IRV_CLEAN, /* reset input */
    IRV_TO_PROCESS, /* key will passed to next flow*/ 
    IRV_DISPLAY_MESSAGE, /* it's a message, so next and prev will not be shown */
    IRV_DISPLAY_CANDWORDS, /* the only different with message it it will show next and prev button */
    IRV_DISPLAY_LAST, /* display the last input word */
    IRV_PUNC,
    IRV_ENG,
    IRV_GET_LEGEND, /* legend word */
    IRV_GET_CANDWORDS, /* send the input to client, close input window */
    IRV_GET_CANDWORDS_NEXT /* send the input to client, dont close input window */
} INPUT_RETURN_VALUE;

typedef struct _SINGLE_HZ {
    char            strHZ[UTF8_MAX_LENGTH + 1];
} SINGLE_HZ;

typedef struct FcitxIMClass {
    void*              (*Create) (struct FcitxInstance* instance);
    void               (*Destroy) (void *arg);
} FcitxIMClass;

typedef boolean            (*FcitxIMInit) (void *arg);
typedef void               (*FcitxIMResetIM) (void *arg);
typedef INPUT_RETURN_VALUE (*FcitxIMDoInput) (void *arg, FcitxKeySym, unsigned int);
typedef INPUT_RETURN_VALUE (*FcitxIMGetCandWords) (void *arg, SEARCH_MODE);
typedef char              *(*FcitxIMGetCandWord) (void *arg, int);
typedef char              *(*FcitxIMGetLegendCandWord) (void *arg, int);
typedef boolean            (*FcitxIMPhraseTips) (void *arg);
typedef void               (*FcitxIMSave) (void *arg);

typedef struct FcitxIM {
    char               strName[MAX_IM_NAME + 1];
    char               strIconName[MAX_IM_NAME + 1];
    FcitxIMResetIM ResetIM;
    FcitxIMDoInput DoInput;
    FcitxIMGetCandWords GetCandWords;
    FcitxIMGetCandWord GetCandWord;
    FcitxIMGetLegendCandWord GetLegendCandWord;
    FcitxIMPhraseTips PhraseTips;
    FcitxIMSave Save;
    FcitxIMInit Init;
    void*              uiprivate;
    void* klass;
} FcitxIM;

typedef enum FcitxKeyEventType {
    FCITX_PRESS_KEY,
    FCITX_RELEASE_KEY
} FcitxKeyEventType;

typedef struct FcitxInputState {
    long unsigned int lastKeyPressedTime;
    boolean bIsDoInputOnly;
    KEY_RELEASED keyReleased;
    int iCodeInputCount;
    char strCodeInput[MAX_USER_INPUT + 1];
    char strStringGet[MAX_USER_INPUT + 1];  //保存输入法返回的需要送到客户程序中的字串
    boolean bIsInLegend;
    
    int iCandPageCount;
    int iCandWordCount;
    time_t timeStart;
    boolean bStartRecordType;
    int iCursorPos;
    boolean bCursorAuto;
    int iCurrentCandPage;
    int iLegendCandWordCount;
    int iLegendCandPageCount;
    int iCurrentLegendCandPage;
    int bShowNext;
    int bShowPrev;
    int iHZInputed;
    int lastIsSingleHZ;
    int iInCap;
    boolean bLastIsNumber;
} FcitxInputState;

boolean IsHotKey(FcitxKeySym sym, int state, HOTKEYS * hotkey);
boolean IsInLegend(FcitxInputState* input);
char* GetOutputString(FcitxInputState* input);
struct FcitxIM* GetCurrentIM(struct FcitxInstance *instance);

void FcitxRegsiterIM(struct FcitxInstance *instance,
                     void *addonInstance,
                     const char* name,
                     const char* iconName,
                     FcitxIMInit Init,
                     FcitxIMResetIM ResetIM, 
                     FcitxIMDoInput DoInput, 
                     FcitxIMGetCandWords GetCandWords, 
                     FcitxIMGetCandWord GetCandWord, 
                     FcitxIMGetLegendCandWord GetLegendCandWord, 
                     FcitxIMPhraseTips PhraseTips, 
                     FcitxIMSave Save                     
);
#endif