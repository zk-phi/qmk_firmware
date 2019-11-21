/* ---- tapping */

#ifndef NO_ACTION_TAPPING

/* in ergo42, TAPPING_TERM is defined in rev1/config.h */
#ifdef TAPPING_TERM
#    undef TAPPING_TERM
#endif

#define TAPPING_TERM 180

/* Prefer "hold" when "down -> up -> down" (recognized immediately as "tap twice" by default) */
#define TAPPING_FORCE_HOLD

/* Prefer "hold" when "Alt Dn -> A Dn -> A Up -> Alt Up" within the TAPPING_TERM */
#define PERMISSIVE_HOLD

#endif

/* ---- mousekey */

#ifdef CUSTOM_MOUSEKEY

#define MOUSEKEY_DELAY             48
#define MOUSEKEY_INTERVAL          24
#define MOUSEKEY_MOVE_DELTA        3
#define MOUSEKEY_MAX_SPEED         11 /* times faster than MOVE_DELTA */
#define MOUSEKEY_TIME_TO_MAX       10

#define MOUSEKEY_WHEEL_DELAY       72
#define MOUSEKEY_WHEEL_INTERVAL    72
#define MOUSEKEY_WHEEL_DELTA       1
#define MOUSEKEY_WHEEL_MAX_SPEED   1 /* times faster */
#define MOUSEKEY_WHEEL_TIME_TO_MAX 1

#define MOUSEKEY_ACL_MAX_SPEED 16
#define MOUSEKEY_ACL_WHEEL_MAX_SPEED 4

#endif
