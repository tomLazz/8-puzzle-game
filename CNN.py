import torch
import torch.nn as nn
import torch.optim as optim 

import struct
import os
 
class CNN(nn.Module):
    def __init__(self):
        super(CNN, self).__init__()

        self.conv1 = nn.Conv2d(in_channels=9, out_channels=32, kernel_size=(2,2))
        self.fc1 = nn.Linear(in_features=128, out_features=256)
        self.fc2 = nn.Linear(in_features=256, out_features=86)
        self.fc3 = nn.Linear(in_features=86, out_features=21)
        self.fc4 = nn.Linear(in_features=21, out_features=4)

        self.relu = torch.nn.ReLU()
        self.softmax = nn.Softmax(dim= 0)

    def forward(self, x): 
        x = self.relu(self.conv1(x))

        x = torch.flatten(x,0)

        x = self.relu(self.fc1(x))
        x = self.relu(self.fc2(x))
        x = self.relu(self.fc3(x))
        x = self.softmax(self.fc4(x))

        return x
    
    def save(self, file_name='model-tst.pth'):
        torch.save(self.state_dict(), file_name)

    def load(self):
        self.load_state_dict(torch.load(r'C:\Users\thoma\Documents\TileProjectFinal\model-tst-large.pth'))
        self.eval()


def one_hot_encode(board):
    to_ret = torch.zeros(9,3,3,dtype=torch.float32)

    num = 0
    for i in range(3):
        for j in range(3):
            to_ret[num][i][j] = board[i][j]
            num +=1

    return to_ret.unsqueeze(0)
    
def train(model):
    model.train()
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.Adam(model.parameters(), lr = 0.001)
    

    with open('moves-data-40k.bin', 'rb') as file:
        data = file.read()
        num_moves = len(data) // 4  
        moves = struct.unpack('<{}i'.format(num_moves), data)

    with open('boards-data-40k.bin', 'rb') as file:
        data = file.read()
        num_ints = len(data) // 4
        boards = struct.unpack('<{}i'.format(num_ints), data)

    correct = 0
    for num in range(len(moves)):
        curr_move = moves[num]
        expected_output = torch.zeros(4, dtype=torch.float32)
        expected_output[curr_move - 1] = 1


        index = num * 9
        input = one_hot_encode([boards[index : index + 3], 
                      boards[index + 3 : index + 6],
                      boards[index + 6 : index + 9]])
        
        optimizer.zero_grad() 


        output = model(input).squeeze()
        loss = criterion(output, expected_output)
        loss.backward()

        optimizer.step()
                                            

    model.save()
        
