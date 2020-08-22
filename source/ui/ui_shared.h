#pragma once
#include "../gfx_d3d/r_material.h"
#include "../gfx_d3d/r_utils.h"
#include "../universal/dvar.h"

struct animParamsDef_t
{
	const char* name;
	rectDef_s rectClient;
	float borderSize;
	vec4_t foreColor;
	vec4_t backColor;
	vec4_t borderColor;
	vec4_t outlineColor;
	float textScale;
	float rotation;
	GenericEventHandler* onEvent;
};

struct columnInfo_s
{
	int elementStyle;
	int maxChars;
	rectDef_s rect;
};

struct editFieldDef_s
{
	int cursorPos[1];
	float minVal;
	float maxVal;
	float defVal;
	float range;
	int maxChars;
	int maxCharsGotoNext;
	int maxPaintChars;
	int paintOffset;
};

enum expDataType
{
	VAL_INT = 0x0,
	VAL_FLOAT = 0x1,
	VAL_STRING = 0x2,
	VAL_ADDRESS = 0x3,
	VAL_DVAR = 0x4,
};

union expressionRpnDataUnion
{
	Operand constant;
	void* cmd;
	int cmdIdx;
};

struct expressionRpn
{
	int type;
	expressionRpnDataUnion data;
};

struct ExpressionStatement
{
	char* filename;
	int line;
	int numRpn;
	expressionRpn* rpn;
};

union focusDefData_t
{
	listBoxDef_s* listBox;
	multiDef_s* multi;
	profileMultiDef_s* profileMulti;
	editFieldDef_s* editField;
	enumDvarDef_s* enumDvar;
	void* data;
};

struct focusItemDef_s
{
	const char* mouseEnterText;
	const char* mouseExitText;
	const char* mouseEnter;
	const char* mouseExit;
	ItemKeyHandler* onKey;
	focusDefData_t focusTypeData;
};

struct gameMsgDef_s
{
	int gameMsgWindowIndex;
	int gameMsgWindowMode;
};

struct GenericEventHandler
{
	const char* name;
	GenericEventScript* eventScript;
	GenericEventHandler* next;
};

struct GenericEventScript
{
	ScriptCondition* prerequisites;
	ExpressionStatement condition;
	int type;
	bool fireOnTrue;
	const char* action;
	int blockID;
	int constructID;
	GenericEventScript* next;
};

struct imageDef_s
{
	ExpressionStatement materialExp;
};

struct itemDef_s
{
	windowDef_t window;
	int type;
	int dataType;
	int imageTrack;
	const char* dvar;
	const char* profileVar;
	const char* dvarTest;
	const char* enableDvar;
	int dvarFlags;
	itemDefData_t typeData;
	menuDef_t* parent;
	rectData_s* rectExpData;
	ExpressionStatement visibleExp;
	unsigned __int64 showBits;
	unsigned __int64 hideBits;
	ExpressionStatement forecolorAExp;
	int ui3dWindowId;
	GenericEventHandler* onEvent;
	UIAnimInfo* animInfo;
};

union itemDefData_t
{
	textDef_s* textDef;
	imageDef_s* imageDef;
	focusItemDef_s* blankButtonDef;
	ownerDrawDef_s* ownerDrawDef;
	void* data;
};

struct ItemKeyHandler
{
	int key;
	GenericEventScript* keyScript;
	ItemKeyHandler* next;
};

struct listBoxDef_s
{
	int mousePos;
	int cursorPos[1];
	int startPos[1];
	int endPos[1];
	int drawPadding;
	float elementWidth;
	float elementHeight;
	int numColumns;
	float special;
	columnInfo_s columnInfo[16];
	int notselectable;
	int noScrollBars;
	int usePaging;
	vec4_t selectBorder;
	vec4_t disableColor;
	vec4_t focusColor;
	vec4_t elementHighlightColor;
	vec4_t elementBackgroundColor;
	Material* selectIcon;
	Material* backgroundItemListbox;
	Material* highlightTexture;
	int noBlinkingHighlight;
	MenuRow* rows;
	int maxRows;
	int rowCount;
};

