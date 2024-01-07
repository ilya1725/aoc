#
# Day 11 of the coding challenge
#

import sys
import copy
import math

data1 = [
    "abccccaaaaaaacccaaaaaaaccccccccccccccccccccccccccccccccccaaaa",
    "abcccccaaaaaacccaaaaaaaaaaccccccccccccccccccccccccccccccaaaaa",
    "abccaaaaaaaaccaaaaaaaaaaaaaccccccccccccccccccccccccccccaaaaaa",
    "abccaaaaaaaaaaaaaaaaaaaaaaacccccccccaaaccccacccccccccccaaacaa",
    "abaccaaaaaaaaaaaaaaaaaacacacccccccccaaacccaaaccccccccccccccaa",
    "abaccccaaaaaaaaaaaaaaaacccccccccccccaaaaaaaaaccccccccccccccaa",
    "abaccccaacccccccccaaaaaacccccccccccccaaaaaaaacccccccccccccccc",
    "abcccccaaaacccccccaaaaaaccccccccijjjjjjaaaaaccccccaaccaaccccc",
    "abccccccaaaaacccccaaaacccccccciiijjjjjjjjjkkkkkkccaaaaaaccccc",
    "abcccccaaaaacccccccccccccccccciiiirrrjjjjjkkkkkkkkaaaaaaccccc",
    "abcccccaaaaaccccccccccccccccciiiirrrrrrjjjkkkkkkkkkaaaaaccccc",
    "abaaccacaaaaacccccccccccccccciiiqrrrrrrrrrrssssskkkkaaaaacccc",
    "abaaaaacaaccccccccccccccccccciiiqqrtuurrrrrsssssskklaaaaacccc",
    "abaaaaacccccccccccaaccccccccciiqqqttuuuurrssusssslllaaccccccc",
    "abaaaaaccccccccaaaaccccccccciiiqqqttuuuuuuuuuuusslllaaccccccc",
    "abaaaaaacccccccaaaaaaccccccciiiqqqttxxxuuuuuuuusslllccccccccc",
    "abaaaaaaccccaaccaaaaacccccchhiiqqtttxxxxuyyyyvvsslllccccccccc",
    "abaaacacccccaacaaaaaccccccchhhqqqqttxxxxxyyyyvvsslllccccccccc",
    "abaaacccccccaaaaaaaacccccchhhqqqqtttxxxxxyyyvvssqlllccccccccc",
    "abacccccaaaaaaaaaaccaaacchhhpqqqtttxxxxxyyyyvvqqqlllccccccccc",
    "SbaaacaaaaaaaaaaaacaaaaahhhhppttttxxEzzzzyyvvvqqqqlllcccccccc",
    "abaaaaaaacaaaaaacccaaaaahhhppptttxxxxxyyyyyyyvvqqqlllcccccccc",
    "abaaaaaaccaaaaaaaccaaaaahhhppptttxxxxywyyyyyyvvvqqqmmcccccccc",
    "abaaaaaaacaaaaaaacccaaaahhhpppsssxxwwwyyyyyyvvvvqqqmmmccccccc",
    "abaaaaaaaaaaaaaaacccaacahhhpppssssssswyyywwvvvvvqqqmmmccccccc",
    "abaaaaaaaacacaaaacccccccgggppppsssssswwywwwwvvvqqqqmmmccccccc",
    "abcaaacaaaccccaaaccccccccgggppppppssswwwwwrrrrrqqqmmmmccccccc",
    "abcaaacccccccccccccccccccgggggpppoosswwwwwrrrrrqqmmmmddcccccc",
    "abccaacccccccccccccccccccccgggggoooosswwwrrrnnnmmmmmddddccccc",
    "abccccccccccccccccccccccccccgggggooossrrrrrnnnnnmmmddddaccccc",
    "abaccccaacccccccccccccccccccccgggfoossrrrrnnnnndddddddaaacccc",
    "abaccaaaaaaccccccccccccccccccccgffooorrrrnnnneeddddddaaaacccc",
    "abaccaaaaaacccccccccccccccccccccfffooooonnnneeeddddaaaacccccc",
    "abacccaaaaaccccccccaaccaaaccccccffffoooonnneeeeccaaaaaacccccc",
    "abcccaaaaacccccccccaaccaaaaccccccffffoooneeeeeaccccccaacccccc",
    "abaccaaaaaccccccccaaaacaaaaccccccafffffeeeeeaaacccccccccccccc",
    "abacccccccccccccccaaaacaaacccccccccffffeeeecccccccccccccccaac",
    "abaaaacccccccaaaaaaaaaaaaaacccccccccfffeeeccccccccccccccccaaa",
    "abaaaacccccccaaaaaaaaaaaaaaccccccccccccaacccccccccccccccccaaa",
    "abaacccccccccaaaaaaaaaaaaaaccccccccccccaacccccccccccccccaaaaa",
    "abaaaccccccccccaaaaaaaaccccccccccccccccccccccccccccccccaaaaaa"
]

