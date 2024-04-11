#include <bits/stdc++.h>
#include "data.h"
using namespace std;

string decimalToTwosComplement(int decimal_num, int hex_length) {
    // Check if the decimal number is negative
    if (decimal_num < 0) {
        // Convert negative decimal number to its 2's complement representation
        decimal_num = (1ULL << 32) + decimal_num;
    }

    // Convert decimal number to hexadecimal
    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setw(hex_length) << std::setfill('0') << decimal_num;
    std::string hex_value = ss.str();

    int sz = hex_value.size();
    hex_value = hex_value.substr(sz-hex_length, hex_length);

    return hex_value;
}

//utility function to check if operand is absolute
bool check_operand_absolute(string operand){
    for(int i=0; i<operand.size(); i++){
        if(operand[i]>='0' && operand[i]<='9') continue;
        else return false;            
    }
    return true;

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

bool pass2_line_scraper(string line, string &line_no, int &locctr, int &program_block_no, string &label, string &opcode, string &operand) {
   
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
    line_no = tokens[0];
    locctr = stoi(tokens[1], nullptr, 16);
    program_block_no = stoi(tokens[2], nullptr, 10);
    if (tokens.size() == 4) {
        label = "";
        opcode = tokens[3];
        operand = "";
    } 
    else if (tokens.size() == 5) {
        string check = tokens[3];
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

// Function to perform arithmetic operations.
int performOperation(char operation, int operand1, int operand2) {
    switch (operation) {
        case '+': return operand1 + operand2;
        case '-': return operand1 - operand2;
        case '*': return operand1 * operand2;
        case '/': return operand1 / operand2;
        default: return 0;
    }
}

// Function to check if a character is an operator.
bool isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
}

// Function to evaluate arithmetic expression using BODMAS rule.
int evaluateExpression(string expression) {
    stack<int> values;
    stack<char> operators;

    for (int i = 0; i < expression.length(); ++i) {
        // If current character is a whitespace, skip it.
        if (expression[i] == ' ')
            continue;

        // If current character is a digit or negative sign, push it to the values stack.
        if (isdigit(expression[i]) || (expression[i] == '-' && (i == 0 || expression[i - 1] == '('))) {
            int operand = 0;
            int sign = 1;
            if (expression[i] == '-') {
                sign = -1;
                ++i;
            }
            while (i < expression.length() && isdigit(expression[i])) {
                operand = operand * 10 + (expression[i] - '0');
                ++i;
            }
            --i; // To compensate for the extra increment in the loop
            values.push(sign * operand);
        }
        // If current character is an opening parenthesis, push it to the operators stack.
        else if (expression[i] == '(') {
            operators.push(expression[i]);
        }
        // If current character is a closing parenthesis, solve the expression inside the parentheses.
        else if (expression[i] == ')') {
            while (!operators.empty() && operators.top() != '(') {
                int operand2 = values.top();
                values.pop();
                int operand1 = values.top();
                values.pop();
                char op = operators.top();
                operators.pop();
                values.push(performOperation(op, operand1, operand2));
            }
            // Pop the opening parenthesis.
            operators.pop();
        }
        // If current character is an operator.
        else if (isOperator(expression[i])) {
            // While top of 'operators' stack has same or greater precedence to current
            // token, which is an operator. Apply operator on top of 'operators' stack
            // to top two elements in values stack.
            while (!operators.empty() && operators.top() != '(' &&
                   ((expression[i] != '*' && expression[i] != '/') ||
                    (operators.top() == '*' || operators.top() == '/'))) {
                int operand2 = values.top();
                values.pop();
                int operand1 = values.top();
                values.pop();
                char op = operators.top();
                operators.pop();
                values.push(performOperation(op, operand1, operand2));
            }
            // Push current operator to 'operators'.
            operators.push(expression[i]);
        }
    }

    // At this point, the expression has been parsed.
    // Apply remaining operators on remaining values.
    while (!operators.empty()) {
        int operand2 = values.top();
        values.pop();
        int operand1 = values.top();
        values.pop();
        char op = operators.top();
        operators.pop();
        values.push(performOperation(op, operand1, operand2));
    }

    // Top of 'values' stack contains the result of the expression.
    return values.top();
}

void handle_expression(string operand,int &value, bool &isValid, bool & isRelative){
    //print SYMTAB
    //cout<<operand<<endl;
    //for(auto x:SYMTAB){
    //    cout<<x.first<<" "<<x.second.value<<" "<<x.second.isValid<<endl;
    //}

    bool isExpression=false;
    //break operand with + , - , * , / 
    vector<string> tokens;
    string token;
    for(int i=0;i<operand.size();i++){
        if(operand[i]=='+' || operand[i]=='-' || operand[i]=='*' || operand[i]=='/'){
            tokens.push_back(token);
            token="";
            token+=operand[i];
            tokens.push_back(token);
            token="";
            isExpression=true;
        }
        else{
            token+=operand[i];
        }
    }
    if(token!="") tokens.push_back(token);

    if(!isExpression){isValid=false; return;}

    //cout<<"TOKENS: ";
    //for(auto x:tokens){
    //    cout<<x<<" ";
    //}

    //creating a check string for checking validity of the expression
    string check="";

    for(auto x:tokens){
        if(x=="+" || x=="-" || x=="*" || x=="/"  ){
            check+=x;
            continue;
        } 
        else if(check_operand_absolute(x)){
           check+="a";
        }
        else if(SYMTAB.find(x)==SYMTAB.end()){
            //cout<<"Error: Symbol "<<x<<" not found in SYMTAB"<<endl;
            isValid=false;//error
            return;
        }
        else{
            if(SYMTAB[x].isValid==1) check+="r";
            else if(SYMTAB[x].isValid==2) check+="a";
        }
    }

    check="+"+check;
    //cout<<"CHECK: "<<check<<endl;
    //Rules
    //consecutive operand for * / should be 'a' only
    int count_rel_positive=0;
    int count_rel_negative=0;

    for(int i=0;i<check.size()-1;i++){
        
        if((((check[i]=='a'|| check[i]=='r') && (check[i+1]=='a'|| check[i+1]=='r'))||
            (!(check[i]=='a'|| check[i]=='r') && !(check[i+1]=='a'|| check[i+1]=='r'))||
            (check[i]=='r' && check[i+1]=='/')||
            (check[i]=='/' && check[i+1]=='r')||
            (check[i]=='r' && check[i+1]=='*')||
            (check[i]=='*' && check[i+1]=='r'))){
                //cout<<"Error: Invalid expression"<<check[i]<<check[i+1]<<endl;
                isValid=false;//error
                return;
        }
        if(check[i]=='+' && check[i+1]=='r'){
            count_rel_positive++;
        }
        if(check[i]=='-' && check[i+1]=='r'){
            count_rel_negative++;
        }
    }
    if((count_rel_positive<count_rel_negative) || (count_rel_positive-count_rel_negative>1)){
        isValid=0;//error
        return;
    }

    if(count_rel_negative==count_rel_positive){
        isRelative=false;
    }
    else{
        isRelative=true;
    }

    //Reaching here means expression is valid and all symbols are present in SYMTAB with their values
    isValid=true;

    //Evaluating the expression using BODMAS rule suing prefix expression
    string generated_expression="";
    for(auto x:tokens){
        if(x=="+" || x=="-" || x=="*" || x=="/"){
            generated_expression+=x;
            continue;
        }
        if(check_operand_absolute(x)){
            generated_expression+=("("+x+")");
        }
        else{
            generated_expression+=("("+to_string(SYMTAB[x].value)+")");
        }
    }
    //cout<<"GENERATED EXPRESSION: "<<generated_expression<<endl;
    //Evaluating the expression
    value=evaluateExpression(generated_expression);

}

string handleFormat2(string opcode, string operand, bool &error_flag) {
    vector<string> tokens;
    string token;
    for(int i=0;i<operand.size();i++){
        if(operand[i]==','){
            tokens.push_back(token);
            token="";
            continue;
        }
        token+=operand[i];
    }
    if(token.size()) {
        tokens.push_back(token);
    }

    if(opcode == "ADDR" || opcode == "COMPR" || opcode == "DIVR" || opcode == "MULR" || opcode == "RMO" || opcode == "SUBR") {

        if(tokens.size() != 2) {
            error_flag = true;
            return "";
        }

        int operand1;
        if(check_operand_absolute(tokens[0])) {
            operand1 = stoi(tokens[0], nullptr, 10);
        }
        else if(REGISTER_SET.find(tokens[0]) != REGISTER_SET.end()) {
            operand1 = REGISTER_SET[tokens[0]];     
        }
        else if(SYMTAB.find(tokens[0]) != SYMTAB.end()) {
            operand1 = SYMTAB[tokens[0]].value;
        }
        else {
            error_flag = true;
            return "";
        }
        if(!(operand1 >= 0 && operand1 <= 9 && operand1 != 7)) {
            error_flag = true;
            return "";
        }

        int operand2;
        if(check_operand_absolute(tokens[1])) {
            operand1 = stoi(tokens[1], nullptr, 10);
        }
        else if(REGISTER_SET.find(tokens[1]) != REGISTER_SET.end()) {
            operand2 = REGISTER_SET[tokens[1]];     
        }
        else if(SYMTAB.find(tokens[1]) != SYMTAB.end()) {
            operand2 = SYMTAB[tokens[1]].value;
        }
        else {
            error_flag = true;
            return "";
        }
        if(!(operand1 >= 0 && operand1 <= 9 && operand1 != 7)) {
            error_flag = true;
            return "";
        }

        return decimalToTwosComplement(operand1, 1) + decimalToTwosComplement(operand2, 1);
    }
    else if(opcode == "SHIFTL" || opcode == "SHIFTR") {
        if(tokens.size() != 2) {
            error_flag = true;
            return "";
        }

        int operand1;
        if(check_operand_absolute(tokens[0])) {
            operand1 = stoi(tokens[0], nullptr, 10);
        }
        else if(REGISTER_SET.find(tokens[0]) != REGISTER_SET.end()) {
            operand1 = REGISTER_SET[tokens[0]];     
        }
        else if(SYMTAB.find(tokens[0]) != SYMTAB.end()) {
            operand1 = SYMTAB[tokens[0]].value;
        }
        else {
            error_flag = true;
            return "";
        }
        if(!(operand1 >= 0 && operand1 <= 9 && operand1 != 7)) {
            error_flag = true;
            return "";
        }

        int operand2;
        if(check_operand_absolute(tokens[1])) {
            operand2 = stoi(tokens[1], nullptr, 10);
        }
        else if(SYMTAB.find(tokens[1]) != SYMTAB.end()) {
            operand2 = SYMTAB[tokens[1]].value;
        }
        else {
            error_flag = true;
            return "";
        }

        if(!(operand2 >= 0 && operand2 <= 15)) {
            error_flag = true;
            return "";
        }

        return decimalToTwosComplement(operand1, 1) + decimalToTwosComplement(operand2, 1);
    }
    else if(opcode == "TIXR" || opcode == "CLEAR") {
        if(tokens.size() != 1) {
            error_flag = true;
            return "";  
        }

        int operand1;
        if(check_operand_absolute(tokens[0])) {
            operand1 = stoi(tokens[0], nullptr, 10);
        }
        else if(REGISTER_SET.find(tokens[0]) != REGISTER_SET.end()) {
            operand1 = REGISTER_SET[tokens[0]];     
        }
        else if(SYMTAB.find(tokens[0]) != SYMTAB.end()) {
            operand1 = SYMTAB[tokens[0]].value;
        }
        else {
            error_flag = true;
            return "";
        }
        if(!(operand1 >= 0 && operand1 <= 9 && operand1 != 7)) {
            error_flag = true;
            return "";
        }

        return decimalToTwosComplement(operand1, 1) + "0";
    }
    else {
        if(tokens.size() != 1) {
            error_flag = true;
            return "";
        }

        int operand1;
        if(check_operand_absolute(tokens[0])) {
            operand1 = stoi(tokens[0], nullptr, 10);
        }
        else if(SYMTAB.find(tokens[0]) != SYMTAB.end()) {
            operand1 = SYMTAB[tokens[0]].value;
        }
        else {
            error_flag = true;
            return "";
        }

        if(!(operand1 >= 0 && operand1 <= 15)) {
            error_flag = true;
            return "";
        }

        return decimalToTwosComplement(operand1, 2) + "0";
    }
}

string write_in_listing_file(string line_no, string locctr, string program_block_no, string label, string opcode, string operand, string object_code) {    string line="";

    int length = 0;
    //line_no
    length = 6 - line_no.size();
    line+=line_no;
    while(length--) line+=" ";

    //locctr
    length = 6 - locctr.size();
    line+=locctr;
    while(length--) line+=" ";

    //program_block_no
    length = 5 - program_block_no.size();
    line+=program_block_no;
    while(length--) line+=" ";

    //label
    length = 8 - label.size();
    line+=label;
    while(length--) line+=" ";

    //opcode
    length = 8 - opcode.size();
    line+=opcode;
    while(length--) line+=" ";

    //operand
    if(operand.size()>18){
        line+=operand;
    }
    else{
        length = 18 - operand.size();
        line+=operand;
        while(length--) line+=" ";
    }
    
    //object_code
    line+=object_code;
    return line;
}

void write_tables(){
    ofstream ss("data_tables.txt");
    ss<<"===================SYMTAB==================="<<endl;
    ss<<"SYMBOL\tBLOCK_NO\tVALUE\t\tFLAG"<<endl;
    ss<<"-------------------------------------------"<<endl;
    for(auto x:SYMTAB){
        char c;
        if(x.second.isValid==0) c='N';
        else if(x.second.isValid==1) c='R';
        else c='A';
        ss<<x.first<<"\t"<< x.second.block_no<<"\t\t\t"<<hex<<x.second.value<<"\t\t\t"<<c<<endl;
    }

   
    ss<<"\n\n=================BLOCKTABLE=================="<<endl;
    ss<<"BLOCK_NAME\tBLOCK_NO\tBLOCK_LENGTH\tSTART_ADDRESS"<<endl;
    ss<<"-------------------------------------------"<<endl;
    for(auto x:BLOCKTABLE){
        ss<<x.first<<"\t\t"<<x.second.block_no<<"\t\t\t"<<x.second.block_length<<"\t\t\t\t"<<x.second.start_address<<endl;
    }
}