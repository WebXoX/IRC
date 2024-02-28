#include "T1.hpp"

/* orth T1 / constructor*/
T1::T1 ()
{
	std::cout << "T1 default constructor" << std::endl;
}

T1::T1 (const T1 &a)
{
	std::cout << "T1 copy constructor " << std::endl;
	*this = a;
}

T1::~T1 ()
{
	std::cout << "T1 distructor called" << std::endl;
}

T1& T1::operator=(const T1& rhs)
{
    if (this != &rhs)
    {
        std::cout << "T1 copy assignment operator = " << std::endl;
		*this = rhs;
    }
    return *this;
}
/* orth T1 */
/*extra*/
/*extra*/
/*getter and setters*/
/*getter and setters*/
