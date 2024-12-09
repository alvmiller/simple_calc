// https://csvparser.github.io/mathparser.html
// https://www.geeksforgeeks.org/how-to-parse-mathematical-expressions-in-cpp/
// https://www.onlinegdb.com/online_c++_compiler
// https://stackoverflow.com/questions/9329406/evaluating-arithmetic-expressions-from-string-in-c
// https://www.partow.net/programming/exprtk/index.html
// https://github.com/cy2301/string-calculator/tree/main
// https://www.partow.net/programming/exprtk/
// https://www.geeksforgeeks.org/parsing-string-of-symbols-to-expression/
// https://ru.stackoverflow.com/questions/23842/%D0%9F%D0%B0%D1%80%D1%81%D0%B5%D1%80-%D0%BC%D0%B0%D1%82%D0%B5%D0%BC%D0%B0%D1%82%D0%B8%D1%87%D0%B5%D1%81%D0%BA%D0%B8%D1%85-%D0%B2%D1%8B%D1%80%D0%B0%D0%B6%D0%B5%D0%BD%D0%B8%D0%B9
// https://habr.com/ru/companies/vdsina/articles/540608/
// https://dzen.ru/a/ZcuSz5RyvVAES05E
// http://mindhalls.ru/symb-expr/
// https://github.com/atastycookie/mathematical-expression-parser/tree/master/cpp
// https://otvet.mail.ru/question/226496864
// https://www.javatpoint.com/postfix-evaluation-in-c
// https://www.geeksforgeeks.org/evaluation-of-postfix-expression/
// https://www.geeksforgeeks.org/how-to-evaluate-a-postfix-expression-using-stack-in-cpp/
// https://www.geeksforgeeks.org/infix-to-postfix-conversion-using-stack-in-cpp/
// https://www.tutorialspoint.com/program-to-evaluate-postfix-notation-in-cplusplus
// https://runestone.academy/ns/books/published/cppds/LinearBasic/InfixPrefixandPostfixExpressions.html
// https://www.javatpoint.com/program-to-convert-infix-to-postfix-expression-in-cpp-using-the-stack-data-structure
// https://guiprojects.com/infix-to-postfix-cpp-using-stack/
// https://simpledevcode.wordpress.com/2015/01/03/convert-infix-to-reverse-polish-notation-c-implementation/
// https://ru.stackoverflow.com/questions/23842/%D0%9F%D0%B0%D1%80%D1%81%D0%B5%D1%80-%D0%BC%D0%B0%D1%82%D0%B5%D0%BC%D0%B0%D1%82%D0%B8%D1%87%D0%B5%D1%81%D0%BA%D0%B8%D1%85-%D0%B2%D1%8B%D1%80%D0%B0%D0%B6%D0%B5%D0%BD%D0%B8%D0%B9
// https://coliru.stacked-crooked.com/a/24ab40c0f0a8558e
// https://www.geeksforgeeks.org/evaluation-of-postfix-expression/
// https://gist.github.com/mycodeschool/7702441
// https://stackoverflow.com/questions/55507483/postfix-expression-evaluation-using-stack
// https://www.javaguides.net/2023/08/cpp-program-to-evaluate-a-postfix-expression.html
// https://gist.github.com/subhamagrawal7/d7d3846f955dda9d5cb5e8a568e4d5a9
// https://www.boardinfinity.com/blog/postfix-expression/

#include <cctype>
#include <cmath>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

void normalize_negative_values(const string& str, string& str_norm)
{
    str_norm = str;
    str_norm.erase(
        remove_if(str_norm.begin(), str_norm.end(), [](const char& element)
        {
            return isspace(element);
        }),
        str_norm.end());
    string::size_type n = str_norm.length();
    for (string::size_type i = 0; i < n; ++i) {
        if (str_norm[i] == '-') {
            if (i == 0 || str_norm[i - 1] == '(') {
                str_norm.insert(i, "0");
                ++i;
                n = str_norm.length();
                continue;
            }
        }
    }

    return;
}

void add_delimeter(const string& str,
    const string& special_symbols,
    char delimeter,
    string& str_upd)
{
    str_upd = str;
    for (const auto el : special_symbols) {
        string::size_type n = 0;
        string s_tmp(1, el);
        string str_delimeter(1, delimeter);
        string str_tmp = str_delimeter + s_tmp + str_delimeter;
        while ((n = str_upd.find(s_tmp, n) ) != string::npos) {
            str_upd.replace(n, s_tmp.size(), str_tmp);
            n += str_tmp.size();
        }
    }

    return;
}

void split(const string& str, char delimiter, vector<string>& tokens)
{
    stringstream ss(str);
    string token = "";
    string str_del(1, delimiter);
    while (getline(ss, token, delimiter)) {
        //if (token != str_del && token.length() > 0) {
        if (token != "") {
            tokens.push_back(token);
        }
    }

    return;
}

inline bool is_number(const string& expression)
{
    istringstream iss(expression);
    double num_tmp = 0.0;
    bool res = iss >> std::noskipws >> num_tmp && iss.eof();

    return res;
}

inline unsigned int precedence(const string& op)
{
    if (op == "+" || op == "-") {
        return 1;
    }
    if (op == "*" || op == "/") {
        return 2;
    }

    return 0;
}

void get_postfix(const vector<string>& infix_tokens, vector<string>& postfix_expression)
{
    stack<string> st = {};
    
    for (const auto& el : infix_tokens) {
        if (is_number(el)) {
            postfix_expression.push_back(el);
        } else if (el == "(") {
            st.push("(");
        } else if (el == ")") {
            while (st.top() != "(") {
                postfix_expression.push_back(st.top());
                st.pop();
            }
            st.pop();
        } else {
            while (!st.empty() && precedence(el) <= precedence(st.top())) {
                postfix_expression.push_back(st.top());
                st.pop();
            }
            st.push(el);
        }
    }

    while (!st.empty()) {
        postfix_expression.push_back(st.top());
        st.pop();
    }

    return;
}

void calculate_postfix(const vector<string>& postfix, double& res)
{
    stack<double> st = {};
    
    for (const auto& el : postfix) {
        if (is_number(el)) {
            double tmp = stod(el);
            st.push(tmp);
        } else {
            double val1 = st.top();
            st.pop();
            double val2 = st.top();
            st.pop();
            if (el == "+") {
                st.push(val2 + val1);
            } else if (el == "-") {
                st.push(val2 - val1);
            } else if (el == "*") {
                st.push(val2 * val1);
            } else if (el == "/") {
                st.push(val2 / val1);
            }
        }
    }
    res = st.top();

    return;
}

double calculate(const string& str)
{
    string str_in = str;
    string str_postfix = "";
    vector<string> v_infix_tokens = {};
    vector<string> v_postfix = {};
    double calculated = 0;
    const string operators = "*/+-()";
    string str_in_upd = "";
    char delimeter = ' ';
    string str_normalized_negative_values = "";

    normalize_negative_values(str_in, str_normalized_negative_values);
    add_delimeter(str_normalized_negative_values, operators, delimeter, str_in_upd);
    split(str_in_upd, delimeter, v_infix_tokens);
    get_postfix(v_infix_tokens, v_postfix);
    calculate_postfix(v_postfix, calculated);
    
    return calculated;
}

int main()
{
    string expression = "-10 +2 *( -4 - 6)";

    double result = calculate(expression);
    cout << "Formula: " << expression << endl;
    cout << "Result: " << result << endl;

    return 0;
}
