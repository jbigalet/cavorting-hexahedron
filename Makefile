TARGET := WINDOWS

DIR := ${CURDIR}

MSVC_OPT_FLAGS := /Ox
MSVC_FLAGS := /I.\3rdp /DEBUG /Zi /std:c++14 -DOS_WINDOWS /FS $(MSVC_OPT_FLAGS)
MSVC_LINKFLAGS :=  opengl32.lib gdi32.lib user32.lib shell32.lib msvcrt.lib Shlwapi.lib .\3rdp\glfw3.lib d3d11.lib .\3rdp\D3DX\Lib\x64\d3dx11.lib

NO_OPT_OBJECTS := main.o
OPT_OBJECTS := io.o
OBJECTS := $(NO_OPT_OBJECTS) $(OPT_OBJECTS)

BUILD:=build/
SHLIB_SUFFIX:=.dll
EXE_SUFFIX:=.exe
RDYN :=
SHARED := /DLL build/main.lib
PYTHON := C:\Python27\python.exe
ifeq ("$(wildcard $(PYTHON))","")
    PYTHON := python
endif
CC := cl $(MSVC_FLAGS)
CC_OPT_FLAGS := $(MSVC_OPT_FLAGS)
LINKFLAGS := $(MSVC_LINKFLAGS)
EXE_OUT := /link /out:
MAKE_PCH_FLAGS := /Yc /Tp
PCH_OUT := /Fp
OBJ_OUT := /Fo
USE_PCH_FLAGS := -Ibuild -DSTD_PCH /Yu 
LINK_OBJECTS := $(patsubst %.o, build/%.o, $(OBJECTS)) build/std.o
CP := copy
RMDIR := rd /s /q
RM := del
MV := rename

$(shell $(PYTHON) gen-cpp-deps.py $(patsubst %.o,%.cpp,$(OBJECTS)))

$(shell mkdir build)
all: main$(EXE_SUFFIX)

include dependencies.make

clean:
	- $(RMDIR) build
	- $(RM) main$(EXE_SUFFIX)

autogen/math.h: math.smic.h
	$(PYTHON) smic_to_cpp.py

build/std.h.pch: std.h basics.cpp autogen/math.h
	$(CC) -c $(MAKE_PCH_FLAGS) std.h $(OBJ_OUT)build/std.o $(PCH_OUT)build/std.h.pch

main$(EXE_SUFFIX): $(patsubst %.o, build/%.o, $(OBJECTS))
	$(CC) $(RDYN) $(LINK_OBJECTS) $(LINKFLAGS) $(EXE_OUT)main$(EXE_SUFFIX)

$(patsubst %.o,build/%.o,$(NO_OPT_OBJECTS)) : build/%.o : build/std.h.pch
	$(CC) $(patsubst build/%.o,%.cpp,$@) $(USE_PCH_FLAGS) -c $(OBJ_OUT)$@
$(patsubst %.o,build/%.o,$(OPT_OBJECTS)) : build/%.o : build/std.h.pch
	$(CC) $(CC_OPT_FLAGS) $(patsubst build/%.o,%.cpp,$@) -c $(OBJ_OUT)$@

