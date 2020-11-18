#include "funcprog.h"

int main()
{
	function* wow1 = TRUE.call(&TRUE);
	std::cout << " : wow1" << wow1->print() << std::endl;
	{
		function A((char*)"&abcd.dbca");
		function B((char*)"&ab.ba");
		A.call();
	}
	std::cout << "WOWS ARE MADE\n";
	function::printstack();
	wow1->remove();
	function::printstack();
	return 0;
}