// Tests de la ampliacion HubFlow: arbol B+, BitSet, Set e integracion.
// Se usa un verificador sencillo para que los resultados sean faciles de leer.

#include "ArbolBPlus.h"
#include "CentroDeDistribucion.h"
#include "BitSet.h"
#include "Set.h"
#include <functional>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>

static int chequeos = 0;
static int fallidos = 0;

static void verificar(bool condicion, const std::string& descripcion) {
    ++chequeos;
    if (condicion) {
        std::cout << "  [OK]   " << descripcion << "\n";
    } else {
        ++fallidos;
        std::cout << "  [FAIL] " << descripcion << "\n";
    }
}

// Captura lo que imprime una funcion para poder comprobar el resultado.
static std::string capturarSalida(const std::function<void()>& accion) {
    std::ostringstream buffer;
    std::streambuf* original = std::cout.rdbuf(buffer.rdbuf());
    accion();
    std::cout.rdbuf(original);
    return buffer.str();
}

static Envio* crearEnvio(const std::string& codigo) {
    return new Envio(codigo, "Destinatario de prueba", "NORTE", 1.0, NivelServicio::ESTANDAR);
}

static void probarArbolBPlus() {
    std::cout << "\nCaso A - Arbol B+\n";

    ArbolBPlus vacio;
    int visitados = -1;
    verificar(vacio.estaVacio(), "Un arbol nuevo esta vacio");
    verificar(vacio.buscar("PKG-0001", visitados) == nullptr && visitados == 0,
              "Buscar en un arbol vacio devuelve nullptr y cero nodos visitados");
    verificar(vacio.buscarPorRango("PKG-0001", "PKG-0005") == 0,
              "Un rango en un arbol vacio devuelve cero resultados");

    const int TOTAL = 80;
    Envio* envios[TOTAL];
    for (int i = 0; i < TOTAL; ++i) {
        char codigo[20];
        std::snprintf(codigo, sizeof(codigo), "PKG-%04d", i + 1);
        envios[i] = crearEnvio(codigo);
    }

    {
        ArbolBPlus arbol;
        bool todasInsertadas = true;
        // El paso 37 genera un orden de insercion distinto del orden de claves.
        // Asi tambien probamos divisiones cuando las claves llegan mezcladas.
        for (int i = 0; i < TOTAL; ++i) {
            int indice = (i * 37) % TOTAL;
            if (!arbol.insertar(envios[indice])) todasInsertadas = false;
        }

        verificar(todasInsertadas, "Inserta todos los envios sin duplicados");
        verificar(arbol.getCantidad() == TOTAL, "La cantidad del arbol coincide con las inserciones");
        verificar(arbol.altura() >= 3, "80 claves provocan divisiones de nodos internos y aumentan la altura");
        verificar(arbol.hojasAlMismoNivel(), "Todas las hojas quedan a la misma profundidad");
        verificar(!arbol.insertar(envios[0]) && arbol.getCantidad() == TOTAL,
                  "Rechaza una clave duplicada sin alterar la cantidad");

        bool todasEncontradas = true;
        for (int i = 0; i < TOTAL; ++i) {
            char codigo[20];
            std::snprintf(codigo, sizeof(codigo), "PKG-%04d", i + 1);
            visitados = 0;
            if (arbol.buscar(codigo, visitados) != envios[i] || visitados <= 0) {
                todasEncontradas = false;
                break;
            }
        }
        verificar(todasEncontradas, "Encuentra cada envio y contabiliza los nodos visitados");
        verificar(arbol.buscar("NO-EXISTE", visitados) == nullptr,
                  "Una busqueda inexistente devuelve nullptr");

        std::string salidaRango;
        int encontrados = 0;
        salidaRango = capturarSalida([&] {
            encontrados = arbol.buscarPorRango("PKG-0015", "PKG-0020");
        });
        verificar(encontrados == 6, "El rango inclusivo devuelve los seis envios esperados");
        // El sufijo " |" distingue la fila del envio del encabezado del rango.
        std::size_t p15 = salidaRango.find("PKG-0015 |");
        std::size_t p16 = salidaRango.find("PKG-0016 |");
        std::size_t p20 = salidaRango.find("PKG-0020 |");
        verificar(p15 != std::string::npos && p16 != std::string::npos && p20 != std::string::npos &&
                      p15 < p16 && p16 < p20 && salidaRango.find("PKG-0014") == std::string::npos &&
                      salidaRango.find("PKG-0021") == std::string::npos,
                  "El rango respeta los limites y devuelve codigos en orden ascendente");
        verificar(arbol.buscarPorRango("PKG-0050", "PKG-0040") == 0,
                  "Un rango invertido se rechaza");
        std::string rangoVacio = capturarSalida([&] {
            encontrados = arbol.buscarPorRango("PKG-0090", "PKG-0099");
        });
        verificar(encontrados == 0 && rangoVacio.find("sin resultados") != std::string::npos,
                  "Un rango valido sin envios informa que no hay resultados");
    } // El arbol libera sus nodos, pero no los objetos Envio referenciados.

    for (int i = 0; i < TOTAL; ++i) delete envios[i];
}

