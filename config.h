/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx	= 5;		/* border pixel of windows */
static const unsigned int gappx		= 5;		/* gap pixel between windows */
static const unsigned int snap		= 32;		/* snap pixel */
static const int rmaster			= 1;		/* 1 means master-area is initially on right */
static const int swallowfloating	= 0;		/* 1 means swallow floating windows by default */
static const int showbar			= 1;		/* 0 means no bar */
static const int topbar				= 1;		/* 0 means bottom bar */
static const int usealtbar			= 1;		/* 1 means use non-dwm status bar */
static const char *altbarclass		= "Polybar";	/* Alternate bar class name */
static const char *alttrayname		= "tray";		/* Polybar tray instance name */
static const char *altbarcmd		= "$HOME/scripts/bar.sh";	/* Alternate bar launch command */
static const char *fonts[]			= { "monospace:size=10" };
static const char dmenufont[]		= "monospace:size=10";
static const char col_gray1[]		= "#002b36";
static const char col_gray2[]		= "#073642";
static const char col_gray3[]		= "#93a1a1";
static const char col_gray4[]		= "#eee8d5";
static const char col_cyan[]		= "#2aa198";
static const char col_orange[]		= "#b58900";
static const char col_urgborder[]	= "#dc322f";
static const char *colors[][3]		= {
	/*               fg         bg         border   */
	[SchemeNorm] = {col_gray3,	col_gray1,	col_gray2},
	[SchemeSel]  = {col_gray4,	col_cyan,	col_orange},
	[SchemeUrg]  = {col_gray4,	col_cyan,	col_urgborder},
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"todoist", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"todoist",	spcmd1},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "-" };
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/*		class				instance    		title		tags	mask	isfloating	isterminal		noswallow		monitor*/
	{"Gimp",				NULL,		NULL,	0,			1,	-1,	0,	0},
	{"st-256color",		NULL,   	NULL,	1 << 0,	0,	1,		0,	-1},
	{"qutebrowser",		NULL,		NULL,	1 << 1,	0,	-1,	0,	-1},
	{"jetbrains-idea",	NULL,		NULL,	1 << 2,	0,	-1,	0,	-1},
	// {"Todoist",			NULL,		NULL,	1 << 3,	0,	-1,	0,	-1},
	{"Element",			NULL,		NULL,	1 << 4,	0,	1,		0,	0},
	{"jetbrains-clion",	NULL,		NULL,	1 << 5,	0,	-1,	0,	-1},
	{"joplin",			NULL,		NULL,	1 << 6,	0,	-1,	0,	-1},
	{"Slack",				NULL,		NULL,	1 << 7,	0,	-1,	0,	-1},
	{NULL,				"spterm",	NULL,	SPTAG(0),1,	-1},
	{NULL,				"spfm",		NULL,	SPTAG(1),1,	-1},
	{NULL,				"keepassxc",NULL,	SPTAG(2),0,	-1},
};

