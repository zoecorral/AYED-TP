#pragma once
#include "Envio.h"
#include "Queue.h"
#include "Stack.h"
#include <iostream>
#include <string>


enum class criterio {
    NINGUNO,
    CODIGO,
    PESO
};

class MesaDeClasificacion {
private:
    Envio** lote;
    int capacidad;
    int tamaño;
    criterio criterioActual;

public:
    MesaDeClasificacion() : lote(nullptr), capacidad(0), tamano(0), criterioActual(criterio::NINGUNO) {}

    ~MesaDeClasificacion() {
        liberarLote();
    }

    void liberarLote() {
        delete[] lote;
        lote = nullptr;
        capacidad = 0;
        tamaño = 0;
        criterioActual = criterio::NINGUNO;
    }

    void crearLotePorZona(ListaPendientes& listaPendientes, std::string zona) {
        liberarLote();

        int cantEncontrada = 0;
        NodoPendiente* aux = listaPendientes.getComienzo();

        while (aux != nullptr) {
            if (aux->envio->getZona() == zona) {
                cantEncontrada++;
            }
            aux = aux->siguiente;
        }

        if (cantEncontrada == 0) {
            std::cout << "No se envios pendientes para la zona seleccionada.\n";
            return;
        }

        capacidad = cantEncontrada;
        lote = new Envio*[capacidad];
        tamaño = 0;

        aux = listaPendientes.getComienzo();

        while (aux != nullptr) {
            if (aux->envio->getZona() == zona) {
                lote[tamaño] = aux->envio;
                tamaño++;
            }
            aux = aux->siguiente;
        }

    }

    void ordenarLote();
};
