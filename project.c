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
  A[i] = (a & mask) ? '1' : '0';
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
void convert_instruc(char Instruc_type, char* operation, char** opcode, char** funct, char** shamt) {
  // and, or, add, sub, slt
  if (Instruc_type == 'R') {
    convert_to_binary_char(0, *opcode, 6);
    
    // and
    if ((strlen(operation) == 3) && (operation[0] == 'a') && (operation[1] == 'n') && (operation[2] == 'd')) {
      *funct = "100100";
      *shamt = "00000";
    }
    // or
    else if ((strlen(operation) == 2) && (operation[0] == 'o') && (operation[1] == 'r')) {
      *funct = "100101";
      *shamt = "00000";
    }
    // add
    else if ((strlen(operation) == 3) && (operation[0] == 'a') && (operation[1] == 'd') && (operation[2] == 'd')) {
      *funct = "100000";
      *shamt = "00000";
    }
    // sub
    else if ((strlen(operation) == 3) && (operation[0] == 's') && (operation[1] == 'u') && (operation[2] == 'b')) {
      *funct = "100010";
      *shamt = "00000";
    }
    // slt
    else if ((strlen(operation) == 3) && (operation[0] == 's') && (operation[1] == 'l') && (operation[2] == 't')) {
      *funct = "101010";
    }
    // jr
    else if ((strlen(operation) == 2) && (operation[0] == 'j') && (operation[1] == 'r')) {
      *funct = "001000";
      *shamt = "00000";
    }
  }
  else if (Instruc_type == 'I') {
    // lw
    if ((strlen(operation) == 2) && (operation[0] == 'l') && (operation[1] == 'w')) {
      *opcode = "100011";
    }
    // sw 
    if ((strlen(operation) == 2) && (operation[0] == 's') && (operation[1] == 'w')) {
      *opcode = "101011";
    }
    // beq
    else if ((strlen(operation) == 3) && (operation[0] == 'b') && (operation[1] == 'e') && (operation[2] == 'q')) {
      *opcode = "000100";
    }
    // addi
    else if ((strlen(operation) == 4) && (operation[0] == 'a') && (operation[1] == 'd') && (operation[2] == 'd') && (operation[3] == 'i')) {
      *opcode = "001000";
    }
  }
  else if (Instruc_type == 'J') {
    // j
    if (strlen(operation) == 1) {
      *opcode = "000010";
    }
    // jal
    if (strlen(operation) == 3) {
      *opcode = "000011";
    }
  }
}

// convert the register into binary
void convert_reg_2_B(char* reg, char** binary_output, int len) {
  //zero
  int num=reg[1]-'0';
  if ((reg[0] == 'z') && (reg[1] == 'e') && (reg[2] == 'r') && (reg[3] == 'o')) {
    convert_to_binary_char(0, *binary_output, 5);
  }
  // sp
  else if ((reg[0] == 's') && (reg[1] == 'p')) {
    convert_to_binary_char(29, *binary_output, 5);
  }    
  // ra
  else if ((reg[0] == 'r') && (reg[1] == 'a')) {
    convert_to_binary_char(31, *binary_output, 5);
  }   
  // all v registers 
  else if ((reg[0] == 'v')) {
    convert_to_binary_char(2+num, *binary_output, 5);
  }    
  // all a register
  else if ((reg[0] == 'a')) {
    convert_to_binary_char(4+num, *binary_output, 5);
  }    
  // all t registers 
  else if ((reg[0] == 't')) {
    convert_to_binary_char(8+num, *binary_output, 5);
  }
  //all s registers     
  else if ((reg[0] == 's')) {
    convert_to_binary_char(16+num, *binary_output, 5);
  }    
}

// covert R type
void convert_Rtype(char* operation, char* reg1, char* reg2, char* reg3, char** reg1_out, char** reg2_out, char** reg3_out) {
  int reg_size = 5;

  if ((operation[0] == 'j') && (operation[1] == 'r')) {
    convert_reg_2_B(reg2, reg2_out, reg_size);
  }
  else {
    convert_reg_2_B(reg1, reg1_out, reg_size);
    convert_reg_2_B(reg2, reg2_out, reg_size);
    convert_reg_2_B(reg3, reg3_out, reg_size);
  }
}

// conver J type
void convert_JType(char* address, char** j_address) {
  int address_int = atoi(address);

  convert_to_binary_char(address_int, *j_address, 26);
}

// convert I type
void convert_IType(char* reg1, char* reg2, char* offset, char** reg1_out, char** reg2_out, char** imm_field) {
  int reg_size = 5;
  int offset_int = atoi(offset);
  
  convert_to_binary_char(offset_int, *imm_field, 16);

  convert_reg_2_B(reg1, reg2_out, reg_size);
  convert_reg_2_B(reg2, reg1_out, reg_size);
}

// output J type
void J_output(char* opcode, char* j_address, BIT Output[32]) {
  int i, count = 0;

  for (i = 0; i < 26; i++) {
    if (j_address[i] == '1') {
      Output[count] = TRUE;
    }
    else {
      Output[count] = FALSE;
    }
    count++;
  }

  // opcode
  for ( i = 5; i >= 0; i--) {
    if (opcode[i] == '1') {
      Output[count] = TRUE;
    }
    else {
      Output[count] = FALSE;
    }
    count++;
  }
}

