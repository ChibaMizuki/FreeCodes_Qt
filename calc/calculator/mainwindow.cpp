#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <cctype>
#include <cmath>
using namespace std;

vector<string> split_to_parts(string a);
vector<string> rpn(vector<string> input);
double calculator(vector<string> rpn);
double calc(double a, double b, string op);
bool is_error;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    is_error = false;

    connect(ui->button_zero,  &QPushButton::clicked, this, [=](){ add_text("0"); });
    connect(ui->button_one,   &QPushButton::clicked, this, [=](){ add_text("1"); });
    connect(ui->button_two,   &QPushButton::clicked, this, [=](){ add_text("2"); });
    connect(ui->button_three, &QPushButton::clicked, this, [=](){ add_text("3"); });
    connect(ui->button_four,  &QPushButton::clicked, this, [=](){ add_text("4"); });
    connect(ui->button_five,  &QPushButton::clicked, this, [=](){ add_text("5"); });
    connect(ui->button_six,   &QPushButton::clicked, this, [=](){ add_text("6"); });
    connect(ui->button_seven, &QPushButton::clicked, this, [=](){ add_text("7"); });
    connect(ui->button_eight, &QPushButton::clicked, this, [=](){ add_text("8"); });
    connect(ui->button_nine,  &QPushButton::clicked, this, [=](){ add_text("9"); });
    connect(ui->button_dot,   &QPushButton::clicked, this, [=](){ add_text("."); });
    connect(ui->button_plus,   &QPushButton::clicked, this, [=](){ add_text("+"); });
    connect(ui->button_minus,  &QPushButton::clicked, this, [=](){ add_text("-"); });
    connect(ui->button_multi,  &QPushButton::clicked, this, [=](){ add_text("*"); });
    connect(ui->button_divide, &QPushButton::clicked, this, [=](){ add_text("/"); });
    connect(ui->button_front,  &QPushButton::clicked, this, [=](){ add_text("("); });
    connect(ui->button_back,   &QPushButton::clicked, this, [=](){ add_text(")"); });
    connect(ui->button_clear,  &QPushButton::clicked, this, [=](){ clear_text(); });
    connect(ui->button_delete, &QPushButton::clicked, this, [=](){ delete_text(); });
    connect(ui->button_equal,  &QPushButton::clicked, this, [=](){
        string s = ui->display->text().toStdString();
        run_calc(s);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::add_text(const QString &text)
{
    if(is_error){
        ui->display->clear();
        is_error = false;
    }
    ui->display->setText(ui->display->text() + text);
}

void MainWindow::clear_text()
{
    if(is_error){
        ui->display->clear();
        is_error = true;
    }
    if(ui->display->text() != "")
    {
        ui->display->clear();
    }
}

void MainWindow::delete_text()
{
    if(ui->display->text() != "" && !is_error)
    {
        QString tmp = ui->display->text();
        tmp.chop(1);
        ui->display->setText(tmp);
    }
}

void MainWindow::run_calc(std::string s)
{
    vector<string> parts = split_to_parts(s);
    if(parts.size() == 1 && parts[0].find("ERROR") != string::npos){
        ui->display->setText("error");
        is_error = true;
        return;
    }
    vector<string> convert = rpn(parts);

    double result = calculator(convert);
    if(!isnan(result))
    {
        QString num = QString::number(result);
        ui->display->setText(num);
    }else
    {
        ui->display->setText("error");
        is_error = true;
    }
}

vector<string> split_to_parts(string a){
    vector<char> ops = {'+', '-', '*', '/', '@'};
    vector<string> parts;
    string num = "";
    string op = "";

    if(a == ""){
        parts.push_back("ERROR: no formula");
        return parts;
    }

    int str_size = a.size();
    if(!isdigit(a[str_size-1]) && a[str_size-1] != ')'){ // 末尾に数字が来ない場合
        parts.push_back("ERROR: end parts is not number");
        return parts;
    }
    if(!isdigit(a[0]) && a[0] != '@' && a[0] != '(' && a[0] != '-'){
        parts.push_back("ERROR: first is not number");
        return parts;
    }
    for(int i = 0; i < str_size; i++){
        if(a[i] == ' ') continue; //空白が混じっていた場合

        auto is_op = find(ops.begin(), ops.end(), a[i]);
        if(isdigit(a[i]) || a[i] == '.'){ // 自然数もしくは小数
            num += a[i];
        } else if(a[i] == '-'){ // 単項マイナス判定
            if(i == 0 || a[i-1] == '(' || find(ops.begin(), ops.end(), a[i-1]) != ops.end()){
                parts.push_back("@");
            } else {
                if(num != "") parts.push_back(num);
                num = "";
                parts.push_back("-");
            }
        } else if(a[i] == '(') {
            op = a[i];
            if(op != "") parts.push_back(op);
            op = "";
        } else if(a[i] == ')' || // 文頭でない、かつ記号が連続していないなら
                   (is_op != ops.end() && i > 0 &&
                    find(ops.begin(), ops.end(), a[i-1]) == ops.end())) { // 演算子
            if(num != "") parts.push_back(num);
            op = a[i];
            if(op != "") parts.push_back(op);
            op = "";
            num = "";
        } else {
            parts.clear();
            parts.push_back("ERROR");
            return parts;
        }
    }
    if(num != "") parts.push_back(num);
    int open = 0, close = 0;
    for(int i = 0; i < parts.size(); i++){
        if(parts[i] == "("){
            open++;
        }else if(parts[i] == ")"){
            close++;
        }
        if(close > open){
            parts.clear();
            parts.push_back("ERROR: () error");
            break;
        }
    }
    if(open != close){
        parts.clear();
        parts.push_back("ERROR: () error");
    }
    return parts;
}

vector<string> rpn(vector<string> input){
    vector<string> ops = {"+", "-", "*", "/", "@"};
    map<string, int> priority_map;
    priority_map["$"] = 0;
    priority_map["+"] = 2;
    priority_map["-"] = 2;
    priority_map["*"] = 3;
    priority_map["/"] = 3;
    priority_map["@"] = 4;

    vector<string> stack;
    vector<string> output;
    string end_op;
    string op;

    stack.push_back("$");

    while(input.size() > 0){
        if(input[0] == "("){ // 前括弧はそのままstack
            stack.push_back(input[0]);
        }else if(input[0] == ")"){ // 後括弧の時は前括弧が現れるまで演算子を出力し続ける
            while(!stack.empty() && stack.back() != "("){
                output.push_back(stack.back());
                stack.pop_back();
            }
            if(stack.back() == "(") stack.pop_back();
        }else if((find(ops.begin(), ops.end(), input[0]) == ops.end())){
            output.push_back(input[0]); // 数値ならそのまま出力
        }else if((find(ops.begin(), ops.end(), input[0]) != ops.end())){ // 演算子の場合
            op = input[0];
            while(stack.size() > 1 &&
                   stack.back() != "(" &&
                   (
                       (op != "@" && priority_map[op] <= priority_map[stack.back()]) || // 演算子の左結合
                       (op == "@" && priority_map[op] <  priority_map[stack.back()])    // 単項マイナスの右結合
                       )
                   ){
                output.push_back(stack.back());
                stack.pop_back();
            }
            stack.push_back(op);
        }

        input.erase(input.begin());
    }

    while(stack.size() > 1){
        output.push_back(stack.back());
        stack.pop_back();
    }
    return output;
}

double calc(double a, double b, string op){
    if(op == "+") return a + b;
    if(op == "-") return a - b;
    if(op == "*") return a * b;
    if(op == "/"){
        if(b == 0.0) return nan("");
        return a / b;
    }
    return nan("");
}

double calculator(vector<string> rpn){
    vector<string> ops = {"+", "-", "*", "/", "@"};
    vector<double> nums;
    double first, second, result;
    while(rpn.size() > 0){
        if((find(ops.begin(), ops.end(), rpn[0]) != ops.end())){
            if(nums.size() >= 1 && rpn[0] == "@"){
                second = nums.back();
                nums.pop_back();
                nums.push_back(second * -1);
                rpn.erase(rpn.begin());
            }else if(nums.size() >= 2){
                second = nums.back();
                nums.pop_back();
                first = nums.back();
                nums.pop_back();

                result = calc(first, second, rpn[0]);
                if(isnan(result)) return nan("");
                nums.push_back(result);
                rpn.erase(rpn.begin());
            }else{
                return nan("");
            }
        }else{
            try{
                nums.push_back(stod(rpn[0]));
            } catch(...) {
                return nan("");
            }
            rpn.erase(rpn.begin());
        }
    }
    if(nums.empty()) return nan("");
    return nums[0];
}
