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
	
	CXXFLAGS=-std=c++14 -m64 -Wall -Wno-deprecated-declarations -g -MMD
	
	WX_INCLUDES=-I"$(WXWIDGETS_HOME)/include" -I"$(WXWIDGETS_HOME)/lib/gcc_dll/mswud"
	WX_MAPLIB_LIBS=-lwxbase30ud -lwxmsw30ud_core
else
	OBJS_ROOT_DIR=obj/Release
	BIN_DIR=bin/Release
	
	MAPEDITOR_LDFLAGS=-m64 -s -mwindows
	MAPEDITOR_LIBS=-lwxbase30u -lwxmsw30u_core -lwxmsw30u_adv_gcc_custom -lsiaimap
	
	CXXFLAGS=-std=c++14 -m64 -Wall -Wno-deprecated-declarations -O2 -MMD
	
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
	
OBJS_SIAI_FILES:=$(patsubst $(SRC_SIAI_DIR)/%.cpp,$(OBJS_SIAI_DIR)/%.o,$(SRC_SIAI_FILES))
OBJS_EDITOR_FILES:=$(patsubst $(SRC_EDITOR_DIR)/%.cpp,$(OBJS_EDITOR_DIR)/%.o,$(SRC_EDITOR_FILES))
OBJS_PAINTER_FILES:=$(patsubst $(SRC_PAINTER_DIR)/%.cpp,$(OBJS_PAINTER_DIR)/%.o,$(SRC_PAINTER_FILES))
OBJS_UTIL_FILES:=$(patsubst $(SRC_UTIL_DIR)/%.cpp,$(OBJS_UTIL_DIR)/%.o,$(SRC_UTIL_FILES))
OBJS_MAP_FILES:=$(patsubst $(SRC_MAP_DIR)/%.cpp,$(OBJS_MAP_DIR)/%.o,$(SRC_MAP_FILES))

#########################Map Library########################
OBJS_MAP_LIBRARY=$(OBJS_SIAI_FILES) $(OBJS_MAP_FILES) $(OBJS_UTIL_FILES) $(OBJS_PAINTER_FILES)

$(TARGET_MAP_LIBRARY): $(OBJS_MAP_LIBRARY)
	g++  $(MAPLIB_LDFLAGS) $(LIB_DIRS) -o $@ $^ $(MAPLIB_LIBS)

###Map Directory
$(OBJS_MAP_DIR)/%.d: $(SRC_MAP_DIR)/%.cpp
	mkdir -p $(@D)
	g++ -MM $(CXXFLAGS) -DBUILD_DLL $(INCLUDE_DIRS) $@ $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : .g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
	
include $(OBJS_MAP_FILES:.o=.d)
	
$(OBJS_MAP_DIR)/%.o: $(SRC_MAP_DIR)/%.cpp
	g++ $(CXXFLAGS) -DBUILD_DLL $(INCLUDE_DIRS) -c -o $@ $(<:.d=.cpp)

###Util Directory
$(OBJS_UTIL_DIR)/%.d: $(SRC_UTIL_DIR)/%.cpp
	mkdir -p $(@D)
	g++ -MM $(CXXFLAGS) $(INCLUDE_DIRS) $@ $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : .g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
	
include $(OBJS_UTIL_FILES:.o=.d)
	
$(OBJS_UTIL_DIR)/%.o: $(SRC_UTIL_DIR)/%.cpp
	g++ $(CXXFLAGS) $(INCLUDE_DIRS) -c -o $@ $(<:.d=.cpp)

#########################Map Editor########################
OBJS_MAP_EDITOR=$(OBJS_SIAI_FILES) $(OBJS_EDITOR_FILES) $(OBJS_PAINTER_FILES)

$(TARGET_MAP_EDITOR): $(OBJS_MAP_EDITOR)
	g++ $(LIB_DIRS) -o $@ $^ $(MAPEDITOR_LD_FLAGS) $(MAPEDITOR_LIBS)

###SIAI Root Directory and other Subfolders that don't have custom rules
$(OBJS_SIAI_DIR)/%.d: $(SRC_SIAI_DIR)/%.cpp
	mkdir -p $(@D)
	g++ -MM $(CXXFLAGS) $(INCLUDE_DIRS) $@ $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : .g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
	
include $(OBJS_SIAI_FILES:.o=.d)
include $(OBJS_EDITOR_FILES:.o=.d)
include $(OBJS_PAINTER_FILES:.o=.d)
	
$(OBJS_SIAI_DIR)/%.o: $(SRC_SIAI_DIR)/%.cpp
	g++ $(CXXFLAGS) $(INCLUDE_DIRS) -c -o $@ $(<:.d=.cpp)
	
clean:
	rm -f $(TARGETS)
	rm -r $(OBJS_ROOT_DIR)
	rm -r $(MAPLIB_DEF_FILE)
	rm -r $(MAPLIB_A_FILE)