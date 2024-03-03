#ifndef INSTRUCTION_H
#define INSTRUCTION_H

int Instruction_Map(int process_id, int virtual_address, int value);
int Instruction_Store(int process_id, int virtual_address, int value);
int Instruction_Load(int process_id, int virtual_address);


#endif
