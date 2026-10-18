#include "ArbolBPlus.h"
//#include "Envio.h"
#include <iostream>

// Constructor
ArbolBPlus::ArbolBPlus() {
    this->raiz = nullptr; // Inicializamos el puntero raiz en nulo
}

// Funcion auxiliar para liberar la memoria de los nodos
void ArbolBPlus::destruirRec(NodoBPlus* actual) {
    if (actual == nullptr) return; // Si el nodo al que llegamos no existe, salimos inmediatamente

    if (!actual->esHoja) { // Preguntamos si el nodo actual no es una hoja
        for (int i = 0; i <= actual->cantidadClaves; i++) { // Recorremos todos los punteros a hijos que tiene este nodo
            destruirRec(actual->hijos[i]); //Llamamos recursivamente a la funcion para limpiar cada hijo
        }
    }

    delete actual;
}

//Destructor
ArbolBPlus::~ArbolBPlus() {
    destruirRec(this->raiz); // Iniciamos el borrado recursivo pasando el nodo raiz
    this->raiz = nullptr;   // Dejamos el puntero raiz limpio en nulo por seguridad
}

// Consulta si el arbol esta vacio
bool ArbolBPlus::estaVacio() const {
    return this->raiz == nullptr; // Devuelve true si la raiz apunta a nulo, o false si tiene al menos un nodo
}

// Funcion auxiliar para calcular la altura
int ArbolBPlus::alturaRec(NodoBPlus* actual) const {
    if (actual == nullptr) return 0; // Si el nodo es nulo, la altura de esa rama es 0
    if (actual->esHoja) return 1;    // Si llegamos a una hoja, contamos 1 nivel de altura

    return 1 + alturaRec(actual->hijos[0]); // Sumamos 1 por el nivel actual y seguimos bajando por el hijo de la izquierda
}

// Devuelve la altura total del arbol
int ArbolBPlus::altura() const {
    return alturaRec(this->raiz); // Ejecuta el calculo de altura comenzando desde el nodo raiz
}

// Función auxiliar recursiva para mostrar los datos de las hojas
void ArbolBPlus::inOrderRec(NodoBPlus* actual) const {
    if (actual == nullptr) 
    {
        return; // Si el nodo actual es nulo, no hay nada que imprimir
    }
    else if (actual->esHoja) { // Si el nodo es una hoja, es donde estan guardados los punteros a los envios reales
        for (int i = 0; i < actual->cantidadClaves; i++) { 
            if (actual->envios[i] != nullptr) { // Verificamos que la referencia al paquete exista
                std::cout << "  ";             
                actual->envios[i]->mostrar();  // Ejecutamos el método mostrar()
            }
        }
    } else { // Si es un nodo interno con claves
        for (int i = 0; i <= actual->cantidadClaves; i++) { // Recorremos todas las ramas o hijos que posee
            inOrderRec(actual->hijos[i]); // Llamamos recursivamente para procesar cada rama en orden
        }
    }
}

// Imprime el contenido completo del arbol en pantalla
void ArbolBPlus::recorridoInOrder() const {
    if (this->raiz == nullptr) { // Verificamos si la raiz esta vacia
        std::cout << "  (arbol B+ vacio)\n";
        return; 
    }
    inOrderRec(this->raiz); // Si tiene elementos, llamamos a la funcion auxiliar desde la raiz
}

// Busca un envio por su codigo buscando de la raiz a las hojas
Envio* ArbolBPlus::buscar(const std::string& codigo) const {
    if (this->raiz == nullptr) 
    {
        return nullptr; // Si el arbol esta vacio se detiene porque no hay nada que buscar
    }
    NodoBPlus* cursor = this->raiz; // Creamos un puntero temporal para movernos sin perder la raiz

    while (!cursor->esHoja) { // Mientras no hayamos llegado a una hoja
        int i = 0; // Inicializamos un indice de busqueda en 0
        while (i < cursor->cantidadClaves && codigo >= cursor->claves[i]) { // Avanzamos mientras la clave buscada sea mayor o igual a la clave del nodo
            i++; // Incrementamos la posicion
        }
        cursor = cursor->hijos[i];
    }

    for (int i = 0; i < cursor->cantidadClaves; i++) { // Una vez parados en la hoja, recorremos sus claves
        if (cursor->claves[i] == codigo) { // Comparamos si encontramos el codigo
            return cursor->envios[i]; // Retornamos la direccion del objeto Envio asociado
        }
    }

    return nullptr; // Si termino de revisar la hoja y no estaba el codigo, retornamos nulo (para asegurarnos que cubra algun error)
}

