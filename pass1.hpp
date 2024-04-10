#include <bits/stdc++.h>
using namespace std;

map<string, pair<int, int>> OPTAB = {
    {"ADD", make_pair(0x18, 3)},
    {"ADDF", make_pair(0x58, 3)},
    {"ADDR", make_pair(0x90, 2)},
    {"AND", make_pair(0x40, 3)},
    {"CLEAR", make_pair(0xB4, 2)},
    {"COMP", make_pair(0x28, 3)},
    {"COMPF", make_pair(0x88, 3)},
    {"COMPR", make_pair(0xA0, 2)},
    {"DIV", make_pair(0x24, 3)},
    {"DIVF", make_pair(0x64, 3)},
    {"DIVR", make_pair(0x9C, 2)},
    {"FIX", make_pair(0xC4, 1)},
    {"FLOAT", make_pair(0xC0, 1)},
    {"HIO", make_pair(0xF4, 1)},
    {"J", make_pair(0x3C, 3)},
    {"JEQ", make_pair(0x30, 3)},
    {"JGT", make_pair(0x34, 3)},
    {"JLT", make_pair(0x38, 3)},
    {"JSUB", make_pair(0x48, 3)},
    {"LDA", make_pair(0x00, 3)},
    {"LDB", make_pair(0x68, 3)},
    {"LDCH", make_pair(0x50, 3)},
    {"LDF", make_pair(0x70, 3)},
    {"LDL", make_pair(0x08, 3)},
    {"LDS", make_pair(0x6C, 3)},
    {"LDT", make_pair(0x74, 3)},
    {"LDX", make_pair(0x04, 3)},
    {"LPS", make_pair(0xD0, 3)},
    {"MUL", make_pair(0x20, 3)},
    {"MULF", make_pair(0x60, 3)},
    {"MULR", make_pair(0x98, 2)},
    {"NORM", make_pair(0xC8, 1)},
    {"OR", make_pair(0x44, 3)},
    {"RD", make_pair(0xD8, 3)},
    {"RMO", make_pair(0xAC, 2)},
    {"RSUB", make_pair(0x4C, 3)},
    {"SHIFTL", make_pair(0xA4, 2)},
    {"SHIFTR", make_pair(0xA8, 2)},
    {"SIO", make_pair(0xF0, 1)},
    {"SSK", make_pair(0xEC, 3)},
    {"STA", make_pair(0x0C, 3)},
    {"STB", make_pair(0x78, 3)},
    {"STCH", make_pair(0x54, 3)},
    {"STF", make_pair(0x80, 3)},
    {"STI", make_pair(0xD4, 3)},
    {"STL", make_pair(0x14, 3)},
    {"STS", make_pair(0x7C, 3)},
    {"STSW", make_pair(0xE8, 3)},
    {"STT", make_pair(0x84, 3)},
    {"STX", make_pair(0x10, 3)},
    {"SUB", make_pair(0x1C, 3)},
    {"SUBF", make_pair(0x5C, 3)},
    {"SUBR", make_pair(0x94, 2)},
    {"SVC", make_pair(0xB0, 2)},
    {"TD", make_pair(0xE0, 3)},
    {"TIO", make_pair(0xF8, 1)},
    {"TIX", make_pair(0x2C, 3)},
    {"TIXR", make_pair(0xB8, 2)},
    {"WD", make_pair(0xDC, 3)}
};

int PROGRAM_LENGTH=-1;
map<string,int> SYMTAB;

void writeSYMTAB(){
    ofstream out("SYMTAB.txt");
    for(auto i:SYMTAB){
        out<<i.first<<" "<<hex<<i.second<<endl;
    }
    out.close();
}


