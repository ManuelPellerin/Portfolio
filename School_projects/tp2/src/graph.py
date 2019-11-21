import file_reader
import node

class Graph:

    def __init__(self):
        # ND format : key:nodeID, data:nodeObject
        self.nodesDict = {}
        self.createGraph()

    # brief: creates a graph from a text file
    def createGraph(self):
        fileReader = file_reader.FileReader()
        self.nodesDict = fileReader.readNodes()
        arcsList = fileReader.readArcs()
        for arc in arcsList:
            # node1Id, node2Id, distance = arc
            node1ID = arc["nodes"][0]
            node2ID = arc["nodes"][1]
            distance = arc["distance"]
            self.nodesDict[node1ID].addNeighbor(node2ID, distance)
            self.nodesDict[node2ID].addNeighbor(node1ID, distance)
    
    # brief: convert the graph to a list
    #
    #  returns:
    #    a list of all the nodes 
    def convertGraphToList(self):
        outputNodesList = []
        for nodeID in self.nodesDict:
            node = self.nodesDict[nodeID]
            outputNodesList.append(node)
        
        return outputNodesList
        
    # brief: Creates a string which represents the graph
    #
    # returns: A string which represents the graph
    def __str__(self):
        outputString = ""
        nodesEnd = len(self.nodesDict)
        for i, nodeKey in enumerate(self.nodesDict, 1):
            node = self.nodesDict[nodeKey]

            # Make the string with all the neighbors
            neighborsString = ""
            neighborsEnd = len(node.neighborsDict)
            for j, neighborID in enumerate(node.neighborsDict, 1):
                neighborsString += "({}, {})".format(neighborID, 
                                                     node.neighborsDict[neighborID])        
                if j != neighborsEnd:
                    neighborsString += ", "

            # Add the node info to the output string
            outputString += "({}, {}, {}, {}, ({}))".format(node.id,
                                                            node.nObjectA,
                                                            node.nObjectB,
                                                            node.nObjectC,
                                                            neighborsString)
            if i != nodesEnd:
                outputString += "\n"

        return outputString

    # brief: Prints the graph
    def printGraph(self):
        print(self)