import numpy as np
import torch
import copy
import queue

from CNN import CNN
from CNN import one_hot_encode

class Gameboard:
    def __init__(self, moves = []):
        self.gameboard = np.array([[0,0,0],[0,0,0],[0,0,0]], np.int32)
        self.moves = moves
        self.prio_list = []
        self.h = 0
        self.f = 0

    def __lt__(self, other):
        return self.f < other.f
    
    def switch(self, move):

        y, x = self.number_position(0)

        if move[0] == 1: #up
            if y == 0:
                pass
            else:
                self.gameboard[y][x] = self.gameboard[y - 1][x]
                self.gameboard[y - 1][x] = 0
        elif move[1] == 1: #down
            if y == 2:
                pass
            else:
                self.gameboard[y][x] = self.gameboard[y + 1][x]
                self.gameboard[y + 1][x] = 0
        elif move[2] == 1: #left
            if x == 0:
                pass
            else:
                self.gameboard[y][x] = self.gameboard[y][x - 1]
                self.gameboard[y][x - 1] = 0
        elif move[3] == 1: #right
            if x == 2:
                pass
            else:
                self.gameboard[y][x] = self.gameboard[y][x + 1]
                self.gameboard[y][x + 1] = 0

    def number_position(self, number): #use 0 to find empty position
        
        for x in range(3):
            for y in range(3):
                if self.gameboard[y][x] == number:
                    return (y,x)
                
    def valid_entry(self, number):
        try:
            number = int(number)
        except ValueError:
            return False
        if number == 0:
            return True
        elif not (0 <= number < 9):
            return False  
        elif number in self.gameboard:
            return False
        
        return True
    
    def find_legal_moves(self):
        to_ret = [[1,0,0,0],[0,1,0,0],[0,0,1,0],[0,0,0,1]]

        y,x = self.number_position(0)

        if y == 0:
            to_ret.remove([1,0,0,0])
        elif y == 2:
            to_ret.remove([0,1,0,0])

        if x == 0:
            to_ret.remove([0,0,1,0])
        elif x == 2:
            to_ret.remove([0,0,0,1])

        return to_ret
    

def is_complete(nparray):
    temp = 1

    for x in range(3):
        for y in range(3):
            if x == 2 and y == 2:
                return True

            if nparray[x][y] != temp:
                return False
            temp += 1

    return True
                    
def is_solvable(nparray):
    #intake flattened np array
    inversions = 0

    for i in range(len(nparray) - 1):
        for j in range(i + 1, len(nparray)):
            if nparray[j] != 0 and nparray[i] != 0 and nparray[i] > nparray[j]:
                inversions += 1

    return inversions % 2 == 0
    


def solve_with_CNN(test_board):
    moves = []
    visited_set = set()
    test_board.model.load()
    temp = copy.deepcopy(test_board.gameboard)

    if not is_solvable(np.ndarray.flatten(test_board.gameboard)):
        return None

    while(not is_complete(test_board.gameboard)):
        input = (torch.tensor(test_board.gameboard, dtype=torch.float32) / 8).unsqueeze(0).unsqueeze(0)
        output = test_board.model(input).squeeze()

        pred_move = [0,0,0,0]
        pred_move[output.argmax()] = 1

        prev = copy.deepcopy(test_board.gameboard)
        test_board.switch(pred_move)

        tup_board = tuple(test_board.gameboard.flatten())
        
        if tup_board in visited_set:
            test_board.gameboard = prev
            pred_move = [0,0,0,0]
            pred_move[(output - output.max()).argmax().item()] = 1
            test_board.switch(pred_move)
        else:
            tup_board = tuple(prev.flatten())
            visited_set.add(tup_board)


        
        

        moves.append(pred_move)


    test_board.gameboard = temp
    return moves


def solve_with_A_star(test_board):
    pass

def inf_loop(one, two):
        if (one == [1,0,0,0] and two == [0,1,0,0]) or (one == [0,1,0,0] and two == [1,0,0,0]):
            return True
        elif (one == [0,0,1,0] and two == [0,0,0,1]) or (one == [0,0,0,1] and two == [0,0,1,0]):
            return True
        else:
            return False


def ladder(initial):
    test_board.moves = []
    model = CNN()
    model.load()

    if not is_solvable(initial.gameboard.flatten()):
        return None

    def inf_loop(one, two):
        if (one == [1,0,0,0] and two == [0,1,0,0]) or (one == [0,1,0,0] and two == [1,0,0,0]):
            return True
        elif (one == [0,0,1,0] and two == [0,0,0,1]) or (one == [0,0,0,1] and two == [0,0,1,0]):
            return True
        else:
            return False
        

    def helper1(test_board, move, visited_set):
        if test_board.moves != [] and inf_loop(move, test_board.moves[-1]):
            return None
        elif move not in test_board.find_legal_moves():
            return None
            
        next = copy.deepcopy(test_board)
        next.switch(move)
        next.moves.append(move)

        

        if is_complete(next.gameboard):
            return next
        elif tuple(test_board.gameboard.flatten()) in visited_set:
            return None 
        elif len(next.moves) > 30:
            return None
        
        else:
            visited_set.add(tuple(test_board.gameboard.flatten()))
            input = (torch.tensor(next.gameboard, dtype=torch.float32) / 8).unsqueeze(0).unsqueeze(0)
            output = model(input).squeeze()

            for i in range(4):
                pred_move = [0,0,0,0]
                pred_move[output.argmax()] = 1
                output[output.argmax()] = -1

                board = helper1(next, pred_move, copy.deepcopy(visited_set))
                if board != None:
                    return board
                
            return None
            
                        
                    

            
    visited_set = set()
    input = (torch.tensor(test_board.gameboard, dtype=torch.float32) / 8).unsqueeze(0).unsqueeze(0)
    output = model(input).squeeze()

    for i in range(4):
        pred_move = [0,0,0,0]
        pred_move[output.argmax()] = 1
        output[output.argmax()] = -1

        
        solved = helper1(initial, pred_move, visited_set)
        if solved != None:
            return solved.moves

    return [9,9,9,9]



def solve_with(test_board):
    test_board.moves = []
    model = CNN()
    model.load()
    model.eval()
    count = 0

    if is_complete(test_board.gameboard):
        return []
    
    node_queue = queue.PriorityQueue()
    visited_set = set()

    node_queue.put(test_board)

    while not node_queue.empty():
        curr = node_queue.get()
        visited_set.add(tuple(curr.gameboard.flatten()))
        count += 1

        
        if is_complete(curr.gameboard):
            return curr.moves
        else:
            input = one_hot_encode(curr.gameboard)
            output = model(input).squeeze()
            for i in range(4):
                test_board = copy.deepcopy(curr)
                pred_move = [0,0,0,0]
                pred_move[output.argmax()] = 1
                output[output.argmax()] = -1

                if pred_move not in test_board.find_legal_moves():
                    continue
                elif test_board.f != 0 and inf_loop(pred_move, test_board.moves[-1]):
                    continue

                
                test_board.switch(pred_move)
                test_board.moves.append(pred_move)
                test_board.prio_list.append(i+1)
                if not tuple(test_board.gameboard.flatten()) in visited_set:
                    test_board.f += (i+1) ** 2
                    node_queue.put(test_board) 