int pass1()
{
    int locctr = 0;
    int line_no = 0;
    bool error=false;
    // cout<<error;

    ifstream in("input.txt");
    ofstream out("intermediate.txt");
    ofstream err("error.txt");

    string line;
    string label, opcode, operand;

    //handling START
    while(getline(in,line)){
        line_no++;
        if(line.empty() || line[0]=='.'){
            out<<"."<<endl;
            continue;
        }
        istringstream iss(line);    
        if(line[0]==' '){
            label="$$$$";
            iss>>opcode>>operand;
        }
        else{
            iss>>label>>opcode>>operand;
        }
        if(opcode=="START"){
            locctr = stoi(operand,nullptr,16);
            SYMTAB[label] = SYMTAB[opcode] = locctr;
            out<<label<<" "<<opcode<<" "<<operand<<endl;
            break;
        }
        else{
            err<<"START not found at line "<<line_no<<endl;
            out <<".--------ERROR"<<endl;
            error=true;
            break;
        }
    }


    while (getline(in, line))
    {
        line_no++;
        // if line is empty or comment
        if (line.empty() || line[0] == '.'){
            out << "." << endl;
            continue;
        }

        istringstream iss(line);
        // if there is space initially, give label value as " "
        if (line[0] == ' '){
            label = "$$$$";
            iss >> opcode >> operand;
        }
        else{
            iss >> label >> opcode >> operand;
        }

        // handling symbol
        if (SYMTAB.find(label) != SYMTAB.end()){
            err << "Duplicate label \"" << label << "\" at line " << line_no << endl;
            out <<".--------ERROR"<<endl;
            error=true;
            continue;
        }
        else if (label != "$$$$") SYMTAB[label] = locctr;

      

        // search SYMTAB for operand length
        if (opcode[0] == '+'){
            auto itr = OPTAB.find(opcode.substr(1));
            if (itr != OPTAB.end()){
                if (itr->second.second == 3) locctr+=4;
                else{
                    err << "Invalid opcode : " << opcode << " for format 4 at line " << line_no << endl;
                    out <<".--------ERROR"<<endl;
                    error=true;
                    continue;
                }
            }
        }
        else if(OPTAB.find(opcode) != OPTAB.end()){
            locctr += OPTAB[opcode].second;
        }

        else if (opcode == "WORD")
        {
            locctr += 3;
        }
        else if (opcode == "RESW")
        {
            locctr += 3 * stoi(operand);
        }
        else if (opcode == "RESB")
        {   
            locctr += stoi(operand);
        }
        else if(opcode=="USE"){
            
        }
        else if (opcode == "BYTE")
        {
            if (operand[0] == 'X'){
                if (!(operand[1] == '\'' && operand[operand.length() - 1] == '\'') || (operand.length() - 3) % 2 != 0){
                    err << "Invalid operand " << operand << " at line " << line_no << endl;
                    out <<".--------ERROR"<<endl;
                    error=true;
                    continue;
                }
                locctr += (operand.length() - 3) / 2;
            }
            else if (operand[0] == 'C'){
                if (!(operand[1] == '\'' && operand[operand.length() - 1] == '\'')){
                    err << "Invalid operand " << operand << " at line " << line_no << endl;
                    out <<".--------ERROR"<<endl;
                    error=true;
                    continue  ;
                }
                locctr += operand.length() - 3;
            }
        }
        else if (opcode == "LTORG")
        {
            out << label << " " << opcode << " " << operand << endl;
        }
        else if (opcode == "EQU")
        {
            out << label << " " << opcode << " " << operand << endl;
        }
        else if (opcode == "ORG")
        {
            out << label << " " << opcode << " " << operand << endl;
        }
        else if (opcode == "BASE")
        {   
            out << label << " " << opcode << " " << operand << endl;
        }
        else if (opcode == "END")
        {   
            SYMTAB[opcode] = locctr;
            out << label << " " << opcode << " " << operand << endl;
            PROGRAM_LENGTH = SYMTAB["END"]-SYMTAB["START"];
            out << "PROGRAM LENGTH: " << hex << PROGRAM_LENGTH << endl;
            out.close();
            continue;
        }
        else
        {
            err << "Invalid opcode " << opcode << " at line " << line_no << endl;
            out <<".--------ERROR"<<endl;
            error=true;
            continue;
        }
        out << label << " " << opcode << " " << operand << endl;
        label=operand=opcode="";
    }
    in.close();
    err.close();
    writeSYMTAB();
    // cout<<error<<endl;
    if(error) return -1;
    return PROGRAM_LENGTH;
}
