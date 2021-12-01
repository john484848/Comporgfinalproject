/* 
Class Project: The logical conclusion (v1.1)
CSCI-2500 Fall 2021
*/

/******************************************************************************/
/* Usual suspects to include  */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define BIT type as a char (i.e., one byte)
typedef char BIT;
#define TRUE 1
#define FALSE 0
#define UNDEF -1

// useful constants
BIT ONE[32] = {TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, 
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};
BIT ZERO[32] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};
BIT REG_THIRTY_ONE[5] = {TRUE, TRUE, TRUE, TRUE, TRUE};
BIT THIRTY_TWO[32] = {FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, 
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};


/******************************************************************************/
/* Function prototypes */
/******************************************************************************/
BIT not_gate(BIT A);
BIT or_gate(BIT A, BIT B);
BIT or_gate3(BIT A, BIT B, BIT C);
BIT and_gate(BIT A, BIT B);
BIT and_gate3(BIT A, BIT B, BIT C);
BIT xor_gate(BIT A, BIT B);
BIT nor_gate(BIT A, BIT B);
BIT nand_gate(BIT A, BIT B);

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3);
BIT multiplexor2(BIT S, BIT I0, BIT I1);
void multiplexor2_32(BIT S, BIT* I0, BIT* I1, BIT* Output);
BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3);

void copy_bits(BIT* A, BIT* B);
void print_binary(BIT* A);
void convert_to_binary(int a, BIT* A, int length);
void convert_to_binary_char(int a, char* A, int length);
int binary_to_integer(BIT* A);

int get_instructions(BIT Instructions[][32]);

void Instruction_Memory(BIT* ReadAddress, BIT* Instruction);
void Control(BIT* OpCode,
  BIT* RegDst, BIT* Jump, BIT* Branch, BIT* MemRead, BIT* MemToReg,
  BIT* ALUOp, BIT* MemWrite, BIT* ALUSrc, BIT* RegWrite);
void Read_Register(BIT* ReadRegister1, BIT* ReadRegister2,
  BIT* ReadData1, BIT* ReadData2);
void Write_Register(BIT RegWrite, BIT* WriteRegister, BIT* WriteData);
void ALU_Control(BIT* ALUOp, BIT* funct, BIT* ALUControl);
void ALU(BIT* ALUControl, BIT* Input1, BIT* Input2, BIT* Zero, BIT* Result);
void Data_Memory(BIT MemWrite, BIT MemRead, 
  BIT* Address, BIT* WriteData, BIT* ReadData);
void Extend_Sign16(BIT* Input, BIT* Output);
void updateState();
  
/******************************************************************************/
/* Functions provided for your convenience */
/******************************************************************************/
BIT not_gate(BIT A)
{
  if (A)
    return FALSE;
  else
    return TRUE;
}

BIT or_gate(BIT A, BIT B)
{
  if (A || B)
    return TRUE;
  else
    return FALSE;
}

BIT or_gate3(BIT A, BIT B, BIT C)
{
  return or_gate(A, or_gate(B, C));
}

BIT and_gate(BIT A, BIT B)
{
  if (A && B)
    return TRUE;
  else
    return FALSE;
}

BIT and_gate3(BIT A, BIT B, BIT C)
{
  return and_gate(A, and_gate(B, C));
}

BIT xor_gate(BIT A, BIT B)
{
  if (A ^ B)
    return TRUE;
  else
    return FALSE;
}

BIT nor_gate(BIT A, BIT B)
{
  return not_gate(or_gate(A, B));
}

BIT nand_gate(BIT A, BIT B)
{
  return not_gate(and_gate(A, B));
}

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3)
{
  // Note: The input -> output mapping is slightly modified from before
  *O0 = and_gate(not_gate(I1), not_gate(I0));
  *O1 = and_gate(not_gate(I1), I0);
  *O2 = and_gate(I1, not_gate(I0));
  *O3 = and_gate(I1, I0);
  
  return;
}

BIT multiplexor2(BIT S, BIT I0, BIT I1)
{
  BIT x0 = and_gate(not_gate(S), I0);
  BIT x1 = and_gate(S, I1);
  return or_gate(x0, x1);  
}

