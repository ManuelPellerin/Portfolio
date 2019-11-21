import os.path
import node

DEFAULT_FILENAME = "entrepot.txt"


class FileReader:
    def __init__(self, fileName=DEFAULT_FILENAME):
        self.filePath = "res/" + fileName
        

    #brief: returns a dictionnary of all the nodes
    def readNodes(self):
        if(not os.path.isfile(self.filePath)):
            raise Exception("Specified file name does not exist")
        nodeDict = {}
        with open(self.filePath, "r") as workingFile:
            for line in workingFile:
                if(line == "\n"):
                    break
                id, nObjectA, nObjectB, nObjectC = map(int, line.strip("\n").split(","))
                tempNode = node.Node(id, nObjectA, nObjectB, nObjectC)
                nodeDict[id] = tempNode
        return nodeDict
        
    #brief: returns a list of all the arcs
    #  arcList form: [{"nodes":[node1ID, node2ID], "distance":distance}, {...}, ...]
    def readArcs(self):
        if(not os.path.isfile(self.filePath)):
            raise Exception("Specified file name does not exist")
        arcsList = []
        with open(self.filePath, "r") as workingFile:
            # Skip the nodes part of the file
            for line in workingFile:
                if(line == "\n"):
                    break
            # Read the arc part of the file
            for line in workingFile:
                if(line == "\n"):
                   break
                node1ID, node2ID, distance = map(int, line.strip("\n").split(","))
                arcsList.append({"nodes":[node1ID, node2ID], "distance":distance})
        return arcsList

