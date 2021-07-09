# ----------------------------
# Makefile Options
# ----------------------------

NAME ?= RISK
ICON ?= icon.png
DESCRIPTION ?= "Risk: World Domination"
COMPRESSED ?= NO
ARCHIVED ?= NO

CFLAGS ?= -Wall -Wextra -Oz
CXXFLAGS ?= -Wall -Wextra -Oz

# ----------------------------

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

include $(CEDEV)/meta/makefile.mk
