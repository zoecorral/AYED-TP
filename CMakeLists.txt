cmake_minimum_required(VERSION 3.20)
project(HubFlow CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR})

# Ejecutable principal
add_executable(HubFlow main.cpp ArbolBST.cpp)

# Test 1: BST
if(EXISTS "${CMAKE_SOURCE_DIR}/test/test_bst.cpp")
    add_executable(test_bst test/test_bst.cpp ArbolBST.cpp)
endif()

# Test 2: Listas
if(EXISTS "${CMAKE_SOURCE_DIR}/test/test_listas.cpp")
    add_executable(test_listas test/test_listas.cpp ArbolBST.cpp)
endif()

# Test 3: Mesa de Clasificación
if(EXISTS "${CMAKE_SOURCE_DIR}/test/test_mesaDeClasificacion.cpp")
    add_executable(test_mesaDeClasificacion test/test_mesaDeClasificacion.cpp ArbolBST.cpp)
endif()

# Test 4: Árbol BST (Corregido)
if(EXISTS "${CMAKE_SOURCE_DIR}/test/test_arbolBST.cpp")
    add_executable(test_arbolBST test/test_arbolBST.cpp ArbolBST.cpp)
endif()