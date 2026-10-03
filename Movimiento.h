#pragma once
#include <string>

// Representa un evento en el ciclo de vida de un envio.
// Implementado por Angela Contrera.
struct Movimiento {
    int numero;
    std::string estado;
    std::string observacion;

    Movimiento(int n, std::string est, std::string obs)
        : numero(n), estado(std::move(est)), observacion(std::move(obs)) {}
};
