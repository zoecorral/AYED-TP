#pragma once
#include "ListaDeEnvios.h"
#include "ListaPendientes.h"
#include "ArbolBST.h"
#include "ArbolBPlus.h"
#include "Set.h"
#include "MesaDeClasificacion.h"
#include "Queue.h"
#include "Stack.h"
#include <iostream>
#include <string>

// Controlador principal: coordina el registro global de envios y la cola
// de pendientes, y gestiona la memoria en cascada.
class CentroDeDistribucion {
private:
    ListaDeEnvios registro;      // duena de todos los Envio* (pendientes y despachados)
    ListaPendientes pendientes;  // cola de prioridad; solo referencia envios del registro
    ArbolBST indice;              // Indice anterior, se conserva para la funcionalidad existente.
    ArbolBPlus indiceAvanzado;     // Indice B+: no es dueño de los Envio, solo los referencia.
    MesaDeClasificacion mesa;
    Queue colaEscaneo;
    Stack pilaDespacho;

    // Traduce los bits activos a nombres de estados para las consultas.
    void mostrarEstadosDeBits(const BitSet& bits) const {
        bool primero = true;
        for (int i = 0; i <= 4; i++) {
            if (bits.test(i)) {
                if (!primero) std::cout << ", ";
                std::cout << estadoToString(static_cast<Estado>(i));
                primero = false;
            }
        }
        if (primero) std::cout << "(ninguno)";
        std::cout << "\n";
    }

public:
    CentroDeDistribucion() = default;

    // Al destruirse, `registro` libera en cascada todos los Envio* (y sus
    // historiales); `pendientes` solo libera sus propios nodos.
    ~CentroDeDistribucion() = default;

    // RF01 — Registrar nuevo envio
    void registrarEnvio(const std::string& cod, const std::string& dest, const std::string& zona, double peso, NivelServicio nivel) {
        if (registro.existeCodigo(cod)) {
            std::cout << "Error: ya existe un envio con codigo " << cod << "\n";
            return;
        }
        Envio* e = new Envio(cod, dest, zona, peso, nivel);
        registro.agregar(e);
        indice.insertar(e);
        indiceAvanzado.insertar(e);
        pendientes.agregar(e);
        std::cout << "Envio " << cod << " registrado correctamente.\n";
    }

    // RF02 — Mostrar pendientes
    void mostrarPendientes() const {
        std::cout << "=== ENVIOS PENDIENTES ===\n";
        pendientes.mostrar();
    }

    // RF03 — Buscar envio (entre TODOS los conocidos, no solo pendientes)
    void buscarEnvio(const std::string& codigo) const {
        Envio* e = registro.buscar(codigo);
        if (e == nullptr) {
            std::cout << "Envio no encontrado.\n";
            return;
        }
        e->mostrar();
    }

    // RF04 — Cambiar estado (tambien cubre RF07: marcar ENTREGADO).
    // Si el nuevo estado es ENTREGADO, se lo saca de pendientes por las
    // dudas (por si todavia no habia pasado por despacharProximo): un envio
    // entregado nunca debe seguir figurando como pendiente.
    void cambiarEstado(const std::string& codigo, Estado nuevoEstado, const std::string& obs) {
        Envio* e = registro.buscar(codigo);
        if (e == nullptr) { std::cout << "Envio no encontrado.\n"; return; }
        e->cambiarEstado(nuevoEstado, obs);
        if (nuevoEstado == Estado::ENTREGADO) pendientes.remover(e);
        std::cout << "Estado actualizado.\n";
    }

    // RF05 — Despachar proximo envio (el primer nodo de pendientes)
    void despacharProximo() {
        Envio* e = pendientes.despachar();
        if (e == nullptr) { std::cout << "No hay envios pendientes.\n"; return; }
        e->cambiarEstado(Estado::EN_REPARTO, "Despachado del centro");
        std::cout << "Despachado: " << e->getCodigo() << " -> " << e->getDestinatario() << "\n";
    }

