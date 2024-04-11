#include <bits/stdc++.h>
using namespace std;

struct op_code{
    int opcode_value;
    int opcode_format;
};

map<string, pair<string,int8_t>> OPTAB={
    {"ADD",{"18",3}},
    {"ADDF",{"58",3}},
    {"ADDR",{"90",2}},
    {"AND",{"40",3}},
    {"CLEAR",{"B4",2}},
    {"COMP",{"28",3}},
    {"COMPF",{"88",3}},
    {"COMPR",{"A0",2}},
    {"DIV",{"24",3}},
    {"DIVF",{"64",3}},
    {"DIVR",{"9C",2}},
    {"FIX",{"C4",1}},
    {"FLOAT",{"C0",1}},
    {"HIO",{"F4",1}},
    {"J",{"3C",3}},
    {"JEQ",{"30",3}},
    {"JGT",{"34",3}},
    {"JLT",{"38",3}},
    {"JSUB",{"48",3}},
    {"LDA",{"00",3}},
    {"LDB",{"68",3}},
    {"LDCH",{"50",3}},
    {"LDF",{"70",3}},
    {"LDL",{"08",3}},
    {"LDS",{"6C",3}},
    {"LDT",{"74",3}},
    {"LDX",{"04",3}},
    {"LPS",{"D0",3}},
    {"MUL",{"20",3}},
    {"MULF",{"60",3}},
    {"MULR",{"98",2}},
    {"NORM",{"C8",1}},
    {"OR",{"44",3}},
    {"RD",{"D8",3}},
    {"RMO",{"AC",2}},
    {"RSUB",{"4C",3}},
    {"SHIFTL",{"A4",2}},
    {"SHIFTR",{"A8",2}},
    {"SIO",{"F0",1}},
    {"SSK",{"EC",3}},
    {"STA",{"0C",3}},
    {"STB",{"78",3}},
    {"STCH",{"54",3}},
    {"STF",{"80",3}},
    {"STI",{"D4",3}},
    {"STL",{"14",3}},
    {"STS",{"7C",3}},
    {"STSW",{"E8",3}},
    {"STT",{"84",3}},
    {"STX",{"10",3}},
    {"SUB",{"1C",3}},
    {"SUBF",{"5C",3}},
    {"SUBR",{"94",2}},
    {"SVC",{"B0",2}},
    {"TD",{"E0",3}},
    {"TIO",{"F8",1}},
    {"TIX",{"2C",3}},
    {"TIXR",{"B8",2}},
    {"WD",{"DC",3}}
    
};

set<string> assembler_directives = {"START", "END", "BYTE", "WORD", "RESB", "RESW", "BASE", "NOBASE", "EQU", "USE", "ORG", "LTORG"};

map<string, int> REGISTER_SET = {
    {"A", 0},
    {"X", 1},
    {"L", 2},
    {"B", 3},
    {"S", 4},
    {"T", 5},
    {"F", 6},
    {"PC", 8},
    {"SW", 9}
};

struct symbol{
    string label;
    int block_no;
    int value;
    int isValid; //0 for normal, 1 for relative, 2 for absolute
};
map<string, symbol> SYMTAB;

//literal=> label b
set <pair<string, int> > LITTAB;

struct block{
    string block_name;
    int block_no;
    int block_length;
    int start_address;  
    int block_locctr;
};
map <string,block> BLOCKTABLE;

struct block_data {
    int start_address;
    int block_length;
};
vector<block_data> BLOCK_DATA;

string program_name;
int program_length;

struct text_record{
    string initial = "T^";
    int start_address;
    int length;
    string object_code = "";
    int object_code_length = 0;
};
vector<text_record> TEXT_RECORDS;