void multiplexor2_32(BIT S, BIT* I0, BIT* I1, BIT* Output)
{
  for (int i = 0; i < 32; ++i) {
    BIT x0 = and_gate(not_gate(S), I0[i]);
    BIT x1 = and_gate(S, I1[i]);
    Output[i] = or_gate(x0, x1);
  }
}

BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3)
{
  BIT x0, x1, x2, x3 = FALSE;
  decoder2(S0, S1, &x0, &x1, &x2, &x3);
  
  BIT y0 = and_gate(x0, I0);
  BIT y1 = and_gate(x1, I1);
  BIT y2 = and_gate(x2, I2);
  BIT y3 = and_gate(x3, I3);
  
  BIT z0 = or_gate(y0, y1);
  BIT z1 = or_gate(y2, y3);
  
  return or_gate(z0, z1);  
}


/******************************************************************************/
/* Helper functions */
/******************************************************************************/
void copy_bits(BIT* A, BIT* B)
{
  for (int i = 0; i < 32; ++i)
    B[i] = A[i]; 
}
void copy_bits2(BIT* A, BIT* B,int length)
{
  for (int i = 0; i <length; ++i)
    B[i] = A[i]; 
}

void print_binary(BIT* A)
{
  for (int i = 31; i >= 0; --i)
    printf("%d", A[i]); 
}

void convert_to_binary(int a, BIT* A, int length)
{
  for(unsigned int i=0; i<length; i++)
{
  unsigned int mask = 1u << (length - 1 - i);
  A[i] = (a & mask) ? TRUE : FALSE;
}
  /* Use your implementation from Lab 6 */
}
void convert_to_binaryr(int a, BIT* A, int length)
{
  for(unsigned int i=0; i<length; i++)
{
  unsigned int mask = 1u << (length - 1 - i);
  A[i] = (a & mask) ? TRUE : FALSE;
}
  /* Use your implementation from Lab 6 */
}

void convert_to_binary_char(int a, char* A, int length)
{
  if (a >= 0) {
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 1 ? '1' : '0');
      a /= 2;
    }
  } else {
    a += 1;
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 0 ? '1' : '0');
      a /= 2;
    }
  }
}
  
int binary_to_integer(BIT* A)
{
  unsigned a = 0;
  unsigned mult = 1;
  
  for (int i = 0; i < 32; ++i) {
    a += A[i]*mult;
    mult *= 2;
  }
  
  return (int)a;
}


/******************************************************************************/
/* Parsing functions */
/******************************************************************************/


