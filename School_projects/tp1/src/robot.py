import node


ROBOT_X_MAX_WEIGHT = 5
ROBOT_X_CONSTANT = 1
ROBOT_X_SLOPE = 1

ROBOT_Y_MAX_WEIGHT = 10
ROBOT_Y_CONSTANT = 1.5 
ROBOT_Y_SLOPE = 0.6

ROBOT_Z_MAX_WEIGHT = 25
ROBOT_Z_CONSTANT = 2.5
ROBOT_Z_SLOPE = 0.2


class Robot:
    def __init__(self, type):
        typeStr = type.upper()
        if typeStr == "X":
            self.type = typeStr
            self.maxWeight = ROBOT_X_MAX_WEIGHT
            self.formulaConstant = ROBOT_X_CONSTANT
            self.formulaSlope = ROBOT_X_SLOPE
            self.weight = 0

        elif typeStr == "Y":
            self.type = typeStr
            self.maxWeight = ROBOT_Y_MAX_WEIGHT
            self.formulaConstant = ROBOT_Y_CONSTANT
            self.formulaSlope = ROBOT_Y_SLOPE
            self.weight = 0
        
        elif typeStr == "Z":
            self.type = typeStr
            self.maxWeight = ROBOT_Z_MAX_WEIGHT
            self.formulaConstant = ROBOT_Z_CONSTANT
            self.formulaSlope = ROBOT_Z_SLOPE
            self.weight = 0

        else:
            raise Exception("Invalid robot type")

