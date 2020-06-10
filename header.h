#pragma once

#include<limits>
#include<iostream>
#include<string>
#include<unordered_map>
#include<stack>

constexpr auto LEFT_ASSOCIATIVE = 1;
constexpr auto RIGHT_ASSOCIATIVE = 2;

std::string rpn;

bool askInfo(std::string &input);
bool isletter(char input);
bool isOp(char input);
unsigned precedence(char input);
unsigned association(char input);
void recordVar(std::unordered_map<char, bool> &vars, std::string input);
void printHeader(std::unordered_map<char, bool> vars);
void printMap(std::unordered_map<char, bool> vars);
void drawTruthTable(std::unordered_map<char, bool> &vars, std::unordered_map<char, bool>::iterator it);
std::string RPN(std::string expr);
bool evalution(std::string rpn, std::unordered_map<char, bool> vars);
