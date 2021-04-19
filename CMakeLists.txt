cmake_minimum_required (VERSION 2.6) 
project(3DSizeFieldFunc)
MESSAGE(STATUS "###################################")
MESSAGE("CMake config files are written by hongfei Ye")
MESSAGE("Config bug report: hfye@zju.edu.cn")
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
IF(APPLE)
MESSAGE(STATUS "TODO： mac doesnot support opemp yet")
ELSE()
FIND_PACKAGE (OpenMP REQUIRED)
ENDIF()
# 设置stack commit size.    (stack reserve size? How)
IF (WIN32)  
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /STACK:200000000")
ENDIF()
IF (CMAKE_SYSTEM_NAME MATCHES "Windows")
add_definitions(
	-DUNICODE
)
ENDIF () 

set(SOURCEFILE 
MY_EPS.h
OcNode.h
singleton.h
BKMesh.h
BKMesh.cpp
SizeOcTree.cpp
SizeOcTree.h
vertex.h
vertex.cpp
)
add_library(libsizeFunc STATIC 
${SOURCEFILE}
)
add_executable(sizeFunc
${SOURCEFILE}
main.cpp
)