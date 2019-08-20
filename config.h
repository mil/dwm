/* grid of tags */
#define DRAWCLASSICTAGS             1 << 0
#define DRAWTAGGRID                 1 << 1

#define SWITCHTAG_UP                1 << 0
#define SWITCHTAG_DOWN              1 << 1
#define SWITCHTAG_LEFT              1 << 2
#define SWITCHTAG_RIGHT             1 << 3
#define SWITCHTAG_TOGGLETAG         1 << 4
#define SWITCHTAG_TAG               1 << 5
#define SWITCHTAG_VIEW              1 << 6
#define SWITCHTAG_TOGGLEVIEW        1 << 7

static const unsigned int drawtagmask = DRAWTAGGRID; /* | DRAWCLASSICTAGS to show classic row of tags */
static const int tagrows = 2;
/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 1;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Terminus:size=10" };
static const char dmenufont[]       = "Terminus:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_lgray[]       = "#e8e8e8";
static const char col_cyan[]        = "#005577";
static const char col_red[]        = "#ff3300";
static const char col_blue[]        = /*"#002aff"; */ "#004292";
static const char col_white[]        = "#ffffff";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_lgray },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan},
	[SchemeHid]  = { col_red, col_cyan,  col_gray2 },
};
static const int focusonwheel       = 0;
static const unsigned int gappx     = 5;        /* gap pixel between windows */
 

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   isterminal nowallow monitor */
	{ "st",       NULL,       NULL,       0,            0,           1,         1,        -1 },
	{ "dmenu",       NULL,       NULL,       0,            1,           0,         1,        -1 },
	{ NULL,       NULL,       "dmenu",       0,            1,           0,         1,        -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

#include "layouts.c"
#include "unfloat.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[=]",      tile },    /* first entry is default */
	{ "[F]",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[D]",      deck },
	{ "[G]",      grid },
	{ "|M|",      centeredmaster },
	{  "☀",       centeredfloatingmaster }
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenul", NULL };
static const char *term[]  = { "stw", NULL };
static const char *termcwd[]  = { "stw_in_cwd", NULL };


static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = term } },
	{ MODKEY,                       XK_Return, spawn,           {.v = termcwd} },

	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },

	{ MODKEY|ShiftMask,             XK_j,      pushdown,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      pushup,    {.i = -1 } },

	{ MODKEY,                       XK_space,  zoom,         {0} },
	{ MODKEY,                       XK_semicolon, switchcol,    {0} },

	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_o,      incnmaster,     {.i = -1 } },

	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.01} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.01} },

	{ MODKEY,                       XK_Tab,    shiftview,      {.i = 1} },
	{ MODKEY|ShiftMask,             XK_Tab,    shiftview,      {.i = -1} },

	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },

	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_comma,  setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_comma,  setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_period, setlayout,      {.v = &layouts[5]} },
	{ MODKEY|ShiftMask,             XK_period, setlayout,      {.v = &layouts[6]} },
	{ MODKEY,                       XK_slash,  setlayout,      {.v = &layouts[3]} },

	{ MODKEY,                       XK_f,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,  unfloatvisible, {0} },

	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },


  // Todo: put on []
	//{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	//{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	//{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	//{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },


	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	//{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	//{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcwd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
