#include<bits/stdc++.h>

using namespace std;

map<string, pair<int, int>> OPTAB2 = {
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

map<string,int> SYMTAB2;

void readSYMTAB(){
    ifstream in("SYMTAB.txt");
    string label;
    int loc;
    while(in>>label>>hex>>loc){
        SYMTAB2[label] = loc;
    }
    in.close();
}

int expression(string s){

    return 0;
}

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


void pass2(int program_length){
    ifstream in("intermediate.txt");
    ofstream out("object_file.txt");
    ofstream list("listing_file.txt");
    
    ofstream err("error.txt");

    string line;
    int line_no=0;
    int locctr=0;
    int ni=0,xbpe=0,PC=0,base=0,opc=0,address=0;
    int length_T=0;
    string code="";
    bool error=false;
    

    string label,opcode,operand;
    string H_record,T_record,M_record,E_record;


    //handling START
    while(getline(in,line)){
        line_no++;
        if(line.empty() || line[0]=='.'){
            // out<<"."<<endl;
            continue;
        }
        istringstream iss(line);    
        
        iss>>label>>opcode>>operand;
        
        if(opcode=="START"){
            H_record="H "+label + " " +decimalToTwosComplement(SYMTAB2["START"],6)+" "+ decimalToTwosComplement(program_length,6) ;
            list<<hex<<decimalToTwosComplement(locctr,6)<<" "<<label<<" "<<opcode<<" "<<decimalToTwosComplement(stoi(operand),6)<<endl;
            break;
        }
        else{
            err<<"START not found at line "<<line_no<<endl;
            out <<".--------ERROR"<<endl;
            error=true;
            break;
        }
    }

    out<<H_record<<endl;
    

    while(getline(in,line)){
        line_no++;
        if(line.empty() || line[0]=='.'){
            // out<<"."<<endl;
            continue;
        }
        istringstream iss(line);    
        
        iss>>label>>opcode>>operand;
        //setting ni
        if(operand[0]=='#'){
            ni=1;
            operand=operand.substr(1);
        }
        else if(operand[0]=='@'){
            ni=2;
            operand=operand.substr(1);
        }
        else{
            ni=3;
        }

        //setting x
        if(operand.find(",X")!=string::npos){
            xbpe=8;
            operand=operand.substr(0,operand.size()-2);
        }
        else{
            xbpe=0;
        }

        //setting e
        if(opcode[0]=='+'){
            xbpe+=1;
            opcode=opcode.substr(1);
        }
        //handle if opcode
        if(OPTAB2.find(operand) != OPTAB2.end()){
            auto itr=OPTAB2.find(operand);
            int format=itr->second.second;
            if(format==1){
                if(operand!=""){
                    err<<"Invalid operand "<<operand<<" at line "<<line_no<<endl;
                    error=true;
                    continue;
                }
                code+=decimalToTwosComplement(itr->second.first,2);
                T_record+=(code+" ");
                length_T+= 1;
                code="";
            }
            else if(format==2){
                opcode=itr->second.first;
                // if(operand.find(",")!=string::npos){
                //     address+=stoi(operand.substr(2));
                // }
                // else{
                //     address+=stoi(operand.substr(1));
                // }
            }
            else if(format==3){
                if(xbpe%2==0){//format 3
                    opc=itr->second.first+ni;
                    //check if opernad is integer value
                    if(operand[0]>='0' && operand[0]<='9'){
                        if(ni!=1){
                            err<<"Immediate addressing mode not used for integer value at line "<<line_no<<endl;
                            out <<".--------ERROR"<<endl;
                            error=true;
                            continue;
                        }
                        address+=stoi(operand);
                        code=code+decimalToTwosComplement(opc,2)+decimalToTwosComplement(xbpe,1)+decimalToTwosComplement(address,3);
                        T_record+=(code+" ");
                        length_T+= 3;
                    }
                    else if(SYMTAB2.find(operand)!=SYMTAB2.end()){
                        int target_address=SYMTAB2[operand];
                        if(target_address-PC<2048 && target_address-PC>-2048){
                            address=target_address-PC;
                            xbpe+=2;//set p bit
                        }
                        else{
                            if(base!=-1){
                                if(target_address-base<4096 && target_address-base>=0){
                                    address=target_address-base;
                                    xbpe+=4;//set b bit
                                }
                                else{
                                    err<<"Address out of range for base relative addressing at line "<<line_no<<endl;
                                    out <<".--------ERROR"<<endl;
                                    error=true;
                                    continue;
                                }
                            }
                            else{
                                err<<"Address out of range for immediate addressing at line "<<line_no<<endl;
                                out <<".--------ERROR"<<endl;
                                error=true;
                                continue;
                            }
                    
                        }
                        code=code+decimalToTwosComplement(opc,2)+decimalToTwosComplement(xbpe,1)+decimalToTwosComplement(address,3);
                        T_record+=(code+" ");  
                        length_T+= 3;
                    
                    }
                    else{
                        err<<"Undefined symbol "<<operand<<" at line "<<line_no<<endl;
                        out <<".--------ERROR"<<endl;
                        error=true;
                        continue;
                    }
                    
                }
                else{
                    opc=itr->second.first+ni;
                    if(operand[0]>='0' && operand[0]<='9'){
                        if(ni!=1){
                            err<<"Immediate addressing mode not used for integer value at line "<<line_no<<endl;
                            out <<".--------ERROR"<<endl;
                            error=true;
                            continue;
                        }
                        address+=stoi(operand);
                        code=code+decimalToTwosComplement(opc,2)+decimalToTwosComplement(xbpe,1)+decimalToTwosComplement(address,5);
                        T_record+=(code+" ");
                        length_T+= 4;
                    }
                    else if(SYMTAB2.find(operand)!=SYMTAB2.end()){
                        int target_address=SYMTAB2[operand];
                        address=target_address;
                        code=code+decimalToTwosComplement(opc,2)+decimalToTwosComplement(xbpe,1)+decimalToTwosComplement(address,5);
                        T_record+=(code+" ");
                        length_T+= 4;
                    }
                    else{
                        err<<"Undefined symbol "<<operand<<" at line "<<line_no<<endl;
                        out <<".--------ERROR"<<endl;
                        error=true;
                        continue;
                    }
                }
                
            }
           

        }
        //handle assembler directive
        else{
            if(opcode=="BASE"){

                base=SYMTAB2[operand];
                continue;
            }
            

        }




        label=operand=opcode="";
    }




}