#    SPDX-FileCopyrightText: 2024 MVellosa <mvellosa@usp.br>
#   
#    SPDX-License-Identifier: GPL-3.0-or-later
#
#    This file is part of SYSeg, available at https://gitlab.com/monaco/syseg.

import sys
import re

# Registers and their addresses
registers = {
    'al': 0,    
    'ah': 4
}

instructions = {
    'movb': 0xB0,
    'int': 0xCD,
    'hlt': 0xF4,
    'jmp': 0xEB
}



def strip_value(value):
    # remove special characters from values
    return value.replace("$", "").replace("'", "").replace("%", "")

def assemble_instruction(instruction):
    if instruction[0] == "movb":
        str_value = strip_value(instruction[1])
        
        # check if value is a register or a hex number
        if str_value.startswith("0x"):
            bin_value = int(str_value, 16)
        else:
            bin_value = ord(str_value)

        # get register address
        register_addr = registers[strip_value(instruction[2])]

        return [instructions["movb"] + register_addr, bin_value]
    elif instruction[0] == "int":
        int_code = int(strip_value(instruction[1]), 16)
        return [instructions["int"], int_code]
    elif instruction[0] == "hlt":
        return [instructions["hlt"]]
    elif instruction[0] == "jmp":
        return [instructions["jmp"], 0xFD] # jump 3 bytes back

def main():

    # Read assembly code from file
    filename = sys.argv[1]
    with open(filename, "r") as f:
        code = f.read() 

    lines = code.strip().split("\n") # Split code into lines

    binary_code = []

    for line in lines:
        line = line.split("/*")[0].strip() # Remove comments to the right of the code

        # Tokenizing intructions
        instruction = list(filter(lambda x: x != "",re.split(',|(?<!\')\s', line)))

        if instruction == []:
            continue
        
        # Assemble instruction and add to binary code
        assembled_instruction = assemble_instruction(instruction)
        if assembled_instruction is not None:
            binary_code.extend(assembled_instruction)

        continue

    # Calculate remaining bytes to fill up to 510
    remaining_bytes = 510 - len(binary_code)
    binary_code.extend([0x00] * remaining_bytes)
    
    binary_code.extend([0x55, 0xAA])  # Boot signature

    filename_noext = filename.split(".")[0]

    # Write binary code to file
    with open(f"{filename_noext}.bin", "wb") as f:
        f.write(bytearray(binary_code))

    print(f"{filename_noext}.bin created successfully.")

if __name__ == "__main__":
    main()