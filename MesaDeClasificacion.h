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

    void ordenarLote(criterio criterio) {
        if (tamaño <= 1) {
            criterioActual = criterio;
            return;
        }

        for (int i = 0; i < tamaño; i++) {
            Envio* aux = lote[i];
            int j = i - 1;

            if (criterio == criterio::CODIGO) {
                while (j >= 0 && lote[j]->getCodigo() == aux->getCodigo()) {
                    lote[j + 1] = lote[j];
                    j--;
                }
            } else if (criterio == criterio::PESO) {
                while (j >= 0 && lote[j]->getPeso() == aux->getPeso()) {
                    lote[j + 1] = lote[j];
                    j--;
                }
            }

            lote[j + 1] = aux;
        }

        criterioActual = criterio;
    }

    void cargarQueue(Queue& colaPreparacion) {
        if (tamaño = 0) {
            return;
        }

        for (int i = 0; i < tamaño; i++) {
            colaPreparacion.enqueue(lote[i]);
        }

        std::cout << "Queue: \n" << std::endl;
        colaPreparacion.mostrarQueue();
    }

    void procesarStack(Queue& colaPreparacion, Stack& pilaProcesados) {
        if (colaPreparacion.isEmpty()) {
            return;
        }

        std::cout << "Procesados en este orden: \n\n";

        while (!colaPreparacion.isEmpty()) {
            Envio* e  = colaPreparacion.dequeue();
            pilaProcesados.push(e);
            std::cout << e->getCodigo() << std::endl;
        }

        pilaProcesados.mostrarStack();
    }

    void consultarUltimoProcesado(Stack& pilaProcesados) const {
        if (pilaProcesados.isEmpty()) {
            return;
        }

        std::cout << "Ultimo envio procesado: "  << pilaProcesados.top()->getCodigo() << "\n";
    }

    
};
