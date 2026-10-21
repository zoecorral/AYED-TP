#include "ArbolBPlus.h"
#include <iostream>

// Constructor
ArbolBPlus::ArbolBPlus() : raiz(nullptr), cantidad(0) {}

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
    destruirRec(raiz); // Iniciamos el borrado recursivo pasando el nodo raiz
    this->raiz = nullptr;   // Dejamos el puntero raiz limpio en nulo por seguridad
}

// Consulta si el arbol esta vacio
bool ArbolBPlus::estaVacio() const {
    return raiz == nullptr; // Devuelve true si la raiz apunta a nulo, o false si tiene al menos un nodo
}

int ArbolBPlus::getCantidad() const {
    return cantidad; 
}

// Funcion auxiliar para calcular la altura
int ArbolBPlus::alturaRec(NodoBPlus* actual) const {
    if (actual == nullptr) return 0; // Si el nodo es nulo, la altura de esa rama es 0
    if (actual->esHoja) return 1;    // Si llegamos a una hoja, contamos 1 nivel de altura

    return 1 + alturaRec(actual->hijos[0]); // Sumamos 1 por el nivel actual y seguimos bajando por el hijo de la izquierda
}

// Devuelve la altura total del arbol
int ArbolBPlus::altura() const {
    return alturaRec(raiz); // Ejecuta el calculo de altura comenzando desde el nodo raiz
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
    if (raiz == nullptr) { // Verificamos si la raiz esta vacia
        std::cout << "  (arbol B+ vacio)\n";
        return; 
    }
    inOrderRec(raiz); // Si tiene elementos, llamamos a la funcion auxiliar desde la raiz
}

void ArbolBPlus::dividirHoja(NodoBPlus* hoja, std::string& claveSube, NodoBPlus*& nuevoDerecho) {
    NodoBPlus* derecha = new NodoBPlus(true);
    int mitad = hoja->cantidadClaves / 2;

    for (int i = mitad; i < hoja->cantidadClaves; i++) {
        derecha->claves[i - mitad] = hoja->claves[i];
        derecha->envios[i - mitad] = hoja->envios[i];
        hoja->claves[i] = "";
        hoja->envios[i] = nullptr;
    }
    derecha->cantidadClaves = hoja->cantidadClaves - mitad;
    hoja->cantidadClaves = mitad;

    derecha->siguiente = hoja->siguiente;
    hoja->siguiente = derecha;

    claveSube = derecha->claves[0];
    nuevoDerecho = derecha;
}

void ArbolBPlus::dividirNodoInterno(NodoBPlus* nodo, std::string& claveSube, NodoBPlus*& nuevoDerecho) {
    NodoBPlus* derecha = new NodoBPlus(false);
    int medio = nodo->cantidadClaves / 2;

    claveSube = nodo->claves[medio];

    int j = 0;
    for (int i = medio + 1; i < nodo->cantidadClaves; i++) {
        derecha->claves[j] = nodo->claves[i];
        derecha->hijos[j] = nodo->hijos[i];
        j++;
    }
    derecha->hijos[j] = nodo->hijos[nodo->cantidadClaves];
    derecha->cantidadClaves = j;

    for (int i = medio; i < nodo->cantidadClaves; i++) {
        nodo->claves[i] = "";
        nodo->hijos[i + 1] = nullptr;
    }
    nodo->cantidadClaves = medio;

    nuevoDerecho = derecha;
}

