/*
 * State.cc
 *
 *  Created on: Dec 1, 2017
 *      Author: sireeshapilaka
 */

#include <mcutils/State.h>

State::State(int brem, int* ulDelta, int* dlDelta, int ulSize, int dlSize) {
    this->brem = brem;
    this->ulDelta = ulDelta;
    this->dlDelta = dlDelta;
    this->ulSize = ulSize;
    this->dlSize = dlSize;
}

State::~State() {
    if(ulDelta!=NULL) delete[] ulDelta;
    if(dlDelta!=NULL) delete[] dlDelta;
}

bool State::isEqual(State* s) {
    if(this->brem != s->brem) return false;
    if(this->ulSize != s->ulSize) return false;
    if(this->dlSize != s->dlSize) return false;

    int i;
    for(i=0; i< ulSize; i++) {
        if(this->ulDelta[i]!=s->ulDelta[i]) return false;
    }

    for(i=0; i<dlSize; i++) {
        if(this->dlDelta[i]!=s->dlDelta[i]) return false;
    }

    return true;
}



