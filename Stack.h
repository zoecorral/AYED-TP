#pragma once
#include "Envio.h"
#include <iostream>

struct NodoPila {
    Envio* envio;
    NodoPila* siguiente;
    NodoPila (Envio* e) : envio(e), siguiente(nullptr) {}
};

class Stack {
private:
    NodoPila* tope;
public:
    Stack () : tope(nullptr) {}
    ~Stack () {
        while (!isEmpty()) {
            pop();
        }
    }

    void push (Envio* e) {
        NodoPila* nuevo = new NodoPila (e);
        nuevo->siguiente = tope;
        tope = nuevo;
    }

    Envio* pop () {
        if (isEmpty()) {
            return nullptr;
        }
        NodoPila* aux = tope;
        Envio* e = aux->envio;
        tope = tope->siguiente;

        delete aux;
        return e;
    }

    Envio* top () {
        if (isEmpty()) {
            return nullptr;
        }

        return tope->envio;
    }

    bool isEmpty () {
        return tope == nullptr;
    }

    void mostrarStack() const {
        if (isEmpty()) {
            return;
        }

        NodoStack* aux = tope;
        
        std::cout << "TOP\n | \n";
        
        while (aux != nullptr) {
            std::cout << aux->envio->getCodigo() << "\n";
            aux = aux->siguiente;
        }
    }
};
