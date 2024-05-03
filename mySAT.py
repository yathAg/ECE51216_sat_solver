import os
import time
import random
import sys

# Function to read clauses from a CNF file "fname".
def readCNF(fname):
    file = open(fname, "r")             
    inputNum = 0                      
    clauses = []                        
    rf = 1                              
    for a in file:                     
        if a[0] != 'c':                 
            if a[0] == 'p':             
                a = a.split()           
                numLiteral = int(a[2])     
                numClause = int(a[3])   
            else:
                if rf == 1:             
                    newclause = []      
                a = a.split()           
                for y in a:
                    x = int(y)          
                    rf = 0              
                    if x == 0:          
                        clauses.append(newclause)   
                        rf = 1                      
                        inputNum += 1             
                        break
                    if -x in newclause:             
                        rf = 1
                        inputNum += 1
                        break
                    if x not in newclause:
                        newclause.append(x)         
                    

    return 1, numLiteral, numClause, clauses

# Boolean Constraint Propagation (BCP) on literal
def bcp(clauses, literal):
    newClauseList = [a[:] for a in clauses]     
    for a in reversed(newClauseList):           
        if literal in a:                        
            newClauseList.remove(a)             
        if -literal in a:                       
            a.remove(-literal)                  
            if not a:                           
                return -1
    return newClauseList

# Unit Propagation: Propagate unit clauses and add implications to assignedList
def unitPropogation(clauses):
    assn = []
    f = 1
    while f != 0:  
        f = 0
        for a in clauses:  
            if len(a) == 1:  
                u = a[0]
                clauses = bcp(clauses, u)  
                assn += [u]
                f = 1
            if clauses == -1:  
                return -1, []
            if not clauses:
                return clauses, assn
    return clauses, assn

# backpropogate to decision level
def backpropogate(assignedList, decisionLv, pos, implicationNum):
    implicationNum = implicationNum + len(assignedList) - len(pos)
    if not pos:
        return -1, -1, implicationNum
    decisionLv = pos.pop()
    literal = assignedList[decisionLv]
    del assignedList[decisionLv:]
    return 0, -literal, implicationNum

# Random Restart
def randomRestart(assignedList, back, pos, prob, numRestart):
    if random.random() < prob:       
        assignedList = back[:]
        pos = []
        prob *= 0.5                  # Decay next restart probability by 50%
        numRestart += 1
        if prob < 0.001:
            prob = 0.2
        if numRestart > len(assignedList) + 10:    
            prob = 0
    return prob, numRestart

# Conflict-Driven Clause Learning (CDCL) solver
def solveCDCL(clauses, numLiteral):
    pos = []  
    assignedList = []  
    clauses, assignedList = unitPropogation(clauses)  
    if clauses == -1:
        return -1, 0, 0, 0, 0  
    back = assignedList[:]  
    clauseCount = initializeVSDIS(clauses, numLiteral)  
    
    # Initialize two-literal watch data structure
    watchList, mapLitWatch = watchlistCDCL(clauses, assignedList, numLiteral)
    prob = 0.9  # Random restart probability, decays with restarts
    numRestart = learnCount = decisionCount = implicationNum = 0
    
    # Main loop
    while not checkAll(numLiteral, len(assignedList)):       
        variable = pickVSIDS(clauseCount, assignedList, numLiteral)    
        decisionCount += 1
        assign(variable, assignedList, pos)
        conflict, watchList = propgateWatchlist(clauses, watchList, mapLitWatch, assignedList, variable)  

        while conflict != -1:                           
            incrementVSIDS(clauseCount, conflict)          
            clauseCount = decayVSIDS(clauseCount, numLiteral)     
            learnedClause = conflictAnalysis(assignedList, conflict, pos)  
            decisionLv = addLearned(clauses, watchList, mapLitWatch, learnedClause, assignedList)  
            learnCount += 1
            jump, var, implicationNum = backpropogate(assignedList, decisionLv, pos, implicationNum)  
            if jump == -1: 
                return -1, numRestart, learnCount, decisionCount, implicationNum
            assignedList.append(var)  
            prob, numRestart = randomRestart(assignedList, back, pos, prob, numRestart)  
            conflict, watchList = propgateWatchlist(clauses, watchList, mapLitWatch, assignedList, var)

    return assignedList, numRestart, learnCount, decisionCount, implicationNum

# Create the 2-literal watch data structure
def watchlistCDCL(clauses, assignedList, numLiteral):
    watchList = {}              
    mapLitWatch = []    
    for x in range(-numLiteral, numLiteral + 1):
        watchList[x] = []
    for x in range(0, len(clauses)):
        newClause = []
        first = 0
        for y in range(0, len(clauses[x])):
            if clauses[x][y] not in assignedList and first == 0:
                watchClause_i = clauses[x][y]
                first = 1
                continue
            if clauses[x][y] not in assignedList and first == 1:
                watchClause_j = clauses[x][y]
                break
        newClause.append(watchClause_i)
        newClause.append(watchClause_j)
        mapLitWatch.append(newClause)
        watchList[watchClause_i].append(x)
        watchList[watchClause_j].append(x)
    return watchList, mapLitWatch

