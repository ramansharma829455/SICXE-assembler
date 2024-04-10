#include<bits/stdc++.h>
#include "data.h"

using namespace std;

bool pass1(){
    ifstream pass1_in("input.txt");
    ofstream pass1_out("intermediate.txt");
    ofstream pass1_err("error.txt");

    string line;
    string label, opcode, operand;

    int LOCCTR = 0;
    bool error = false;

    int current_block_no = 0;
    string current_block_name = "default";
    int total_blocks = 0;
    int line_no=0;
    int star_count=0;
    int literal_count=0;

    //handling start and use

    while(getline(pass1_in, line)){
        line_no++;
        //comment or blank line
        if(!pass1_line_scraper(line, label, opcode, operand)){
            pass1_out<<line<<endl;
            continue;
        }

        //check label has length <=6
        if(label.size()>6) {
            pass1_err<<"Label size greater than 6 at line "<<line_no<<endl;
            pass1_out<<".--------ERROR--------------"<<endl;
            error=true;
            continue;
        }

        if(opcode=="START"){
            //store program name
            program_name = label;
            LOCCTR = stoi(operand, nullptr, 16);                                 //initial location counter
            if(BLOCKTABLE.find("default") == BLOCKTABLE.end()) {
                BLOCKTABLE["default"] = {"default",  total_blocks, 0, 0, 0};
                total_blocks++;
                current_block_no=BLOCKTABLE["default"].block_no;
                pass1_out<<line_no<< " "<<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" "<<label<<" "<<opcode<<" "<<operand<<endl;
            }
            
            break;
        }
        else if(opcode=="USE"){
            //add in BLOCKTABLE
            if(operand==""){
                operand="default";
            }
            BLOCKTABLE[operand] = {operand, total_blocks, 0, 0, 0};
            total_blocks++;
            current_block_no=BLOCKTABLE[operand].block_no;
            LOCCTR = BLOCKTABLE[operand].block_locctr;
            pass1_out<<line_no<< " " <<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" "<<label<<" "<<opcode<<" "<<operand<<endl;
            continue;
        }
        else{
            pass1_err<<"START not found at line "<<line_no<<endl;
            pass1_out<<".--------ERROR--------------"<<endl;
            error=true;
            break;
        }
         
    }

    //handling further lines
    while(getline(pass1_in, line)){
        
        line_no++;
        //comment or blank line
        if(!pass1_line_scraper(line, label, opcode, operand)){
            pass1_out<<line<<endl;
            continue;
        }
        //checklabel length <=6
        if(label.size()>6) {
            pass1_err<<"Label size greater than 6 at line "<<line_no<<endl;
            pass1_out<<".--------ERROR--------------"<<endl;
            error=true;
            continue;
        }

        else if(opcode=="START"){
            pass1_err<<"START double time "<<line_no<<endl;
            pass1_out<<".--------ERROR--------------"<<endl;
            error=true;
            continue;
        }
        
        else if(opcode=="USE"){
            if(label!=""){
                pass1_err<<"Assigning label to USE at "<<line_no<<endl;
                pass1_out<<".--------ERROR--------------"<<endl;
                error=true;
                continue;
            }
            //update LOCCTR of previous block
            BLOCKTABLE[current_block_name].block_locctr=LOCCTR;

            if(operand==""){
               operand="default";
            }

            if(BLOCKTABLE.find(operand) == BLOCKTABLE.end()){
                BLOCKTABLE[operand] = {operand, total_blocks, 0, 0, 0};
                total_blocks++;
                current_block_no=BLOCKTABLE[operand].block_no;
                LOCCTR = BLOCKTABLE[operand].block_locctr;
                pass1_out<<line_no<< " " <<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" "<<label<<" "<<opcode<<" "<<operand<<endl;
            }
            else{
                current_block_no=BLOCKTABLE[operand].block_no;
                LOCCTR = BLOCKTABLE[operand].block_locctr;
                pass1_out<<line_no<< " " <<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" "<<label<<" "<<opcode<<" "<<operand<<endl;
            }

            current_block_name=operand;
        }
        
        else if(opcode=="EQU"){
            //cout<<label<<" "<<opcode<<" "<<operand<<endl;
        
            if(label==""){
                pass1_err<<"No label assigned label to "<< opcode <<" at "<<line_no<<endl;
                pass1_out<<".--------ERROR--------------"<<endl;
                error=true;
                continue;
            }
           
            if(operand=="" || operand[0]=='='){
                pass1_err<<"Assigning wrong operand to "<< opcode <<" at "<<line_no<<endl;
                pass1_out<<".--------ERROR--------------"<<endl;
                error=true;
                continue;
            }

            //if operand is decimal value
            else if(check_operand_absolute(operand)){
                int value = stoi(operand, nullptr, 10);
                SYMTAB[label] = {label, current_block_no, value, 2};//absolute label
                pass1_out<<line_no<< " " <<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" "<<label<<" "<<opcode<<" "<<operand<<endl;
            }
            // if operand is a symbol
            else if(SYMTAB.find(operand) != SYMTAB.end()){
                SYMTAB[label] = {label, current_block_no, SYMTAB[operand].value, 1};//relative label
                pass1_out<<line_no<< " " <<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" "<<label<<" "<<opcode<<" "<<operand<<endl;
            }
            else if(operand == "*") {
                //direct value assignment to label
                SYMTAB[label] = {label, current_block_no, LOCCTR, 1};//relative label
                pass1_out<<line_no<< " " <<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" "<<label<<" "<<opcode<<" "<<operand<<endl;
            }
            
            else{
                //handle expression 
                bool isValid = true;
                bool isRelative = false;
                int value = 0;
                handle_expression(operand, value, isValid, isRelative);
                if(isValid){
                    if(isRelative){
                        SYMTAB[label] = {label, current_block_no, value, 1};//relative label
                    }
                    else{
                        SYMTAB[label] = {label, current_block_no, value, 2};//absolute label
                    }
                    pass1_out<<line_no<< " " <<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" "<<label<<" "<<opcode<<" "<<operand<<endl;
                }
                else{
                    pass1_err<<"Invalid expression for EQU at "<<line_no<<endl;
                    pass1_out<<".--------ERROR--------------"<<endl;
                    error=true;
                    continue;
                }
                 
            }
        
          
        }
        
        else if(opcode=="ORG"){
            if(label!=""){
                pass1_err<<"Assigning label to "<< opcode <<" at "<<line_no<<endl;
                pass1_out<<".--------ERROR--------------"<<endl;
                error=true;
                continue;
            }
           
            if(operand=="" || operand[0]=='='){
                pass1_err<<"Assigning wrong operand to "<< opcode <<" at "<<line_no<<endl;
                pass1_out<<".--------ERROR--------------"<<endl;
                error=true;
                continue;
            }

            //if operand is decimal value
            else if(check_operand_absolute(operand)){
                LOCCTR = stoi(operand, nullptr, 10);
                pass1_out<<line_no<< " " <<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" "<<label<<" "<<opcode<<" "<<operand<<endl;
            }
            // if operand is a symbol
            else if(SYMTAB.find(operand) != SYMTAB.end()){
                LOCCTR=SYMTAB[operand].value;
                pass1_out<<line_no<< " " <<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" "<<label<<" "<<opcode<<" "<<operand<<endl;
            }
            else{
                //handle expression
                bool isValid = true;
                bool isRelative = false;
                int value = 0;
                handle_expression(operand, value, isValid, isRelative);
                if(isValid){
                    if(isRelative){
                        LOCCTR = value;
                    }
                    else{
                        LOCCTR = value;
                    }
                    pass1_out<<line_no<< " " <<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" "<<label<<" "<<opcode<<" "<<operand<<endl;
                }
                else{
                    pass1_err<<"Invalid expression for ORG at "<<line_no<<endl;
                    pass1_out<<".--------ERROR--------------"<<endl;
                    error=true;
                    continue;
                }

            }
        
        }

        else if(opcode=="BASE"){
            if(label!=""){
                pass1_err<<"Assigning label to BASE at "<<line_no<<endl;
                pass1_out<<".--------ERROR--------------"<<endl;
                error=true;
                continue;
            }
            //literal
            if(operand=="*"){
                star_count++;
                char ch='0'+star_count;
                operand+=ch;
                LITTAB.insert({operand, LOCCTR});
                pass1_out<<line_no<< " " <<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" "<<label<<" "<<opcode<<" "<<operand<<endl;
            }
            //further pass 2 will handle for label or expression

            //label
            // else if(SYMTAB.find(operand) != SYMTAB.end()){
            //     pass1_out<<line_no<< " " <<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" "<<label<<" "<<opcode<<" "<<operand<<endl;
            // }
            // //immmediate value
            // else if(check_operand_absolute(operand)){
            //     pass1_out<<line_no<< " " <<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" "<<label<<" "<<opcode<<" "<<operand<<endl;
            // }
            // else {
            //     pass1_err<<"Assigning wrong label to BASE at "<<line_no<<endl;
            //     pass1_out<<".--------ERROR--------------"<<endl;
            //     error=true;
            //     continue;
            // }
            

            
        }

        else if(opcode=="NOBASE"){
            if(!(label!="" && operand!="")){
                //error
            }
            pass1_out<<line_no<< " "  
            <<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" "<<label<<" "<<opcode<<" "<<operand<<endl;
        }

        else if(opcode=="LTORG"){
            if(label!="" || operand!=""){
                //error
                if(label!="") pass1_err<<"Assigning label to LTORG at "<<line_no<<endl;
                else pass1_err<<"Assigning operand to LTORG at "<<line_no<<endl;
                pass1_out<<".--------ERROR--------------"<<endl;
                error=true;
                continue;
            }
            pass1_out<< "   " <<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" "<<label<<" "<<opcode<<" "<<operand<<endl;

            for(auto i:LITTAB){
                if(i.first[0]=='*'){//this is for BASE *
                    SYMTAB[i.first] = {i.first, current_block_no, i.second, 1};//relative label
                    // pass1_out<<"    "  <<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" BYTE "<<i.first<<" "<<i.second<<" "<<endl;
                }
                else if(i.first[0]=='='){
                    SYMTAB[i.first] = {i.first, current_block_no, LOCCTR, 1};//relative label
                    if(i.first[1]=='C'){
                        LOCCTR+=i.first.length()-4;
                    }
                    else if(i.first[1]=='X' && i.first.length()%2==0){
                        LOCCTR+=(i.first.length()-4)/2;
                    }
                    pass1_out<<"    "  <<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" BYTE "<<i.first<<" "<<i.second<<" "<<endl;
                }
    
            }
            LITTAB.clear();
        }

        else if(opcode=="WORD"){
            if(operand==""){
                //error
                pass1_err<<"No operand for WORD at "<<line_no<<endl;
                pass1_out<<".--------ERROR--------------"<<endl;
                error=true;
                continue;
            }
            if(label!=""){
                if(SYMTAB.find(label)!=SYMTAB.end()){
                    pass1_err<<"Duplicate symbol present at "<<line_no<<endl;
                    pass1_out<<".--------ERROR--------------"<<endl;
                    error=true;
                    continue;
                }
                else{
                    SYMTAB[label] = {label, current_block_no, LOCCTR, 1};       //relative label
                }
            }

            if(!check_operand_absolute(operand)) {
                //error
                pass1_err<<"Invalid value for WORD present at "<<line_no<<endl;
                pass1_out<<".--------ERROR--------------"<<endl;
                error=true;
                continue;
            }

            pass1_out<<line_no<< " "<<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" "<<label<<" "<<opcode<<" "<<operand<<endl;
            LOCCTR+=3;
        }

        else if(opcode=="BYTE"){
            if(operand==""){
                pass1_err<<"No operand for BYTE at "<<line_no<<endl;
                pass1_out<<".--------ERROR--------------"<<endl;
                error=true;
                continue;
            }
            if(label!=""){
                if(SYMTAB.find(label)!=SYMTAB.end()){
                    //error
                    pass1_err<<"Duplicate symbol present at "<<line_no<<endl;
                    pass1_out<<".--------ERROR--------------"<<endl;
                    error=true;
                    continue;
                }
                else{
                    SYMTAB[label] = {label, current_block_no, LOCCTR, 1};//relative label
                }
            }

            if(operand[0]=='X'){
                if(!(operand[1]=='\'' && operand[operand.length()-1]=='\'') || (operand.length()-3)%2!=0){
                   //error
                    pass1_err<<"Invalid value for BYTE present at "<<line_no<<endl;
                    pass1_out<<".--------ERROR--------------"<<endl;
                    error=true;
                    continue;
                }
                pass1_out<<line_no<< " "  
                <<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" "<<label<<" "<<opcode<<" "<<operand<<endl;
                LOCCTR+=(operand.length()-3)/2;
            }
            else if(operand[0]=='C'){
                if(!(operand[1]=='\'' && operand[operand.length()-1]=='\'')){
                    // error
                    pass1_err<<"Invalid value for BYTE present at "<<line_no<<endl;
                    pass1_out<<".--------ERROR--------------"<<endl;
                    error=true;
                    continue;
                }
                pass1_out<<line_no<< " "  
                <<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" "<<label<<" "<<opcode<<" "<<operand<<endl;
                LOCCTR+=operand.length()-3;
            }
            else{
                //error
                pass1_err<<"Invalid value for BYTE present at "<<line_no<<endl;
                pass1_out<<".--------ERROR--------------"<<endl;
                error=true;
                continue;
            }
        }

        else if(opcode=="RESW"){
            if(operand==""){
                //error
                pass1_err<<"NO value for BYTE present at "<<line_no<<endl;
                pass1_out<<".--------ERROR--------------"<<endl;
                error=true;
                continue;
            }
            if(label!=""){
                if(SYMTAB.find(label)!=SYMTAB.end()){
                    //error
                    pass1_err<<"Duplicate symbol present at "<<line_no<<endl;
                    pass1_out<<".--------ERROR--------------"<<endl;
                    error=true;
                    continue;
                }
                else{
                    SYMTAB[label] = {label, current_block_no, LOCCTR, 1};//relative label
                }
            }
            if(check_operand_absolute(operand)) {
                pass1_out<<line_no<< " "  
                <<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" "<<label<<" "<<opcode<<" "<<operand<<endl;
                LOCCTR+=3*stoi(operand, nullptr, 10);
            }
            else{
                //error
                pass1_err<<"Invalid value for RESW present at "<<line_no<<endl;
                pass1_out<<".--------ERROR--------------"<<endl;
                error=true;
                continue;
            }
        }

        else if(opcode=="RESB"){
            if(operand==""){
                //error
                pass1_err<<"Duplicate symbol present at "<<line_no<<endl;
                pass1_out<<".--------ERROR--------------"<<endl;
                error=true;
                continue;
            }
            if(label!=""){
                if(SYMTAB.find(label)!=SYMTAB.end()){
                    //error
                    pass1_err<<"Duplicate symbol present at "<<line_no<<endl;
                    pass1_out<<".--------ERROR--------------"<<endl;
                    error=true;
                    continue;
                }
                else{
                    SYMTAB[label] = {label, current_block_no, LOCCTR, 1};//relative label
                }
            }
            if(check_operand_absolute(operand)) {
                pass1_out<<line_no<< " " <<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" "<<label<<" "<<opcode<<" "<<operand<<endl;
                LOCCTR+=stoi(operand, nullptr, 10);
            }
            else{
                //error
                pass1_err<<"Invalid value for RESB present at "<<line_no<<endl;
                pass1_out<<".--------ERROR--------------"<<endl;
                error=true;
                continue;
            }
            
        }

        else if(opcode=="END"){
            pass1_out<<line_no<< " " <<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" "<<label<<" "<<opcode<<" "<<operand<<endl;

            //clear LITTAB
            for(auto i:LITTAB){
                if(i.first[0]=='*'){//this is for BASE *
                    SYMTAB[i.first] = {i.first, current_block_no, i.second, 1};//relative label
                    // pass1_out<<"    "  <<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" BYTE "<<i.first<<endl;
                    
                }
                else if(i.first[0]=='='){
                    SYMTAB[i.first] = {i.first, current_block_no, LOCCTR, 1};//relative label
                    if(i.first[1]=='C'){
                        LOCCTR+=i.first.length()-4;
                    }
                    else if(i.first[1]=='X' && i.first.length()%2==0){
                        LOCCTR+=(i.first.length()-4)/2;
                    }
                    pass1_out<<"    "  <<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" BYTE "<<i.first<<endl;
                    
                }
    
            }
            BLOCKTABLE[current_block_name].block_locctr=LOCCTR;
            LITTAB.clear();
            break;
        }
       
        else if(opcode[0]=='+'){
            if(OPTAB.find(opcode.substr(1)) != OPTAB.end() && OPTAB[opcode.substr(1)].opcode_format==3){
                //handle label
                if(label != "") {
                    if(SYMTAB.find(label)!=SYMTAB.end()){
                        //error
                        pass1_err<<"Duplicate symbol present at "<<line_no<<endl;
                        pass1_out<<".--------ERROR--------------"<<endl;
                        error=true;
                        continue;
                    }
                    else{
                        SYMTAB[label] = {label, current_block_no, LOCCTR, 1};//relative label
                    }
                }
                //handle operand
                if(operand==""){
                }
                else if(operand=="=*"){
                    operand = "";
                    operand += "#";
                    operand += to_string(LOCCTR);

                }
                else if(operand[0]=='=' && SYMTAB.find(operand.substr(1)) != SYMTAB.end()){
                    int value=SYMTAB[operand.substr(1)].value;
                    operand = "";
                    operand += "#";
                    operand += to_string(value);

            
                }
                else if( (operand.size()>4) && ((operand.substr(0,3)=="=X\'"&& operand[operand.size()-1]=='\'') ||( operand.substr(0,3) == "=C\'" && operand[operand.size()-1]=='\''))){
                    // literal_count++;
                    LITTAB.insert({operand, 0});
                }
                
                pass1_out<<line_no<< " "<<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" "<<label<<" "<<opcode<<" "<<operand<<endl;
                
                LOCCTR+=4;
            }
            else{
                //error
                pass1_err<<"Given opcode is not a format 4 instruction at "<<line_no<<endl;
                pass1_out<<".--------ERROR--------------"<<endl;
                error=true;
                continue;
            }
        }
        
        else if(OPTAB.find(opcode)!=OPTAB.end()){
            if(label!=""){
                if(SYMTAB.find(label)!=SYMTAB.end()){
                    //error
                    pass1_err<<"Duplicate symbol present at "<<line_no<<endl;
                    pass1_out<<".--------ERROR--------------"<<endl;
                    error=true;
                    continue;
                }
                else{
                    SYMTAB[label] = {label, current_block_no, LOCCTR, 1};//relative label
                }

                            
            }

            //handle operand
            if(operand==""){
            }
            else if(operand=="=*"){
                operand = "";
                operand += "#";
                operand += to_string(LOCCTR);

            }
            else if(operand[0]=='=' && SYMTAB.find(operand.substr(1)) != SYMTAB.end()){
                int value=SYMTAB[operand.substr(1)].value;
                operand = "";
                operand += "#";
                operand += to_string(value);

        
            }
            else if( (operand.size()>4) && ((operand.substr(0,3)=="=X\'"&& operand[operand.size()-1]=='\'') ||( operand.substr(0,3) == "=C\'" && operand[operand.size()-1]=='\''))){
                // literal_count++;
                LITTAB.insert({operand, 0});
            }
            pass1_out<<line_no<< " "<<decimalToTwosComplement(LOCCTR,5)<<" "<< current_block_no<<" "<<label<<" "<<opcode<<" "<<operand<<endl;
            
            LOCCTR+=OPTAB[opcode].opcode_format;
        }
       
        else{
            //error
            pass1_err<<"Invalid opcode present at "<<line_no<<endl;
            pass1_out<<".--------ERROR--------------"<<endl;
            error=true;
        }

    }//END OF FILE

    pass1_out.close();
    pass1_err.close();
    pass1_in.close();
    //print SYMTAB

    cout<<"---------------SYMTAB ------------------"<<endl;
    for(auto i:SYMTAB){
        cout<<i.second.label<<" "<<i.second.block_no<<" "<<i.second.value<<" "<<endl;
    }

    //update BLOCKTABLE and program_length
    for(auto &i : BLOCKTABLE) {
        i.second.block_length = i.second.block_locctr;
    }

    vector<pair<int, pair<int, string> > > vec;
    for(auto i:BLOCKTABLE){
        vec.push_back({i.second.block_no, {i.second.block_length, i.second.block_name}});
    }
    sort(vec.begin(), vec.end());

    //update starting address of each block
    int sum=0;
    for(auto i:vec){
        BLOCKTABLE[i.second.second].start_address=sum;
        sum+=i.second.first;
    }
    program_length = sum;                   //total length of program  


    // print BLOCKTABLE
    cout<<"---------------BLOCKTABLE ------------------"<<endl;
    for(auto i:BLOCKTABLE){
        cout<<i.second.block_name<<" "<<i.second.block_no<<" "<<hex<<i.second.block_locctr<<" "<<i.second.start_address<<endl;
    }

    // update BLOCK_DATA
    for(auto data: vec){
        BLOCK_DATA.push_back({BLOCKTABLE[data.second.second].start_address, data.second.first});
    }

    return !error;
}

