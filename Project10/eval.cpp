#include "eval.h"

Eval::Eval(std::string val)
{
	valStr = val;
}

void Eval::changeExpressionValue(std::string val)
{
	valStr = val;
}

double Eval::makeEvaluation()
{
	return calc(valStr);
}

namespace util
{
	template <typename T> std::string to_string(const T& t)
	{
		if (t == 0) return "0";
		std::stringstream strstr;
		strstr << std::fixed << std::setprecision(20) << t;
		std::string str = strstr.str();
		int offset{ 1 };
		if (str.find_last_not_of('0') == str.find('.'))
		{
			offset = 0;
		}
		str.erase(str.find_last_not_of('0') + offset, std::string::npos);
		return str;
	}
}

bool Eval::isOperand(char c)
{
	return c == '+' || c == '*' || c == '/' || c == '-';
}

double Eval::makeStodConv(std::string val)
{
	try
	{
		const auto ret = std::stod(val);
		return ret;
	}
	catch (const std::exception &exc)
	{
		std::cerr << exc.what() << val;
	}
	return 0;
}

std::string Eval::executeMathExp(std::string val1, std::string val2, std::string operand)
{
	auto dVal1 = makeStodConv(val1);
	auto dVal2 = makeStodConv(val2);
	auto cOperand = operand[0];
	double dRetVal = 0;
	switch (cOperand)
	{
	case '+':
		dRetVal = dVal1 + dVal2;
		break;
	case '-':
		dRetVal = dVal1 - dVal2;
		break;
	case '*':
		dRetVal = dVal1 * dVal2;
		break;
	case '/':
		dRetVal = dVal1 / dVal2;
		break;
	default:
		break;
	}
	return util::to_string(dRetVal);
}

void Eval::reduceOperands(std::vector<std::string>& arr)
{
	for (auto it = arr.begin(); it != arr.end(); ++it)
	{
		auto val = *it;
		if (val == "-")
		{
			auto nextVal = *(++it);
			if (nextVal == "+")
			{
				it = arr.erase(it);
			}
			else if (nextVal == "-")
			{
				arr.emplace_back("+");
				auto end = arr.end() - 1;
				std::iter_swap(it, end);
				it = arr.erase(it - 1);
				arr.erase(arr.end() - 1);
			}
			--it;
		}
		if (val == "+")
		{
			auto nextVal = *(++it);
			if (nextVal == "+")
			{
				it = arr.erase(it);
			}
			else if (nextVal == "-")
			{
				it = arr.erase(it - 1);
			}
			--it;
		}
	}
}
void Eval::replaceExpression(std::vector<std::string>& arr, std::vector<std::string>::iterator& it, std::string val)
{
	it = arr.insert(it + 2, val);
	it = arr.erase(it - 1);
	--it;
	it = arr.erase(it - 1);
	it = arr.erase(it);
}

bool Eval::checkNegativeNum(std::string expression)
{
	std::string pom = "%" + expression;
	std::vector<char> arr;

	arr.reserve(pom.size());
	for (int i = 0; i < pom.size(); ++i)
	{
		arr.push_back(pom[i]);
	}

	for (int i = 0; i < arr.size(); ++i)
	{
		if (isOperand(arr[i]) && arr[i] != '-')
		{
			return false;
		}
	}

	for (auto it = arr.begin(); it != arr.end(); ++it)
	{
		char val = *it;

		if (val == '-')
		{
			return *(it - 1) == '%';
		}
	}
	return false;
}


void Eval::reduceMinusSigns(std::string& expression)
{

	if (expression[0] == '-' && expression[1] == '-')
	{
		expression.erase(std::remove(expression.begin(), expression.end(), '-'), expression.end());
	}
}

