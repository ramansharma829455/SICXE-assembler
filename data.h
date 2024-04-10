#include <bits/stdc++.h>
using namespace std;

struct op_code{
    string opcode_name;
    int opcode_value;
    int opcode_format;
};

map<string, op_code> OPTAB={
    {"ADD", {"18", 3, 3}},
    {"ADDF", {"58", 3, 3}},
    {"ADDR", {"90", 2, 2}},
    {"AND", {"40", 3, 3}},
    {"CLEAR", {"B4", 2, 2}},
    {"COMP", {"28", 3, 3}},
    {"COMPF", {"88", 3, 3}},
    {"COMPR", {"A0", 2, 2}},
    {"DIV", {"24", 3, 3}},
    {"DIVF", {"64", 3, 3}},
    {"DIVR", {"9C", 2, 2}},
    {"FIX", {"C4", 1, 1}},
    {"FLOAT", {"C0", 1, 1}},
    {"HIO", {"F4", 1, 1}},
    {"J", {"3C", 3, 3}},
    {"JEQ", {"30", 3, 3}},
    {"JGT", {"34", 3, 3}},
    {"JLT", {"38", 3, 3}},
    {"JSUB", {"48", 3, 3}},
    {"LDA", {"00", 3, 3}},
    {"LDB", {"68", 3, 3}},
    {"LDCH", {"50", 3, 3}},
    {"LDF", {"70", 3, 3}},
    {"LDL", {"08", 3, 3}},
    {"LDS", {"6C", 3, 3}},
    {"LDT", {"74", 3, 3}},
    {"LDX", {"04", 3, 3}},
    {"LPS", {"D0", 3, 3}},
    {"MUL", {"20", 3, 3}},
    {"MULF", {"60", 3, 3}},
    {"MULR", {"98", 2, 2}},
    {"NORM", {"C8", 1, 1}},
    {"OR", {"44", 3, 3}},
    {"RD", {"D8", 3, 3}},
    {"RMO", {"AC", 2, 2}},
    {"RSUB", {"4C", 3, 3}},
    {"SHIFTL", {"A4", 2, 2}},
    {"SHIFTR", {"A8", 2, 2}},
    {"SIO", {"F0", 1, 1}},
    {"SSK", {"EC", 3, 3}},
    {"STA", {"0C", 3, 3}},
    {"STB", {"78", 3, 3}},
    {"STCH", {"54", 3, 3}},
    {"STF", {"80", 3, 3}},
    {"STI", {"D4", 3, 3}},
    {"STL", {"14", 3, 3}},
    {"STS", {"7C", 3, 3}},
    {"STSW", {"E8", 3, 3}},
    {"STT", {"84", 3, 3}},
    {"STX", {"10", 3, 3}},
    {"SUB", {"1C", 3, 3}},
    {"SUBF", {"5C", 3, 3}},
    {"SUBR", {"94", 2, 2}},
    {"SVC", {"B0", 2, 2}},
    {"TD", {"E0", 3, 3}},
    {"TIO", {"F8", 1, 1}},
    {"TIX", {"2C", 3, 3}},
    {"TIXR", {"B8", 2, 2}},
    {"WD", {"DC", 3, 3}}
};

set<string> assembler_directives = {"START", "END", "BYTE", "WORD", "RESB", "RESW", "BASE", "NOBASE", "EQU", "USE", "ORG", "LTORG"};


struct SS{
    string label;
    int block_no;
    int value;
    int flag; //0 for normal, 1 for relative, 2 for absolute
};
map<string, SS> SYMTAB;

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

string program_name;
int program_length;

string decimalToTwosComplement(int decimalNumber, int hexStringLength) {
    // Check if the number fits in the specified length
    if (decimalNumber >= -(1 << (4 * hexStringLength - 1)) && decimalNumber < (1 << (4 * hexStringLength - 1))) {
        // Convert negative numbers to their equivalent positive values in twos complement
        if (decimalNumber < 0) {
            decimalNumber += (1 << (4 * hexStringLength));
        }

        // Convert the positive decimal number to hexadecimal string
        std::stringstream ss;
        ss << std::uppercase << std::hex << std::setw(hexStringLength) << std::setfill('0') << decimalNumber;
        return ss.str();
    } else {
        // If the number doesn't fit in the specified length, return an empty string
        return "";
    }
}

bool pass1_line_scraper(string line, string &label, string &opcode, string &operand) {
   
    istringstream iss(line);
    vector<string> tokens;
    string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    if (tokens.size() == 0 || tokens[0][0] == '.') {
        label=opcode=operand="";
        return false;
    }
    if (tokens.size() == 1) {
        label = "";
        opcode = tokens[0];
        operand = "";
    } 
    else if (tokens.size() == 2) {
        string check = tokens[0];
        if(tokens[0][0] == '+') {
            check = tokens[0].substr(1);
        }
        if((OPTAB.find(check) == OPTAB.end()) && (assembler_directives.find(check) == assembler_directives.end())){
            label = tokens[0];
            opcode = tokens[1];
            operand = "";
        } else {
            label = "";
            opcode = tokens[0];
            operand = tokens[1];
        }

    } else {
        label = tokens[0];
        opcode = tokens[1];
        operand = tokens[2];
    }
    return true;
    
    
}

// int handle_operand(string operand, ){

// }



//PASS 2

struct text_record{
    string initial = "T";
    int start_address;
    int length;  
    string object_code;
};

vector<text_record> TEXT_RECORDS;

bool pass2_line_scraper(string line, string &label, string &opcode, string &operand) {
   
    istringstream iss(line);
    vector<string> tokens;
    string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    if (tokens.size() == 0 || tokens[0][0] == '.') {
        label=opcode=operand="";
        return false;
    }
    if (tokens.size() == 4) {
        label = "";
        opcode = tokens[3];
        operand = "";
    } 
    else if (tokens.size() == 5) {
        string check = tokens[0];
        if(tokens[3][0] == '+') {
            check = tokens[3].substr(1);
        }
        if((OPTAB.find(check) == OPTAB.end()) && (assembler_directives.find(check) == assembler_directives.end())){
            label = tokens[3];
            opcode = tokens[4];
            operand = "";
        } else {
            label = "";
            opcode = tokens[3];
            operand = tokens[4];
        }

    } else {
        label = tokens[3];
        opcode = tokens[4];
        operand = tokens[5];
    }
    return true;
    
    
}
