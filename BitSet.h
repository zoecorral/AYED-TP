#pragma once
#include "Estado.h"
#include <iostream>


class BitSet {
private:
    unsigned char bit; // tipo de 8 bits, aunque solo usamos 5
public:
    BitSet() : bit(0) {} // inicializa los bits en 0

    void set (int pos) {
        if (pos >= 0 && pos <= 4) {
            bit |= (1 << pos); //activa un bit por posicion (de 0 a 4)
        }
    }

    void reset(int pos) {
        if (pos >= 0 && pos <= 4) {
            bit &= ~(1 << pos); // desactiva un bit por posicion (de 0 a 4)
        }
    }

    bool test (int pos) const {
        if (pos < 0  || pos > 4) {
            return false;
        }
        return (bit & (1 << pos)) != 0; // ve si el bit esta activo
    }

    int count () {
        int c = 0;
        for (int i = 0; i < 4; i++) {
            if (test (i)) {
                c++;
            }
        }
        return c; //cuenta bits activos
    }

    bool any () const {
        return (bit & 0x1F) != 0; //determina si existe algun bit activo utilizando la mascara 0x1F (0001 1111)
    }

    BitSet operator& (const BitSet& otro) {
        BitSet res;
        res.bit = (this->bit & otro.bit) & 0x1F;
        return res;
    }

    BitSet operator| (const BitSet& otro) {
        BitSet res;
        res.bit = (this->bit | otro.bit) & 0x1F;
        return res;
    }

    BitSet operator^ (const BitSet& otro) {
        BitSet res;
        res.bit = (this->bit ^ otro.bit) & 0x1F;
        return res;
    }

    void mostrar() const {
        for (int i = 0; i < 4; i++) {
            std::cout << (test(i) ? '1' : '0');
        }
        std::cout << std::endl;
    }
};