void pass2(){
    ifstream pass2_in("intermediate.txt");
    ofstream pass2_list("listing_file.txt");
    ofstream pass2_obj("object_code.txt");
    ofstream pass2_err("error.txt");

    string line;
    string line_no, label, opcode, operand;
    int locctr, program_block_no;
    string current_block_name = "default";
    int BASE_REGISTER = 0;
    bool BASE_RELATIVE_ADDRESSING = false;

    struct text_record current_text_record;
    current_text_record.start_address = 0;
    string current_object_code;
    int prev_block_no;
    bool prev_RESW_RESB = 0;
    vector<string> MODIFICATION_RECORDS;

    //handling start and use for header record
    string header_record="H^";
    while(getline(pass2_in,line)){
        //cout<<line<<endl;
        //neglecting comments and blank lines
        if(!pass2_line_scraper(line, line_no, locctr, program_block_no, label, opcode, operand)){
            pass2_list<<line<<endl;
            continue;
        }
        
        else if(opcode=="START"){
            int sz = 6-program_name.length();
            header_record+=program_name;
            while(sz--) {
                header_record += "_";
            }
            header_record+="^";

            string start_address = decimalToTwosComplement(stoi(operand, nullptr, 16),6);
            header_record += start_address;
            header_record+="^";
            string header_program_length = decimalToTwosComplement(program_length,6);
            header_record += header_program_length;
            pass2_obj<<header_record<<endl;
            pass2_list<<line<<endl;

            prev_block_no = program_block_no;

            break;
        }
        else if(opcode=="USE"){
            prev_block_no = program_block_no;
            continue;
        }
    }

    //handling further lines
    while(getline(pass2_in, line)) {
        //neglecting comments and blank lines
        if(!pass2_line_scraper(line, line_no, locctr, program_block_no, label, opcode, operand)){
            pass2_list<<line<<endl;
            continue;
        }

        if(prev_RESW_RESB) {
            prev_RESW_RESB = 0;
            current_text_record.start_address = BLOCK_DATA[program_block_no].start_address + locctr;
        }

        if(opcode == "BASE") {
            pass2_list<<line<<endl;
            if(SYMTAB.find(operand) != SYMTAB.end()) {
                BASE_REGISTER = SYMTAB[operand].value;
                BASE_RELATIVE_ADDRESSING = 1;
            }
            else if(check_operand_absolute(operand)) {
                BASE_REGISTER = stoi(operand, nullptr, 10);
                BASE_RELATIVE_ADDRESSING = 1;
            }
            else {
                //handle expressions
                bool isValid = true;
                bool isRelative = false;
                int value = 0;
                handle_expression(operand, value, isValid, isRelative);
                if(isValid){
                    BASE_REGISTER = value;
                    BASE_RELATIVE_ADDRESSING = 1;
                }
                else{
                    pass2_err<<"Invalid expression for BASE at "<<line_no<<endl;
                    continue;
                }
            }
        }

        else if(opcode == "NOBASE") {
            pass2_list<<line<<endl;
            BASE_RELATIVE_ADDRESSING = 0;
        }

        else if(opcode == "LTORG") {
            pass2_list<<line<<endl;
            continue;
        }

        else if(opcode == "EQU") {
            pass2_list<<line<<endl;
            continue;
        }

        else if(opcode == "ORG") {
            pass2_list<<line<<endl;
            continue;
        }

        else if(opcode == "BYTE") {
            pass2_list<<line<<endl;
            if(operand[0]=='=') {
                if(operand[1]=='C') {
                    for(int i=3; i<operand.length()-1; i++) {
                        char ch = operand[i];
                        current_object_code = decimalToTwosComplement((int)ch, 2);
                    }
                }
                else {
                    for(int i=3; i<operand.length()-1; i++) {
                        current_object_code = operand[i];
                    }
                }
            }
            else {
                if(operand[0]=='C') {
                    for(int i=2; i<operand.length()-1; i++) {
                        char ch = operand[i];
                        current_object_code = decimalToTwosComplement((int)ch, 2);
                    }
                }
                else {
                    for(int i=2; i<operand.length()-1; i++) {
                        current_object_code = operand[i];
                    }
                }
            }

            if(program_block_no != prev_block_no || current_text_record.object_code.length() + current_object_code.length() <= 60) {
                current_text_record.length = current_text_record.object_code.length()/2;
                TEXT_RECORDS.push_back(current_text_record);

                current_text_record = text_record();
                current_text_record.start_address = BLOCK_DATA[program_block_no].start_address + locctr;
                current_text_record.object_code += "^" + current_object_code;
                prev_block_no = program_block_no;
            }
            else {
                current_text_record.object_code += "^" + current_object_code;
            }
        }
        else if(opcode == "WORD") {
            pass2_list<<line<<endl;
            current_object_code = decimalToTwosComplement(stoi(operand, nullptr, 10), 6);
            if(program_block_no != prev_block_no || current_text_record.object_code.length() + current_object_code.length() <= 60) {
                current_text_record.length = current_text_record.object_code.length()/2;
                TEXT_RECORDS.push_back(current_text_record);

                current_text_record = text_record();
                current_text_record.start_address = BLOCK_DATA[program_block_no].start_address + locctr;
                current_text_record.object_code += "^" + current_object_code;
                prev_block_no = program_block_no;
            }
            else {
                current_text_record.object_code += "^" + current_object_code;
            }
        }
        else if(opcode == "RESW" || opcode == "RESB") {
            pass2_list<<line<<endl;
            if(current_text_record.object_code.length() != 0) {
                current_text_record.length = current_text_record.object_code.length()/2;
                TEXT_RECORDS.push_back(current_text_record);

                current_text_record = text_record();
            }
            prev_RESW_RESB = 1;
        }
        else if(opcode == "USE") {
            pass2_list<<line<<endl;
            continue;
        }

        // format-4
        else if(opcode[0] == '+') {
            int ni = 3;
            bool need_modification_record = 0;
            int xbpe = 1;
            int opcode_value = OPTAB[opcode.substr(1)].opcode_value;
            if(operand.size() > 2 && operand.substr(operand.size()-2) == ",X") {
                xbpe += 8;
                operand = operand.substr(0, operand.size()-2);
            }

            if(operand[0] == '#') {
                ni = 1;
                operand = operand.substr(1);
            }
            else if(operand[0] == '@') {
                ni = 2;
                operand = operand.substr(1);
            }

            if(operand.size() > 0) {
                if(opcode != "RSUB") {
                    bool isValid = true;
                    bool isRelative = false;
                    int value = 0;
                    handle_expression(operand, value, isValid, isRelative);
                    if(isValid) {
                        // is an expression
                        if(isRelative) {
                            need_modification_record = 1;
                        }
                    }
                    else if(check_operand_absolute(operand)) {
                        value = stoi(operand, nullptr, 10);
                    }
                    else if(SYMTAB.find(operand) != SYMTAB.end()) {
                        if(SYMTAB[operand].isValid == 1) {
                            need_modification_record = 1;
                        }
                        value = SYMTAB[operand].value;
                    }
                    else {
                        pass2_err<<"Invalid operand for format-4 instruction at "<<line_no<<endl;
                        continue;
                    }

                    current_object_code = decimalToTwosComplement(opcode_value + ni, 2) + decimalToTwosComplement(xbpe, 1) + decimalToTwosComplement(value, 5);
                }
                else {
                    pass2_err<<"Invalid operand for format-4 instruction at "<<line_no<<endl;
                    continue;
                }
            }
            else {
                if(opcode == "RSUB") {
                    current_object_code = "4F100000";
                }
                else {
                    pass2_err<<"Invalid operand for format-4 instruction at "<<line_no<<endl;
                    continue;
                }
            }

            if(program_block_no != prev_block_no || current_text_record.object_code.length() + current_object_code.length() <= 60) {
                current_text_record.length = current_text_record.object_code.length()/2;
                TEXT_RECORDS.push_back(current_text_record);

                current_text_record = text_record();
                current_text_record.start_address = BLOCK_DATA[program_block_no].start_address + locctr;
                current_text_record.object_code += "^" + current_object_code;
                prev_block_no = program_block_no;
            }
            else {
                current_text_record.object_code += "^" + current_object_code;
            }

            //modification record
            if(need_modification_record) {
                string modification_record = "M^";
                int address = BLOCK_DATA[program_block_no].start_address + locctr + 1;
                modification_record += decimalToTwosComplement(address, 6);
                modification_record += "^05";
                MODIFICATION_RECORDS.push_back(modification_record);
            }

        }

        // format-1,2,3
        else if(OPTAB.find(opcode) != OPTAB.end()) {
            int format = OPTAB[opcode].opcode_format;
            int opcode_value = OPTAB[opcode].opcode_value;
            if(format == 1) {
                current_object_code = decimalToTwosComplement(opcode_value, 2);
            }
            else if(format == 2) {
                bool error_flag = 0;
                string str = handleFormat2(operand, current_object_code, error_flag);
                if(error_flag) {
                    pass2_err<<"Invalid operand for format-2 instruction at "<<line_no<<endl;
                    continue;
                }

                current_object_code = decimalToTwosComplement(opcode_value, 2) + str;
            }
            else {
                int ni = 3;
                int xbpe = 0;
                int opcode_value = OPTAB[opcode].opcode_value;
                if(operand.size() > 2 && operand.substr(operand.size()-2) == ",X") {
                    xbpe += 8;
                    operand = operand.substr(0, operand.size()-2);
                }

                if(operand[0] == '#') {
                    ni = 1;
                    operand = operand.substr(1);
                }
                else if(operand[0] == '@') {
                    ni = 2;
                    operand = operand.substr(1);
                }

                if(operand.size() > 0) {
                    if(opcode != "RSUB") {
                        bool isValid = true;
                        bool isRelative = false;
                        int value = 0;
                        handle_expression(operand, value, isValid, isRelative);
                        if(isValid) {
                            // is an expression
                        }
                        else if(check_operand_absolute(operand)) {
                            value = stoi(operand, nullptr, 10);
                        }
                        else if(SYMTAB.find(operand) != SYMTAB.end()) {
                            if(SYMTAB[operand].isValid == 1) {
                                isRelative = 1;
                            }
                            value = SYMTAB[operand].value;
                        }
                        else {
                            pass2_err<<"Invalid operand for format-3 instruction at "<<line_no<<endl;
                            continue;
                        }

                        if(isRelative) {
                            int PC_value = locctr + 3;
                            int displacement = value - PC_value;
                            if(displacement >= -2048 && displacement <= 2047) {
                                xbpe += 2;
                                value = displacement;
                            }
                            else {
                                if(BASE_RELATIVE_ADDRESSING) {
                                    displacement = value - BASE_REGISTER;
                                    if(displacement >= 0 && displacement <= 4095) {
                                        xbpe += 4;
                                        value = displacement;
                                    }
                                    else {
                                        pass2_err<<"Displacement is out of bounds for both PC-relative and BASE-relative addressing in format-3 instruction at "<<line_no<<endl;
                                        continue;
                                    }
                                }
                                else {
                                    pass2_err<<"Displacement is out of bounds for PC-relative addressing and BASE-relative addressing is not enabled in format-3 instruction at "<<line_no<<endl;
                                    continue;
                                }
                            }
                        }

                        current_object_code = decimalToTwosComplement(opcode_value + ni, 2) + decimalToTwosComplement(xbpe, 1) + decimalToTwosComplement(value, 3);
                    }
                    else {
                        pass2_err<<"Operand given for RSUB in format-3 instruction at "<<line_no<<endl;
                        continue;
                    }
                }
                else {
                    if(opcode == "RSUB") {
                        current_object_code = "4F0000";
                    }
                    else {
                        pass2_err<<opcode<<" requires operand for format-3 instruction at "<<line_no<<endl;
                        continue;
                    }
                }

                if(program_block_no != prev_block_no || current_text_record.object_code.length() + current_object_code.length() <= 60) {
                    current_text_record.length = current_text_record.object_code.length()/2;
                    TEXT_RECORDS.push_back(current_text_record);

                    current_text_record = text_record();
                    current_text_record.start_address = BLOCK_DATA[program_block_no].start_address + locctr;
                    current_text_record.object_code += "^" + current_object_code;
                    prev_block_no = program_block_no;
                }
                else {
                    current_text_record.object_code += "^" + current_object_code;
                }

            }

        }
        
        else if(opcode == "END") {
            if(current_text_record.object_code.length() != 0) {
                current_text_record.length = current_text_record.object_code.length()/2;
                TEXT_RECORDS.push_back(current_text_record);
            }

            //write text records
            for(auto i:TEXT_RECORDS) {
                string text_record = "T^";
                text_record += decimalToTwosComplement(i.start_address, 6);
                text_record += "^";
                text_record += decimalToTwosComplement(i.length, 2);
                text_record += i.object_code;
                pass2_obj<<text_record<<endl;
            }

            // write modification records
            for(auto i:MODIFICATION_RECORDS) {
                pass2_obj<<i<<endl;
            }

            //write end record

            string end_record = "E^";
            if(operand != "") {
                if(SYMTAB.find(operand) != SYMTAB.end()) {
                    end_record += decimalToTwosComplement(SYMTAB[operand].value, 6);
                }
                else {
                    pass2_err<<"Undefined label present at "<<line_no<<endl;
                    break;
                }
            }
            else {
                end_record += decimalToTwosComplement(BLOCK_DATA[0].start_address, 6);
            }

            pass2_obj<<end_record<<endl;
            break;
        }
        else {
            pass2_err<<"Invalid opcode present at "<<line_no<<endl;
            continue;
        }
    }
}

int main() {
   
    bool pass1_completed_without_error=pass1();
    if(pass1_completed_without_error){
        cout<<"PASS 1 completed successfully"<<endl;
    }
    else{
        cout<<"PASS 1 completed with errors"<<endl;
    }
    pass2();

    return 0;
}