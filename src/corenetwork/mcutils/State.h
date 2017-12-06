/*
 * State.h
 *
 *  Created on: Dec 1, 2017
 *      Author: sireeshapilaka
 */

#ifndef CORENETWORK_MCUTILS_STATE_H_
#define CORENETWORK_MCUTILS_STATE_H_

#include <dao/AppBwReq.h>

class State {
private:
    // The state tuple
    int brem;
    int* ulDelta;
    int* dlDelta;

    // Other Properties
    int ulSize = 0;
    int dlSize = 0;
    int index = 0;

public:
    State(int index, int brem, int* ulDelta, int* dlDelta, int ulSize, int dlSize);
    virtual ~State();
    bool isLess(State* s);
};

#endif /* CORENETWORK_MCUTILS_STATE_H_ */
