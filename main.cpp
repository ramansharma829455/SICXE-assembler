#include<bits/stdc++.h>
// #include "data.hpp"
#include "pass1.hpp"
#include "pass2.hpp"

using namespace std;

int main() {
   
    int program_length=pass1();
    // cout<<program_length;
    if(program_length==-1){
        cout<<"Error in pass1"<<endl;
        return 0;
    }
    else{
        cout<<"Pass1 completed successfully"<<endl;
        pass2(program_length);
    }

    return 0;
}