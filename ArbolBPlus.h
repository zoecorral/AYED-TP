#ifndef ARBOLBPLUS_H
#define ARBOLBPLUS_H
#include "Envio.h"
#include <string>

const int ORDEN = 4; // esto es para hacer que el arbol solo pueda tener hasta 4 hijos.
const int MAX_CLAVES = ORDEN - 1; //esto es el máximo de claves que puede tener (teoricamente debería tener 1 menos que el orden SIEMPRE)
struct NodoBPlus{
    bool esHoja; // a diferencia del arbol binario en este no podemos preguntar si izq o der son nulos, por lo que se opto por una estructura booleana
    int cantidadClaves;// Marca cuántos lugares ocupamos del nodo. Arranca en 0 y sube hasta 3 cuando insertamos.
    // Usamos +1 como casillero extra temporal para meter el 4° elemento de una, 
    // ordenar los 4 dentro del arreglo y recién ahí hacer la división (split) a la mitad.
    std::string claves[MAX_CLAVES + 1];  // Códigos PKG-... ordenados alfabéticamente
                                         // Básicamente son como los carteles de señalización
    NodoBPlus* hijos[ORDEN + 1];         // Son los punteros que apuntan a cada clave (solo si esHoja == false)
    Envio* envios[MAX_CLAVES + 1];      // Guarda la dirección directa al paquete real para no duplicar datos.
                                        // Si claves[i] es el código, envios[i] te lleva directo a la información completa de ese paquete.
    
    NodoBPlus* siguiente;    //Nodo que apunta al proximo elemento

    // Constructor
    NodoBPlus(bool esHoja) {
        this->esHoja = esHoja;
        this->cantidadClaves = 0;
        this->siguiente = nullptr;
        
        for (int i = 0; i <= ORDEN; i++) hijos[i] = nullptr;
        for (int i = 0; i <= MAX_CLAVES; i++) envios[i] = nullptr;
    }
};
/** @brief Indice B+ de orden 4; guarda referencias no propietarias a los envios. */
class ArbolBPlus {
private:
    NodoBPlus* raiz;  // El primer nodo del programa por el cual se inicia el programa
    int cantidad;

    void destruirRec(NodoBPlus* actual);
    int alturaRec(NodoBPlus* actual) const;
    void inOrderRec(NodoBPlus* actual) const;
    bool insertarRec(NodoBPlus* nodo, Envio* envio, std::string& claveSube, NodoBPlus*& nuevoDerecho); // Inserta y devuelve false si el codigo ya existe.
    void dividirHoja(NodoBPlus* hoja, std::string& claveSube, NodoBPlus*& nuevoDerecho);
    void dividirNodoInterno(NodoBPlus* nodo, std::string& claveSube, NodoBPlus*& nuevoDerecho);
    int profundidadHojas(NodoBPlus* actual, int nivel, bool& parejo, int& nivelHojas) const;
    void mostrarEstructuraRec (NodoBPlus* actual, int nivel) const;

public:
    ArbolBPlus();   // Inicializa la raíz en nullptr
    ArbolBPlus (const ArbolBPlus&) = delete;
    ArbolBPlus& operator=(const ArbolBPlus&) = delete;
    ~ArbolBPlus();  // Libera la memoria consumida por los nodos

    bool estaVacio() const;
    int getCantidad() const;
    int altura() const;

    //Devuelve false (sin insertar) si el envio es nulo o el código ya existe
    /** @brief Inserta un envio; devuelve false si es nulo o su codigo ya existe. */
    bool insertar(Envio* envio);

    // Busca un PKG exacto navegando de la raíz a las hojas
    /** @brief Busca un codigo y devuelve la cantidad de nodos visitados. */
    Envio* buscar(const std::string& codigo, int& visitados) const;
    Envio* buscar(const std::string& codigo) const;

    // Busca los envios con codigo dentro del intervalo, siguiendo los enlaces entre hojas. Devuelve la cantidad de envios encontrados.
    int buscarPorRango(const std::string& desde, const std::string& hasta) const;

    void recorridoInOrder() const;
    bool hojasAlMismoNivel() const;
    void mostrarEstructura() const;
};

#endif // ARBOLBPLUS_H
