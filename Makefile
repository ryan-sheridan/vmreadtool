TARGET := iphone:clang:latest:7.0

include $(THEOS)/makefiles/common.mk

TOOL_NAME = vmreadtool

vmreadtool_FILES = main.c
vmreadtool_CFLAGS = -fobjc-arc
vmreadtool_CODESIGN_FLAGS = -Sentitlements.plist
vmreadtool_INSTALL_PATH = /usr/local/bin

include $(THEOS_MAKE_PATH)/tool.mk