data2 = [
    "Sabqponm",
    "abcryxxl",
    "accszExk",
    "acctuvwj",
    "abdefghi"
]

class Node():

    def __init__(self, location, value):
        self.location = location
        self.value = value
        self.distance = math.inf
        self.prev = None

    def __str__(self):
        return f"{self.location}:{self.value}:{self.distance}"

    def GetValue(self):
        return self.value

    def GetDistance(self):
        return self.distance

    def UpdateDistance(self, value, neighbor):
        if math.isfinite(self.distance) == True or value < self.distance:
            self.distance = value
            self.prev = neighbor
        return self.distance

    def GetEdge(self, other):
        return other.GetValue() - self.GetValue()

    def GetPrev(self):
        return self.prev

    def GetLocation(self):
        return self.location

    def GetNeighbors(self):
        output = [[self.location[0], self.location[1]-1],[self.location[0]-1, self.location[1]],[self.location[0]+1, self.location[1]],[self.location[0], self.location[1]+1]]
        return output

    def count_to_start(self):
        # Go from self to the node with None
        if self.prev == None:
            return 0
        else:
            print(self)
            return 1 + self.prev.count_to_start()

def SortFunc(node):
    return node.distance

def conversion(data):
    result = []
    for i in range(len(data)):
        line = []
        item = data[i]
        for j in range(len(item)):
            s = item[j]
            if s == 'S':
                s = 'a'
            elif s == 'E':
                s = 'z'

            obj = Node([i, j], ord(s))
            line.append(obj)
        result.append(line)

    return result

def get_position(data, item_name):
    for i in range(len(data)):
        for j in range(len(data[i])):
            if data[i][j] == item_name:
                return [i, j]
    return None

def create_queue(data):
    queue = []
    for line in data:
        for item in line:
            queue.append(item)

    return queue

def is_in(data, item):
    if item[0] < 0 or item[1] < 0:
        return None
    for i in data:
        location = i.GetLocation()
        if item[0] == location[0] and item[1] == location[1]:
            return i

    return None

def route(data, start, stop):
    processed = conversion(data)

    queue = create_queue(processed)
    processed[start[0]][start[1]].distance = 0

    # print(processed)
    queue.sort(key=SortFunc)

    while len(queue) > 0:
        u = queue.pop(0)
        if math.isfinite(u.GetDistance()) == False:
            print(f"No more data {u}")
            break

        neighbors = u.GetNeighbors()
        for neighbor in neighbors:
            other_neighbor = is_in(queue, neighbor)
            if other_neighbor != None and u.GetEdge(other_neighbor) <= 1:
                alt = u.GetDistance() + 1
                other_neighbor.UpdateDistance(alt, u)

        queue.sort(key=SortFunc)

        processed[u.GetLocation()[0]][u.GetLocation()[1]] = u

    # for line in processed:
    #     for i in line: print(f"{i} ", end='')
    #     print()

    # print(f"Path: {processed[stop[0]][stop[1]].count_to_start()}")
    return processed[stop[0]][stop[1]].GetDistance()

# -----------------------------------------------------------------------------
def main():

    count = 0
    output1 = 0
    output2 = 0

    # round 1
    if 0:
        data = data1
        processed = conversion(data)
        start = get_position(data, 'S')
        stop = get_position(data, 'E')

        queue = create_queue(processed)
        processed[start[0]][start[1]].distance = 0

        # print(processed)
        queue.sort(key=SortFunc)
        # for i in queue: print(i)

        while len(queue) > 0:
            u = queue.pop(0)
            if math.isfinite(u.GetDistance()) == False:
                print(f"No more data {u}")
                break
            if u.GetLocation() == stop:
                print(f"Found end: {u}")
                break

            neighbors = u.GetNeighbors()
            for neighbor in neighbors:
                other_neighbor = is_in(queue, neighbor)
                if other_neighbor != None and u.GetEdge(other_neighbor) <= 1:
                    alt = u.GetDistance() + 1
                    other_neighbor.UpdateDistance(alt, u)

            queue.sort(key=SortFunc)

            # print(f"--- {u.GetLocation()} {len(queue)}")
            # for i in queue: print(i)

            processed[u.GetLocation()[0]][u.GetLocation()[1]] = u

        # for line in processed:
        #     for i in line: print(f"{i} ", end='')
        #     print()

        # print(f"Path: {processed[stop[0]][stop[1]].count_to_start()}")
        print(f"Path: {processed[stop[0]][stop[1]]}")

    # round 2
    if 1:
        data = data1
        processed = conversion(data)
        start = get_position(data, 'S')
        stop = get_position(data, 'E')

        a_start = []
        for line in processed:
            a_line = []
            for s in line:
                if s.GetValue() == ord('a'):
                    length = route(data, s.GetLocation(), stop)
                    a_line.append(length)
                else:
                    a_line.append(0)
            a_start.append(a_line)
            print("Line")

        for line in a_start:
            for node in line: print(f"{node} ", end='')
            print()

    print(f"E: {count}:{output1}:{output2}")

if __name__ == "__main__":
    main()

