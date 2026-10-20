// Tests unitarios de Envio: datos iniciales, cambios de estado,
// intentos de entrega, formato de salida e historial bidireccional.

#include "Envio.h"
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

static int total = 0;
static int fallidos = 0;

static void verificar(bool condicion, const std::string& descripcion) {
    ++total;
    if (condicion) {
        std::cout << "  [OK]   " << descripcion << "\n";
    } else {
        ++fallidos;
        std::cout << "  [FAIL] " << descripcion << "\n";
    }
}

static std::string capturarSalida(const std::function<void()>& accion) {
    std::ostringstream buffer;
    std::streambuf* original = std::cout.rdbuf(buffer.rdbuf());
    std::ios formatoOriginal(nullptr);
    formatoOriginal.copyfmt(std::cout);

    accion();

    std::cout.copyfmt(formatoOriginal);
    std::cout.rdbuf(original);
    return buffer.str();
}

static void testDatosIniciales() {
    std::cout << "\nCaso 1 - Datos iniciales del envio\n";
    Envio envio("PKG-001", "Ana Perez", "NORTE", 2.5, NivelServicio::EXPRESS);

    verificar(envio.getCodigo() == "PKG-001", "Guarda el codigo recibido");
    verificar(envio.getDestinatario() == "Ana Perez", "Guarda el destinatario");
    verificar(envio.getZona() == "NORTE", "Guarda la zona");
    verificar(envio.getPeso() == 2.5, "Guarda el peso");
    verificar(envio.getNivel() == NivelServicio::EXPRESS, "Guarda el nivel de servicio");
    verificar(envio.getEstado() == Estado::RECIBIDO, "El estado inicial es RECIBIDO");
    verificar(envio.getIntentos() == 0, "Los intentos iniciales son cero");
    verificar(!envio.estaEntregado(), "Un envio nuevo no figura como entregado");
}

static void testMostrar() {
    std::cout << "\nCaso 2 - Mostrar informacion del envio\n";
    Envio envio("PKG-002", "Luis Gomez", "SUR", 1.25, NivelServicio::PRIORITARIO);
    std::string salida = capturarSalida([&] { envio.mostrar(); });

    verificar(salida.find("PKG-002") != std::string::npos, "Muestra el codigo");
    verificar(salida.find("SUR") != std::string::npos, "Muestra la zona");
    verificar(salida.find("1.25 kg") != std::string::npos, "Muestra el peso con dos decimales");
    verificar(salida.find("PRIORITARIO") != std::string::npos, "Muestra el nivel de servicio");
    verificar(salida.find("RECIBIDO") != std::string::npos, "Muestra el estado");
    verificar(salida.find("intentos: 0") != std::string::npos, "Muestra la cantidad de intentos");
}

static void testCambiosEstadoEIntentos() {
    std::cout << "\nCaso 3 - Cambios de estado e intentos\n";
    Envio envio("PKG-003", "Marta Ruiz", "CENTRO", 3.0, NivelServicio::ESTANDAR);

    envio.cambiarEstado(Estado::CLASIFICADO, "Clasificado");
    verificar(envio.getEstado() == Estado::CLASIFICADO, "Actualiza el estado a CLASIFICADO");

    envio.sumarIntento();
    envio.sumarIntento();
    verificar(envio.getIntentos() == 2, "Cada sumarIntento incrementa el contador");

    envio.cambiarEstado(Estado::ENTREGADO, "Entregado al destinatario");
    verificar(envio.getEstado() == Estado::ENTREGADO, "Actualiza el estado a ENTREGADO");
    verificar(envio.estaEntregado(), "estaEntregado devuelve true al entregar el envio");
}

static void testHistorialBidireccional() {
    std::cout << "\nCaso 4 - Historial cronologico e inverso\n";
    Envio envio("PKG-004", "Sofia Diaz", "OESTE", 0.8, NivelServicio::EXPRESS);
    envio.cambiarEstado(Estado::CLASIFICADO, "Paso por clasificacion");
    envio.cambiarEstado(Estado::EN_REPARTO, "Salio a reparto");
    envio.cambiarEstado(Estado::ENTREGADO, "Recepcion confirmada");

    std::string cronologico =
        capturarSalida([&] { envio.mostrarHistorialCronologico(); });
    std::string inverso =
        capturarSalida([&] { envio.mostrarHistorialInverso(); });

    std::size_t cRecibido = cronologico.find("RECIBIDO");
    std::size_t cClasificado = cronologico.find("CLASIFICADO");
    std::size_t cReparto = cronologico.find("EN_REPARTO");
    std::size_t cEntregado = cronologico.find("ENTREGADO");

    verificar(cRecibido < cClasificado && cClasificado < cReparto &&
                  cReparto < cEntregado,
              "El historial cronologico respeta el orden de los movimientos");
    verificar(cronologico.find("Ingreso al centro de distribucion") != std::string::npos,
              "El historial conserva el movimiento inicial");
    verificar(cronologico.find("Recepcion confirmada") != std::string::npos,
              "El historial conserva las observaciones");

    std::size_t iEntregado = inverso.find("ENTREGADO");
    std::size_t iReparto = inverso.find("EN_REPARTO");
    std::size_t iClasificado = inverso.find("CLASIFICADO");
    std::size_t iRecibido = inverso.find("RECIBIDO");
    verificar(iEntregado < iReparto && iReparto < iClasificado &&
                  iClasificado < iRecibido,
              "El historial inverso recorre del movimiento mas reciente al mas antiguo");
}

int main() {
    std::cout << "========== TESTS UNITARIOS: ENVIO ==========\n";
    testDatosIniciales();
    testMostrar();
    testCambiosEstadoEIntentos();
    testHistorialBidireccional();

    std::cout << "\n============================================\n";
    std::cout << (total - fallidos) << "/" << total << " checks OK\n";
    if (fallidos > 0) {
        std::cout << fallidos << " checks FALLIDOS\n";
        return 1;
    }
    std::cout << "Todos los tests de Envio pasaron correctamente.\n";
    return 0;
}
