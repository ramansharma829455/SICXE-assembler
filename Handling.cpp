#include<bits/stdc++.h>
#include "assembler.h"
using namespace std;


//===================================MAIN FUNCTION=======================================
int main() {

    cout<<"=================SIC/XE Assembler=================\n";
    cout<<"Made by : RAMAN SHARMA, Enrollment No. 22114076, IIT Roorkee Btech II Year\n";
    cout<<"==================================================\n\n";

   
    string input_file = "input.txt";
    cout << "Enter the input file name (with extension e.g. input.txt): ";
    cin >> input_file;

    handle(input_file);

}