// convert instructions into opcode, funct, shamt
void convert_instruc(char Instruc_type, char* operation, BIT Output[32]) {
  // and, or, add, sub, slt

  if (Instruc_type == 'R') {
    convert_to_binary(0, Output+26, 6);
    BIT shamt[5]={FALSE,FALSE,FALSE,FALSE,FALSE};
    copy_bits2(shamt,Output+6,5);    
    // and
    if (strcmp("and\0",operation)==0) {
      BIT funct[6]={FALSE,FALSE,TRUE,FALSE,FALSE,TRUE};
      copy_bits2(funct,Output,6);
      //shamt = "00000";
    }
    // or
    else if (strcmp("or\0",operation)==0) {
      BIT funct[6]={TRUE,FALSE,TRUE,FALSE,FALSE,TRUE};
      copy_bits2(funct,Output,6);
    }
    // add
    else if (strcmp("add\0",operation)==0) {
      BIT funct[6]={FALSE,FALSE,FALSE,FALSE,FALSE,TRUE};
      copy_bits2(funct,Output,6);
      //funct = "100000";
    }
    // sub
    else if (strcmp("sub\0",operation)==0) {
      BIT funct[6]={FALSE,TRUE,FALSE,FALSE,FALSE,TRUE};
      copy_bits2(funct,Output,6);
      //funct = "100010";
    }
    // slt
    else if (strcmp("slt\0",operation)==0) {
      BIT funct[6]={FALSE,TRUE,FALSE,TRUE,FALSE,TRUE};
      copy_bits2(funct,Output,6);
      //funct = "101010";
    }
    // jr
    else if (strcmp("jr\0",operation)) {
      BIT funct[6]={FALSE,FALSE,FALSE,TRUE,FALSE,FALSE};
      copy_bits2(funct,Output,6);
      //funct = "001000";
      //shamt = "00000";
    }
  }
  else if (Instruc_type == 'I') {
    // lw
    if (strcmp("lw\0",operation)==0) {
      BIT opcode[6]={TRUE,TRUE,FALSE,FALSE,FALSE,TRUE};
      copy_bits2(opcode,Output+26,6);
    }
    // sw 
    else if (strcmp("sw\0",operation)==0) {
      BIT opcode[6]={TRUE,TRUE,FALSE,TRUE,FALSE,TRUE};
      copy_bits2(opcode,Output+26,6);
    }
    // beq
    else if (strcmp("beq\0",operation)==0) {
      BIT opcode[6]={FALSE,FALSE,TRUE,FALSE,FALSE,FALSE};
      copy_bits2(opcode,Output+26,6);
    }
    // addi
    else if (strcmp("addi",operation)==0) {
      BIT opcode[6]={FALSE,FALSE,FALSE,TRUE,FALSE,FALSE};
      copy_bits2(opcode,Output+26,6);
    }
  }
  else if (Instruc_type == 'J') {
    
    // j
    if (strlen(operation) == 1) {
      BIT opcode[6]={FALSE,TRUE,FALSE,FALSE,FALSE,FALSE};
      copy_bits2(opcode,Output+26,6);
    }
    // jal
    else if (strlen(operation) == 3) {
      BIT opcode[6]={TRUE,TRUE,FALSE,FALSE,FALSE,FALSE};
      copy_bits2(opcode,Output+26,6);
    }
  }
}

// convert the register into binary
void convert_reg_2_B(char* reg, BIT Output[]) {
  //zero
  BIT Output2[5];
  int num=reg[1]-'0';
  if (strcmp("zero\0",reg)==0) {
    convert_to_binary(0, Output2, 5);
  }
  // sp
  else if (strcmp("sp\0",reg)==0) {
    convert_to_binary(29, Output2, 5);
  }    
  // ra
  else if (strcmp("ra\0",reg)==0) {
    convert_to_binary(31, Output2, 5);
  }   
  // all v registers 
  else if (reg[0] == 'v') {
    convert_to_binary(2+num, Output2, 5);
  }    
  // all a register
  else if (reg[0] == 'a') {
    convert_to_binary(4+num, Output2, 5);
  }    
  // all t registers 
  else if (reg[0] == 't') {
    convert_to_binary(8+num, Output2, 5);
  }
  //all s registers     
  else if (reg[0] == 's') {
    convert_to_binary(16+num, Output2, 5);
  }
  for(int i=0;i<5;i++){
    Output[i]=Output2[(-1)*(i-4)];
  }    
}

// covert R type
void convert_Rtype(char* operation, char* reg1, char* reg2, char* reg3, BIT Output[]) {
  int reg_size = 5;

  if ((operation[0] == 'j') && (operation[1] == 'r')) {
    convert_reg_2_B(reg2,Output+5);
  }
  else {
    convert_reg_2_B(reg2, Output+21);
    convert_reg_2_B(reg3, Output+16);
    convert_reg_2_B(reg1,Output+11);
  }
}

// convert J type
void convert_JType(char* address,BIT Output[]) {
  int address_int = atoi(address);
  BIT Output2[26];
  convert_to_binary(address_int, Output2, 26);
  for(int i=0;i<26;i++){
    Output[i]=Output2[(-1)*(i-25)];
  } 
}

// convert I type
void convert_IType(char* reg1, char* reg2, char* offset, BIT Output[32]) {
  int offset_int = atoi(offset);
  BIT Output2[26];
  convert_to_binary(offset_int, Output2, 16);
  for(int i=0;i<16;i++){
    Output[i]=Output2[(-1)*(i-15)];
  } 
  convert_reg_2_B(reg1, Output+16);
  convert_reg_2_B(reg2, Output+21);
}
// TODO: Implement any helper functions to assist with parsing

