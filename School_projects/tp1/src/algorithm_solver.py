import graph
import node
import robot
import order
import sys
import heapq
import itertools
import order
import copy


PICKUP_TIME = 10.0


class AlgorithmSolver:
    
    def __init__(self):
        pass
    
    # brief: Applies the dijkstra algorithm to the nodes of an input graph
    #
    # returns: A dictionnary with the following format:
    #          key:nodeID, data:{'totalDistance':dist, 'previousNodeID':id}
    #
    # params:
    #   graph: The input graph
    #   startNodeID: the node from which to apply the algorithm
    def dijkstra(self, graph, startNodeID):
        # VN format : key:nodeID, data:{'totalDistance':dist, 'previousNodeID':id}
        visitedNodes = {}
        # PQ format : (totalDistance, nodeID, previousNodeID)
        priorityQueue = []

        # Push every node in the priorityQueue
        for nodeID in graph.nodesDict:
            initialDistance = sys.maxsize
            if(nodeID == startNodeID):
                initialDistance = 0
            priorityQueue.append((initialDistance, nodeID, None))
        heapq.heapify(priorityQueue)

        while len(priorityQueue) > 0:
            currentNodeElements = heapq.heappop(priorityQueue)
            currentNodeID = currentNodeElements[1]
            currentNode = graph.nodesDict[currentNodeID]
            visitedNodes[currentNodeID] = {"totalDistance":currentNodeElements[0], "previousNodeID":currentNodeElements[2]}

            for neighborID in currentNode.neighborsDict:
                if neighborID not in visitedNodes:
                    testDistance = currentNodeElements[0] + currentNode.neighborsDict[neighborID]
                    # Find the node in the PQ 
                    for i in range(len(priorityQueue)):
                        if(priorityQueue[i][1] != neighborID): 
                            continue
                        if(testDistance < priorityQueue[i][0]):
                            priorityQueue[i] = (testDistance, priorityQueue[i][1], currentNodeID)
                        break
            heapq.heapify(priorityQueue)

        return visitedNodes

    # brief: Calculates every possible dijkstra paths for a graph
    #
    # returns: a dictionnary containing every dijkstra path for a graph.
    #          The dictionnary has the following format :
    #          {key:startNodeId data:{key:targetNodeID  data:{'totalDistance':dist, 'previousNodeID':id}}}
    #
    # params:
    #   graph: the graph we want to analyse
    def getEveryDijkstraPath(self, graph):
        # EDD format : {key:startNodeId data:{key:targetNodeID  data:{'totalDistance':dist, 'previousNodeID':id}}}
        everyDijkstraDict = {}
        for i in range(len(graph.nodesDict)):
            dijkstraDict = self.dijkstra(graph, i)
            tempDict = {}
            for j in range(len(dijkstraDict)):
                tempDict[j] = dijkstraDict[j]
            everyDijkstraDict[i] = tempDict
        return everyDijkstraDict

    # brief: removes permutations that do not contain a sufficient ammount of each object type to fill the order
    #
    # return: a filtered permutation list
    #   
    # params:
    #   order: The order of objects
    #   permuatationsIterator: An iterator over a list containing the node permutations. 
    #                          Every permutation is a list of node.
    def filterPermutations(self, order, permutationsIterator):
        permutationsList = []
        for permutation in permutationsIterator:
            nObjectAInPermutation = 0
            nObjectBInPermutation = 0
            nObjectCInPermutation = 0
            currentPermutationsList = []
            for node in permutation:
                nObjectAInPermutation += node.nObjectA
                nObjectBInPermutation += node.nObjectB
                nObjectCInPermutation += node.nObjectC
                currentPermutationsList.append(node)
            if (nObjectAInPermutation >= order.nObjectA and 
                nObjectBInPermutation >= order.nObjectB and 
                nObjectCInPermutation >= order.nObjectC):
                permutationsList.append(currentPermutationsList)
        return permutationsList

    # brief: finds the node permutation in permutationList with the shortest total travel distance
    #   
    # return: a list of the the shortest node permutation 
    #   
    # params:
    #   permutationsList: a list of all the node permutations with a sufficent ammount of objects to fulfill the order
    #     shape of permutationsList: [[node1, node2 ....], [node1, node2...] ...] 
    #
    #   everyDijkstraDict:a dictionnary containing a dijkstra shortest Path tree for every node in the graph
    #     shape of everyDijkstraDict: {key:startNodeId data:{key:targetNodeID  data:{'totalDistance':dist, 'previousNodeID':id}}} 
    #        
    #   startNodeID: startNodeID   
    def findShortestDistance(self, permutationsList, everyDijkstraDict, startNodeID):
            shortestDistance = sys.maxsize 
            shortestPermutationsList = [] 
            for permutation in permutationsList:
                tempDistance = 0
                previousNodeID = startNodeID
                for node in permutation:
                    tempDistance += everyDijkstraDict[previousNodeID][node.id]["totalDistance"]
                    previousNodeID = node.id
                tempDistance += everyDijkstraDict[previousNodeID][startNodeID]["totalDistance"]
                if tempDistance == shortestDistance:
                    shortestPermutationsList.append(permutation)
                elif tempDistance < shortestDistance:
                    shortestDistance = tempDistance
                    shortestPermutationsList.clear()
                    shortestPermutationsList.append(permutation)
            return (shortestDistance, shortestPermutationsList)

    # brief: Creates a list containing all the nodes that form the shortest path from starting node to starting through the nodes in permList
    #   
    # return: a list of the the shortest node permutation 
    #   
    # params:
    #   permutationsList: a list of all the node permutations with a sufficent ammount of objects to fulfill the order
    #                     shape of permutationsList: [[node1, node2 ....], [node1, node2...] ...] 
    #
    #   everyDijkstraDict:a dictionary containing a dijkstra shortest Path tree for every node in the graph
    #       shape of everyDijkstraDict: {key:startNodeId data:{key:targetNodeID  data:{'totalDistance':dist, 'previousNodeID':id}}} 
    #
    #   nodesDict: a dictionary of all the nodes in the graph
    #     shape of nodesDict: {key:nodeID, data:nodeObject}
    #   startNodeID: startingNodeID
    def addMissingNodesInPerm(self, permutationList, everyDijkstraDict, nodesDict, startNodeID):
        NodeList = copy.deepcopy(permutationList)
        #path finishes at starting node 
        NodeList.append(nodesDict[startNodeID])
                
        visitedNodeList = []
        previousStartNodeID = startNodeID
        for i in range(len(NodeList)):
            tempList = []
            currentNodeID = NodeList[i].id
            while True:
                if(currentNodeID == previousStartNodeID):
                    #first iteration special case
                    if i == 0:
                        tempList.append(nodesDict[currentNodeID])
                    previousStartNodeID = NodeList[i].id
                    break

                tempList.append(nodesDict[currentNodeID])
                currentNodeID = everyDijkstraDict[previousStartNodeID][currentNodeID]["previousNodeID"]
                
            tempList.reverse()
            for i in range(len(tempList)):
                visitedNodeList.append(tempList[i])
                
        return visitedNodeList

    # brief: calculates the order completion time for an optimized path 
    #
    # return: a dictionary containing the shortest time and the robot used
    #
    # params:
    #   permutation: A list of nodes containing the number of objects
    #                at every node.
    #   order: object order

    def findShortestTime(self, permutation, order):
        # OD format : {'time':time, 'robotType':type}
        robotX = robot.Robot("x")
        robotY = robot.Robot("y")
        robotZ = robot.Robot("z")
        robotsList = [robotX, robotY, robotZ]
        orderWeight = order.getTotalWeight()
        outputDict = {'time':sys.maxsize, 'robotType':None}
        for currentRobot in robotsList:
            if orderWeight <= currentRobot.maxWeight:
                totalTime = 0.0
                previousNode = permutation[0]
                for node in permutation:
                    if node.id == previousNode.id:
                        continue
                    totalTime += (node.getNbObjects() * PICKUP_TIME)
                    distance = node.neighborsDict[previousNode.id]
                    timeConstant = (currentRobot.formulaConstant + 
                                   (currentRobot.weight * currentRobot.formulaSlope))
                    travelTime = (timeConstant * distance)
                    totalTime += travelTime
                    currentRobot.weight += node.getNodeWeight()
                    previousNode = node

                if totalTime < outputDict['time']:
                    outputDict['time'] = totalTime
                    outputDict['robotType'] = currentRobot.type
        return outputDict
    
    # brief: Prints the final path
    #
    # params: 
    #   time: The final fastest time
    #   robotType: The fastest robot's type
    #   permutation: The list of nodes giving the fastest path
    def printFastestPath(self, time, robotType, permutation):
        if robotType == None:
            print("Cannot fulfill the order")
            return
        print("Best path: ")
        for node in permutation:
            print("  " + str(node))
            if node.nObjectA != 0:
                print("  Collected " + str(node.nObjectA) + " A objects")
            if node.nObjectB != 0:
                print("  Collected " + str(node.nObjectB) + " B objects")
            if node.nObjectC != 0:
                print("  Collected " + str(node.nObjectC) + " C objects")
        print("\nBest time: " + str(time) + " seconds")
        print("\nRobot used: Robot " + robotType)

    # brief : determines the optimal node pick-up order
    #
    # return: a list of nodes with the number of objects that are to be picked up from 
    #         the nodes in the graph.
    #
    # params:
    #   order: object order
    #   permutation: node permutation 
    def findPickupOrder(self, order, permutation):
        tempOrder = copy.deepcopy(order)
        outputNodesList = []
        for referenceNode in reversed(permutation):
            pickupNode = node.Node(referenceNode.id, 0, 0, 0)
            for neighborID in referenceNode.neighborsDict:
                pickupNode.addNeighbor(neighborID, referenceNode.neighborsDict[neighborID])
            while (pickupNode.nObjectA < referenceNode.nObjectA and
                   tempOrder.nObjectA > 0):
                   tempOrder.nObjectA -= 1
                   pickupNode.nObjectA += 1
            while (pickupNode.nObjectB < referenceNode.nObjectB and
                   tempOrder.nObjectB > 0):
                   tempOrder.nObjectB -= 1
                   pickupNode.nObjectB += 1
            while (pickupNode.nObjectC < referenceNode.nObjectC and
                   tempOrder.nObjectC > 0):
                   tempOrder.nObjectC -= 1
                   pickupNode.nObjectC += 1
            outputNodesList.append(pickupNode)
        
        outputNodesList.reverse()
        return outputNodesList
              
    #brief: determine the fastest path in a graph to complete an order of objects of different types. 
    #       Each node in the graph has a given ammount of objects of diffent types. Edges in the graph
    #       represent distances between nodes. There are multiple types of robots, which move at different
    #       speeds and can carry an specific ammount of weight (objects have a weight value)
    #
    #params:
    #  graph: graph of nodes
    #  startNodeID: starting node ID  -- order has to leave from this node and return to it
    #  order: object order. Contains multiple objects of different types. Each object type has a weight assigned to it.
    def findShortestPath(self, graph, startNodeID, order):
        orderWeight = order.getTotalWeight()
        if orderWeight > max([robot.ROBOT_X_MAX_WEIGHT, robot.ROBOT_Y_MAX_WEIGHT, robot.ROBOT_Z_MAX_WEIGHT]):
            print("The order's weight is too high")
            return
        elif orderWeight == 0:
            print("The order is empty")
            return
            
        # EDD format : key:startNodeId data: {key:targetNodeID  data:distance:Y}
        everyDijkstraDict = self.getEveryDijkstraPath(graph)

        # Start by removing every node with 0 objects
        nodesList = []
        for nodeID in graph.nodesDict:
            node = graph.nodesDict[nodeID]
            if node.nObjectA == 0 and node.nObjectB == 0 and node.nObjectC == 0:
                pass
            else:
                nodesList.append(node)
        
        nTests = 0
        totalShortestDistance = sys.maxsize
        totalShortestPermutationsList = []
        for i in range(len(graph.nodesDict)):
            # Make a list with containing len(graph.nodesDict)P(i+1) permutations
            permutationsIterator = itertools.permutations(nodesList, i+1)

            # Remove every permutation without a succifient quantity of every object type. Returns filtered list
            permutationsList = self.filterPermutations(order, permutationsIterator)
            
            if len(permutationsList) == 0:
                continue
            
            # Find the shortest distance in the permutations
            shortestDistAndPermutation = self.findShortestDistance(permutationsList, everyDijkstraDict, startNodeID)
            permListShortestDistance = shortestDistAndPermutation[0]
            permListShortestPermutation = shortestDistAndPermutation[1]

            nTests += len(permutationsList)

            #if no improvement is detected over previous path, previous path is considered the shortest path
            if permListShortestDistance < totalShortestDistance:
                totalShortestDistance = permListShortestDistance
                totalShortestPermutationsList = permListShortestPermutation 
            else:
                break
        
        shortestTime = {'time':sys.maxsize, 'robotType':None, 'permutation':None}
        for permutation in totalShortestPermutationsList:
            permutation = self.addMissingNodesInPerm(permutation, everyDijkstraDict, graph.nodesDict, startNodeID)
            pickupPermutation = self.findPickupOrder(order, permutation)
            currentTime = self.findShortestTime(pickupPermutation, order)
            if currentTime['time'] < shortestTime['time']:
                shortestTime['time'] = currentTime['time']
                shortestTime['robotType'] = currentTime['robotType']
                shortestTime['permutation'] = pickupPermutation
            
        self.printFastestPath(shortestTime['time'], shortestTime['robotType'], shortestTime['permutation'])
        
        return
        