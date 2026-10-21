# AYED-TP

📦 **Trabajo Práctico Integrador:** Estructuras de Datos y POO en C++  
Sistema de gestión de envíos de última milla.

---

## 📌 Info del TP

| Campo | Detalle |
| :--- | :--- |
| 🎯 **Modalidad** | Trabajo en equipos |
| ⏱️ **Duración estimada** | 2 horas |
| 🧩 **Temas integrados** | Programación Orientada a Objetos, recursividad, listas simplemente enlazadas, listas doblemente enlazadas, Stack, Queue, árboles binarios de búsqueda (BST/ABB), algoritmos de ordenamiento, búsqueda lineal, búsqueda binaria y análisis de complejidad Big O |
| 📤 **Entrega** | Google Docs + código fuente |

---

## ✨ Objetivos

- Diseñar una solución orientada a objetos.
- Implementar una lista simplemente enlazada propia.
- Implementar una lista doblemente enlazada propia.
- Implementar una Stack propia.
- Implementar una Queue propia.
- Implementar un árbol binario de búsqueda (BST/ABB) propio como índice por código de seguimiento.
- Implementar al menos un algoritmo de ordenamiento manual.
- Implementar y comparar búsqueda lineal y búsqueda binaria.
- Utilizar memoria dinámica mediante new y delete.
- Manipular las estructuras mediante raw pointers.
- Resolver al menos una operación mediante recursividad.
- Analizar la complejidad temporal de las principales operaciones utilizando notación Big O.
- Justificar decisiones de diseño y gestión de memoria.

---

## ⛔ Restricciones Técnicas

- ❌ **Prohibido:** `std::list`, `std::forward_list`, `std::vector`, `std::deque`, `std::stack`, `std::queue`, `std::priority_queue`, `std::std::map/ std::multimapstd::map / std::multimap`, `std::set / std::multiset`, `std::sort`, `std::stable_sort`, `std::binary_search`, `std::lower_bound / std::upper_bound`, smart pointers y bibliotecas, contenedores o algoritmos de la STL que reemplacen las estructuras y algoritmos solicitados.
- ✅ **Obligatorio:** Nodos propios, raw pointers, `new`/`delete` explícitos, destructores, implementación manual de Stack y Queue, implementación manual de un árbol binario de búsqueda (BST/ABB), implementación manual del algoritmo de ordenamiento elegido, implementación manual de búsqueda lineal y búsqueda binaria.

---

## 🛠️ Compilar y ejecutar

```bash
g++ -std=c++17 -Wall -Wextra -o hubflow *.cpp
./hubflow
```

---

## 📁 Estructura

- `*.cpp` / `*.hpp` — código fuente del sistema.

---

## Ampliación: consultas avanzadas de envíos

La opción **11. Consultas avanzadas de envíos** integra las estructuras nuevas sin reemplazar el menú anterior:

- **Árbol B+ de orden 4:** indexa los envíos por código. Las hojas guardan el código y un `Envio*`, y están enlazadas para recorrer rangos en orden. La búsqueda individual informa cuántos nodos visitó. No se permiten códigos duplicados.
- **BitSet:** cada objeto `Envio` mantiene cinco bits históricos: `RECIBIDO`, `CLASIFICADO`, `EN_REPARTO`, `REPROGRAMADO` y `ENTREGADO`. El constructor activa `RECIBIDO` y cada cambio de estado activa el bit correspondiente sin desactivar los anteriores.
- **Set:** lista simplemente enlazada ordenada de códigos sin duplicados. Permite insertar, eliminar, consultar pertenencia, unión, intersección y diferencia. Los resultados recuperan la información completa mediante el índice B+.

### Gestión de memoria

- `ListaDeEnvios registro` es propietaria de los objetos `Envio` registrados en HubFlow.
- `ListaDeEnvios enviosAuxiliares` es propietaria de los objetos creados por métodos auxiliares antiguos para la mesa, la pila y la cola. Así siguen vivos mientras esas estructuras los referencian y se liberan al destruir el centro.
- `ArbolBST`, `ArbolBPlus`, `ListaPendientes`, la mesa, la pila y la cola no destruyen los objetos que referencian. Cada estructura libera únicamente sus propios nodos o arreglos.
- El `BitSet` es un miembro de `Envio`, por lo que su vida útil coincide con la del envío.
- Cada `Set` es propietario de sus nodos y guarda copias de los códigos, no punteros a `Envio`.
- El orden de los miembros de `CentroDeDistribucion` hace que las estructuras no propietarias se destruyan antes que `enviosAuxiliares` y `registro`, que liberan los objetos.

### Complejidad temporal

Sea `n` la cantidad de envíos o códigos de un conjunto, `m` la cantidad del segundo conjunto y `k` la cantidad de resultados del rango. El orden del B+ es fijo (4) y mantiene todas las hojas a la misma profundidad.

| Estructura | Operación | Mejor caso | Peor caso |
|---|---|---:|---:|
| B+ | Insertar | O(log n) | O(log n) |
| B+ | Buscar por código | O(log n) | O(log n) |
| B+ | Buscar por rango | O(log n) | O(log n + k), hasta O(n) |
| BitSet | `set`, `reset`, `test` | O(1) | O(1) |
| BitSet | `count` | O(1) | O(1) |
| BitSet | AND, OR, XOR | O(1) | O(1) |
| Set | Insertar | O(1) | O(n) |
| Set | Eliminar | O(1) | O(n) |
| Set | Pertenencia (`contains`) | O(1) | O(n) |
| Set | Unión | O(1) | O(n + m) |
| Set | Intersección | O(1) | O(n + m) |
| Set | Diferencia | O(1) | O(n + m) |

El mejor caso de las operaciones básicas de `Set` ocurre cuando el elemento está al comienzo o el conjunto está vacío. Para las operaciones entre conjuntos, el mejor caso constante corresponde a entradas vacías; el peor caso recorre las dos listas ordenadas. `count()` y las operaciones del `BitSet` son O(1) porque el número de bits está fijado en cinco.

### Compilar y ejecutar los tests

```bash
cmake -S . -B build
cmake --build build -j4
ctest --test-dir build --output-on-failure
```

La suite `ConsultasAvanzadas` prueba divisiones del B+, búsquedas, rangos, duplicados, los cinco estados históricos, operaciones de bits, conjuntos, integración con el registro y el ciclo de vida de los envíos creados por los métodos auxiliares. La suite nueva contiene 42 comprobaciones. Se compila con `-Wall -Wextra -Wpedantic` sin advertencias y las cinco suites de CTest deben ejecutarse correctamente.