static void probarBitSet() {
    std::cout << "\nCaso B - BitSet\n";
    BitSet a;
    verificar(!a.any() && a.count() == 0, "Un BitSet nuevo no tiene bits activos");

    a.set(0);
    a.set(1);
    a.set(2);
    verificar(a.test(0) && a.test(1) && a.test(2), "set y test activan y consultan bits");
    verificar(a.count() == 3 && a.any(), "count y any reconocen los bits activos");
    a.reset(1);
    verificar(!a.test(1) && a.count() == 2, "reset desactiva el bit solicitado");
    a.set(-1);
    a.set(5);
    a.reset(8);
    verificar(!a.test(-1) && !a.test(5) && a.count() == 2,
              "Las posiciones invalidas no modifican el BitSet y test devuelve false");

    BitSet b;
    b.set(2);
    b.set(3);
    BitSet comunes = a & b;
    BitSet unionBits = a | b;
    BitSet distintos = a ^ b;
    verificar(comunes.count() == 1 && comunes.test(2), "AND conserva solo los bits comunes");
    verificar(unionBits.count() == 3 && unionBits.test(0) && unionBits.test(2) && unionBits.test(3),
              "OR activa los bits presentes en cualquiera de los conjuntos");
    verificar(distintos.count() == 2 && distintos.test(0) && distintos.test(3) && !distintos.test(2),
              "XOR conserva los bits presentes en exactamente uno de los conjuntos");
}

static void probarSet() {
    std::cout << "\nCaso C - Set\n";
    Set a;
    Set b;
    verificar(a.empty() && a.size() == 0, "Un Set nuevo esta vacio");
    verificar(a.insert("PKG-2") && a.insert("PKG-1") && a.insert("PKG-3"),
              "Inserta codigos nuevos");
    verificar(!a.insert("PKG-2") && a.size() == 3, "No admite codigos duplicados");
    b.insert("PKG-2");
    b.insert("PKG-3");
    b.insert("PKG-4");

    Set unionAB = Set::unionDe(a, b);
    Set interseccionAB = Set::interseccion(a, b);
    Set diferenciaAB = Set::diferencia(a, b);
    verificar(unionAB.size() == 4 && unionAB.contains("PKG-1") && unionAB.contains("PKG-4"),
              "La union incluye todos los elementos sin repetirlos");
    verificar(interseccionAB.size() == 2 && interseccionAB.contains("PKG-2") && interseccionAB.contains("PKG-3"),
              "La interseccion contiene solo los elementos compartidos");
    verificar(diferenciaAB.size() == 1 && diferenciaAB.contains("PKG-1"),
              "La diferencia A-B excluye los elementos que estan en B");
    verificar(!a.remove("NO-EXISTE") && a.size() == 3,
              "Eliminar un elemento inexistente no altera el conjunto");
    verificar(a.remove("PKG-2") && !a.contains("PKG-2"), "remove elimina un elemento existente");

    Set vacio;
    Set unionVacia = Set::unionDe(a, vacio);
    Set interseccionVacia = Set::interseccion(a, vacio);
    Set interseccionActualizada = Set::interseccion(a, b);
    Set conjuntoDisjunto;
    conjuntoDisjunto.insert("OTRO-CODIGO");
    Set interseccionDisjunta = Set::interseccion(a, conjuntoDisjunto);
    verificar(unionVacia.size() == a.size(), "La union con un conjunto vacio conserva los elementos");
    verificar(interseccionVacia.empty(), "La interseccion con un conjunto vacio queda vacia");
    verificar(interseccionActualizada.size() == 1 && interseccionActualizada.contains("PKG-3"),
              "La interseccion se actualiza correctamente luego de eliminar un elemento");
    verificar(interseccionDisjunta.empty(), "La interseccion de conjuntos disjuntos queda vacia");
}

