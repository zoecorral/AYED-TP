#include "ArbolBST.h"
#include "Envio.h"
#include <iostream>

void test_BST() {
    std::cout << "=== PROBANDO MI ARBOL BST ===\n\n";

    ArbolBST arbol;

    // Crear envios de prueba 
    Envio* e1 = new Envio("PKG-1002", "Bruno Diaz", "NORTE", 0.75, NivelServicio::EXPRESS);
    Envio* e2 = new Envio("PKG-1001", "Ana Torres", "CENTRO", 1.20, NivelServicio::ESTANDAR);
    Envio* e3 = new Envio("PKG-1003", "Carla Ruiz", "SUR", 4.10, NivelServicio::PRIORITARIO);

    std::cout << "1. Insertando elementos...\n";
    arbol.insertar(e1);
    arbol.insertar(e2);
    arbol.insertar(e3);

    std::cout << "Cantidad guardada: " << arbol.getCantidad() << " (esperado: 3)\n";
    std::cout << "Altura del arbol: " << arbol.altura() << "\n\n";

    std::cout << "2. Recorrido In-Order:\n";
    arbol.recorridoInOrder();
    std::cout << "\n";

    std::cout << "3. Probando busqueda:\n";
    int visitados = 0;
    Envio* hallado = arbol.buscar("PKG-1001", visitados, true);

    if (hallado) {
        std::cout << "-> OK! Encontrado en " << visitados << " visitas.\n\n";
    } else {
        std::cout << "-> ERROR! No se encontro el codigo.\n\n";
    }

    delete e1; 
    delete e2; 
    delete e3;

    std::cout << "=== FIN DE PRUEBAS ===\n";
}
