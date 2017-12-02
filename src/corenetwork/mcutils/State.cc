/*
 * State.cc
 *
 *  Created on: Dec 1, 2017
 *      Author: sireeshapilaka
 */

#include <mcutils/State.h>

State::State(int brem, int* ulDelta, int* dlDelta) {
    this->brem = brem;
    this->ulDelta = ulDelta;
    this->dlDelta = dlDelta;
}

State::~State() {
    if(ulDelta!=NULL) delete[] ulDelta;
    if(dlDelta!=NULL) delete[] dlDelta;
}

