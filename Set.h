#ifndef SET_H
#define SET_H
#include <string>
#include <iostream>

// Set de codigos de seguimiento, sin repetidos.
// Estructura: lista simplemente enlazada ordenada.
// Ownership: el Set es duenio de sus NodoSet (los crea con new y los libera en el destructor).
// Guarda copias de los codigos (strings), no punteros a Envio, no hay riesgo de punteros colgantes.
struct NodoSet {
    std::string clave;
    NodoSet* siguiente;
    NodoSet(const std::string& c) : clave(c), siguiente(nullptr) {}
};

class Set {
private:
    NodoSet* cabeza;
    int cantidad;

    void vaciar() {
        while (cabeza) { NodoSet* aux = cabeza; cabeza = cabeza->siguiente; delete aux; }
        cantidad = 0;
    }
    // Agrega al final (funcion auxiliar usada por union/interseccion/diferencia, que ya recorren en orden)
    void agregarAlFinal(NodoSet*& ultimo, const std::string& c) {
        NodoSet* n = new NodoSet(c);
        if (!cabeza) cabeza = n; else ultimo->siguiente = n;
        ultimo = n;
        cantidad++;
    }
public:
    Set() : cabeza(nullptr), cantidad(0) {}
    Set(const Set& o) : cabeza(nullptr), cantidad(0) {   // copia profunda
        NodoSet* ult = nullptr;
        for (NodoSet* p = o.cabeza; p; p = p->siguiente) agregarAlFinal(ult, p->clave);
    }
    Set& operator=(const Set& o) {
        if (this != &o) {
            vaciar();
            NodoSet* ult = nullptr;
            for (NodoSet* p = o.cabeza; p; p = p->siguiente) agregarAlFinal(ult, p->clave);
        }
        return *this;
    }
    ~Set() { vaciar(); }

    // Inserta si no existe. Devuelve false si ya estaba. O(n)
    bool insert(const std::string& c) {
        if (!cabeza || c < cabeza->clave) {
            NodoSet* n = new NodoSet(c);
            n->siguiente = cabeza; cabeza = n; cantidad++;
            return true;
        }
        if (c == cabeza->clave) return false;
        NodoSet* act = cabeza;
        while (act->siguiente && act->siguiente->clave < c) act = act->siguiente;
        if (act->siguiente && act->siguiente->clave == c) return false;
        NodoSet* n = new NodoSet(c);
        n->siguiente = act->siguiente; act->siguiente = n; cantidad++;
        return true;
    }
    // Elimina si existe. Devuelve false si no estaba. O(n)
    bool remove(const std::string& c) {
        if (!cabeza || c < cabeza->clave) return false;
        if (c == cabeza->clave) {
            NodoSet* aux = cabeza; cabeza = cabeza->siguiente; delete aux; cantidad--;
            return true;
        }
        NodoSet* act = cabeza;
        while (act->siguiente && act->siguiente->clave < c) act = act->siguiente;
        if (!act->siguiente || act->siguiente->clave != c) return false;
        NodoSet* aux = act->siguiente; act->siguiente = aux->siguiente; delete aux; cantidad--;
        return true;
    }
    // O(n) peor caso (corta antes si se pasa, por estar ordenado)
    bool contains(const std::string& c) const {
        for (NodoSet* p = cabeza; p && p->clave <= c; p = p->siguiente)
            if (p->clave == c) return true;
        return false;
    }
    int size() const { return cantidad; }
    bool empty() const { return cantidad == 0; }

    // Union: merge de dos listas ordenadas. O(n+m)
    static Set unionDe(const Set& a, const Set& b) {
        Set r; NodoSet* ult = nullptr;
        NodoSet *i = a.cabeza, *j = b.cabeza;
        while (i || j) {
            if (!j || (i && i->clave < j->clave)) { r.agregarAlFinal(ult, i->clave); i = i->siguiente; }
            else if (!i || j->clave < i->clave)   { r.agregarAlFinal(ult, j->clave); j = j->siguiente; }
            else { r.agregarAlFinal(ult, i->clave); i = i->siguiente; j = j->siguiente; }
        }
        return r;
    }
    // Interseccion. O(n+m)
    static Set interseccion(const Set& a, const Set& b) {
        Set r; NodoSet* ult = nullptr;
        NodoSet *i = a.cabeza, *j = b.cabeza;
        while (i && j) {
            if (i->clave < j->clave) i = i->siguiente;
            else if (j->clave < i->clave) j = j->siguiente;
            else { r.agregarAlFinal(ult, i->clave); i = i->siguiente; j = j->siguiente; }
        }
        return r;
    }
    // Diferencia A - B. O(n+m)
    static Set diferencia(const Set& a, const Set& b) {
        Set r; NodoSet* ult = nullptr;
        NodoSet *i = a.cabeza, *j = b.cabeza;
        while (i) {
            if (!j || i->clave < j->clave) { r.agregarAlFinal(ult, i->clave); i = i->siguiente; }
            else if (j->clave < i->clave) j = j->siguiente;
            else { i = i->siguiente; j = j->siguiente; }
        }
        return r;
    }

    // Para recorrer desde afuera
    const NodoSet* primero() const { return cabeza; }

    void mostrar() const {
        std::cout << "{";
        for (NodoSet* p = cabeza; p; p = p->siguiente)
            std::cout << p->clave << (p->siguiente ? ", " : "");
        std::cout << "}\n";
    }
};
#endif
