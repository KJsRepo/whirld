#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Perlin/PerlinNoise.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-lmingw32 -static-libgcc -static-libstdc++
CXXFLAGS=-lmingw32 -static-libgcc -static-libstdc++

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/C/Development/SDL2-2.0.8/i686-w64-mingw32/include/SDL2 /C/Development/SDL2-2.0.8/i686-w64-mingw32/lib/libSDL2main.a /C/Development/SDL2-2.0.8/i686-w64-mingw32/lib/libSDL2.dll.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/whirld.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/whirld.exe: /C/Development/SDL2-2.0.8/i686-w64-mingw32/lib/libSDL2main.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/whirld.exe: /C/Development/SDL2-2.0.8/i686-w64-mingw32/lib/libSDL2.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/whirld.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/whirld ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/Perlin/PerlinNoise.o: Perlin/PerlinNoise.cpp
	${MKDIR} -p ${OBJECTDIR}/Perlin
	${RM} "$@.d"
	$(COMPILE.cc) -g -I. -I/C/Development/SDL2-2.0.8/i686-w64-mingw32/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Perlin/PerlinNoise.o Perlin/PerlinNoise.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I. -I/C/Development/SDL2-2.0.8/i686-w64-mingw32/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
