#ifndef ARBOLBPLUS_H
#define ARBOLBPLUS_H
#include <string>
class Envio {
public:
    void mostrar() const {}
    std::string getCodigo() const { return ""; }
};
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
//Clase principal del programa
class ArbolBPlus {
private:
    NodoBPlus* raiz;  // El primer nodo del programa por el cual se inicia el programa
    void insertarAux(NodoBPlus* nodo, Envio* envio); //funcion recursiva auxiliar para el insertar
    void destruirRec(NodoBPlus* actual);
    int alturaRec(NodoBPlus* actual) const;
    void inOrderRec(NodoBPlus* actual) const;
    // Funciones internas para partir nodos cuando se llenan
    void dividirHoja(NodoBPlus* padre, int indiceHijo, NodoBPlus* hijo);
    void dividirNodoInterno(NodoBPlus* padre, int indiceHijo, NodoBPlus* hijo);
    void insertarEnNodoNoLleno(NodoBPlus* nodo, Envio* envio);
    Envio* buscarAux(NodoBPlus* nodo, std::string codigo);

public:
    ArbolBPlus();   // Inicializa la raíz en nullptr
    ~ArbolBPlus();  // Libera la memoria consumida por los nodos
    bool estaVacio() const;
    Envio* buscar(std::string codigo);
    int altura() const;
    void recorridoInOrder() const;
    //Inserta un envío manteniendo el orden y balanceo
    void insertar(Envio* envio);

    // Busca un PKG exacto navegando de la raíz a las hojas
    Envio* buscar(const std::string& codigo) const;

    // Busca la hoja inicial y recorre la lista enlazada juntando envíos
    void buscarPorRango(const std::string& desde, const std::string& hasta) const;
};

#endif // ARBOLBPLUS_H