    // RF06 — Reprogramar envio: vuelve a pendientes respetando su prioridad.
    // Un envio ENTREGADO nunca puede volver a pendientes. Se lo saca primero
    // de pendientes (remover es no-op si no estaba) para que reprogramar un
    // envio que todavia no fue despachado no lo deje duplicado en la lista.
    void reprogramarEnvio(const std::string& codigo, const std::string& motivo) {
        Envio* e = registro.buscar(codigo);
        if (e == nullptr) { std::cout << "Envio no encontrado.\n"; return; }
        if (e->estaEntregado()) { std::cout << "El envio ya fue entregado, no puede reprogramarse.\n"; return; }
        e->sumarIntento();
        e->cambiarEstado(Estado::REPROGRAMADO, motivo);
        pendientes.remover(e);
        pendientes.agregar(e);
        std::cout << "Envio " << codigo << " reprogramado (intento " << e->getIntentos() << ").\n";
    }

    // RF08 — Mostrar historial bidireccional
    void mostrarHistorial(const std::string& codigo) const {
        Envio* e = registro.buscar(codigo);
        if (e == nullptr) { std::cout << "Envio no encontrado.\n"; return; }
        std::cout << "--- Historial cronologico (antiguo -> reciente) ---\n";
        e->mostrarHistorialCronologico();
        std::cout << "--- Historial inverso (reciente -> antiguo) ---\n";
        e->mostrarHistorialInverso();
    }

    // Resumen recursivo por zona (sobre los envios pendientes)
    void resumenZona(const std::string& zona) const {
        ListaPendientes::ResumenZona r = pendientes.resumenPorZona(zona);
        std::cout << "Zona: " << zona << "\n";
        std::cout << "  Cantidad de paquetes: " << r.cantidad << "\n";
        std::cout << "  Peso total pendiente: " << r.pesoTotal << " kg\n";
        std::cout << "  Cantidad EXPRESS: " << r.cantExpress << "\n";
    }

    // Desafio adicional — envio de mayor peso pendiente de una zona
    void envioMasPesadoDeZona(const std::string& zona) const {
        Envio* e = pendientes.envioMasPesadoDeZona(zona);
        if (e == nullptr) { std::cout << "No hay envios pendientes en la zona " << zona << ".\n"; return; }
        std::cout << "Zona consultada: " << zona << "\n";
        std::cout << "Envio mas pesado: " << e->getCodigo() << "\n";
        std::cout << "Peso: " << e->getPeso() << " kg\n";
    }

    void buscarEnBST(const std::string& cod) const {
        int visitados;
        std::cout << "Buscar: " << cod << "\n";
        Envio* e = indice.buscar(cod, visitados);
        if (e == nullptr) std::cout << "Envio no encontrado en el BST.\n";
        else e->mostrar();
        std::cout << "Nodos visitados: " << visitados << "\n";
    }

    /** @brief Busca un envio mediante el indice B+ e informa los nodos visitados. */
    void buscarEnBPlus(const std::string& codigo) const {
        int visitados = 0;
        Envio* e = indiceAvanzado.buscar(codigo, visitados);
        if (e == nullptr) {
            std::cout << "Envio no encontrado en el arbol B+.\n";
        } else {
            e->mostrar();
        }
        std::cout << "Nodos B+ visitados: " << visitados << "\n";
    }

    /** @brief Consulta un rango inclusivo de codigos usando los enlaces entre hojas. */
    void buscarRangoBPlus(const std::string& desde, const std::string& hasta) const {
        indiceAvanzado.buscarPorRango(desde, hasta);
    }

    /** @brief Muestra los estados historicos registrados en el BitSet de un envio. */
    void mostrarBitSetEnvio(const std::string& codigo) const {
        Envio* e = indiceAvanzado.buscar(codigo);
        if (e == nullptr) {
            std::cout << "Envio no encontrado.\n";
            return;
        }

        const BitSet& bits = e->getEstadosRegistrados();
        std::cout << "Estados historicos de " << codigo << " (bits 4..0): ";
        bits.mostrar();
        std::cout << "Estados alcanzados: ";
        mostrarEstadosDeBits(bits);
        std::cout << "Cantidad de estados diferentes: " << bits.count() << "\n";
        std::cout << "Paso alguna vez por REPROGRAMADO: "
                  << (e->estuvoReprogramado() ? "SI" : "NO") << "\n";
    }

