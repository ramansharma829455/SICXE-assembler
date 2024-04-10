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


struct text_record{
    string initial = "T^";
    int start_address;
    int length;
    string object_code = "";
};

vector<text_record> TEXT_RECORDS;

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

