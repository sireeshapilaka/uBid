import random
import math
import os

lenOfDay = 120
numOfUsers = 500
maxAsksPerDay = 10
# 120 auctions per day, so don't have max asks higher than that
# duration = 1 realistically corresponds to 12 minutes

modes = [400, 800, 1200, 1600]
directory = "NonMC"
if not os.path.exists(directory):
    os.makedirs(directory)

for userId in xrange(numOfUsers):
    # generate atleast one ask per user
    asksPerDay = random.randrange(1,maxAsksPerDay+1)
    rpis = []

    for ask in xrange(asksPerDay):
        # Each ask
        time = random.randrange(1, lenOfDay)
        ulDuration = 0
        dlDuration = 0
        ulThroughput = 0
        dlThroughput = 0
        # 3 types of rpis - upload, download and both
        type = random.randrange(1,4)
        if type==1:
            # Skype real-time - duration can be max 5 = 5*12 = 60 min = 1 hour
            ulDuration = random.randrange(1, 6)
            dlDuration = ulDuration
            modeIdx = random.randrange(0, 4)
            ulThroughput = modes[modeIdx]
            modeIdx = random.randrange(0,4)
            dlThroughput = modes[modeIdx]
        elif type==2:
            ulDuration = random.randrange(1, 6)
            modeIdx = random.randrange(0,4)
            ulThroughput = modes[modeIdx]
        else:
            dlDuration = random.randrange(1, 6)
            modeIdx = random.randrange(0,4)
            dlThroughput = modes[modeIdx]
        # elif type==2:
        #     # Only Upload
        #     # Maximum support 10 Gb = 10^6 Kb = 100*10^4 Kb
        #     # Minimum support - least mode & least time duration = 400Kbps * 12min = (approx) 300 Mb = 30*10^4 Kb
        #     data = random.randrange(30, 101)
        #     modeIdx = random.randrange(0, 4)
        #     ulThroughput = modes[modeIdx]
        #     # Duration = (x*10^4)/M seconds = (x/M)*(10000/(12*60)) fake seconds = (x/M)*(1000/72) fake seconds
        #     # where M = throughput in Kbps
        #     ulDuration = int(math.ceil((data*1000.0)/(ulThroughput*72.0)))
        # else:
        #     # Only Download - Limits same as only upload case
        #     data = random.randrange(30,101)
        #     modeIdx = random.randrange(0,4)
        #     dlThroughput = modes[modeIdx]
        #     dlDuration = int(math.ceil((data*1000.0)/(dlThroughput*72.0)))

        # Insert into a sorted list (sorted as per time)
        pos = 0
        for r in rpis:
            if r[0]==time:
                # Don't bid twice at the same time, so changing time
                time +=1
                continue
            if r[0]>time:
                break
            pos +=1

        # Check the previous rpi's duration
        if pos!=0:
            prev_rpi = rpis[pos-1]
            prev_end = prev_rpi[0]+ max(prev_rpi[1],prev_rpi[3])
            if prev_end>=time:
                time = prev_end+1

        valid_rpi = 1
        next_time = 120
        if pos!=len(rpis):
            next_time = rpis[pos][0]

        ul_end = time + ulDuration
        if ul_end>=next_time:
            ulDuration = next_time - time - 1
        dl_end = time + dlDuration
        if dl_end>=next_time:
            dlDuration = next_time - time - 1

        # Don't insert the rpi if invalid durations
        if ulDuration<1 and dlDuration<1:
            valid_rpi = 0
        if ulDuration<0 or dlDuration<0:
            valid_rpi = 0

        if valid_rpi==1:
            current_rpi = (time, ulDuration, ulThroughput, dlDuration, dlThroughput)
            rpis.insert(pos, current_rpi)

    usrDir = directory+"/"+str(userId)
    if not os.path.exists(usrDir):
        os.makedirs(usrDir)
    filePath = usrDir+"/RealtimeVideo.config"
    f = open(filePath, "w")
    for r in rpis:
        s = str(r[0])+","+str(r[1])+","+str(r[2])+","+str(r[3])+","+str(r[4])+"\n"
        f.write(s)
    f.close()
