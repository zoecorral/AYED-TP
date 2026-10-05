// Tests de HubFlow — cubren los "Casos de prueba obligatorios" de la consigna
// (Seccion 13). No usan ningun framework externo: cada caso arma su propio
// CentroDeDistribucion vacio, captura la salida por consola (cout) de las
// operaciones y verifica el contenido con chequear(...).
//
// Para correrlos: compilar el target HubFlowTests (ver CMakeLists.txt) y
// ejecutarlo. Termina con codigo 0 si todo paso, o 1 si algo fallo.

#include "CentroDeDistribucion.h"
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

static int totalChequeos = 0;
static int chequeosFallidos = 0;

void chequear(bool condicion, const std::string& descripcion) {
    totalChequeos++;
    if (condicion) {
        std::cout << "  [OK]   " << descripcion << "\n";
    } else {
        chequeosFallidos++;
        std::cout << "  [FAIL] " << descripcion << "\n";
    }
}

// Redirige cout a un buffer mientras se ejecuta `accion`, y restaura tanto
// el buffer como el formato (fixed/precision) que tenia cout antes de
// llamarla, para que un test no quede afectado por el formato que haya
// dejado seteado otro test anterior (Envio::mostrar usa fixed/setprecision).
std::string capturarSalida(const std::function<void()>& accion) {
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
// Caso 1 — Prioridades
// ============================================================
void testCaso1_Prioridades() {
    std::cout << "\nCaso 1 - Prioridades\n";
    CentroDeDistribucion cd;
    cd.registrarEnvio("A", "D1", "CENTRO", 1.0, NivelServicio::ESTANDAR);
    cd.registrarEnvio("B", "D2", "CENTRO", 1.0, NivelServicio::EXPRESS);
    cd.registrarEnvio("C", "D3", "CENTRO", 1.0, NivelServicio::PRIORITARIO);

    std::string salida = capturarSalida([&] { cd.mostrarPendientes(); });
    chequear(salida.find("B") < salida.find("C") && salida.find("C") < salida.find("A"),
             "La lista queda ordenada EXPRESS > PRIORITARIO > ESTANDAR");
}

// ============================================================
// Caso 2 — Prioridad estable
// ============================================================
void testCaso2_PrioridadEstable() {
    std::cout << "\nCaso 2 - Prioridad estable\n";
    CentroDeDistribucion cd;
    cd.registrarEnvio("X1", "D1", "CENTRO", 1.0, NivelServicio::PRIORITARIO);
    cd.registrarEnvio("X2", "D2", "CENTRO", 1.0, NivelServicio::PRIORITARIO);
    cd.registrarEnvio("X3", "D3", "CENTRO", 1.0, NivelServicio::PRIORITARIO);

    std::string salida = capturarSalida([&] { cd.mostrarPendientes(); });
    chequear(salida.find("X1") < salida.find("X2") && salida.find("X2") < salida.find("X3"),
             "Envios de igual prioridad conservan el orden de llegada");
}

// ============================================================
// Caso 3 — Despacho
// ============================================================
void testCaso3_Despacho() {
    std::cout << "\nCaso 3 - Despacho\n";
    CentroDeDistribucion cd;
    cd.registrarEnvio("D1", "Dest", "CENTRO", 1.0, NivelServicio::EXPRESS);

    std::string salidaDespacho = capturarSalida([&] { cd.despacharProximo(); });
    chequear(salidaDespacho.find("Despachado: D1") != std::string::npos,
             "Se informa el despacho del envio");

    std::string salidaBuscar = capturarSalida([&] { cd.buscarEnvio("D1"); });
    chequear(salidaBuscar.find("EN_REPARTO") != std::string::npos, "El estado cambio a EN_REPARTO");
    chequear(salidaBuscar.find("D1") != std::string::npos,
             "El objeto Envio se conserva (se lo puede seguir consultando)");

    std::string salidaHistorial = capturarSalida([&] { cd.mostrarHistorial("D1"); });
    chequear(salidaHistorial.find("RECIBIDO") != std::string::npos &&
                 salidaHistorial.find("EN_REPARTO") != std::string::npos,
             "Se creo el movimiento EN_REPARTO sin perder el RECIBIDO inicial");

    std::string salidaPendientes = capturarSalida([&] { cd.mostrarPendientes(); });
    chequear(salidaPendientes.find("D1") == std::string::npos,
             "El nodo se elimino de la lista de pendientes");
}

// ============================================================
// Caso 4 — Reprogramacion
// ============================================================
void testCaso4_Reprogramacion() {
    std::cout << "\nCaso 4 - Reprogramacion\n";
    CentroDeDistribucion cd;
    cd.registrarEnvio("R1", "Dest", "CENTRO", 1.0, NivelServicio::ESTANDAR);
    cd.registrarEnvio("R2", "Dest", "CENTRO", 1.0, NivelServicio::EXPRESS);

    capturarSalida([&] { cd.despacharProximo(); });  // despacha R2 (EXPRESS)
    capturarSalida([&] { cd.reprogramarEnvio("R2", "Destinatario ausente"); });

    std::string salidaBuscar = capturarSalida([&] { cd.buscarEnvio("R2"); });
    chequear(salidaBuscar.find("intentos: 1") != std::string::npos,
             "Aumento la cantidad de intentos de entrega");
    chequear(salidaBuscar.find("REPROGRAMADO") != std::string::npos, "El estado paso a REPROGRAMADO");

    std::string salidaHistorial = capturarSalida([&] { cd.mostrarHistorial("R2"); });
    chequear(salidaHistorial.find("Destinatario ausente") != std::string::npos,
             "Se registro el movimiento con la observacion indicada");

    std::string salidaPendientes = capturarSalida([&] { cd.mostrarPendientes(); });
    chequear(salidaPendientes.find("R2") != std::string::npos, "El envio volvio a la lista de pendientes");
    chequear(salidaPendientes.find("R2") < salidaPendientes.find("R1"),
             "Se reinserto respetando su prioridad (EXPRESS antes que ESTANDAR)");
}

// ============================================================
// Caso 5 — Historial (directo e inverso)
// ============================================================
void testCaso5_Historial() {
    std::cout << "\nCaso 5 - Historial\n";
    CentroDeDistribucion cd;
    cd.registrarEnvio("H1", "Dest", "CENTRO", 1.0, NivelServicio::ESTANDAR);
    capturarSalida([&] { cd.cambiarEstado("H1", Estado::CLASIFICADO, "Clasificado en zona"); });
    capturarSalida([&] { cd.despacharProximo(); });

    std::string historial = capturarSalida([&] { cd.mostrarHistorial("H1"); });

    std::size_t posRecibido = historial.find("RECIBIDO");
    std::size_t posClasificado = historial.find("CLASIFICADO");
    std::size_t posReparto = historial.find("EN_REPARTO");
    chequear(posRecibido < posClasificado && posClasificado < posReparto,
             "Orden cronologico: del mas antiguo al mas reciente");

    std::size_t inicioInverso = historial.find("Historial inverso");
    std::size_t posRepartoInv = historial.find("EN_REPARTO", inicioInverso);
    std::size_t posRecibidoInv = historial.find("RECIBIDO", inicioInverso);
    chequear(posRepartoInv < posRecibidoInv,
             "Orden inverso: del mas reciente al mas antiguo");
}

// ============================================================
// Caso 6 — Recursividad (resumen por zona + desafio opcional)
// ============================================================
void testCaso6_Recursividad() {
    std::cout << "\nCaso 6 - Recursividad\n";
    CentroDeDistribucion cd;
    cd.registrarEnvio("Z1", "D", "NORTE", 1.0, NivelServicio::EXPRESS);
    cd.registrarEnvio("Z2", "D", "NORTE", 2.0, NivelServicio::PRIORITARIO);
    cd.registrarEnvio("Z3", "D", "NORTE", 3.0, NivelServicio::ESTANDAR);
    cd.registrarEnvio("Z4", "D", "SUR", 9.0, NivelServicio::EXPRESS);  // otra zona: no debe contar

    std::string salida = capturarSalida([&] { cd.resumenZona("NORTE"); });
    chequear(salida.find("Cantidad de paquetes: 3") != std::string::npos,
             "Cuenta solo los paquetes de la zona pedida");
    chequear(salida.find("Peso total pendiente: 6") != std::string::npos,
             "Suma el peso total de la zona (1+2+3 = 6kg)");
    chequear(salida.find("Cantidad EXPRESS: 1") != std::string::npos,
             "Cuenta solo los EXPRESS de la zona pedida");

    std::string salidaPeso = capturarSalida([&] { cd.envioMasPesadoDeZona("NORTE"); });
    chequear(salidaPeso.find("Z3") != std::string::npos,
             "Desafio opcional: identifica el envio mas pesado de la zona");
}

// ============================================================
// Caso 7 — Casos limite
// ============================================================
void testCaso7_CasosLimite() {
    std::cout << "\nCaso 7 - Casos limite\n";
    CentroDeDistribucion cd;

    std::string salidaVacia = capturarSalida([&] { cd.mostrarPendientes(); });
    chequear(salidaVacia.find("no hay envios pendientes") != std::string::npos,
             "Lista de pendientes vacia se informa sin romper el programa");

    std::string salidaDespachoVacio = capturarSalida([&] { cd.despacharProximo(); });
    chequear(salidaDespachoVacio.find("No hay envios pendientes") != std::string::npos,
             "Despachar con la lista vacia no rompe el programa");

    std::string salidaBusquedaInexistente =
