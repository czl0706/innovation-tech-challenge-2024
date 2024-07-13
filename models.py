import torch
import torch.nn as nn

# class BalanceClassifier(nn.Module):
#     def __init__(self):
#         super(BalanceClassifier, self).__init__()
#         self.conv1 = nn.Conv1d(6, 12, kernel_size=5, stride=2, padding=2)
#         self.conv2 = nn.Conv1d(12, 12, kernel_size=3, stride=1, padding=1)
#         self.relu = nn.ReLU()
        
#         self.fc = nn.Sequential(
#             nn.Flatten(),
#             nn.Linear(360, 4),
#         )
    
#     def forward(self, x):
#         x = self.relu(self.conv1(x))
#         x = self.relu(self.conv2(x))
#         x = self.fc(x)
#         return x
    
# class WeakPartClassifier(nn.Module):
#     def __init__(self):
#         super(WeakPartClassifier, self).__init__()
#         self.conv1 = nn.Conv1d(6, 12, kernel_size=5, stride=2, padding=2)
#         self.conv2 = nn.Conv1d(12, 12, kernel_size=3, stride=1, padding=1)
#         self.relu = nn.ReLU()
        
#         self.fc = nn.Sequential(
#             nn.Flatten(),
#             nn.Linear(360, 2),
#         )
    
#     def forward(self, x):
#         x = self.relu(self.conv1(x))
#         x = self.relu(self.conv2(x))
#         x = self.fc(x)
#         return x

# class BWClassifier(nn.Module):
#     def __init__(self):
#         super(BWClassifier, self).__init__()
#         self.conv1 = nn.Conv1d(6, 12, kernel_size=5, stride=2, padding=2)
#         self.conv2 = nn.Conv1d(12, 6, kernel_size=3, stride=1, padding=1)
#         self.relu = nn.ReLU()
#         self.flatten = nn.Flatten()
        
#         self.fc1 = nn.Linear(180, 4)
#         self.fc2 = nn.Linear(180, 2)
    
#     def forward(self, x):
#         x = self.relu(self.conv1(x))
#         x = self.relu(self.conv2(x))
#         x = self.flatten(x)
        
#         return self.fc1(x), self.fc2(x)

# class BWClassifier(nn.Module):
#     def __init__(self):
#         super(BWClassifier, self).__init__()
#         self.conv1 = nn.Conv1d(6, 6, kernel_size=5, stride=2, padding=2)
#         self.conv2 = nn.Conv1d(6, 6, kernel_size=3, stride=1, padding=1)
#         self.relu = nn.ReLU()
#         self.flatten = nn.Flatten()
        
#         self.fc1 = nn.Linear(180, 4)
#         self.fc2 = nn.Linear(180, 2)
    
#     def forward(self, x):
#         x = self.relu(self.conv1(x))
#         x = self.relu(self.conv2(x))
#         x = self.flatten(x)
        
#         return self.fc1(x), self.fc2(x)



import torch
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