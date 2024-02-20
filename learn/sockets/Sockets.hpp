#pragma once
#ifndef __BITCOINEXCHANGE_HPP__
#define __BITCOINEXCHANGE_HPP__
#include <iostream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <map>

/*
*/
class BitcoinExchange
{
	/* VARIABLES*/
	private:
		std::map<std::string, int> KeyVal;
		
    /* VARIABLES*/
	public:
	/*orth form*/
    	BitcoinExchange ();
    	BitcoinExchange (const BitcoinExchange &a);
    	~BitcoinExchange ();
		BitcoinExchange& operator=(const BitcoinExchange &a);
	/*orth BitcoinExchange*/
	/*exception*/

	
	/*exception*/
	/*getters and setters*/
	void setKeyVal(std::string key, int val);
	/*getters and setters*/
	/*extra*/
	void exchange(std::string date_save, float fvalue);
	/*extra*/
};
#endif
