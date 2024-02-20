#include "BitcoinExchange.hpp"

/* orth BitcoinExchange / constructor*/
BitcoinExchange::BitcoinExchange ()
{
	std::cout << "BitcoinExchange default constructor" << std::endl;
}

BitcoinExchange::BitcoinExchange (const BitcoinExchange &a)
{
	std::cout << "BitcoinExchange copy constructor " << std::endl;
	*this = a;
}

BitcoinExchange::~BitcoinExchange ()
{
	std::cout << "BitcoinExchange distructor called" << std::endl;
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& rhs)
{
    if (this != &rhs)
    {
        std::cout << "BitcoinExchange copy assignment operator = " << std::endl;
		*this = rhs;
    }
    return *this;
}
/* orth BitcoinExchange */
/*extra*/
void BitcoinExchange::exchange(std::string date_save, float fvalue)
{
	    for (std::map<std::string, int>::iterator i = this->KeyVal.begin(); i != this->KeyVal.end(); ++i)
        {
            if(i->first == date_save)
            {
                std::cout << i->first << "=> " << i->second << " = " << i->second * fvalue << std::endl;
                return ;
            }
            if (i->first > date_save)
            {
                std::cout << (--i)->first << ":"  "=> " << (--i)->second << " = " << (--i)->second * fvalue << std::endl;
                return ;
            }
        }
}
/*extra*/
/*getter and setters*/

void BitcoinExchange::setKeyVal(std::string key, int val)
{
	this->KeyVal[key] = val;
}
/*getter and setters*/
