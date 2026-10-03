#pragma once
#include "Envio.h"
#include <iostream>
#include <string>

struct NodoBST {
    Envio* envio;
    NodoBST* izq;
    NodoBST* der;

    NodoBST (Envio* env) : envio (env), izq (nullptr), der(nullptr) {}
};

class ArbolBST {
private:
    NodoBST* raiz;

    void destruirRec(NodoBST* nodo) {
        if (nodo == nullptr) return;
        destruirRec(nodo->izq);
        destruirRec(nodo->der);
        delete nodo; // No libera envio* (ownership externo)
    }

public:
    ArbolBST () : raiz (nullptr) {}
    ~ArbolBST () {
        destruirRec(raiz);
    }
    
    NodoBST* insertar(Envio* nuevo);
    Envio* buscar();
    void recorridoInOrder();
    int altura();
};
