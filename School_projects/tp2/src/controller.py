import os
import graph
import order
import algorithm_solver

START_NODE_ID = 0

class Controller:
    def __init__(self):
        # os.name :
        #   "nt" for Windows
        #   "posix" for unix systems
        self.system = os.name
        self.graph = None
        self.order = None
        self.algoSolver = algorithm_solver.AlgorithmSolver()

    # brief: User interface main loop
    def start(self):
        statusString = ""
        while True:
            self.showMainMenu()
            print("Status :", statusString, end="\n\n")
            print("Select the action you want to take : ", end="")
            try:
                action = int(input())
                statusString = self.processAction(action)
            except ValueError:
                statusString = "Please enter a valid input"      

    # brief: Clears the terminal
    def clearScreen(self):
        if self.system == "nt":
            os.system("cls")
        else:
            os.system("clear")

    # brief: Processes the action selected by the user
    #
    # returns: A string with the status of the action
    #
    # params:
    #   action: An int representing the action to take
    def processAction(self, action):
        statusString = ""
        if action == 1:
            self.graph = graph.Graph()
            statusString = "The graph has been created successfully"
        elif action == 2:
            if self.graph != None:
                self.clearScreen()
                self.graph.printGraph()
                input("\nPress enter to return to the main menu...")
                statusString = "The graph was displayed successfully"
            else:
                statusString = "Error : The graph hasn't been created yet"
        elif action == 3:
            self.clearScreen()
            self.order = order.Order()
            self.order.takeOrder()
            statusString = "The order was taken successfully"
        elif action == 4:
            if self.order != None:
                self.clearScreen()
                self.order.printOrder()
                input("\nPress enter to return to the main menu...")
                statusString = "The order was displayed successfully"
            else:
                statusString = "An order hasn't been created yet"
        elif action == 5:
            if (self.graph != None) and (self.order != None):
                self.clearScreen()
                self.algoSolver.findShortestPath(self.graph, START_NODE_ID, self.order)
                input("\nPress enter to return to the main menu...")
                statusString = "The shortest path was displayed successfully"
            elif (self.graph == None) and (self.order == None):
                statusString = "The graph and the order haven't been created yet"
            elif self.graph == None:
                statusString = "The graph hasn't been created yet"
            elif self.order == None:
                statusString = "An order hasn't been created yet"
        elif action == 6:
            exit()
                
        return statusString

    # brief: Prints the main menu
    def showMainMenu(self):
        self.clearScreen()
        menuString = (
        "Main menu :\n\n" +
        "    1. Create a graph\n" +
        "    2. Display the graph\n" +
        "    3. Take an order\n" +
        "    4. Display the order\n" +
        "    5. Find the shortest path\n" +
        "    6. Quit\n")
        print(menuString)


if __name__ == "__main__":
    testController = Controller()
    testController.start()
