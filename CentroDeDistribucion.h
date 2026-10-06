#pragma once
#include "ListaDeEnvios.h"
#include "ListaPendientes.h"
#include "ArbolBST.h"
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
    ArbolBST indice;
    MesaDeClasificacion mesa;
    Queue colaEscaneo;
    Stack pilaDespacho;

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
            std::cout << e->getCodigo() << std::endl; // Imprime 'ENV-2', luego 'ENV-1'
        }
    }

    void encolarEscaneo(const std::string& id) {
        Envio* e = new Envio(id, "Desconocido", "General", 0.0, NivelServicio::ESTANDAR);
        colaEscaneo.enqueue(e);
    }

    void desencolarEscaneo() {
        if (!colaEscaneo.isEmpty()) {
            Envio* e = colaEscaneo.dequeue();
            std::cout << e->getCodigo() << std::endl; // Imprime 'PKG-A', luego 'PKG-B'
        }
    }

    void busquedaLinealMesa(const std::string& id) {
        mesa.compararBusquedas(id);
    }

    void busquedaBinariaMesa(const std::string& id) {
        // En tu clase la búsqueda binaria se realiza por código std::string:
        mesa.busquedaBinaria(id);
    }
};