double Eval::makeCalculation(std::string expression)
{
	expression.erase(std::remove(expression.begin(), expression.end(), ' '), expression.end());

	if (expression.find('+') == std::string::npos &&
		expression.find('*') == std::string::npos &&
		expression.find('/') == std::string::npos)
	{
		if (expression.find('-') == std::string::npos)
		{
			return makeStodConv(expression);
		}
		if (expression.find('-') != std::string::npos)
		{
			reduceMinusSigns(expression);
			if (checkNegativeNum(expression))
			{
				return makeStodConv(expression);
			}
		}
	}

	expression.erase(std::remove(expression.begin(), expression.end(), ' '), expression.end());

	auto arr = std::vector<std::string>{};

	std::string currentStr;

	bool addMinus = false;

	for (auto i = expression.rbegin(); i != expression.rend(); ++i)
	{
		auto z = expression.rbegin();
		if (i + 1 == expression.rend())
		{
			if (*i == '-')
			{
				currentStr = "-" + currentStr;
				arr.push_back(currentStr);
				break;
			}
		}
		char currentVal = *i;
		addMinus = false;

		if (isOperand(currentVal))
		{
			if (currentVal == '-')
			{
				auto nextVal = *(std::next(i, 1));
				if (isOperand(nextVal))
				{
					currentStr = currentVal + currentStr;
				}
				else
				{
					addMinus = true;
				}
				arr.push_back(currentStr);
				currentStr.clear();
				std::prev(i, 1);
				if (addMinus) arr.emplace_back(1, currentVal);
			}
			else
			{
				if (!currentStr.empty()) arr.emplace_back(currentStr);
				arr.emplace_back(1, currentVal);
				currentStr.clear();
			}
		}
		else
		{
			currentStr = currentVal + currentStr;
		}
		if (++i == expression.crend())
		{
			arr.push_back(currentStr);
		}
		--i;
	}

	std::reverse(arr.begin(), arr.end());
	arr.erase(std::remove(arr.begin(), arr.end(), ""), arr.end());

	reduceOperands(arr);

	for (auto it = arr.begin(); it != arr.end(); ++it)
	{
		auto val = *it;
		if (val == "*" || val == "/")
		{
			auto calculatedVal = executeMathExp(*(it - 1), *(it + 1), val);
			replaceExpression(arr, it, calculatedVal);
		}
	}
	for (auto it = arr.begin(); it != arr.end(); ++it)
	{
		auto val = *it;
		if (val == "-" || val == "+")
		{
			auto calculatedVal = executeMathExp(*(it - 1), *(it + 1), val);
			replaceExpression(arr, it, calculatedVal);
		}
	}
	return makeStodConv(arr[0]);
}

std::string Eval::processBrackets(std::string expression)
{
	while (expression.find(')') != std::string::npos)
	{
		auto end = expression.find(')');
		int start = 0;

		for (int i = 0; i < expression.size(); ++i)
		{
			if (expression[i] == '(' && i < end)
			{
				start = i;
			}
		}

		double value = makeCalculation(expression.substr(start + 1, end - 1 - start));

		std::string valStr = util::to_string(value);

		expression.replace(start, end - start + 1, valStr);
	}
	return expression;
}

double Eval::calc(std::string expression)
{
	expression = processBrackets(expression);
	return makeCalculation(expression);
}

void Eval::tests()
{
	std::cout << calc("1 + 1") << "  -  2" << std::endl;
	std::cout << calc("8/16") << "  -  0.5" << std::endl;
	std::cout << calc("3 -(-1)") << "  -  4" << std::endl;
	std::cout << calc("2 + -2") << "  -  0" << std::endl;
	std::cout << calc("10- 2- -5") << "  -  13" << std::endl;
	std::cout << calc("(((10)))") << "  -  10" << std::endl;
	std::cout << calc("3 * 5") << "  -  15" << std::endl;
	std::cout << calc("-7 * -(6 / 3)") << "  -  14" << std::endl;
	std::cout << calc("(1 - 2) + -(-(-4 + 4))") << "  -  -1" << std::endl;
	std::cout << calc("1 + 2 * 3 * 3 - 8") << "  -  11" << std::endl;
}