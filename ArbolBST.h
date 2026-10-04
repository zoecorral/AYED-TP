#pragma once
#include "Envio.h"
#include <string>

struct NodoBST {
    Envio* envio;
    NodoBST* izq;
    NodoBST* der;

    NodoBST(Envio* env);
};

class ArbolBST {
private:
    NodoBST* raiz;
    int cantidad;

    void destruirRec(NodoBST* nodo);
    void inOrderRec(NodoBST* nodo) const;
    int alturaRec(NodoBST* nodo) const;

public:
    ArbolBST();
    ArbolBST(const ArbolBST&) = delete;
    ArbolBST& operator=(const ArbolBST&) = delete;
    ~ArbolBST();

    bool insertar(Envio* nuevo);
    Envio* buscar(const std::string& codigo, int& visitados, bool mostrarCamino = true) const;
    void recorridoInOrder() const;
    int altura() const;
    int getCantidad() const;
    bool estaVacio() const;
};