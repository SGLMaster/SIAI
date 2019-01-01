WXWIDGETS_HOME=C:/wxWidgets

SRC_SIAI_DIR=source/siai
SRC_EDITOR_DIR=$(SRC_SIAI_DIR)/editor
SRC_PAINTER_DIR=$(SRC_SIAI_DIR)/painter
SRC_UTIL_DIR=$(SRC_SIAI_DIR)/util
SRC_MAP_DIR=$(SRC_SIAI_DIR)/map

SRC_SIAI_FILES:=$(wildcard $(SRC_SIAI_DIR)/*.cpp)
SRC_EDITOR_FILES:=$(wildcard $(SRC_EDITOR_DIR)/*.cpp) $(wildcard $(SRC_EDITOR_DIR)/forms/*.cpp)
SRC_PAINTER_FILES:=$(wildcard $(SRC_PAINTER_DIR)/*.cpp)
SRC_UTIL_FILES:=$(wildcard $(SRC_UTIL_DIR)/*.cpp)
SRC_MAP_FILES:=$(wildcard $(SRC_MAP_DIR)/*.cpp) $(wildcard $(SRC_MAP_DIR)/cmd/*.cpp) $(wildcard $(SRC_MAP_DIR)/entities/*.cpp)

DEBUG ?= 1
ifeq ($(DEBUG), 1)
	OBJS_ROOT_DIR=obj/Debug
	BIN_DIR=bin/Debug
	
	MAPEDITOR_LDFLAGS=-m64 -mwindows
	MAPEDITOR_LIBS=-lwxbase30ud -lwxmsw30ud_core -lwxmsw30ud_adv_gcc_custom -lsiaimap
	
	CXXFLAGS=-std=c++14 -m64 -Wall -Wno-deprecated-declarations -g
	
	WX_INCLUDES=-I"$(WXWIDGETS_HOME)/include" -I"$(WXWIDGETS_HOME)/lib/gcc_dll/mswud"
	WX_MAPLIB_LIBS=-lwxbase30ud -lwxmsw30ud_core
else
	OBJS_ROOT_DIR=obj/Release
	BIN_DIR=bin/Release
	
	MAPEDITOR_LDFLAGS=-m64 -s -mwindows
	MAPEDITOR_LIBS=-lwxbase30u -lwxmsw30u_core -lwxmsw30u_adv_gcc_custom -lsiaimap
	
	CXXFLAGS=-std=c++14 -m64 -Wall -Wno-deprecated-declarations -O2
	
	WX_INCLUDES=-I"$(WXWIDGETS_HOME)\include" -I"$(WXWIDGETS_HOME)\lib\gcc_dll\mswu"
	WX_MAPLIB_LIBS=-lwxbase30u -lwxmsw30u_core
endif

INCLUDE_DIRS=-I"include/siai" -Iforms $(WX_INCLUDES)
LIB_DIRS=-L"$(WXWIDGETS_HOME)/lib/gcc_dll" -L"$(BIN_DIR)"

MAPLIB_DEF_FILE=$(BIN_DIR)/libsiaimap.def
MAPLIB_A_FILE=$(BIN_DIR)/libsiaimap.a

MAPLIB_LDFLAGS=-shared -Wl,--output-def=$(MAPLIB_DEF_FILE) -Wl,--out-implib=$(MAPLIB_A_FILE) -Wl,--dll
MAPLIB_LIBS=$(WX_MAPLIB_LIBS) -luser32

OBJS_SIAI_DIR:=$(OBJS_ROOT_DIR)/$(SRC_SIAI_DIR)
OBJS_EDITOR_DIR:=$(OBJS_ROOT_DIR)/$(SRC_EDITOR_DIR)
OBJS_PAINTER_DIR:=$(OBJS_ROOT_DIR)/$(SRC_PAINTER_DIR)
OBJS_UTIL_DIR:=$(OBJS_ROOT_DIR)/$(SRC_UTIL_DIR)
OBJS_MAP_DIR:=$(OBJS_ROOT_DIR)/$(SRC_MAP_DIR)

TARGET_MAP_LIBRARY=$(BIN_DIR)/libsiaimap.dll
TARGET_MAP_EDITOR=$(BIN_DIR)/map-editor.exe

TARGETS=$(TARGET_MAP_LIBRARY) $(TARGET_MAP_EDITOR)

all: $(TARGETS)
	
#Building Map Editor
OBJS_SIAI_FILES:=$(patsubst $(SRC_SIAI_DIR)/%.cpp,$(OBJS_SIAI_DIR)/%.o,$(SRC_SIAI_FILES))
OBJS_EDITOR_FILES:=$(patsubst $(SRC_EDITOR_DIR)/%.cpp,$(OBJS_EDITOR_DIR)/%.o,$(SRC_EDITOR_FILES))
OBJS_PAINTER_FILES:=$(patsubst $(SRC_PAINTER_DIR)/%.cpp,$(OBJS_PAINTER_DIR)/%.o,$(SRC_PAINTER_FILES))
OBJS_UTIL_FILES:=$(patsubst $(SRC_UTIL_DIR)/%.cpp,$(OBJS_UTIL_DIR)/%.o,$(SRC_UTIL_FILES))
OBJS_MAP_FILES:=$(patsubst $(SRC_MAP_DIR)/%.cpp,$(OBJS_MAP_DIR)/%.o,$(SRC_MAP_FILES))


$(TARGET_MAP_LIBRARY): $(OBJS_SIAI_FILES) $(OBJS_MAP_FILES) $(OBJS_UTIL_FILES) $(OBJS_PAINTER_FILES)
	g++  $(MAPLIB_LDFLAGS) $(LIB_DIRS) -o $@ $^ $(MAPLIB_LIBS)
	
$(OBJS_MAP_DIR)/%.o: $(SRC_MAP_DIR)/%.cpp
	mkdir -p $(@D)
	g++ $(CXXFLAGS) -DBUILD_DLL $(INCLUDE_DIRS) -c -o $@ $^
	
$(OBJS_UTIL_DIR)/%.o: $(SRC_UTIL_DIR)/%.cpp
	mkdir -p $(@D)
	g++ $(CXXFLAGS) $(INCLUDE_DIRS) -c -o $@ $^

$(TARGET_MAP_EDITOR): $(OBJS_SIAI_FILES) $(OBJS_EDITOR_FILES) $(OBJS_FORMS_FILES) $(OBJS_PAINTER_FILES)
	g++ $(LIB_DIRS) -o $@ $^ $(MAPEDITOR_LD_FLAGS) $(MAPEDITOR_LIBS)
	
$(OBJS_SIAI_DIR)/%.o: $(SRC_SIAI_DIR)/%.cpp
	mkdir -p $(@D)
	g++ $(CXXFLAGS) $(INCLUDE_DIRS) -c -o $@ $^
	
clean:
	rm -f $(TARGETS)
	rm -r $(OBJS_ROOT_DIR)
	rm -r $(MAPLIB_DEF_FILE)
	rm -r $(MAPLIB_A_FILE)