bool ArbolBPlus::insertarRec(NodoBPlus* nodo, Envio* envio, std::string& claveSube, NodoBPlus*& nuevoDerecho) {
    const std::string& codigo = envio->getCodigo();
    nuevoDerecho = nullptr;

    if (nodo->esHoja) {
        int i = 0;
        while (i < nodo->cantidadClaves && nodo->claves[i] < codigo) i++;

        if (i < nodo->cantidadClaves && nodo->claves[i] == codigo) return false;

        for (int j = nodo->cantidadClaves; j > i; j--) {
            nodo->claves[j] = nodo->claves[j - 1];
            nodo->envios[j] = nodo->envios[j - 1];
        }
        nodo->claves[i] = codigo;
        nodo->envios[i] = envio;
        nodo->cantidadClaves++;

        if (nodo->cantidadClaves > MAX_CLAVES) dividirHoja(nodo, claveSube, nuevoDerecho);
        return true;
    }

    int i = 0;
    while (i < nodo->cantidadClaves && codigo >= nodo->claves[i]) i++;

    std::string claveHijo;
    NodoBPlus* derechoHijo = nullptr;
    if (!insertarRec(nodo->hijos[i], envio, claveHijo, derechoHijo)) return false;

    if (derechoHijo != nullptr) {
        for (int j = nodo->cantidadClaves; j > i; j--) {
            nodo->claves[j] = nodo->claves[j - 1];
            nodo->hijos[j + 1] = nodo->hijos[j];
        }
        nodo->claves[i] = claveHijo;
        nodo->hijos[i + 1] = derechoHijo;
        nodo->cantidadClaves++;

        if (nodo->cantidadClaves > MAX_CLAVES) dividirNodoInterno(nodo, claveSube, nuevoDerecho);
    }
    return true;
}

bool ArbolBPlus::insertar(Envio* envio){ //función para insertar datos en el arbol
    if (envio == nullptr){ //caso base: envio inexistente
        return false;
    }
    if (raiz == nullptr){ //caso base 2: cuando la raiz apunta al nulo desde un comienzo, es decir, cuando no hay mas nodos
        raiz = new NodoBPlus(true); //creamos una hoja inicial
        raiz->claves[0] = envio->getCodigo(); //guarda la clave del envio del usuario
        raiz->envios[0] = envio;
        raiz->cantidadClaves = 1; //aumentamos la cantidad de claves que hay en el arbol
        cantidad++;
        return true; //hacemos el return luego de la inserción
    }
    std::string claveSube;
    NodoBPlus* nuevoDerecho = nullptr;
    if (!insertarRec(raiz, envio, claveSube, nuevoDerecho)) return false; // el codigo ya existia

    if (nuevoDerecho != nullptr) {
        NodoBPlus* nuevaRaiz = new NodoBPlus(false);
        nuevaRaiz->claves[0] = claveSube;
        nuevaRaiz->hijos[0] = raiz;
        nuevaRaiz->hijos[1] = nuevoDerecho;
        nuevaRaiz->cantidadClaves = 1;
        raiz = nuevaRaiz;
    }
    cantidad++;
    return true;
}

// Busca un envio por su codigo buscando de la raiz a las hojas
Envio* ArbolBPlus::buscar(const std::string& codigo, int& visitados) const {
    visitados = 0;
    if (raiz == nullptr) 
    {
        return nullptr; // Si el arbol esta vacio se detiene porque no hay nada que buscar
    }
    NodoBPlus* cursor = raiz; // Creamos un puntero temporal para movernos sin perder la raiz
    visitados++;

    while (!cursor->esHoja) { // Mientras no hayamos llegado a una hoja
        int i = 0; // Inicializamos un indice de busqueda en 0
        while (i < cursor->cantidadClaves && codigo >= cursor->claves[i]) { // Avanzamos mientras la clave buscada sea mayor o igual a la clave del nodo
            i++; // Incrementamos la posicion
        }
        cursor = cursor->hijos[i]; 
        visitados++;
    }

    for (int i = 0; i < cursor->cantidadClaves; i++) { // Una vez parados en la hoja, recorremos sus claves
        if (cursor->claves[i] == codigo) { // Comparamos si encontramos el codigo
            return cursor->envios[i]; // Retornamos la direccion del objeto Envio asociado
        }
    }
    return nullptr; // Si termino de revisar la hoja y no estaba el codigo, retornamos nulo (para asegurarnos que cubra algun error)
}

