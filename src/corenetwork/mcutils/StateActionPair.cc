/*
 * StateActionPair.cc
 *
 *  Created on: Dec 1, 2017
 *      Author: sireeshapilaka
 */

#include <StateActionPair.h>

StateActionPair::StateActionPair(State* state, int bid) {
    this->state = state;
    this->bid = bid;
}

StateActionPair::~StateActionPair() {
    // TODO Auto-generated destructor stub
}

int StateActionPair::getAction() {
    return bid;
}

bool StateActionPair::isLess(StateActionPair* s) {
    if(this->bid > s->bid) return false;
    if(this->bid < s->bid) return true;
    return state->isLess(s->state);
}

