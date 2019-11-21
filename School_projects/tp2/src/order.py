
OBJECT_A_WEIGHT = 1
OBJECT_B_WEIGHT = 3
OBJECT_C_WEIGHT = 6

class Order:
    def __init__(self, nObjectA=0, nObjectB=0, nObjectC=0):
        self.nObjectA = nObjectA
        self.nObjectB = nObjectB
        self.nObjectC = nObjectC

    # brief: prompts the user to enter the number of objects in the order
    def takeOrder(self):
        while True:
            try:
                self.nObjectA = int(input("How many objects of type A do you want? "))
                self.nObjectB = int(input("How many objects of type B do you want? "))
                self.nObjectC = int(input("How many objects of type C do you want? "))

            except ValueError:
                print("Please enter a valid number of objects")
                continue
            break

    # brief: Calculates the total weight of the order
    #
    # returns: the order's total weight
    def getTotalWeight(self):
        totalWeight = (self.nObjectA * OBJECT_A_WEIGHT + 
                       self.nObjectB * OBJECT_B_WEIGHT + 
                       self.nObjectC * OBJECT_C_WEIGHT)
        return totalWeight

    # brief: Prints the order
    def printOrder(self):
        print(self)

    # brief: Makes a string representing the order
    #
    # returns: The string which represents the order
    def __str__(self):
        outputString =  ("Number of objects type A: " + str(self.nObjectA) + "\n"
                        "Number of objects type B: " + str(self.nObjectB) + "\n"
                        "Number of objects type C: " + str(self.nObjectC))
        return outputString