// Realiza una busqueda por rango dentro del intervalo [desde, hasta]
void ArbolBPlus::buscarPorRango(const std::string& desde, const std::string& hasta) const {
    if (this->raiz == nullptr) 
    {
        return; // Si el arbol esta vacio salimos
    }
    NodoBPlus* cursor = this->raiz; // Creamos el puntero auxiliar

    while (!cursor->esHoja) { // Descendemos por las claves internas para ubicar la hoja inicial
        int i = 0; // Arrancamos la comparacion en la primera clave
        while (i < cursor->cantidadClaves && desde >= cursor->claves[i]) { // Comparamos la clave con el limite inferior 'desde'
            i++;
        }
        cursor = cursor->hijos[i]; // Bajamos al hijo correspondiente
    }

    std::cout << "Resultados en rango [" << desde << " - " << hasta << "]\n"; 

    while (cursor != nullptr) { // Recorremos las hojas secuencialmente gracias a la lista enlazada
        for (int i = 0; i < cursor->cantidadClaves; i++) { // Revisamos todas las claves guardadas en la hoja actual
            if (cursor->claves[i] >= desde && cursor->claves[i] <= hasta) { // Verificamos si la clave entra en el rango
                if (cursor->envios[i] != nullptr) { // Confirmamos que el puntero al envio sea valido
                    cursor->envios[i]->mostrar(); // Mostramos en pantalla los datos del paquete
                }
            }
            if (cursor->claves[i] > hasta) { // Si la clave de la hoja supera el limite maximo 'hasta'
                return; 
            }
        }
        cursor = cursor->siguiente; // Nos movemos a la hoja siguiente
    }
}
void ArbolBPlus::insertar(Envio* envio){ //función para insertar datos en el arbol
    if (envio == nullptr){ //caso base: envio inexistente
        return;
    }
    if (raiz == nullptr){ //caso base 2: cuando la raiz apunta al nulo desde un comienzo, es decir, cuando no hay mas nodos
        raiz = new NodoBPlus(true); //creamos una hoja inicial
        raiz -> claves[0] = envio -> getCodigo(); //guarda la clave del envio del usuario
        raiz->envios[0] = envio;
        raiz->cantidadClaves = 1; //aumentamos la cantidad de claves que hay en el arbol
        return; //hacemos el return luego de la inserción
    }
    insertarAux(raiz, envio); //hacemos un caso recursivo con otra función
}
void ArbolBPlus::insertarAux(NodoBPlus* nodo, Envio* envio){ // se utiliza para el caso recursivo
    int i = 0;

    // 1. CASO BASE: Si llegamos a una hoja
    if (nodo->esHoja){ 
        // Buscamos la posición 'i' donde debe ir la nueva clave
        while (i < nodo->cantidadClaves && envio->getCodigo() > nodo->claves[i]){
            i++;
        }
        // Desplazamos elementos a la derecha para abrir espacio
        for (int j = nodo->cantidadClaves; j > i; j--){
            nodo->claves[j] = nodo->claves[j - 1];
            nodo->envios[j] = nodo->envios[j - 1];
        }
        // Guardamos el envío y aumentamos el contador
        nodo->claves[i] = envio->getCodigo();
        nodo->envios[i] = envio;
        nodo->cantidadClaves++;
        
        return; // Salimos de la recursión
    }
    // 2. CASO RECURSIVO: Si el nodo es interno
    while (i < nodo->cantidadClaves){
        if (nodo->claves[i] < envio->getCodigo()){
            i++;
        } else {
            break;
        } 
    }
    // Llamada recursiva para bajar al hijo correspondiente
    insertarAux(nodo->hijos[i], envio);
}
Envio* ArbolBPlus::buscar(std::string codigo) {
    if (raiz == nullptr) return nullptr;
    return buscarAux(raiz, codigo);
}

Envio* ArbolBPlus::buscarAux(NodoBPlus* nodo, std::string codigo) {
    int i = 0;
    while (i < nodo->cantidadClaves && codigo > nodo->claves[i]) i++;

    if (nodo->esHoja) {
        if (i < nodo->cantidadClaves && nodo->claves[i] == codigo) {
            return nodo->envios[i];
        }
        return nullptr;
    }

    return buscarAux(nodo->hijos[i], codigo);
}
