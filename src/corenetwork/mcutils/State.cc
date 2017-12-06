/*
 * State.cc
 *
 *  Created on: Dec 1, 2017
 *      Author: sireeshapilaka
 */

#include <mcutils/State.h>

State::State(int index, int brem, int* ulDelta, int* dlDelta, int ulSize, int dlSize) {
    this->index = index;
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

bool State::isLess(State* s) {
    if(this->index > s->index) return false;
    if(this->index < s->index) return true;
    if(this->brem > s->brem) return false;
    if(this->brem < s->brem) return true;
    if(this->ulSize > s->ulSize) return false;
    if(this->ulSize < s->ulSize) return true;
    if(this->dlSize > s->dlSize) return false;
    if(this->dlSize < s->dlSize) return true;

    int i;
    for(i=0; i< ulSize; i++) {
        if(this->ulDelta[i] > s->ulDelta[i]) return false;
        if(this->ulDelta[i] < s->ulDelta[i]) return true;
    }

    for(i=0; i<dlSize; i++) {
        if(this->dlDelta[i] > s->dlDelta[i]) return false;
        if(this->dlDelta[i] < s->dlDelta[i]) return true;
    }

    return false;
}