/* layout(s) */
static const float mfact		= 0.35;	/* factor of master area size [0.05..0 * .95] */
static const int nmaster		= 1;	/* number of clients in master area */
static const int resizehints	= 1;	/* 1 means respect size hints in tiled resizals */
static const int attachbelow	= 1;	/* 1 means attach after the currently active window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{"[T]",	tile},			/* first entry is default */
	{"[F]",	NULL},	/* no layout function means floating behavior */
	{"[M]",	monocle},
	{"[D]",	deck},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{MODKEY,						KEY,	comboview,		{.ui = 1 << TAG}}, \
	{MODKEY|ControlMask,			KEY,	toggleview,		{.ui = 1 << TAG}}, \
	{MODKEY|ShiftMask,				KEY,	combotag,		{.ui = 1 << TAG}}, \
	{MODKEY|ControlMask|ShiftMask,	KEY,	toggletag,		{.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

#include "movestack.c"
static Key keys[] = {
        /* modifier							key		function		argument */
        {MODKEY,				47,		swapfocus,		{.i = -1}},		// s
		{MODKEY|ShiftMask,		47,		swapmaster,		{.i = -1}},		// shift+s
		{MODKEY,				57,		togglebar,		{0}},		// b
		{MODKEY,				54,		focusstack,		{.i = +1}},		// j
		{MODKEY,				55,		focusstack,		{.i = -1}},		// k
		{MODKEY|ShiftMask,		54,		movestack,		{.i = +1}},		// shift+j
		{MODKEY|ShiftMask,		55,		movestack,		{.i = -1}},		// shift+k
		{MODKEY|ShiftMask,		42,		incnmaster,		{.i = +1}},		// shift+i
		{MODKEY|ShiftMask,		43,		incnmaster,		{.i = -1}},		// shift+d
		{MODKEY,				44,		setmfact,		{.f = -0.05}},	// h
		{MODKEY,				33,		setmfact,		{.f = +0.05}},	// l
        {MODKEY|ShiftMask,		44,		setcfact,		{.f = -0.25}},	// shift+h
        {MODKEY|ShiftMask,		33,		setcfact,		{.f = +0.25}},	// shift+l
        {MODKEY|ShiftMask,		39,		setcfact,		{.f =  0.00}},	// shift+o
        {MODKEY|ShiftMask,		53,		killclient,		{0}},		// shift+q
        {MODKEY,				58,		focusmaster,	{0}},		// m
        {MODKEY|ShiftMask,		58,		zoom,			{0}},		// shift+m
		{MODKEY|ControlMask,	45,		setlayout,		{.v = &layouts[0]}},	// ctrl+t
		{MODKEY|ControlMask,	43,		setlayout,		{.v = &layouts[3]}},	// ctrl+d
        {MODKEY|ShiftMask,		65,		togglefloating,	{0}},		// shift+space
        {MODKEY,				16,		comboview,		{.ui = ~0}},	// 0
        {MODKEY|ShiftMask,		16,		combotag,		{.ui = ~0 }},	// shift+0
        {MODKEY,				25,		focusmon,		{.i = -1}},		// comma
        {MODKEY,				26,		focusmon,		{.i = +1}},		// period
        {MODKEY|ShiftMask,		25,		tagmon,			{.i = -1}},		// shift+comma
        {MODKEY|ShiftMask,		26,		tagmon,			{.i = +1}},		// shift+period
        {MODKEY,				29,		fullscreen,		{0}},		// f
        {MODKEY,				48,		togglescratch,	{0}},		// -
		{MODKEY|ShiftMask,		40,		quit,			{0}},		// e
		TAGKEYS(                14,                    0)                 // 1
        TAGKEYS(                17,                    1)                 // 2
        TAGKEYS(                13,                    2)                 // 3
        TAGKEYS(                18,                    3)                 // 4
        TAGKEYS(                12,                    4)                 // 5
        TAGKEYS(                19,                    5)                 // 6
        TAGKEYS(                11,                    6)                 // 7
        TAGKEYS(                20,                    7)                 // 8
        TAGKEYS(                15,                    8)                 // 9
		TAGKEYS(                48,                    9)                 // -
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/*	click                	event mask      button		function		argument */
	{ClkLtSymbol,			0,		Button1,	setlayout,		{0}},
	{ClkLtSymbol,			0,		Button3,	setlayout,		{.v = &layouts[2]}},
	{ClkWinTitle,			0,		Button2,	zoom,			{0}},
	{ClkStatusText,		0,		Button2,	spawn,			{.v = termcmd}},
	{ClkClientWin,			MODKEY,	Button1,	movemouse,		{0}},
	{ClkClientWin,			MODKEY,	Button2,	togglefloating,	{0}},
	{ClkClientWin,			MODKEY,	Button3,	resizemouse,	{0}},
	{ClkTagBar,			0,		Button1,	comboview,		{0}},
	{ClkTagBar,			0,		Button3,	toggleview,		{0}},
	{ClkTagBar,			MODKEY,	Button1,	combotag,		{0}},
	{ClkTagBar,			MODKEY,	Button3,	toggletag,		{0}},
};

static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
  IPCCOMMAND(comboview,		1,	{ARG_TYPE_UINT}),
  IPCCOMMAND(toggleview,	1,	{ARG_TYPE_UINT}),
  IPCCOMMAND(combotag,		1,	{ARG_TYPE_UINT}),
  IPCCOMMAND(toggletag,		1,	{ARG_TYPE_UINT}),
  IPCCOMMAND(tagmon,		1,	{ARG_TYPE_UINT}),
  IPCCOMMAND(focusmon,		1,	{ARG_TYPE_SINT}),
  IPCCOMMAND(focusstack,	1,	{ARG_TYPE_SINT}),
  IPCCOMMAND(zoom,			1,	{ARG_TYPE_NONE}),
  IPCCOMMAND(incnmaster,	1,	{ARG_TYPE_SINT}),
  IPCCOMMAND(killclient,	1,	{ARG_TYPE_SINT}),
  IPCCOMMAND(togglefloating,1,	{ARG_TYPE_NONE}),
  IPCCOMMAND(setmfact,		1,	{ARG_TYPE_FLOAT}),
  IPCCOMMAND(setlayoutsafe,	1,	{ARG_TYPE_PTR}),
  IPCCOMMAND(quit,			1,	{ARG_TYPE_NONE})
};

