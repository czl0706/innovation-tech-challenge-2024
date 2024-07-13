import torch
import numpy as np
from models import *

def export_params_to_header(model, filename):
    """
    Export PyTorch model parameters to a C header file.

    Args:
        model (torch.nn.Module): PyTorch model.
        filename (str): Name of the output header file.
    """
    params = {}

    # Collect model parameters
    for name, param in model.named_parameters():
        if param.requires_grad:
            params[name] = param.data.cpu().numpy()
            print(name, param.data.cpu().numpy().shape)

    # Export params to C header file
    with open(filename, 'w') as f:
        for name, param in params.items():
            write_parameter_to_header(f, name, param)

def write_parameter_to_header(file, name, param):
    """
    Write a parameter to a C header file.

    Args:
        file: File object for writing.
        name (str): Name of the parameter.
        param (numpy.ndarray): Parameter data.
    """
    # Flatten the parameter array
    flat_param = param.flatten()
    length = flat_param.size

    # Write the flattened parameter array to the header file
    file.write('const float {}[{}] = '.format(name.replace('.', '_'), length))
    file.write('{')
    write_array_elements(file, flat_param)
    file.write('};\n')

def write_array_elements(file, array):
    """
    Write array elements to the header file.

    Args:
        file: File object for writing.
        array (numpy.ndarray): Array data.
    """
    for i, value in enumerate(array):
        file.write(str(value))
        if i != len(array) - 1:
            file.write(', ')

# # Example usage
# model = BalanceClassifier()
# model.load_state_dict(torch.load('model_balance.pth'))
# model.eval()

# export_params_to_header(model, 'model_balance.h')

# model = WeakPartClassifier()
# model.load_state_dict(torch.load('model_weakpart.pth'))
# model.eval()

# export_params_to_header(model, 'model_weakpart.h')

model = BWClassifier()
model.load_state_dict(torch.load('model.pth'))
model.eval()

export_params_to_header(model, 'model.h')
