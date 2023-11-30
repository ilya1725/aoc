#
# Day 15 of the coding challenge
# https://adventofcode.com/2022/day/15
#

import sys
import copy
import math
from enum import IntEnum, unique

DOUBLE_RIGHT_TOP = u'\u2557'        # ╗
DOUBLE_RIGHT_BOTTOM = u'\u255d'     # ╝
DOUBLE_HORIZ_PIPE = u'\u2550'       # ═

@unique
class Result(IntEnum):
    def __str__(self):
        return str(self.name)

    right = 0
    more = 1
    not_right = 2

data1 = [
    "Sensor at x=155404, y=2736782: closest beacon is at x=2062250, y=2735130",
    "Sensor at x=2209843, y=541855: closest beacon is at x=2159715, y=2000000",
    "Sensor at x=3437506, y=3046523: closest beacon is at x=3174767, y=2783059",
    "Sensor at x=925392, y=1508378: closest beacon is at x=941123, y=1223290",
    "Sensor at x=2349988, y=3272812: closest beacon is at x=1912017, y=3034331",
    "Sensor at x=292610, y=374034: closest beacon is at x=941123, y=1223290",
    "Sensor at x=2801735, y=1324309: closest beacon is at x=2159715, y=2000000",
    "Sensor at x=3469799, y=2027984: closest beacon is at x=3174767, y=2783059",
    "Sensor at x=3292782, y=2910639: closest beacon is at x=3174767, y=2783059",
    "Sensor at x=3925315, y=2646100: closest beacon is at x=3174767, y=2783059",
    "Sensor at x=1883646, y=2054943: closest beacon is at x=2159715, y=2000000",
    "Sensor at x=2920303, y=3059306: closest beacon is at x=3073257, y=3410773",
    "Sensor at x=2401153, y=2470599: closest beacon is at x=2062250, y=2735130",
    "Sensor at x=2840982, y=3631975: closest beacon is at x=3073257, y=3410773",
    "Sensor at x=1147584, y=3725625: closest beacon is at x=1912017, y=3034331",
    "Sensor at x=2094987, y=2782172: closest beacon is at x=2062250, y=2735130",
    "Sensor at x=3973421, y=982794: closest beacon is at x=3751293, y=-171037",
    "Sensor at x=2855728, y=2514334: closest beacon is at x=3174767, y=2783059",
    "Sensor at x=1950500, y=2862580: closest beacon is at x=1912017, y=3034331",
    "Sensor at x=3233071, y=2843812: closest beacon is at x=3174767, y=2783059",
    "Sensor at x=2572577, y=3883463: closest beacon is at x=3073257, y=3410773",
    "Sensor at x=3791570, y=3910685: closest beacon is at x=3073257, y=3410773",
    "Sensor at x=3509554, y=311635: closest beacon is at x=3751293, y=-171037",
    "Sensor at x=1692070, y=2260914: closest beacon is at x=2159715, y=2000000",
    "Sensor at x=1265756, y=1739058: closest beacon is at x=941123, y=1223290"
]
data2 = [
    "Sensor at x=2, y=18: closest beacon is at x=-2, y=15",
    "Sensor at x=9, y=16: closest beacon is at x=10, y=16",
    "Sensor at x=13, y=2: closest beacon is at x=15, y=3",
    "Sensor at x=12, y=14: closest beacon is at x=10, y=16",
    "Sensor at x=10, y=20: closest beacon is at x=10, y=16",
    "Sensor at x=14, y=17: closest beacon is at x=10, y=16",
    "Sensor at x=8, y=7: closest beacon is at x=2, y=10",
    "Sensor at x=2, y=0: closest beacon is at x=2, y=10",
    "Sensor at x=0, y=11: closest beacon is at x=2, y=10",
    "Sensor at x=20, y=14: closest beacon is at x=25, y=17",
    "Sensor at x=17, y=20: closest beacon is at x=21, y=22",
    "Sensor at x=16, y=7: closest beacon is at x=15, y=3",
    "Sensor at x=14, y=3: closest beacon is at x=15, y=3",
    "Sensor at x=20, y=1: closest beacon is at x=15, y=3"
]

