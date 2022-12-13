#
# Day 11 of the coding challenge
#

import sys
import copy
import math

data1 = [
"noop",
"noop",
"noop",
"addx 4",
"addx 3",
"addx 3",
"addx 3",
"noop",
"addx 2",
"addx 1",
"addx -7",
"addx 10",
"addx 1",
"addx 5",
"addx -3",
"addx -7",
"addx 13",
"addx 5",
"addx 2",
"addx 1",
"addx -30",
"addx -8",
"noop",
"addx 3",
"addx 2",
"addx 7",
"noop",
"addx -2",
"addx 5",
"addx 2",
"addx -7",
"addx 8",
"addx 2",
"addx 5",
"addx 2",
"addx -12",
"noop",
"addx 17",
"addx 3",
"addx -2",
"addx 2",
"noop",
"addx 3",
"addx -38",
"noop",
"addx 3",
"addx 4",
"noop",
"addx 5",
"noop",
"noop",
"noop",
"addx 1",
"addx 2",
"addx 5",
"addx 2",
"addx -3",
"addx 4",
"addx 2",
"noop",
"noop",
"addx 7",
"addx -30",
"addx 31",
"addx 4",
"noop",
"addx -24",
"addx -12",
"addx 1",
"addx 5",
"addx 5",
"noop",
"noop",
"noop",
"addx -12",
"addx 13",
"addx 4",
"noop",
"addx 23",
"addx -19",
"addx 1",
"addx 5",
"addx 12",
"addx -28",
"addx 19",
"noop",
"addx 3",
"addx 2",
"addx 5",
"addx -40",
"addx 4",
"addx 32",
"addx -31",
"noop",
"addx 13",
"addx -8",
"addx 5",
"addx 2",
"addx 5",
"noop",
"noop",
"noop",
"addx 2",
"addx -7",
"addx 8",
"addx -7",
"addx 14",
"addx 3",
"addx -2",
"addx 2",
"addx 5",
"addx -40",
"noop",
"noop",
"addx 3",
"addx 4",
"addx 1",
"noop",
"addx 2",
"addx 5",
"addx 2",
"addx 21",
"noop",
"addx -16",
"addx 3",
"noop",
"addx 2",
"noop",
"addx 1",
"noop",
"noop",
"addx 4",
"addx 5",
"noop",
"noop",
"noop",
"noop",
"noop",
"noop",
"noop"
]

data2 = [
"addx 15",
"addx -11",
"addx 6",
"addx -3",
"addx 5",
"addx -1",
"addx -8",
"addx 13",
"addx 4",
"noop",
"addx -1",
"addx 5",
"addx -1",
"addx 5",
"addx -1",
"addx 5",
"addx -1",
"addx 5",
"addx -1",
"addx -35",
"addx 1",
"addx 24",
"addx -19",
"addx 1",
"addx 16",
"addx -11",
"noop",
"noop",
"addx 21",
"addx -15",
"noop",
"noop",
"addx -3",
"addx 9",
"addx 1",
"addx -3",
"addx 8",
"addx 1",
"addx 5",
"noop",
"noop",
"noop",
"noop",
"noop",
"addx -36",
"noop",
"addx 1",
"addx 7",
"noop",
"noop",
"noop",
"addx 2",
"addx 6",
"noop",
"noop",
"noop",
"noop",
"noop",
"addx 1",
"noop",
"noop",
"addx 7",
"addx 1",
"noop",
"addx -13",
"addx 13",
"addx 7",
"noop",
"addx 1",
"addx -33",
"noop",
"noop",
"noop",
"addx 2",
"noop",
"noop",
"noop",
"addx 8",
"noop",
"addx -1",
"addx 2",
"addx 1",
"noop",
"addx 17",
"addx -9",
"addx 1",
"addx 1",
"addx -3",
"addx 11",
"noop",
"noop",
"addx 1",
"noop",
"addx 1",
"noop",
"noop",
"addx -13",
"addx -19",
"addx 1",
"addx 3",
"addx 26",
"addx -30",
"addx 12",
"addx -1",
"addx 3",
"addx 1",
"noop",
"noop",
"noop",
"addx -9",
"addx 18",
"addx 1",
"addx 2",
"noop",
"noop",
"addx 9",
"noop",
"noop",
"noop",
"addx -1",
"addx 2",
"addx -37",
"addx 1",
"addx 3",
"noop",
"addx 15",
"addx -21",
"addx 22",
"addx -6",
"addx 1",
"noop",
"addx 2",
"addx 1",
"noop",
"addx -10",
"noop",
"noop",
"addx 20",
"addx 1",
"addx 2",
"addx 2",
"addx -6",
"addx -11",
"noop",
"noop",
"noop"
]

def main():

    count = 0
    output1 = 0
    output2 = 0

    # round 1
    if 0:
        data = data1

        clock = []
        next_value = 1
        cycle = 19
        cycle_step = 40
        cycle_cnt = 0
        output1 = 0

        for cmd in data:
            split = cmd.split()
            if split[0] == 'noop':
                clock.append(next_value)
            else:
                clock.append(next_value)
                clock.append(next_value)
                next_value = clock[-1] + int(split[1])
            # print(f"{cmd}:{len(clock)} - {clock[-10:]}")

            result = 0
            if len(clock) == (cycle + cycle_step*cycle_cnt):
                result = (len(clock)+1) * next_value
                print(f"S:{len(clock)}:{next_value} {result}")
                output1 += result
                cycle_cnt += 1
            elif (len(clock) - 1) == (cycle + cycle_step*cycle_cnt):
                result = len(clock) * clock[-1]
                print(f"F:{len(clock)-1}:{clock[-1]} {result}")
                output1 += result
                cycle_cnt += 1


    # round 2
    if 1:
        data = data1

        clock = []
        next_value = 1
        cycle = 19
        cycle_step = 40
        cycle_cnt = 0
        output1 = 0
        output_map = [['.' for i in range(cycle_step)] for i in range(cycle_step)]

        for cmd in data:
            split = cmd.split()
            if split[0] == 'noop':
                clock.append(next_value)
            else:
                clock.append(next_value)
                clock.append(next_value)
                next_value = clock[-1] + int(split[1])
            print(f"{cmd}:{len(clock)} - {clock[-10:]}")

            size = len(clock)
            pixel = (size % cycle_step) - 1
            if pixel == 0:
                pixel_min = 0
            else:
                pixel_min = pixel - 1
            if pixel == (cycle_step-1):
                pixel_max = cycle_step-1
            else:
                pixel_max = pixel + 1

            if pixel_min <= clock[-1] and clock[-1] <= pixel_max:
                output_map[int(size / cycle_step)][pixel] = '#'
                print(f"S:{pixel}:{clock[-1]}:{output_map[int(size / cycle_step)]}")

            if split[0] != 'noop':
                # two cycles
                size = len(clock) - 1
                pixel = (size % cycle_step) - 1
                if pixel == 0:
                    pixel_min = 0
                else:
                    pixel_min = pixel - 1
                if pixel == (cycle_step-1):
                    pixel_max = cycle_step-1
                else:
                    pixel_max = pixel + 1

                if pixel_min <= clock[-2] and clock[-2] <= pixel_max:
                    output_map[int(size / cycle_step)][pixel] = '#'
                    print(f"F:{pixel}:{clock[-2]}:{output_map[int(size / cycle_step)]}")

        for line in output_map:
            for l in line:
                print(f"{l}", end='')
            print("")


    print(f"E: {count}:{output1}:{output2}")

if __name__ == "__main__":
    main()

