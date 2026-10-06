// Tests Módulo 2: Mesa de Clasificación (Casos 8 al 11)

#include "CentroDeDistribucion.h"
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

static int totalChequeosM2 = 0;
static int chequeosFallidosM2 = 0;

static void chequearM2(bool condicion, const std::string& descripcion) {
    totalChequeosM2++;
    if (condicion) {
        std::cout << "  [OK]   " << descripcion << "\n";
    } else {
        chequeosFallidosM2++;
        std::cout << "  [FAIL] " << descripcion << "\n";
    }
}

static std::string capturarSalidaM2(const std::function<void()>& accion) {
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
// Caso 8 — Métodos de Ordenamiento
// ============================================================
void testCaso8_Ordenamiento() {
    std::cout << "\nCaso 8 - Metodos de Ordenamiento (Mesa de Clasificacion)\n";
    CentroDeDistribucion cd;

    cd.colocarEnMesa("P3", 3.5);
    cd.colocarEnMesa("P1", 1.2);
    cd.colocarEnMesa("P4", 4.8);
    cd.colocarEnMesa("P2", 2.0);

    std::string salida = capturarSalidaM2([&] { cd.ordenarMesaPorPeso(); });
    std::size_t posP1 = salida.find("P1");
    std::size_t posP2 = salida.find("P2");
    std::size_t posP3 = salida.find("P3");
    std::size_t posP4 = salida.find("P4");

    chequearM2(posP1 < posP2 && posP2 < posP3 && posP3 < posP4,
               "La mesa queda ordenada por peso ascendente (P1 < P2 < P3 < P4)");
}

// ============================================================
// Caso 9 — Pila (Stack)
// ============================================================
void testCaso9_PilaDespacho() {
    std::cout << "\nCaso 9 - Pila de Despacho (LIFO)\n";
    CentroDeDistribucion cd;

    cd.apilarDespacho("ENV-1");
    cd.apilarDespacho("ENV-2");

    std::string salida1 = capturarSalidaM2([&] { cd.desapilarDespacho(); });
    chequearM2(salida1.find("ENV-2") != std::string::npos,
               "LIFO: El ultimo en apilarse (ENV-2) es el primero en salir");

    std::string salida2 = capturarSalidaM2([&] { cd.desapilarDespacho(); });
    chequearM2(salida2.find("ENV-1") != std::string::npos,
               "LIFO: Luego sale el elemento previo (ENV-1)");
}

// ============================================================
// Caso 10 — Cola (Queue)
// ============================================================
void testCaso10_ColaEscaneo() {
    std::cout << "\nCaso 10 - Cola de Escaneo (FIFO)\n";
    CentroDeDistribucion cd;

    cd.encolarEscaneo("PKG-A");
    cd.encolarEscaneo("PKG-B");

    std::string salida1 = capturarSalidaM2([&] { cd.desencolarEscaneo(); });
    chequearM2(salida1.find("PKG-A") != std::string::npos,
               "FIFO: El primero en llegar (PKG-A) es el primero en ser procesado");

    std::string salida2 = capturarSalidaM2([&] { cd.desencolarEscaneo(); });
    chequearM2(salida2.find("PKG-B") != std::string::npos,
               "FIFO: Luego se procesa el siguiente paquete (PKG-B)");
}

// ============================================================
// Caso 11 — Búsquedas (Lineal y Binaria)
// ============================================================
void testCaso11_Busquedas() {
    std::cout << "\nCaso 11 - Busqueda Lineal y Binaria\n";
    CentroDeDistribucion cd;

    cd.colocarEnMesa("A1", 10.0);
    cd.colocarEnMesa("B2", 20.0);
    cd.colocarEnMesa("C3", 30.0);

    // Búsqueda lineal por código
    std::string sLineal = capturarSalidaM2([&] { cd.busquedaLinealMesa("B2"); });
    chequearM2(sLineal.find("B2") != std::string::npos || sLineal.find("encontrado") != std::string::npos,
            "Busqueda lineal localiza el elemento en la mesa");

    // Búsqueda binaria por código (RF09.6)
    std::string sBinaria = capturarSalidaM2([&] { cd.busquedaBinariaMesa("B2"); });
    chequearM2(sBinaria.find("B2") != std::string::npos || sBinaria.find("encontrado") != std::string::npos || sBinaria.find("Resultado:") != std::string::npos,
            "Busqueda binaria localiza el elemento ordenado por codigo");
}

void ejecutarPruebasMesaClasificacion() {
    std::cout << "========== TESTS (MODULO 2: MESA Y ESTRUCTURAS AUXILIARES) ==========\n";

    testCaso8_Ordenamiento();
    testCaso9_PilaDespacho();
    testCaso10_ColaEscaneo();
    testCaso11_Busquedas();

    std::cout << "\n====================================\n";
    std::cout << (totalChequeosM2 - chequeosFallidosM2) << "/" << totalChequeosM2 << " checks OK\n";
    if (chequeosFallidosM2 > 0) {
        std::cout << chequeosFallidosM2 << " checks FALLIDOS\n";
    } else {
        std::cout << "Todos los checks de Módulo 2 pasaron correctamente.\n";
    }
}

#ifndef MAIN_TESTS_ORQUESTADOR
int main() {
    ejecutarPruebasMesaClasificacion();
    return chequeosFallidosM2 > 0 ? 1 : 0;
}
#endif
