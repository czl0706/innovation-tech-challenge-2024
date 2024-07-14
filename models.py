import torch.nn as nn

class BWClassifier(nn.Module):
    def __init__(self):
        super(BWClassifier, self).__init__()
        self.conv1 = nn.Conv1d(6, 4, kernel_size=5, stride=2, padding=2)
        self.conv2 = nn.Conv1d(4, 6, kernel_size=3, stride=1, padding=1)
        self.relu = nn.ReLU()
        self.flatten = nn.Flatten()
        
        self.fc1 = nn.Linear(180, 4)
        self.fc2 = nn.Linear(180, 2)
    
    def forward(self, x):
        x = self.relu(self.conv1(x))
        x = self.relu(self.conv2(x))
        x = self.flatten(x)
        
        return self.fc1(x), self.fc2(x)