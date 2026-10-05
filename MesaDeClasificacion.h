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

    void busquedaBinaria (std::string cod) {
        if (tamaño == 0) {
            return;
        }

        if (criterioActual != criterio::CODIGO) {
            ordenarLote(criterio::CODIGO);
        }

        int izq = 0;
        int der = tamaño - 1;

        std::cout << "Buscar: " << cod << "\n\n";

        while (izq <= der) {
            int mid = izq + (der - izq) / 2;

            std::cout << "izq = " << izq << std::endl;
            std::cout << "der = " << der << std::endl;
            std::cout << "mid: " << mid << "\n" << std::endl;

            if (lote[mid]->getCodigo() == cod) {
                std::cout << "Resultado:\n";
                std::cout << lote[mid]->getCodigo() << " encontrado\n";
                return;
            }

            if (lote[mid]->getCodigo()  < cod) {
                izq = mid + 1;
            } else {
                der = mid - 1;
            }
        }
        
        std::cout << "El envio " << cod << " no fue encontrado.\n";
    }

    void compararBusquedas (std::string cod) {
        if (tamaño == 0) {
            return;
        }

        if (criterioActual != criterio::CODIGO) {
            ordenarLote(criterio::CODIGO);
        }

        // Ejecucion y conteo de busqueda lineal
        int comparacionesLineal = 0;

        for (int i = 0; i < tamaño; i++) {
            comparacionesLineal++;

            if (lote[i]->getCodigo() == cod) {
                break;
            }
        }

        //Ejecucion y conteo de busqueda binaria
        int comparacionesBinaria = 0;

        int izq = 0;
        int der = tamaño - 1;

        while (izq <= der) {
            comparacionesBinaria++;
            int mid = izq + (der - izq) / 2;

            if (lote[mid]->getCodigo() == cod) {
                break;
            }

            if (lote[mid]->getCodigo() < cod) {
                izq = mid + 1;
            } else {
                der = mid - 1;
            }
        }

        std::cout << "Codigo buscado: " << cod << "\n\n";

        std::cout << "Busqueda Lineal: " << "\n";
        std::cout << "Comparaciones: " << comparacionesLineal << "\n";
        std::cout << "Mejor caso: O(1)\n";
        std::cout << "Peor caso: O(n)\n\n";

        std::cout << "Busqueda Binaria: " << "\n";
        std::cout << "Comparaciones: " << comparacionesBinaria << "\n";
        std::cout << "Mejor caso: O(1)\n";
        std::cout << "Peor caso: O(log n)\n";
    }
    
};
