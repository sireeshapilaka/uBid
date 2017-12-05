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

bool StateActionPair::isState(State* s) {
    return state->isEqual(s);
}

int StateActionPair::getAction() {
    return bid;
}

bool StateActionPair::isEqual(StateActionPair* s) {
    if(s->bid != this->bid) return false;
    return state->isEqual(s->state);
}