int get_instructions(BIT Instructions[][32]) {
  char line[256] = {0};
  int instruction_count = 0;
  while (fgets(line, 256, stdin) != NULL) {     
    // TODO: perform conversion of instructions to binary
    // Input: coming from stdin via: ./a.out < input.txt
    // Output: Convert instructions to binary in the instruction memory
    // Return: Total number of instructions
    // Note: you are free to use if-else and external libraries here
    // Note: you don't need to implement circuits for saving to inst. mem.
    // My approach:
    // - Use sscanf on line to get strings for instruction and registers
    // - Use instructions to determine op code, funct, and shamt fields
    // - Convert immediate field and jump address field to binary
    // - Use registers to get rt, rd, rs fields
    // Note: I parse everything as strings, then convert to BIT array at end

    char* Operation = calloc(20, 20 * sizeof(char));
    char* reg1 = calloc(20, 20 * sizeof(char));
    char* reg2 = calloc(20, 20 * sizeof(char));
    char* reg3 = calloc(20, 20 * sizeof(char));
    char Instruc_type;
    sscanf(line,"%s %s %s %s",Operation,reg1,reg2,reg3);

    // - Use sscanf on line to get strings for instruction and registers
    // 1. if it is J instruction
    if (strcmp(Operation,"jal\0")==0 || strcmp(Operation,"j\0")==0) {
      Instruc_type = 'J';
    }
    // if it is I Instruction:lw                       sw                                  beq                              addi
    else if (strcmp("lw\0",Operation)==0 || strcmp("sw\0",Operation)==0 || strcmp("beq\0",Operation)==0 || strcmp("addi\0",Operation)==0) {
      Instruc_type = 'I';
    } 
    // if it is R Instruction:and                      or                          add                      sub                slt                                  jr
    else if (strcmp("and\0",Operation) || strcmp("or\0",Operation) || strcmp("add\0",Operation) || strcmp("sub\0",Operation) || strcmp("slt\0",Operation) || strcmp("jr\0",Operation)) {
      Instruc_type = 'R';
    }
    convert_instruc(Instruc_type, Operation, Instructions[instruction_count]);
    if (Instruc_type == 'R') {
      convert_Rtype(Operation, reg1, reg2, reg3,Instructions[instruction_count]);
    }   
    else if (Instruc_type == 'J') {
      convert_JType(reg1,Instructions[instruction_count]);
    }
    else if (Instruc_type == 'I') {
      convert_IType(reg1, reg2, reg3,Instructions[instruction_count]);
      print_binary(Instructions[instruction_count]);
      //I_output(opcode, reg1_out, reg2_out, imm_field, Instruc_temp);
    }
    instruction_count++;
  }
  
  return instruction_count;
}

/******************************************************************************/
/* Program state - memory spaces, PC, and control */
/******************************************************************************/
BIT PC[32]                  = {FALSE};
BIT MEM_Instruction[32][32] = {FALSE};
BIT MEM_Data[32][32]        = {FALSE};
BIT MEM_Register[32][32]    = {FALSE};

BIT RegDst    = FALSE;
BIT Jump      = FALSE;
BIT Branch    = FALSE;
BIT MemRead   = FALSE;
BIT MemToReg  = FALSE;
BIT ALUOp[2]  = {FALSE};
BIT MemWrite  = FALSE;
BIT ALUSrc    = FALSE;
BIT RegWrite  = FALSE;
BIT Zero      = FALSE;
BIT ALUControl[4] = {FALSE};

void print_instruction()
{
  unsigned pc = binary_to_integer(PC);
  printf("PC: %d\n", pc);
  printf("Instruction: ");
  print_binary(MEM_Instruction[pc]);
  printf("\n");
}

void print_state()
{
  printf("Data: ");
  for (int i = 0; i < 32; ++i) {
    printf("%d ", binary_to_integer(MEM_Data[i]));
  } 
  printf("\n");  
  
  printf("Register: ");
  for (int i = 0; i < 32; ++i) {
    printf("%d ", binary_to_integer(MEM_Register[i]));
  } 
  printf("\n");
}


