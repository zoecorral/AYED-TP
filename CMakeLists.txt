cmake_minimum_required(VERSION 3.20)
project(HubFlow CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_SOURCE_DIR})

# Archivos de implementación compartidos por el programa y los tests.
set(HUBFLOW_SOURCES
    ArbolBST.cpp
    ArbolBPlus.cpp
)

# Programa principal.
add_executable(HubFlow main.cpp ${HUBFLOW_SOURCES})

# Tests existentes.
add_executable(test_bst test/test_bst.cpp ${HUBFLOW_SOURCES})
add_executable(test_listas test/test_listas.cpp ${HUBFLOW_SOURCES})
add_executable(test_mesaDeClasificacion test/test_mesaDeClasificacion.cpp ${HUBFLOW_SOURCES})
add_executable(test_arbolBST test/test_arbolBST.cpp ${HUBFLOW_SOURCES})

# Tests nuevos de la ampliación: B+, BitSet, Set e integración.
add_executable(test_consultas_avanzadas test/test_consultas_avanzadas.cpp ${HUBFLOW_SOURCES})

enable_testing()
add_test(NAME BST COMMAND test_bst)
add_test(NAME Listas COMMAND test_listas)
add_test(NAME MesaDeClasificacion COMMAND test_mesaDeClasificacion)
add_test(NAME ArbolBST COMMAND test_arbolBST)
add_test(NAME ConsultasAvanzadas COMMAND test_consultas_avanzadas)