# Two-literal watch propagation
def propgateWatchlist(clauses, watchList, mapLitWatch, assignedList, variable):
    propogateList = [variable]          
    while len(propogateList) != 0:      
        variable = propogateList.pop()  
        for assignedClause in reversed(watchList[-variable]):  
            assnCaluse = clauses[assignedClause][:]
            watchClause_i = mapLitWatch[assignedClause][0]
            watchClause_j = mapLitWatch[assignedClause][1]
            watchClause_m = watchClause_i
            watchClause_n = watchClause_j
            status, assignedList, watchClause_i, watchClause_j, u = checkSAT(assnCaluse, assignedList, watchClause_i, watchClause_j)
            if status == "Unit":
                propogateList.append(u)
                assignedList.append(u)             
            elif status == "unsat":  
                return assnCaluse, watchList
            watchList[watchClause_m].remove(assignedClause)
            watchList[watchClause_n].remove(assignedClause)
            mapLitWatch[assignedClause][0] = watchClause_i
            mapLitWatch[assignedClause][1] = watchClause_j
            watchList[watchClause_i].append(assignedClause)
            watchList[watchClause_j].append(assignedClause)
    return -1, watchList

# Initialization: Generate counters for the number of times a literal appears
def initializeVSDIS(clauses, numLiteral):
    clauseCount = {}
    for a in range(-numLiteral, numLiteral + 1):
        clauseCount[a] = 0
    for clause in clauses:
        for literal in clause:
            clauseCount[literal] += 1
    return clauseCount

# Conflict: Increment counters of literals in conflict clause to increase their chances of getting selected
def incrementVSIDS(clauseCount, conflictClause):
    for literal in conflictClause:
        clauseCount[literal] += 1
    return clauseCount

# Decay: Reduce counters by 5% for all literals at each conflict
def decayVSIDS(clauseCount, numLiteral):
    for x in range(-numLiteral, numLiteral + 1):
        clauseCount[x] = clauseCount[x] * 95 / 100
    return clauseCount

# Decide: Pick a variable not yet in assignedList based on max clauseCount value
def pickVSIDS(clauseCount, assignedList, numLiteral):
    max = 0
    var = 0
    for a in range(-numLiteral, numLiteral + 1):
        if clauseCount[a] >= max and a not in assignedList and -a not in assignedList:
            max = clauseCount[a]
            var = a
    return var

# Check status of the clause
def checkSAT(clause, assignedList, watchClause_i, watchClause_j):
    u = 0
    if watchClause_i in assignedList or watchClause_j in assignedList:                    
        return "sat", assignedList, watchClause_i, watchClause_j, u
    sym = []                                
    for literal in clause:                  
        if -literal not in assignedList:
            sym.append(literal)
        if literal in assignedList:
            if -watchClause_i not in assignedList:
                return "sat", assignedList, watchClause_i, literal, u
            return "sat", assignedList, literal, watchClause_j, u
    if len(sym) == 1:                       
        return "Unit", assignedList, watchClause_i, watchClause_j, sym[0]
    if len(sym) == 0:                       
        return "unsat", assignedList, watchClause_i, watchClause_j, u
    else:
        return "Unresolved", assignedList, sym[0], sym[1], u  

# Check if all variables are assigned
def checkAll(numLiteral, assignedListLen):
    if assignedListLen >= numLiteral:
        return True
    return False

# Verify the sol
def verify(assignedList, clauses):
    for c in clauses:                       
        f = 0
        for lit in c:
            if lit in assignedList:                    
                f = 1
                break
        if f == 0:
            return False
    return True

# Analyze Conflict
def conflictAnalysis(assignedList, conflict, pos):
    learn = []
    for a in pos:
        learn.append(-assignedList[a])
    return learn

# Assign decision literal to assignedList 
def assign(variable,assignedList,pos):             
    pos.append(len(assignedList))
    assignedList.append(variable)

# Add learned clause to clauses
def addLearned(clauses, watchList, mapLitWatch, learnedClause, assignedList):
    if len(learnedClause) == 0:
        return -1
    if len(learnedClause) == 1:         
        assignedList.append(learnedClause[0])
        return 1, learnedClause[0]
    clauses.append(learnedClause)       
    watchClause_i = learnedClause[0]
    watchClause_j = learnedClause[1]
    x = len(clauses) - 1
    newClause = []
    newClause.append(watchClause_i)
    newClause.append(watchClause_j)
    mapLitWatch.append(newClause)
    watchList[watchClause_i].append(x)
    watchList[watchClause_j].append(x)
    return 0

# Main function
def MAIN():

    fname = sys.argv[1]

    # Read input file
    start = time.process_time()
    a, numLiteral, numClause, clauses = readCNF(fname)
    stop = time.process_time()
    
    # Print read time and status
    if a == 1:
        print("Successfully read the file")
        print("Vars : " + str(numLiteral) + " Clauses : " + str(numClause))
        print("Read time: " + str(stop - start) + " sec")

    # Solve CNF by CDCL
    start = time.process_time()
    sol = solveCDCL(clauses, numLiteral)
    stop = time.process_time()

    # Print statistics
    print()
    print("Statistics:")
    print("=============================================")
    print("# Restarts: " + str(sol[1]))
    print("# Learned Clauses: " + str(sol[2]))
    print("# Decisions: " + str(sol[3]))
    print("# Implications: " + str(sol[4]))
    print("# Solve time: " + str(stop - start) + " sec")
    print("=============================================")
    
    # Print result
    print("RESULT: ", end='')
    if sol[0] != -1:
        print("SAT")
        print("ASSIGNMENT:")
        assignment = sol[0][:]
        assignment.sort(key=abs)
        result = " ".join("{}={}".format(index + 1, 1 if value > 0 else 0) for index, value in enumerate(assignment))
        print(result)
    else:
        print("UNSAT")

if __name__ == "__main__":
    MAIN()
