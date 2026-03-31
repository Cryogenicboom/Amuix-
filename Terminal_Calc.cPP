#include <iostream>
#include <string>
#include <stack>
#include <windows.h>

int intro(){
    SetConsoleOutputCP(65001);

    std::cout << "████████╗███████╗██████╗ ███╗   ███╗██╗███╗   ██╗ █████╗ ██╗          ██████╗ █████╗ ██╗      ██████╗\n";
    std::cout << "╚══██╔══╝██╔════╝██╔══██╗████╗ ████║██║████╗  ██║██╔══██╗██║         ██╔════╝██╔══██╗██║     ██╔══\n";
    std::cout << "   ██║   █████╗  ██████╔╝██╔████╔██║██║██╔██╗ ██║███████║██║         ██║     ███████║██║     ██║  \n";
    std::cout << "   ██║   ██╔══╝  ██╔══██╗██║╚██╔╝██║██║██║╚██╗██║██╔══██║██║         ██║     ██╔══██║██║     ██║  \n";
    std::cout << "   ██║   ███████╗██║  ██║██║ ╚═╝ ██║██║██║ ╚████║██║  ██║███████╗    ╚██████╗██║  ██║███████╗ ██████\n";
    std::cout<<std::endl;
    return 0;
 
}

int precedence(char op){
    if(op == '+' || op == '-') return 1;
    if(op == '*' || op == '/') return 2;
    return 0;
}

void calc(std::stack<int>& operand, std::stack<char>& op){
    int right_side = operand.top(); operand.pop();
    int left_side = operand.top(); operand.pop();

    char oper = op.top(); op.pop();

    int result;

    if(oper == '+') result = left_side + right_side;
    else if(oper == '-') result = left_side - right_side;
    else if(oper == '*') result = left_side * right_side;
    else if(oper == '/') result = left_side / right_side;

    operand.push(result);
}

// operator > operand > operator
int parsing(std::string exp){
    std::stack<int> operand;
    std::stack<char> op;

    int num = 0;
    for(int i = 0; i < exp.length(); i++){
        if(exp[i] == ' '){
            continue;
        }

        if(isdigit(exp[i])){
            num = num*10 + (exp[i]-'0');
        }

        else{
            operand.push(num);
            num = 0;

            while(!op.empty() && precedence(op.top()) >= precedence(exp[i])){
                calc(operand, op);
            }

            op.push(exp[i]);
        }

    }

    operand.push(num);
    while(!op.empty()){
        calc(operand, op);
    }
    return operand.top();
    
}


int main(){
    intro();
    while(true){
        
        std::cout<<std::endl;
        std::cout<<"guest/home:user$ ";
        std::string expression;
        std::getline(std::cin, expression);
        int result = parsing(expression);

        std::cout<<result;
        
    }
    return 0;
}


            // if(!op.empty() && precedence(exp[i]) >= precedence(op.top()) ){
            //     op.push(exp[i]);
            // }
            // else if(!op.empty() && precedence(exp[i]) < precedence(op.top())){
            //     int num_right = operand.top(); operand.pop();
            //     if(op.top() == '+'){
            //         operand.top() = operand.top() + num_right;
            //         op.pop();
            //     }
            //     else if(op.top() == '-'){
            //         operand.top() = operand.top() - num_right;
            //         op.pop();
            //     }
            //     if(op.top() == '*'){
            //         operand.top() = operand.top() * num_right;
            //         op.pop();
            //     }
            //     if(op.top() == '/'){
            //         operand.top() = operand.top() / num_right;
            //         op.pop();
            //     }
            //     else{
            //         std::cout<<"Invalid operator\nprogramme execution state: ";
            //         return 0;
            //     }
            // }

