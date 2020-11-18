#pragma once
#include <iostream>
#include <string.h>

#define MAX 20
#define MAX_NAME 21
#define LAMBDA '&'

//NOTE TO SELF
/*
* Checking if 2 functions are the same should be done by
*	no. of variables are equal
*	seeing if the variables are located in the same order
*	etc
* 
* Bracket have to be read......(To be implemented)
* 
* Names for each Function.
*/

//NOTES ON HOW TO USE
/*
* Deleting the stack should be done in such in the opposite order it was created for maximum efficiency.
* 
*/

// Used to substitute values passed to a function into their rightful place
/*
* When replacing with a function, use brackets to avoid confusion
*/
void replace(char* main ,char *newmain, char* toreplace, char key, int start = 0, int skip = 0)
{
	int i = start, j = skip, k;
	while (main[i] != '\0')
	{
		if (j >= MAX)
			throw "Out Of Bound error";

		if (main[i] == key)
		{
			k = 0;
			while (toreplace[k] != '\0')
			{
				if (toreplace[k] == ' ')
					++k;
				else
					newmain[j++] = toreplace[k++];
			}
			++i;
		}
		else if (main[i] == ' ') // spaces are ignored
		{
			++i;
		}
		else
		{
			newmain[j++] = main[i++];
		}
	}
	newmain[j] = '\0';
}

// Used to make the variables used to be in order
/*
* occurance such as
* 
* "(&     )"
* 
* must be unaffected / call it seperately recursively.
*/
void arrange_variables(char* A)
{
	if (A[0] != LAMBDA) // Not a function, hence no change is allowable
		return;

	int var = 1, i, j;

	while (A[var++] != '.');
	var -= 2; // var is the number of variables in the function

	char toreplace = 'a', key;
	for(i = 0; i < var; ++i)
	{
		key = A[1 + i];
		j = i + 1;
		while (A[j] != '\0' && A[j] != LAMBDA)
			if (A[j++] == key) A[j - 1] = toreplace;
		++toreplace;
	}
}

// Beta Reduction
/*
* Unnecesssary Brackets have to be removed.
* "&a.&b.&c" ==> "&c" .... No occurance of the variable
* "&a&b&c.cba" ==> "&abc.cba" .... Clubing the functions into one
*/
void betaR(char* A)
{

}

class function
{
	int deletetype;
protected:
	static function* top;

	//char name[MAX_NAME];
	char func[MAX];
	int pos;
	function* old;

public:
	static void printstack();
	char print() const
	{
		std::cout << func;
		return 0;
	}

	static function* search(char*);
	function* call(function* arg);

	// Constructors
	function(char* F)
	{
		deletetype = 0;
		int i, nos = -1;
		pos = 0;
		for (i = 0; F[i] != '\0'; ++i)
		{
			func[i] = F[i];
			if (F[i] == LAMBDA)
				++nos;
			else if (F[i] == '.')
			{
				if (nos == 0)
					pos = i + 1;
				else
					--nos;
			}
		}
		for (; i < MAX; ++i)
			func[i] = 0;

		old = top;
		top = this;
	}
	function(function& a)
	{
		deletetype = 0;
		int i;
		pos = 0;
		for (i = 0; a.func[i] < MAX; ++i)
		{
			a.func[i] = a.func[i];
			if (a.func[i] == '.' and pos == 0)
				pos = i + 1;
		}
		func[i] = 0;

		old = top;
		top = this;
	}
	function(function* a)
	{
		deletetype = 0;
		int i;
		pos = 0;
		for (i = 0; a->func[i] < MAX; ++i)
		{
			a->func[i] = a->func[i];
			if (a->func[i] == '.' and pos == 0)
				pos = i + 1;
		}
		func[i] = 0;

		old = top;
		top = this;
	}

	// Destructors
	int remove()
	{
		if (deletetype == 0)
			return -1;
		else if (deletetype == 1) // Created automatically during function call
		{
			delete this;
			return 1;
		}
		else if (deletetype == 2)
			return 0;
	}
	~function()
	{
		if (top == this)
			top = this->old;
		else
		{
			function* pre = top;
			while (this != pre->old) pre = pre->old;
			pre->old = this->old;
		}
	}
};

void function::printstack()
{
	std::cout << "STACK\n";
	function* X = top;
	while (X)
	{
		X->print();
		std::cout << std::endl;
		X = X->old;
	}
}

function* function::top = NULL;
function* function::search(char* S)
{
	int flag = 0;
	function* X = top;
	while (X)
	{
		if (strcmp(S, X->func) == 0)
		{
			flag = 1;
			break;
		}
		X = X->old;
	}
	if (flag)
		return X;
	else
		return NULL;
}
function* function::call(function* arg)
{
	if (func[0] != LAMBDA) // NOT A FUNCTION, HENCE CANNOT BE CALLED
		return NULL;

	char newfunc[MAX] = { 0 };
	if (arg->func[0] != '\0')	//argument a function or an object
	{
		int i = 0;
		if (pos > 3)
		{
			newfunc[0] = LAMBDA; // [0] is copied, [1]  first variable is skipped
			i = 1;
			do
			{
				newfunc[i] = func[i + 1];
				++i;
			} while (func[i] != '.');
		}

		replace(func, newfunc, arg->func, func[1], pos, i);
		arrange_variables(newfunc);

		function* newf = search(newfunc);
		if (newf) // Checking if this function already exists
		{
			std::cout << "Predefined Function\n";
			return newf;
		}

		newf = new function(newfunc);
		if(newf)
			newf->deletetype = 1;
		return newf;
	}
	else
	{
		return this;
	}
}



// COMMONLY USED FUNCTIONS
/*
* Identity function is also used to detect completion of the program.
* If this function is deleted, it means thet the stack is now empty.
*/
function I((char*)"&a.a");		// I	Idiot, Identity
function M((char*)"&a.aa");		// M	MockingBird
function TRUE((char*)"&ab.a");	// K	Kestrel
function FALSE((char*)"&ab.b");	// KI	Kite
function NOT((char*)"&ab.ba");	// C	Cardinal
function AND((char*)"&ab.aba");
function OR((char*)"&ab.aab");
function EQ((char*)"&ab.ab((&ab.ba)q)");