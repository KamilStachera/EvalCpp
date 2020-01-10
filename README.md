# EvalCpp

## How to use

Just put the "eval.cpp" and "eval.h" in your project directory, then include header file:

```c++
#include "eval.h"
```

After that, you simply create Eval object (passing the expression string in constructor) and call makeEvaluation function

```c++
std::string expression = "3 * (3 + 3)";
auto eval = new Eval(expression);
auto value = eval->makeEvaluation();
```
## TODO

* more tests
* custom to_string method (standard library one has too small precision)
