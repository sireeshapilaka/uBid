/*
 * UeMC.h
 *
 *  Created on: Nov 30, 2017
 *      Author: sireeshapilaka
 */

#ifndef CORENETWORK_NODES_UEMC_H_
#define CORENETWORK_NODES_UEMC_H_

#include <Ue.h>
#include <UserAgentMC.h>

class UeMC: public Ue {
public:
    UeMC();
    virtual ~UeMC();
    int getDailybudget();
protected:
    virtual void initialize() override;

private:
    const int dayLength = dayLen;
    const int numOfDays = days;
    int dailybudget = 10;

};

#endif /* CORENETWORK_NODES_UEMC_H_ */
