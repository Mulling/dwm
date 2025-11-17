/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx = 1;        /* border pixel of windows */
static const unsigned int gappx    = 6;        /* gap pixel between windows */
static const unsigned int snap     = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft  = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray       = 1;        /* 0 means no systray */
static const int showbar           = 1;        /* 0 means no bar */
static const int topbar            = 1;        /* 0 means bottom bar */
static const char *fonts[]         = {"Comic Code-11"};
static const char dmenufont[]      = "Comic Code-11";
static const char col_gray1[]      = "#1D2021";
static const char col_gray2[]      = "#32302F"; /* 25292A */
static const char col_gray3[]      = "#D6C9A4";
static const char col_gray4[]      = "#EBDBB2";
static const char col_cyan[]       = "#AF3A03";
#define SCHEME_SIZE 4
static const char *colors[][SCHEME_SIZE] = {
    /*               fg         bg         border   */
    [SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
    [SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
    [SchemeBar]  = { col_gray4, col_gray2, col_gray2 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    { "Gimp",     NULL,       NULL,       0,            1,           -1 },
    { "Peek",     NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55;  /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "=[]",      tile },    /* first entry is default */
    { "[~]",      NULL },    /* no layout function means floating behavior */
    { "[0]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, "-p", "run:", NULL };
static const char *termcmd[]  = { "/usr/local/bin/st", NULL };
static const char *vup[]      = { "volume", "+5", NULL };
static const char *vdown[]    = { "volume", "-5", NULL };
static const char *vmute[]    = { "/usr/bin/pactl", "set-sink-mute",   "@DEFAULT_SINK@", "toggle",  NULL };
static const char *lock[]     = { "/bin/bash", "-c", "cat /etc/issue | head -n 14 | xargs -0 -I {} physlock -m -p \"{}Locked $(uname -r)\"", NULL};
static const char *pdf[]      = { "pdf", NULL };

void set_layout(Arg const* arg ) {
    static int last_layout = 0;
    static int curr_layout = 0;

    if (curr_layout == arg->i) {
        curr_layout = last_layout;
        last_layout = arg->i;
    } else {
        last_layout = curr_layout;
        curr_layout = arg->i;
    }

    Arg layout = {.v = &layouts[curr_layout]};
    setlayout(&layout);
}


static const Key keys[] = {
    /* modifier                     key        function        argument */
    { MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
    { MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
    { MODKEY,                       XK_b,      togglebar,      {0} },
    { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
    { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
    { MODKEY,                       XK_minus,  incnmaster,     {.i = +1 } },
    { MODKEY,                       XK_equal,  incnmaster,     {.i = -1 } },
    { MODKEY|ShiftMask,             XK_h,      setmfact,       {.f = +0.05} },
    { MODKEY|ShiftMask,             XK_l,      setmfact,       {.f = -0.05} },
    { MODKEY,                       XK_Return, zoom,           {0} },
    { MODKEY,                       XK_Tab,    view,           {0} },
    { MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
    { MODKEY,                       XK_t,      set_layout,      {.i = 0} },
    { MODKEY,                       XK_f,      set_layout,      {.i = 1} },
    { MODKEY,                       XK_m,      set_layout,      {.i = 2} },
//  { MODKEY,                       XK_space,  setlayout,      {0} },
    { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
    { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
    { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
    { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
    TAGKEYS(                        XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_5,                      4)
    TAGKEYS(                        XK_6,                      5)
    TAGKEYS(                        XK_7,                      6)
    TAGKEYS(                        XK_8,                      7)
    TAGKEYS(                        XK_9,                      8)
    { MODKEY|ShiftMask,             XK_q,      quit,           {0} },
#include <X11/XF86keysym.h>
    { 0,         XF86XK_AudioLowerVolume,      spawn,          {.v = vdown } },
    { 0,                XF86XK_AudioMute,      spawn,          {.v = vmute } },
    { 0,         XF86XK_AudioRaiseVolume,      spawn,          {.v = vup } },
    { MODKEY,                       XK_Down,   spawn,          {.v = vdown } },
    { MODKEY,                       XK_Right,  spawn,          {.v = vmute } },
    { MODKEY,                       XK_Up,     spawn,          {.v = vup } },
    { MODKEY,                       XK_o,      spawn,          {.v = pdf } },
    { MODKEY|ShiftMask,             XK_f,      spawn,          {.v = lock } },
    { MODKEY,                       XK_space,  kbl,            {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

