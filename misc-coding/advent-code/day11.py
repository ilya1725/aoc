#
# Day 11 of the coding challenge
#

import sys
import copy
import math

class Base():
    const1 = (23*13*19*17)
    const2 = (17*3*19*7*2*5*11*13)

    def __init__(self, init_list):
        self.list = init_list
        self.inspected = 0

    def Append(self, value):
        self.list.append(value)

    def Inspected(self):
        return self.inspected

# -----------------------------------------------------------------------------
class Monkey0(Base):

    def __init__(self):
        Base.__init__(self,[79, 98])

    def Process(self):
        # process the current list and return the values and destinations
        # return - list of destination,value pairs
        result = []
        for item in self.list:
            new = item * 19
            # new = int(new/3)
            new = new % Base.const1

            if (new % 23) == 0:
                destination = 2
            else:
                destination = 3

            result.append([destination, new])
            self.inspected += 1

        self.list.clear()
        return result

class Monkey1(Base):
    def __init__(self):
        Base.__init__(self, [54, 65, 75, 74])

    def Process(self):
        # process the current list and return the values and destinations
        # return - list of destination,value pairs
        result = []
        for item in self.list:
            new = item + 6
            # new = int(new/3)
            new = new % Base.const1

            if (new % 19) == 0:
                destination = 2
            else:
                destination = 0

            result.append([destination, new])
            self.inspected += 1

        self.list.clear()
        return result

class Monkey2(Base):
    def __init__(self):
        Base.__init__(self, [79, 60, 97])

    def Process(self):
        # process the current list and return the values and destinations
        # return - list of destination,value pairs
        result = []
        for item in self.list:
            new = item * item
            # new = int(new/3)
            new = new % Base.const1

            if (new % 13) == 0:
                destination = 1
            else:
                destination = 3

            result.append([destination, new])
            self.inspected += 1

        self.list.clear()
        return result

class Monkey3(Base):
    def __init__(self):
        Base.__init__(self, [74])

    def Process(self):
        # process the current list and return the values and destinations
        # return - list of destination,value pairs
        result = []
        for item in self.list:
            new = item + 3
            # new = int(new/3)
            new = new % Base.const1

            if (new % 17) == 0:
                destination = 0
            else:
                destination = 1

            result.append([destination, new])
            self.inspected += 1

        self.list.clear()
        return result

# -----------------------------------------------------------------------------
class BMonkey0(Base):
    def __init__(self):
        Base.__init__(self,[72, 64, 51, 57, 93, 97, 68])

    def Process(self):
        # process the current list and return the values and destinations
        # return - list of destination,value pairs
        result = []
        for item in self.list:
            new = item * 19
            # new = int(new/3)
            new = new % Base.const2

            if (new % 17) == 0:
                destination = 4
            else:
                destination = 7

            result.append([destination, new])
            self.inspected += 1

        self.list.clear()
        return result

class BMonkey1(Base):
    def __init__(self):
        Base.__init__(self,[62])

    def Process(self):
        # process the current list and return the values and destinations
        # return - list of destination,value pairs
        result = []
        for item in self.list:
            new = item * 11
            # new = int(new/3)
            new = new % Base.const2

            if (new % 3) == 0:
                destination = 3
            else:
                destination = 2

            result.append([destination, new])
            self.inspected += 1

        self.list.clear()
        return result

class BMonkey2(Base):
    def __init__(self):
        Base.__init__(self,[57, 94, 69, 79, 72])

    def Process(self):
        # process the current list and return the values and destinations
        # return - list of destination,value pairs
        result = []
        for item in self.list:
            new = item + 6
            # new = int(new/3)
            new = new % Base.const2

            if (new % 19) == 0:
                destination = 0
            else:
                destination = 4

            result.append([destination, new])
            self.inspected += 1

        self.list.clear()
        return result

class BMonkey3(Base):
    def __init__(self):
        Base.__init__(self,[80, 64, 92, 93, 64, 56])

    def Process(self):
        # process the current list and return the values and destinations
        # return - list of destination,value pairs
        result = []
        for item in self.list:
            new = item + 5
            # new = int(new/3)
            new = new % Base.const2

            if (new % 7) == 0:
                destination = 2
            else:
                destination = 0

            result.append([destination, new])
            self.inspected += 1

        self.list.clear()
        return result

class BMonkey4(Base):
    def __init__(self):
        Base.__init__(self,[70, 88, 95, 99, 78, 72, 65, 94])

    def Process(self):
        # process the current list and return the values and destinations
        # return - list of destination,value pairs
        result = []
        for item in self.list:
            new = item + 7
            # new = int(new/3)
            new = new % Base.const2

            if (new % 2) == 0:
                destination = 7
            else:
                destination = 5

            result.append([destination, new])
            self.inspected += 1

        self.list.clear()
        return result

class BMonkey5(Base):
    def __init__(self):
        Base.__init__(self,[57, 95, 81, 61])

    def Process(self):
        # process the current list and return the values and destinations
        # return - list of destination,value pairs
        result = []
        for item in self.list:
            new = item * item
            # new = int(new/3)
            new = new % Base.const2

            if (new % 5) == 0:
                destination = 1
            else:
                destination = 6

            result.append([destination, new])
            self.inspected += 1

        self.list.clear()
        return result

class BMonkey6(Base):
    def __init__(self):
        Base.__init__(self,[79, 99])

    def Process(self):
        # process the current list and return the values and destinations
        # return - list of destination,value pairs
        result = []
        for item in self.list:
            new = item + 2
            # new = int(new/3)
            new = new % Base.const2

            if (new % 11) == 0:
                destination = 3
            else:
                destination = 1

            result.append([destination, new])
            self.inspected += 1

        self.list.clear()
        return result

class BMonkey7(Base):
    def __init__(self):
        Base.__init__(self,[68, 98, 62])

    def Process(self):
        # process the current list and return the values and destinations
        # return - list of destination,value pairs
        result = []
        for item in self.list:
            new = item + 3
            # new = int(new/3)
            new = new % Base.const2

            if (new % 13) == 0:
                destination = 5
            else:
                destination = 6

            result.append([destination, new])
            self.inspected += 1

        self.list.clear()
        return result

# -----------------------------------------------------------------------------
def main():

    count = 0
    output1 = 0
    output2 = 0

    # round 1
    if 0:
        data = [Monkey0(), Monkey1(), Monkey2(), Monkey3()]
        # data = [BMonkey0(), BMonkey1(), BMonkey2(), BMonkey3(), BMonkey4(), BMonkey5(), BMonkey6(), BMonkey7()]

        for count in range(20):
            for d in data:
                result = d.Process()

                for r in result:
                    # destination,value
                    data[r[0]].Append(r[1])

            print(f"{count}")
            for d in data:
                print(f"{d.Inspected()}:{d.list}")


    # round 2
    if 1:
        # data = [Monkey0(), Monkey1(), Monkey2(), Monkey3()]
        data = [BMonkey0(), BMonkey1(), BMonkey2(), BMonkey3(), BMonkey4(), BMonkey5(), BMonkey6(), BMonkey7()]

        for count in range(1, 10001):
            for d in data:
                result = d.Process()

                for r in result:
                    # destination,value
                    data[r[0]].Append(r[1])

            if (count % 1000) == 0:
                print(f"{count}")
                for d in data:
                    print(f"{d.Inspected()}:{len(d.list)}")


    print(f"E: {count}:{output1}:{output2}")

if __name__ == "__main__":
    main()

