### Terminal Calculator Documentation 

<br>

> this documentation is written in raw manner, your support in reframing and documenting it would be appreciated

<br>

#### int main()

1. calls parsing function and passes the user input string.
<br>

2. exits id user_input == "quit"
<br>

3. 
> std::getline(std::cin, expression) 
<br>
inputs the whole string instead of stopping at first whitespace.
<br>

#### int precedence()

since of precendence of "*" and "/" are more than "+" and "-" therefore they return 2 and 1 respectively.

#### void calc()

1. it takes two arrays (stack) as parameters, parameters are passed by reference.
<br>

2. uses top two elements of "operands" stack where topmost is right side operand and other is leftside operand. 
<br>

3. extracts the operator from "op" stack and pop it.
<br>

4. does the calculations as instructed and push the result into "operand" stack.
<br>


#### int parsing()

This function takes a mathematical expression as a string like "240 + 12 - 3 * 8 + 1" and evaluates it correctly, respecting operator precedence.
<br>

> Instead of solving the expression directly, we simulate how a calculator works using:
> Operand stack → stores numbers
> Operator stack → stores +, -, *, /

1. Building Numbers (ASCII → Integer)
<br> 

> num = num * 10 + (exp[i] - '0')
> <br>
> This converts chars into int.
<br>

2. Two Stack System We maintain:
<br>

> operand = [numbers]
> operator = [+, -, *, /]
<br>

3. Precedence Rule 
<br>

if incoming operator ≤ top of stack operator → pop and solve
> while (!op.empty() && precedence(op.top()) >= precedence(current))
>    solve
<br>

ensures: * and / are evaluated before + and -