/******************************************************************************/
/* Functions that you will implement */
/******************************************************************************/
void Instruction_Memory(BIT* ReadAddress, BIT* Instruction)
{
  // TODO: Implement instruction memory
  // Input: 32-bit instruction address
  // Output: 32-bit binary instruction
  // Note: Useful to use a 5-to-32 decoder here
  
}

void Control(BIT* OpCode,
  BIT* RegDst, BIT* Jump, BIT* Branch, BIT* MemRead, BIT* MemToReg,
  BIT* ALUOp, BIT* MemWrite, BIT* ALUSrc, BIT* RegWrite)
{
  // TODO: Set control bits for everything
  // Input: opcode field from the instruction
  // OUtput: all control lines get set 
  // Note: Can use SOP or similar approaches to determine bits
  
}

void Read_Register(BIT* ReadRegister1, BIT* ReadRegister2,
  BIT* ReadData1, BIT* ReadData2)
{
  // TODO: Implement register read functionality
  // Input: two 5-bit register addresses
  // Output: the values of the specified registers in ReadData1 and ReadData2
  // Note: Implementation will be very similar to instruction memory circuit
  
}

void Write_Register(BIT RegWrite, BIT* WriteRegister, BIT* WriteData)
{
  // TODO: Implement register write functionality
  // Input: one 5-bit register address, data to write, and control bit
  // Output: None, but will modify register file
  // Note: Implementation will again be similar to those above
  
}

void ALU_Control(BIT* ALUOp, BIT* funct, BIT* ALUControl)
{
  // TODO: Implement ALU Control circuit
  // Input: 2-bit ALUOp from main control circuit, 6-bit funct field from the
  //        binary instruction
  // Output:4-bit ALUControl for input into the ALU
  // Note: Can use SOP or similar approaches to determine bits
  
}

void ALU(BIT* ALUControl, BIT* Input1, BIT* Input2, BIT* Zero, BIT* Result)
{   
  // TODO: Implement 32-bit ALU
  // Input: 4-bit ALUControl, two 32-bit inputs
  // Output: 32-bit result, and zero flag big
  // Note: Can re-use prior implementations (but need new circuitry for zero)
  
}

void Data_Memory(BIT MemWrite, BIT MemRead, 
  BIT* Address, BIT* WriteData, BIT* ReadData)
{
  // TODO: Implement data memory
  // Input: 32-bit address, control flags for read/write, and data to write
  // Output: data read if processing a lw instruction
  // Note: Implementation similar as above
  
}

void Extend_Sign16(BIT* Input, BIT* Output)
{
  for(int i=0;i<=16;i++){
    Output[i]=Input[i];
  }
  BIT a=Input[16];
  for(int i=17;i<32;i++){
    Output[i]=a;
  }
  // TODO: Implement 16-bit to 32-bit sign extender
  // Copy Input to Output, then extend 16th Input bit to 17-32 bits in Output
  
}

void updateState()
{
  // TODO: Implement the full datapath here
  // Essentially, you'll be figuring out the order in which to process each of 
  // the sub-circuits comprising the entire processor circuit. It makes it 
  // easier to consider the pipelined version of the process, and handle things
  // in order of the pipeline. The stages and operations are:
  // Fetch - load instruction from instruction memory
  // Decode - set control bits and read from the register file
  // Execute - process ALU
  // Memory - read/write data memory
  // Write Back - write to the register file
  // Update PC - determine the final PC value for the next instruction
  
}


/******************************************************************************/
/* Main */
/******************************************************************************/

int main()
{
  setbuf(stdout, NULL);
    
  // parse instructions into binary format
  int counter = get_instructions(MEM_Instruction);
  
  // load program and run
  copy_bits(ZERO, PC);
  copy_bits(THIRTY_TWO, MEM_Register[29]);
  
  while (binary_to_integer(PC) < counter) {
    print_instruction();
    updateState();
    print_state();
  }

  return 0;
}

