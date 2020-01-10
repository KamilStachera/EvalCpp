#include "eval.h"

/*Example code
 * in first cout, makeEvalueation should return 8
 * in second cout 18
 */
int main()
{
	auto eval = new Eval("3+5");
	std::cout << eval->makeEvaluation();

	eval->changeExpressionValue("3 * (3 + 3)");
	std::cout << eval->makeEvaluation();

	return 0;
}