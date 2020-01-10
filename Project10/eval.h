#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
#include <iomanip>


class Eval
{
public:
	Eval(std::string val);
	void changeExpressionValue(std::string val);
	double makeEvaluation();
private:
	std::string valStr;
	bool isOperand(char c);
	double makeStodConv(std::string val);
	std::string executeMathExp(std::string val1, std::string val2, std::string operand);
	void reduceOperands(std::vector<std::string>& arr);
	void replaceExpression(std::vector<std::string>& arr, std::vector<std::string>::iterator& it, std::string val);
	bool checkNegativeNum(std::string expression);
	void reduceMinusSigns(std::string& expression);
	double makeCalculation(std::string expression);
	std::string processBrackets(std::string expression);
	double calc(std::string expression);
	void tests();
};