def process(data):
    result = []
    for line in data:
        line_array = []
        split_line = line.split(" ")

        # remove some leftovers
        for i in range(len(split_line)):
            split_line[i] = split_line[i].split(",")[0]
            split_line[i] = split_line[i].split(":")[0]

        sensor = [int(split_line[2].split("=")[1]), int(split_line[3].split("=")[1])]
        beacon = [int(split_line[8].split("=")[1]), int(split_line[9].split("=")[1])]
        line_array.append(sensor)
        line_array.append(beacon)

        result.append(line_array)

    return result

class Scan():

    start = [500, 0]

    def __init__(self, data, second = False) -> None:
        self.data = data
        self.count = 0
        self.exit_flag = False

        # find limits
        self.top_left = [None, None]
        self.bottom_right = [None, None]
        for line in self.data:
            for item in line:
                if self.top_left[0] == None or item[0] < self.top_left[0]:
                    self.top_left[0] = item[0]
                if self.bottom_right[0] == None or item[0] > self.bottom_right[0]:
                    self.bottom_right[0] = item[0]

                if self.top_left[1] == None or item[1] < self.top_left[1]:
                    self.top_left[1] = item[1]
                if self.bottom_right[1] == None or item[1] > self.bottom_right[1]:
                    self.bottom_right[1] = item[1]

        self.top_left[0] -= 1
        self.bottom_right[0] += 1
        self.bottom_right[1] += 1

        # adjust for the second round
        if second:
            self.bottom_right[1] += 2
            extend = self.bottom_right[1] - self.top_left[1] + 1
            self.top_left[0] -= extend
            self.bottom_right[0] += extend

        print(f"{self.top_left} {self.bottom_right}")

        self.structure = [['.'] * (self.bottom_right[0] - self.top_left[0] + 1) for _ in range(self.bottom_right[1] - self.top_left[1])]
        print(f"{len(self.structure)} {len(self.structure[0])}")

        if second:
            # add the bottom structure
            for i in range(len(self.structure[0])):
                self.structure[-1][i] = '#'

        # Add the data
        for line in self.data:
            sensor = line[0]
            beacon = line[1]

            self.structure[sensor[1] - self.top_left[1]][sensor[0] - self.top_left[0]] = 'S'
            self.structure[beacon[1] - self.top_left[1]][beacon[0] - self.top_left[0]] = 'B'

        # print(f"{self.top_left} {self.bottom_right}")
        # print(self.structure)

    def beacon(self, info):
        sensor = info[0]
        beacon = info[1]
        sensor = [sensor[1] - self.top_left[1]][sensor[0] - self.top_left[0]]
        beacon = [beacon[1] - self.top_left[1]][beacon[0] - self.top_left[0]]

        while True:
            

    def __str__(self):
        output = "   " + " " * len(self.structure[0]) + "\n"
        for i in range(len(self.structure)):
            output += f"{i:>2} "
            for j in range(len(self.structure[0])):
                if i == self.start[1] and (self.top_left[0] + j) == self.start[0]:
                    output += "+"
                else:
                    output += self.structure[i][j]
            output += "\n"

        return output

# -----------------------------------------------------------------------------
def main():

    count = 0
    output1 = 0
    output2 = 0

    # round 1
    if 1:
        data = data2
        lines = process(data)
        print(lines)

        scanner = Scan(lines)
        print(scanner)

        for line in lines:
            scanner.beacon(line)
            print(scanner)


    # round 2
    if 0:
        data = data1
        lines = process(data)

        scanner = Scan(lines, True)
        print(scanner)

        for _ in range(30000):
            scanner.add_sand([500,0])
            print(f"{scanner.count} {scanner.exit_flag}")

            if scanner.exit_flag:
                print(scanner)
                break

    print(f"E: {count}:{output1}:{output2}")

if __name__ == "__main__":
    main()

