
OBJECT_A_WEIGHT = 1
OBJECT_B_WEIGHT = 3
OBJECT_C_WEIGHT = 6

class Node: 
    def __init__(self, id, nObjectA, nObjectB, nObjectC):
        self.id = id
        self.nObjectA = nObjectA
        self.nObjectB = nObjectB
        self.nObjectC = nObjectC
        self.neighborsDict = {}

    # brief: Creates a string with the node and its id
    #
    # returns: A string with the node and its id
    def __str__(self):
        return "Node " + str(self.id)

    # brief: Creates a string with the node's informations
    #
    # returns: A string with the node's informations
    def __repr__(self):
        return "<Node with id: " + str(self.id) + ", a: " + str(self.nObjectA) + ", b: "+ str(self.nObjectB) + ", c: " + str(self.nObjectC) + ">"
    
    # brief: Adds a neighbor node to the neighbor's list
    #
    # params:
    #   id: The id of the neighbor node
    #   distande: the distance to the neighbor from the current node
    def addNeighbor(self, id, distance):
        self.neighborsDict[id] = distance

    # brief: Calculates the total weight of the node
    #
    # returns: The total weight of the node  
    def getNodeWeight(self):
        nodeWeight = (self.nObjectA * OBJECT_A_WEIGHT + 
                       self.nObjectB * OBJECT_B_WEIGHT + 
                       self.nObjectC * OBJECT_C_WEIGHT)
        return nodeWeight
    
    # brief: Calculates the total number of objects in the node
    #
    # returns: The total number of objects in the node
    def getNbObjects(self):
        totalObjects = self.nObjectA + self.nObjectB + self.nObjectC
        return totalObjects