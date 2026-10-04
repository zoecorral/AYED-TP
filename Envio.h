#pragma once
#include "Estado.h"
#include "HistorialDeMovimiento.h"
#include <iostream>
#include <iomanip>
#include <string>

// Representa un paquete que circula por el centro de distribucion.
//
// OWNERSHIP: un Envio es dueno de su HistorialDeMovimientos (lo tiene como
// miembro por valor), pero NO es dueno de si mismo: quien lo crea con `new`
// y lo destruye es ListaDeEnvios (ver ListaDeEnvios.h).
class Envio {
private:
    std::string codigo;
    std::string destinatario;
    std::string zona;
    double peso;
    NivelServicio nivel;
    Estado estado;
    int intentos;
    HistorialDeMovimientos historial;

public:
    Envio(std::string cod, std::string dest, std::string zon, double p, NivelServicio niv)
        : codigo(std::move(cod)), destinatario(std::move(dest)), zona(std::move(zon)), peso(p),
          nivel(niv), estado(Estado::RECIBIDO), intentos(0) {
        historial.agregarMovimiento(estadoToString(estado), "Ingreso al centro de distribucion");
    }

    // Un Envio administra su historial: no se permite copiar (evita un
    // doble delete si dos envios terminaran compartiendo los mismos nodos).
    Envio(const Envio&) = delete;
    Envio& operator=(const Envio&) = delete;

    // ---- consultas ----
    const std::string& getCodigo()       const { return codigo; }
    const std::string& getDestinatario() const { return destinatario; }
    const std::string& getZona()         const { return zona; }
    double getPeso()                const { return peso; }
    NivelServicio getNivel()        const { return nivel; }
    Estado getEstado()              const { return estado; }
    int getIntentos()                const { return intentos; }
    bool estaEntregado()            const { return estado == Estado::ENTREGADO; }

    // Cambia el estado Y deja constancia en el historial en un solo paso,
    // para que sea imposible actualizar uno sin el otro.
    void cambiarEstado(Estado nuevoEstado, const std::string& observacion) {
        estado = nuevoEstado;
        historial.agregarMovimiento(estadoToString(nuevoEstado), observacion);
    }

    void sumarIntento() { intentos++; }

    void mostrarHistorialCronologico() const { historial.mostrarCronologico(); }
    void mostrarHistorialInverso()     const { historial.mostrarInverso(); }

    // RF02 — Codigo | Zona | Peso | Servicio | Estado | Intentos
    void mostrar() const {
        std::cout << codigo
                  << " | " << zona
                  << " | " << std::fixed << std::setprecision(2) << peso << " kg"
                  << " | " << nivelToString(nivel)
                  << " | " << estadoToString(estado)
                  << " | intentos: " << intentos << "\n";
    }
};
