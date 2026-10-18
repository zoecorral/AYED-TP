// Tests Modulo 3: Árbol Binario de Búsqueda (Caso 12)
// Basado en las pruebas de BST con metadatos de altura y nodos visitados.

#include "ArbolBST.h"
#include "CentroDeDistribucion.h"
#include "Envio.h"
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

static int totalChequeosM3 = 0;
static int chequeosFallidosM3 = 0;

static void chequearM3(bool condicion, const std::string& descripcion) {
    totalChequeosM3++;
    if (condicion) {
        std::cout << "  [OK]   " << descripcion << "\n";
    } else {
        chequeosFallidosM3++;
        std::cout << "  [FAIL] " << descripcion << "\n";
    }
}

static std::string capturarSalidaM3(const std::function<void()>& accion) {
    std::ostringstream buffer;
    std::streambuf* original = std::cout.rdbuf(buffer.rdbuf());
    std::ios formatoOriginal(nullptr);
    formatoOriginal.copyfmt(std::cout);

    accion();

    std::cout.copyfmt(formatoOriginal);
    std::cout.rdbuf(original);
    return buffer.str();
}

// ============================================================
// Caso 12 — Árbol Binario de Búsqueda (BST)
// ============================================================
void testCaso12_ArbolBST() {
    std::cout << "\nCaso 12 - Arbol Binario de Busqueda (BST)\n";
    
    ArbolBST arbol;

    // 1. Inserción de envíos
    Envio* e1 = new Envio("PKG-1002", "Bruno Diaz", "NORTE", 0.75, NivelServicio::EXPRESS);
    Envio* e2 = new Envio("PKG-1001", "Ana Torres", "CENTRO", 1.20, NivelServicio::ESTANDAR);
    Envio* e3 = new Envio("PKG-1003", "Carla Ruiz", "SUR", 4.10, NivelServicio::PRIORITARIO);

    arbol.insertar(e1);
    arbol.insertar(e2);
    arbol.insertar(e3);

    chequearM3(arbol.getCantidad() == 3, "El arbol reporta la cantidad exacta de elementos (3)");
    chequearM3(arbol.altura() > 0, "La altura del arbol es mayor a cero");

    // 2. Recorrido In-Order (debe salir ordenado alfabéticamente: PKG-1001 < PKG-1002 < PKG-1003)
    std::string salidaInOrder = capturarSalidaM3([&] { arbol.recorridoInOrder(); });
    std::size_t pos1001 = salidaInOrder.find("PKG-1001");
    std::size_t pos1002 = salidaInOrder.find("PKG-1002");
    std::size_t pos1003 = salidaInOrder.find("PKG-1003");

    chequearM3(pos1001 < pos1002 && pos1002 < pos1003,
               "El recorrido In-Order muestra los paquetes ordenados por codigo alfabeticamente");

    // 3. Búsqueda con contador de nodos visitados
    int visitados = 0;
    Envio* hallado = arbol.buscar("PKG-1001", visitados, true);

    chequearM3(hallado != nullptr && hallado->getCodigo() == "PKG-1001",
               "La busqueda localiza correctamente el paquete en el BST");
    chequearM3(visitados > 0,
               "Se registra la cantidad de visitas realizadas durante la busqueda");

    // Limpieza de memoria
    delete e1;
    delete e2;
    delete e3;
}

void ejecutarPruebasBST() {
    std::cout << "========== TESTS (MODULO 3: ARBOL BST) ==========\n";

    testCaso12_ArbolBST();

    std::cout << "\n====================================\n";
    std::cout << (totalChequeosM3 - chequeosFallidosM3) << "/" << totalChequeosM3 << " checks OK\n";
    if (chequeosFallidosM3 > 0) {
        std::cout << chequeosFallidosM3 << " checks FALLIDOS\n";
    } else {
        std::cout << "Todos los checks de Módulo 3 pasaron correctamente.\n";
    }
}

#ifndef MAIN_TESTS_ORQUESTADOR
int main() {
    ejecutarPruebasBST();
    return chequeosFallidosM3 > 0 ? 1 : 0;
}
#endif