struct MenuCell
{
	int type;
	int maxChars;
	char* stringValue;
};

struct menuDef_t
{
	windowDef_t window;
	const char* font;
	int fullScreen;
	int ui3dWindowId;
	int itemCount;
	int fontIndex;
	int cursorItem[1];
	int fadeCycle;
	int priority;
	float fadeClamp;
	float fadeAmount;
	float fadeInAmount;
	float blurRadius;
	int openSlideSpeed;
	int closeSlideSpeed;
	int openSlideDirection;
	int closeSlideDirection;
	rectDef_s initialRectInfo;
	int openFadingTime;
	int closeFadingTime;
	int fadeTimeCounter;
	int slideTimeCounter;
	GenericEventHandler* onEvent;
	ItemKeyHandler* onKey;
	ExpressionStatement visibleExp;
	unsigned __int64 showBits;
	unsigned __int64 hideBits;
	const char* allowedBinding;
	const char* soundName;
	int imageTrack;
	int control;
	vec4_t focusColor;
	vec4_t disableColor;
	ExpressionStatement rectXExp;
	ExpressionStatement rectYExp;
	itemDef_s** items;
};

struct MenuRow
{
	MenuCell* cells;
	char* eventName;
	char* onFocusEventName;
	bool disableArg;
	int status;
	int name;
};

struct multiDef_s
{
	const char* dvarList[32];
	const char* dvarStr[32];
	float dvarValue[32];
	int count;
	int actionOnEnterPressOnly;
	int strDef;
};

struct Operand
{
	expDataType dataType;
	operandInternalDataUnion internals;
};

union operandInternalDataUnion
{
	int intVal;
	float floatVal;
	const char* string;
	const dvar_t* dvar;
};

struct ownerDrawDef_s
{
	ExpressionStatement dataExp;
};

struct profileMultiDef_s
{
	const char* profileVarList[32];
	const char* profileVarStr[32];
	float profileVarValue[32];
	int count;
	int actionOnEnterPressOnly;
	int strDef;
};

struct rectData_s
{
	ExpressionStatement rectXExp;
	ExpressionStatement rectYExp;
	ExpressionStatement rectWExp;
	ExpressionStatement rectHExp;
};

struct rectDef_s
{
	float x;
	float y;
	float w;
	float h;
	int horzAlign;
	int vertAlign;
};

struct ScriptCondition
{
	bool fireOnTrue;
	int constructID;
	int blockID;
	ScriptCondition* next;
};

struct textDef_s
{
	rectDef_s textRect[1];
	int alignment;
	int fontEnum;
	int itemFlags;
	int textAlignMode;
	float textalignx;
	float textaligny;
	float textscale;
	float textpadding;
	int textStyle;
	vec4_t fontColor;
	vec4_t glowColor;
	vec4_t shadowColor;
	float fontBlur;
	float glowSize;
	float shadowSize;
	vec2_t shadowOffset;
	const char* text;
	textExp_s* textExpData;
	textDefData_t textTypeData;
};

union textDefData_t
{
	focusItemDef_s* focusItemDef;
	gameMsgDef_s* gameMsgDef;
	void* data;
};

struct textExp_s
{
  ExpressionStatement textExp;
};

struct UIAnimInfo
{
	int animStateCount;
	animParamsDef_t** animStates;
	animParamsDef_t currentAnimState;
	animParamsDef_t nextAnimState;
	int animating;
	int animStartTime;
	int animDuration;
};

struct windowDef_t
{
	const char* name;
	rectDef_s rect;
	rectDef_s rectClient;
	const char* group;
	char style;
	char border;
	char modal;
	char frameSides;
	float frameTexSize;
	float frameSize;
	int ownerDraw;
	int ownerDrawFlags;
	float borderSize;
	int staticFlags;
	int dynamicFlags[1];
	int nextTime;
	vec4_t foreColor;
	vec4_t backColor;
	vec4_t borderColor;
	vec4_t outlineColor;
	float rotation;
	Material* background;
};

class ui_shared
{
};
