#pragma once
#include "Envio.h"
#include <iostream>

struct NodoCola {
    Envio* envio;
    NodoCola* siguiente;
    NodoCola (Envio* e) : envio(e), siguiente(nullptr) {}
};

class Queue {
private:
    NodoCola* frente;
    NodoCola* final;

public:
    Queue () : frente(nullptr), final(nullptr) {}
    ~Queue () {
        while (!isEmpty()) {
            dequeue();
        }
    }


    void enqueue (Envio* e) {
        NodoCola* nodo = new NodoCola (e);
        if (frente == nullptr) {
            frente = final = nodo;
        } else {
            final->siguiente = nodo;
            final = nodo;
        }
    }

    Envio* dequeue () {
        if (frente == nullptr) {
            return nullptr;
        }

        NodoCola* aux = frente;
        Envio* e = aux->envio;
        frente = frente->siguiente;

        if (frente == nullptr) {
            final = nullptr;
        }

        delete aux;
        return e;
    }

    Envio* front () {
        if (frente == nullptr) {
            return nullptr;
        }
        return frente->envio;
    }

    bool isEmpty() const {
        return frente == nullptr;
    }
};