    /** @brief Compara dos BitSet: AND para estados comunes y XOR para los exclusivos. */
    void compararBitSets(const std::string& codigoA, const std::string& codigoB) const {
        Envio* a = indiceAvanzado.buscar(codigoA);
        Envio* b = indiceAvanzado.buscar(codigoB);
        if (a == nullptr || b == nullptr) {
            std::cout << "No se pueden comparar: verifica ambos codigos.\n";
            return;
        }

        BitSet comunes = a->getEstadosRegistrados() & b->getEstadosRegistrados();
        BitSet diferentes = a->getEstadosRegistrados() ^ b->getEstadosRegistrados();
        std::cout << "Estados en comun entre " << codigoA << " y " << codigoB << ": ";
        mostrarEstadosDeBits(comunes);
        std::cout << "Estados que aparecen en solo uno de los dos: ";
        mostrarEstadosDeBits(diferentes);
    }

    /** @brief Devuelve un Set con los codigos de todos los envios de una zona. */
    Set conjuntoPorZona(const std::string& zona) const {
        Set resultado;
        for (const NodoEnvio* nodo = registro.primero(); nodo != nullptr; nodo = nodo->siguiente) {
            if (nodo->envio->getZona() == zona) resultado.insert(nodo->envio->getCodigo());
        }
        return resultado;
    }

    /** @brief Devuelve un Set con los codigos de envios de un nivel de servicio. */
    Set conjuntoPorServicio(NivelServicio nivel) const {
        Set resultado;
        for (const NodoEnvio* nodo = registro.primero(); nodo != nullptr; nodo = nodo->siguiente) {
            if (nodo->envio->getNivel() == nivel) resultado.insert(nodo->envio->getCodigo());
        }
        return resultado;
    }

    /** @brief Muestra los codigos de un Set y recupera los datos completos desde el B+. */
    void mostrarConjunto(const Set& conjunto, const std::string& nombre) const {
        std::cout << nombre << " (" << conjunto.size() << " envios): ";
        conjunto.mostrar();
        for (const NodoSet* nodo = conjunto.primero(); nodo != nullptr; nodo = nodo->siguiente) {
            Envio* e = indiceAvanzado.buscar(nodo->clave);
            if (e != nullptr) {
                std::cout << "  ";
                e->mostrar();
            } else {
                // Si ocurre esto, el Set y el índice no están sincronizados.
                std::cout << "  Error: el codigo " << nodo->clave << " no esta en el B+.\n";
            }
        }
    }

    /** @brief Muestra A, B, union, interseccion y diferencia con los datos de cada envio. */
    void mostrarOperacionesConjuntos(const Set& a, const Set& b) const {
        Set unionAB = Set::unionDe(a, b);
        Set interseccionAB = Set::interseccion(a, b);
        Set diferenciaAB = Set::diferencia(a, b);
        mostrarConjunto(a, "A");
        mostrarConjunto(b, "B");
        mostrarConjunto(unionAB, "A union B");
        mostrarConjunto(interseccionAB, "A interseccion B");
        mostrarConjunto(diferenciaAB, "A - B");
    }

    void inOrderBST() const { indice.recorridoInOrder(); }
    void alturaBST() const { std::cout << "Altura del BST: " << indice.altura() << "\n"; }

    void colocarEnMesa(const std::string& id, double peso) {
        Envio* nuevoEnvio = new Envio(id, "Desconocido", "General", peso, NivelServicio::ESTANDAR);
        mesa.agregarEnvio(nuevoEnvio);
    }

    void ordenarMesaPorPeso() {
        mesa.ordenarLote(criterio::PESO);
        mesa.mostrarLote();
    }

    void apilarDespacho(const std::string& id) {
        Envio* e = new Envio(id, "Desconocido", "General", 0.0, NivelServicio::ESTANDAR);
        pilaDespacho.push(e);
    }

    void desapilarDespacho() {
        if (!pilaDespacho.isEmpty()) {
            Envio* e = pilaDespacho.top();
            pilaDespacho.pop();
            std::cout << e->getCodigo() << std::endl; 
        }
    }

    void encolarEscaneo(const std::string& id) {
        Envio* e = new Envio(id, "Desconocido", "General", 0.0, NivelServicio::ESTANDAR);
        colaEscaneo.enqueue(e);
    }

    void desencolarEscaneo() {
        if (!colaEscaneo.isEmpty()) {
            Envio* e = colaEscaneo.dequeue();
            std::cout << e->getCodigo() << std::endl; 
        }
    }

    void busquedaLinealMesa(const std::string& id) {
        mesa.compararBusquedas(id);
    }

    void busquedaBinariaMesa(const std::string& id) {
        mesa.busquedaBinaria(id);
    }
};
