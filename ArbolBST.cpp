#include "ArbolBST.h"
#include <iostream>

NodoBST::NodoBST(Envio* env) : envio(env), izq(nullptr), der(nullptr) {}

ArbolBST::ArbolBST() : raiz(nullptr), cantidad(0) {}

ArbolBST::~ArbolBST() {
    destruirRec(raiz);
    raiz = nullptr;
}

void ArbolBST::destruirRec(NodoBST* nodo) {
    if (nodo == nullptr) return;
    destruirRec(nodo->izq);
    destruirRec(nodo->der);
    delete nodo;
}

void ArbolBST::inOrderRec(NodoBST* nodo) const {
    if (nodo == nullptr) return;
    inOrderRec(nodo->izq);
    std::cout << "  ";
    nodo->envio->mostrar();
    inOrderRec(nodo->der);
}

int ArbolBST::alturaRec(NodoBST* nodo) const {
    if (nodo == nullptr) return 0;
    int hi = alturaRec(nodo->izq);
    int hd = alturaRec(nodo->der);
    return 1 + (hi > hd ? hi : hd);
}

bool ArbolBST::insertar(Envio* nuevo) {
    if (nuevo == nullptr) return false;

    if (raiz == nullptr) {
        raiz = new NodoBST(nuevo);
        cantidad++;
        return true;
    }

    const std::string& cod = nuevo->getCodigo();
    NodoBST* actual = raiz;

    while (true) {
        const std::string& codActual = actual->envio->getCodigo();

        if (cod == codActual) return false;

        if (cod < codActual) {
            if (actual->izq == nullptr) {
                actual->izq = new NodoBST(nuevo);
                cantidad++;
                return true;
            }
            actual = actual->izq;
        } else {
            if (actual->der == nullptr) {
                actual->der = new NodoBST(nuevo);
                cantidad++;
                return true;
            }
            actual = actual->der;
        }
    }
}

Envio* ArbolBST::buscar(const std::string& codigo, int& visitados, bool mostrarCamino) const {
    visitados = 0;
    NodoBST* actual = raiz;

    while (actual != nullptr) {
        visitados++;
        const std::string& codActual = actual->envio->getCodigo();

        if (codigo == codActual) {
            if (mostrarCamino) std::cout << "  " << codActual << " -> encontrado\n";
            return actual->envio;
        }
        if (codigo < codActual) {
            if (mostrarCamino) std::cout << "  " << codActual << " -> izquierda\n";
            actual = actual->izq;
        } else {
            if (mostrarCamino) std::cout << "  " << codActual << " -> derecha\n";
            actual = actual->der;
        }
    }
    return nullptr;
}

void ArbolBST::recorridoInOrder() const {
    if (raiz == nullptr) {
        std::cout << "  (indice BST vacio)\n";
        return;
    }
    inOrderRec(raiz);
}

int ArbolBST::altura() const {
    return alturaRec(raiz);
}

int ArbolBST::getCantidad() const {
    return cantidad;
}

bool ArbolBST::estaVacio() const {
    return raiz == nullptr;
}