// output R type
void R_output(char* opcode, char* rd_out, char* rs_out, char* rt_out, char* shamt, char* funct, BIT Output[32]) {
  int i, count = 0;

  // funct
  for (i = 5; i >= 0; i--) {
    if (funct[i] == '1') {
      Output[count] = TRUE;
    }
    else{
      Output[count] = FALSE;
    }
    count++;
  }  

  // shamt
  for (i = 4; i >= 0; i--) {
    if (shamt[i] == '1') {
      Output[count] = TRUE;
    }
    else{
      Output[count] = FALSE;
    }
    count++;
  }

  // rd
  for (i = 0; i <=4; i++) {
    if (rd_out[i] == '1') {
      Output[count] = TRUE;
    }
    else{
      Output[count] = FALSE;
    }
    count++;
  }

  // rt
  for (i = 0; i <=4; i++) {
    if (rt_out[i] == '1') {
      Output[count] = TRUE;
    }
    else{
      Output[count] = FALSE;
    }
    count++;
  }

  // rs
  for (i = 0; i <=4; i++) {
    if (rs_out[i] == '1') {
      Output[count] = TRUE;
    }
    else {
      Output[count] = FALSE;
    }
    count++;
  } 

  // opcode
  for ( i = 5; i >= 0; i--) {
    if (opcode[i] == '1') {
      Output[count] = TRUE;
    }
    else {
      Output[count] = FALSE;
    }
    count++;
  }
}

// output I type
void I_output(char* opcode, char* rs_out, char* rt_out, char* imm_field, BIT Output[32]) {
  int i, count = 0;

  // immdiate_field
  for (i = 0; i < 16; i++) {
    if (imm_field[i] == '1') {
      Output[count] = TRUE;
    }
    else {
      Output[count] = FALSE;
    }
    count++;
  }

  // rt
  for (i = 0; i < 5; i++) {
    if (rt_out[i] == '1') {
      Output[count] = TRUE;
    }
    else{
      Output[count] = FALSE;
    }
    count++;
  }

  // rs
  for (i = 0; i < 5; i++) {
    if (rs_out[i] == '1') {
      Output[count] = TRUE;
    }
    else {
      Output[count] = FALSE;
    }
    count++;
  } 

  // opcode
  for ( i = 5; i >= 0; i--) {
    
    if (opcode[i] == '1') {
      Output[count] = TRUE;
    }
    else {
      Output[count] = FALSE;
    }

    count++;
  }
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
    char* offset = calloc(20, 20 * sizeof(char));
    char* address = calloc(20, 20 * sizeof(char));
    char Instruc_type;

    // - Use sscanf on line to get strings for instruction and registers
    // 1. if it is J instruction
    if ((line[0] == 'j') && (line[1] != 'r')) {
      sscanf(line, "%s %s", Operation, address);
      Instruc_type = 'J';
    }
    // if it is I Instruction:lw                       sw                                        beq                                                            addi
    else if (((line[0] == 'l') && (line[1] == 'w')) || ((line[0] == 's') && (line[1] == 'w')) || ((line[0] == 'b') && (line[1] == 'e') && (line[2] == 'q')) || ((line[0] == 'a') && (line[1] == 'd') && (line[2] == 'd') && (line[3] == 'i'))) {
      sscanf(line, "%s %s %s %s", Operation, reg1, reg2, offset);
      Instruc_type = 'I';
    } 
    // if it is R Instruction:and                                           or                                       add                                                           sub                                                           slt                                                            jr
    else if (((line[0] == 'a') && (line[1] == 'n') && (line[2] == 'd')) || ((line[0] == 'o') && (line[1] == 'r')) || ((line[0] == 'a') && (line[1] == 'd') && (line[2] == 'd')) || ((line[0] == 's') && (line[1] == 'u') && (line[2] == 'b')) || ((line[0] == 's') && (line[1] == 'l') && (line[2] == 't')) || ((line[0] == 'j') && (line[1] == 'r'))) {
      if ((line[0] == 'j') && (line[1] == 'r')) {
        sscanf(line, "%s %s", Operation, reg2);
      }
      else {
        sscanf(line, "%s %s %s %s", Operation, reg1, reg2, reg3);
      }
      
      Instruc_type = 'R';
    }
    
    //- Use instructions to determine op code, funct, and shamt fields
    char* opcode = calloc(20, 20 * sizeof(char));
    char* funct = calloc(20, 20 * sizeof(char));
    char* shamt = calloc(20, 20 * sizeof(char));
    char* type = calloc(20, 20 * sizeof(char)); 
    convert_instruc(Instruc_type, Operation, &opcode, &funct, &shamt);

    // - Convert immediate field and jump address field to binary
    char* reg1_out = calloc(5, 5 * sizeof(char));
    char* reg2_out = calloc(5, 5 * sizeof(char));
    char* reg3_out = calloc(5, 5 * sizeof(char));
    memset(reg1_out, '0', 5);
    memset(reg2_out, '0', 5);
    memset(reg3_out, '0', 5);
    char* imm_field = calloc(16, 16 * sizeof(char));
    memset(imm_field, '0', 16);
    char* j_address = calloc(26, 26 * sizeof(char));
    memset(j_address, '0', 26);

    BIT Instruc_temp[32] = {FALSE};
    int i;

    if (Instruc_type == 'R') {
      convert_Rtype(Operation, reg1, reg2, reg3, &reg1_out, &reg2_out, &reg3_out);
      R_output(opcode, reg1_out, reg2_out, reg3_out, shamt, funct, Instruc_temp);
    }   
    else if (Instruc_type == 'J') {
      convert_JType(address, &j_address);
      J_output(opcode, j_address, Instruc_temp);
    }
    else if (Instruc_type == 'I') {
      convert_IType(reg1, reg2, offset, &reg1_out, &reg2_out, &imm_field);
      I_output(opcode, reg1_out, reg2_out, imm_field, Instruc_temp);
    }
 
    for (i = 0; i < 32; i++) {
      Instructions[instruction_count][i] = Instruc_temp[i];
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

