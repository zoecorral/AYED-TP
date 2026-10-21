#include "CentroDeDistribucion.h"
#include <iostream>
#include <string>
#include <limits>

void mostrarMenu() {
    std::cout << "\n========== HUBFLOW ==========\n";
    std::cout << "1. Mostrar envios pendientes\n";
    std::cout << "2. Registrar nuevo envio\n";
    std::cout << "3. Buscar envio\n";
    std::cout << "4. Cambiar estado\n";
    std::cout << "5. Despachar proximo envio\n";
    std::cout << "6. Reprogramar envio\n";
    std::cout << "7. Mostrar historial\n";
    std::cout << "8. Resumen recursivo por zona\n";
    std::cout << "9. Envio mas pesado por zona (desafio)\n";
    std::cout << "10. Consultar indice BST por codigo\n";
    std::cout << "11. Consultas avanzadas de envios (B+, BitSet y Set)\n";
    std::cout << "12. Finalizar\n";
    std::cout << "Opcion: ";
}

// Pide un criterio y construye un Set de codigos a partir del registro actual.
Set pedirConjunto(CentroDeDistribucion& cd, const std::string& nombre) {
    int criterio;
    std::cout << "Criterio para el conjunto " << nombre << ":\n";
    std::cout << "  1. Zona\n";
    std::cout << "  2. Nivel de servicio\n";
    std::cout << "Opcion: ";
    if (!(std::cin >> criterio)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Criterio invalido; se usara un conjunto vacio.\n";
        return Set();
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (criterio == 1) {
        std::string zona;
        std::cout << "Zona: ";
        std::getline(std::cin, zona);
        return cd.conjuntoPorZona(zona);
    }
    if (criterio == 2) {
        int nivelOpcion;
        NivelServicio nivel;
        std::cout << "Nivel (1=EXPRESS, 2=PRIORITARIO, 3=ESTANDAR): ";
        if (!(std::cin >> nivelOpcion)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Nivel invalido; se usara un conjunto vacio.\n";
            return Set();
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (intANivel(nivelOpcion, nivel)) return cd.conjuntoPorServicio(nivel);
        std::cout << "Nivel invalido; se usara un conjunto vacio.\n";
        return Set();
    }

    std::cout << "Criterio invalido; se usara un conjunto vacio.\n";
    return Set();
}

// Submenu de RF14: consultas mediante las tres estructuras nuevas.
void menuConsultasAvanzadas(CentroDeDistribucion& cd) {
    int opcionAvanzada = 0;
    do {
        std::cout << "\n--- CONSULTAS AVANZADAS ---\n";
        std::cout << "1. Buscar envio por codigo (arbol B+)\n";
        std::cout << "2. Buscar envios por rango de codigos\n";
        std::cout << "3. Consultar estados historicos (BitSet)\n";
        std::cout << "4. Comparar estados de dos envios (BitSet)\n";
        std::cout << "5. Crear conjuntos y operar A/B (Set)\n";
        std::cout << "6. Mostrar conjuntos por zona o servicio (Set)\n";
        std::cout << "0. Volver al menu principal\n";
        std::cout << "Opcion: ";

        if (!(std::cin >> opcionAvanzada)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Opcion invalida.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (opcionAvanzada == 1) {
            std::string codigo;
            std::cout << "Codigo: ";
            std::getline(std::cin, codigo);
            cd.buscarEnBPlus(codigo);
        } else if (opcionAvanzada == 2) {
            std::string desde, hasta;
            std::cout << "Desde: ";
            std::getline(std::cin, desde);
            std::cout << "Hasta: ";
            std::getline(std::cin, hasta);
            cd.buscarRangoBPlus(desde, hasta);
        } else if (opcionAvanzada == 3) {
            std::string codigo;
            std::cout << "Codigo: ";
            std::getline(std::cin, codigo);
            cd.mostrarBitSetEnvio(codigo);
        } else if (opcionAvanzada == 4) {
            std::string codigoA, codigoB;
            std::cout << "Primer codigo: ";
            std::getline(std::cin, codigoA);
            std::cout << "Segundo codigo: ";
            std::getline(std::cin, codigoB);
            cd.compararBitSets(codigoA, codigoB);
        } else if (opcionAvanzada == 5) {
            Set a = pedirConjunto(cd, "A");
            Set b = pedirConjunto(cd, "B");
            cd.mostrarOperacionesConjuntos(a, b);
        } else if (opcionAvanzada == 6) {
            Set conjunto = pedirConjunto(cd, "consulta");
            cd.mostrarConjunto(conjunto, "Resultado");
        } else if (opcionAvanzada != 0) {
            std::cout << "Opcion invalida.\n";
        }
    } while (opcionAvanzada != 0);
}

void cargarDataset(CentroDeDistribucion& cd) {
    cd.registrarEnvio("PKG-1001", "Ana Torres",    "CENTRO", 1.20, NivelServicio::ESTANDAR);
    cd.registrarEnvio("PKG-1002", "Bruno Diaz",    "NORTE",  0.75, NivelServicio::EXPRESS);
    cd.registrarEnvio("PKG-1003", "Carla Ruiz",    "SUR",    4.10, NivelServicio::PRIORITARIO);
    cd.registrarEnvio("PKG-1004", "Diego Lopez",   "CENTRO", 2.30, NivelServicio::ESTANDAR);
    cd.registrarEnvio("PKG-1005", "Elena Castro",  "NORTE",  1.90, NivelServicio::PRIORITARIO);
    cd.registrarEnvio("PKG-1006", "Franco Gomez",  "SUR",    0.50, NivelServicio::EXPRESS);
    cd.registrarEnvio("PKG-1007", "Gabriela Soto", "CENTRO", 6.20, NivelServicio::ESTANDAR);
    cd.registrarEnvio("PKG-1008", "Hugo Perez",    "NORTE",  3.40, NivelServicio::PRIORITARIO);
}

int main() {
    CentroDeDistribucion cd;

    std::cout << "Cargando dataset inicial...\n";
    cargarDataset(cd);

    int opcion = 0;
    do {
        mostrarMenu();
        std::cin >> opcion;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Opcion invalida.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (opcion == 1) {
            cd.mostrarPendientes();

        } else if (opcion == 2) {
            std::string cod, dest, zona;
            double peso;
            int nivelOpcion;
            NivelServicio nivel;
            std::cout << "Codigo: "; std::getline(std::cin, cod);
            std::cout << "Destinatario: "; std::getline(std::cin, dest);
            std::cout << "Zona: "; std::getline(std::cin, zona);
            std::cout << "Peso (kg): "; std::cin >> peso;
            std::cout << "Nivel (1=EXPRESS, 2=PRIORITARIO, 3=ESTANDAR): "; std::cin >> nivelOpcion;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (!intANivel(nivelOpcion, nivel)) {
                std::cout << "Nivel invalido.\n";
                continue;
            }
            cd.registrarEnvio(cod, dest, zona, peso, nivel);

        } else if (opcion == 3) {
            std::string cod;
            std::cout << "Codigo: "; std::getline(std::cin, cod);
            cd.buscarEnvio(cod);

        } else if (opcion == 4) {
            std::string cod, obs;
            int estOpcion;
            Estado est;
            std::cout << "Codigo: "; std::getline(std::cin, cod);
            std::cout << "Nuevo estado (0=RECIBIDO 1=CLASIFICADO 2=EN_REPARTO 3=REPROGRAMADO 4=ENTREGADO): ";
            std::cin >> estOpcion;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (!intAEstado(estOpcion, est)) {
                std::cout << "Estado invalido.\n";
                continue;
            }
            std::cout << "Observacion: "; std::getline(std::cin, obs);
            cd.cambiarEstado(cod, est, obs);

        } else if (opcion == 5) {
            cd.despacharProximo();

        } else if (opcion == 6) {
            std::string cod, motivo;
            std::cout << "Codigo: "; std::getline(std::cin, cod);
            std::cout << "Motivo: "; std::getline(std::cin, motivo);
            cd.reprogramarEnvio(cod, motivo);

        } else if (opcion == 7) {
            std::string cod;
            std::cout << "Codigo: "; std::getline(std::cin, cod);
            cd.mostrarHistorial(cod);

        } else if (opcion == 8) {
            std::string zona;
            std::cout << "Zona (NORTE/SUR/CENTRO): "; std::getline(std::cin, zona);
            cd.resumenZona(zona);

        } else if (opcion == 9) {
            std::string zona;
            std::cout << "Zona (NORTE/SUR/CENTRO): "; std::getline(std::cin, zona);
            cd.envioMasPesadoDeZona(zona);

        } else if (opcion == 10) {
            std::string cod;
            std::cout << "Codigo: "; std::getline(std::cin, cod);
            cd.buscarEnBST(cod);
            std::cout << "--- Recorrido in-order ---\n";
            cd.inOrderBST();
            cd.alturaBST();

        } else if (opcion == 11) {
            menuConsultasAvanzadas(cd);

        } else if (opcion == 12) {
            std::cout << "Finalizando... liberando memoria.\n";

        } else {
            std::cout << "Opcion invalida.\n";
        }

    } while (opcion != 12);

    return 0;
    // Al salir de main, ~CentroDeDistribucion() libera todo en cascada:
    // ~ListaDeEnvios() destruye cada Envio (y su historial), y
    // ~ListaPendientes() destruye sus propios nodos.
}