static void probarIntegracion() {
    std::cout << "\nCaso D - Integracion con HubFlow\n";
    CentroDeDistribucion cd;
    capturarSalida([&] {
        cd.registrarEnvio("PKG-2001", "Ana", "NORTE", 1.0, NivelServicio::EXPRESS);
        cd.registrarEnvio("PKG-2002", "Bruno", "NORTE", 2.0, NivelServicio::ESTANDAR);
        cd.registrarEnvio("PKG-2003", "Carla", "SUR", 3.0, NivelServicio::EXPRESS);
    });

    std::string busqueda = capturarSalida([&] { cd.buscarEnBPlus("PKG-2001"); });
    verificar(busqueda.find("PKG-2001") != std::string::npos &&
                  busqueda.find("Nodos B+ visitados:") != std::string::npos,
              "Un envio registrado puede buscarse por el indice B+");

    capturarSalida([&] {
        cd.cambiarEstado("PKG-2001", Estado::CLASIFICADO, "Clasificado");
        cd.cambiarEstado("PKG-2001", Estado::EN_REPARTO, "En reparto");
        cd.reprogramarEnvio("PKG-2001", "No habia nadie en el domicilio");
    });
    std::string bitset = capturarSalida([&] { cd.mostrarBitSetEnvio("PKG-2001"); });
    std::string comparacionBits = capturarSalida([&] {
        cd.compararBitSets("PKG-2001", "PKG-2002");
    });
    verificar(bitset.find("RECIBIDO") != std::string::npos &&
                  bitset.find("CLASIFICADO") != std::string::npos &&
                  bitset.find("EN_REPARTO") != std::string::npos &&
                  bitset.find("REPROGRAMADO") != std::string::npos &&
                  bitset.find("Cantidad de estados diferentes: 4") != std::string::npos,
              "Los cambios de estado actualizan el BitSet historico automaticamente");
    verificar(comparacionBits.find("Estados en comun") != std::string::npos &&
                  comparacionBits.find("Estados que aparecen en solo uno") != std::string::npos,
              "La integracion permite comparar estados comunes y exclusivos de dos envios");

    Set norte = cd.conjuntoPorZona("NORTE");
    Set express = cd.conjuntoPorServicio(NivelServicio::EXPRESS);
    verificar(norte.size() == 2 && norte.contains("PKG-2001") && norte.contains("PKG-2002"),
              "El Set por zona incluye todos los envios de esa zona");
    verificar(express.size() == 2 && express.contains("PKG-2001") && express.contains("PKG-2003"),
              "El Set por servicio incluye todos los envios de ese nivel");

    std::string operaciones = capturarSalida([&] { cd.mostrarOperacionesConjuntos(norte, express); });
    verificar(operaciones.find("A interseccion B") != std::string::npos &&
                  operaciones.find("PKG-2001") != std::string::npos &&
                  operaciones.find("A union B") != std::string::npos &&
                  operaciones.find("A - B") != std::string::npos,
              "Las operaciones entre Set recuperan los datos completos desde el B+");

    std::string rango = capturarSalida([&] { cd.buscarRangoBPlus("PKG-2001", "PKG-2002"); });
    verificar(rango.find("PKG-2001") != std::string::npos && rango.find("PKG-2002") != std::string::npos,
              "La consulta por rango funciona con los envios del registro integrado");
}

int main() {
    std::cout << "========== TESTS DE CONSULTAS AVANZADAS ==========" << std::endl;
    probarArbolBPlus();
    probarBitSet();
    probarSet();
    probarIntegracion();

    std::cout << "\nResultado: " << (chequeos - fallidos) << "/" << chequeos << " checks OK.\n";
    if (fallidos != 0) std::cout << fallidos << " checks fallaron.\n";
    return fallidos == 0 ? 0 : 1;
}
