#include "stdafx.h"
#include "Cat.hxx"

const std::vector<Cat>& CatTestDataset() {
	static std::vector<Cat> result;

	{
		Cat cat;
		cat.ID = 1;
		cat.Name = "Somecat";
		cat.Address.ID = 10;
		cat.Address.Number = 10;
		cat.Address.Street = "Ten St.";

		result.push_back(cat);
	}
	{
		Cat cat;
		cat.ID = 2;
		cat.Name = "Garfield";
		cat.Address.ID = 20;
		cat.Address.Number = 20;
		cat.Address.Street = "Twenty St.";

		result.push_back(cat);
	}

	{
		Cat cat;
		cat.ID = 3;
		cat.Name = "Felix";
		cat.Address.ID = 30;
		cat.Address.Number = 30;
		cat.Address.Street = "Thirty St.";

		result.push_back(cat);
	}

	{
		Cat cat;
		cat.ID = 4;
		cat.Name = "Bob";
		cat.Address.ID = 40;
		cat.Address.Number = 40;
		cat.Address.Street = "Forty St.";

		result.push_back(cat);
	}

	return result;
}
