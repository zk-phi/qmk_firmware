MOUSEKEY_ENABLE = no
TAP_DANCE_ENABLE = yes
RGBLIGHT_ENABLE = yes

ifndef QUANTUM_DIR
	include ../../../../Makefile
endif

SRC += analog.c
