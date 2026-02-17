#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
using namespace std;


struct income
{
    string name;
    string source_day;
    string source_place;
    int sum;
};

string input(){
    cout << "Enter data by template:\nIncome: date(yyyy.mm.dd) source(strng in \"quotes\") sum(number)\n";
    string user_input;
    getline(cin, user_input);
    return user_input + " ";
}

vector<string> splitting(string input){
    vector<string> data;
    data.push_back(input.substr(0, input.find(":")));
    int start = input.find(":")+1;
    int end = input.find(" ");
    while (end != string::npos) {
        if (end-start > 0) data.push_back(input.substr(start, end-start));
        start = end + 1;
        end = input.find(' ', start);
    }
    return data;
}

income assign(vector<string> input) {
    income out;
    bool f_quo = 0;
    for (int i = 0; i < input.size(); i++){   
        if (f_quo == 1) out.source_place = out.source_place + ' ' + input[i];
        if (input[i].find('.') != string::npos){
            out.source_day = input[i];
        }
        else if (input[i].find('"') != string::npos){
            if (input[i].find('"',1) == string::npos) {
                f_quo=1;
                out.source_place = input[i];
            }
            else {
                f_quo = 0;
            }
            }
        else if (f_quo == 0) {
            bool f_num = 0;
            string nums = "0123456789";
            for (char a : input[i]){
                if (nums.find(a) == string::npos) f_num = 1;
                a++;
            }
            if (f_num==0) out.sum = stoi(input[i]);
            else out.name = input[i];
        }
    }
    return out;
}


void income_printout(income in){
    cout << "Name: " << in.name << '\n';
    cout << "Day: " << in.source_day << '\n';
    cout << "Source: " << in.source_place << '\n';
    cout << "Sum: " << in.sum << '\n';
}

int main(){
    string user_input = input();
    income money;
    
    money = assign(splitting(user_input));

    income_printout(money);
    return 1;
}