Envio* ArbolBPlus::buscar(const std::string& codigo) const {
    int visitados;
    return buscar(codigo, visitados);
}

// Realiza una busqueda por rango dentro del intervalo [desde, hasta]
int ArbolBPlus::buscarPorRango(const std::string& desde, const std::string& hasta) const {
    if (raiz == nullptr) {
        std::cout << "  (arbol B+ vacio)\n";
        return 0;
    }
    
    if (desde > hasta) {
        std::cout << "  Rango invalido: 'desde' es mayor que 'hasta'.\n";
        return 0;
    }
    
    NodoBPlus* cursor = raiz; // Creamos el puntero auxiliar

    while (!cursor->esHoja) { // Descendemos por las claves internas para ubicar la hoja inicial
        int i = 0; // Arrancamos la comparacion en la primera clave
        while (i < cursor->cantidadClaves && desde >= cursor->claves[i]) { // Comparamos la clave con el limite inferior 'desde'
            i++;
        }
        cursor = cursor->hijos[i]; // Bajamos al hijo correspondiente
    }

    std::cout << "Resultados en rango [" << desde << " - " << hasta << "]\n";
   
    int encontrados = 0;
    while (cursor != nullptr) {
        for (int i = 0; i < cursor->cantidadClaves; i++) {
            if (cursor->claves[i] > hasta) {
                if (encontrados == 0) std::cout << "  (sin resultados)\n";
                return encontrados;
            }
            if (cursor->claves[i] >= desde) {
                std::cout << "  ";
                cursor->envios[i]->mostrar();
                encontrados++;
            }
        }
        cursor = cursor->siguiente; // Nos movemos a la hoja siguiente    
    }
    
    if (encontrados == 0) {
        std::cout << "  (sin resultados)\n";
    }
    return encontrados;
}

// Verifica el nivel de las hojas, Parejo pasa a false si difieren
int ArbolBPlus::profundidadHojas(NodoBPlus* actual, int nivel, bool& parejo, int& nivelHojas) const {
    if (actual == nullptr) return 0;
 
    if (actual->esHoja) {
        if (nivelHojas == -1) nivelHojas = nivel;
        else if (nivelHojas != nivel) parejo = false;
        return 1;
    }
 
    int hojas = 0;
    for (int i = 0; i <= actual->cantidadClaves; i++) {
        hojas += profundidadHojas(actual->hijos[i], nivel + 1, parejo, nivelHojas);
    }
    return hojas;
}

bool ArbolBPlus::hojasAlMismoNivel() const {
    bool parejo = true;
    int nivelHojas = -1;
    profundidadHojas(raiz, 1, parejo, nivelHojas);
    return parejo;
}

// imprimir jerarquicamente la estructura interna del B+
void ArbolBPlus::mostrarEstructuraRec(NodoBPlus* actual, int nivel) const {
    if (actual == nullptr) return;
 
    for (int i = 0; i < nivel; i++) std::cout << "    ";
    if (actual->esHoja) {
        std::cout << "Hoja (";
        for (int i = 0; i < actual->cantidadClaves; i++) {
            std::cout << actual->claves[i] << (i + 1 < actual->cantidadClaves ? ", " : "");
        }
        std::cout << ")\n";
    } else {
        std::cout << "Interno [";
        for (int i = 0; i < actual->cantidadClaves; i++) {
            std::cout << actual->claves[i] << (i + 1 < actual->cantidadClaves ? " | " : "");
        }
        std::cout << "]\n";
        for (int i = 0; i <= actual->cantidadClaves; i++) {
            mostrarEstructuraRec(actual->hijos[i], nivel + 1);
        }
    }
}

void ArbolBPlus::mostrarEstructura() const {
    if (raiz == nullptr) {
        std::cout << "  (arbol B+ vacio)\n";
        return;
    }
    mostrarEstructuraRec(raiz, 0);
}
