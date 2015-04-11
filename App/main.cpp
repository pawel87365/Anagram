#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <list>
#include <vector>
#include <algorithm>    // std::sort
#include <windows.h>

using namespace std;

// Optimized version originally written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
int wildcmp(const char *wild, const char *string) 
{	
	const char *cp = NULL, *mp = NULL;

	while (*string) 
	{
		if ((*wild != *string) && (*wild != '?')) 
		{
			return 0;
		}
		wild++;
		string++;
	}

	while (*string) 
	{
	if ((*wild == *string) || (*wild == '?')) 
		{
			wild++;
			string++;
		} 
		else 
		{
			wild = mp;
			string = cp++;
		}
	}

	return !*wild;
}

std::set<std::string> getParentsAnagrams(const std::string &word, std::set<std::string> &dictionary)
{
	std::string input = word;

	std::sort(input.begin(), input.end());
	std::set<std::string>::iterator it;

	std::set<std::string> listToBeReturned;
	const size_t inputSize = input.size();

	/// set ? to all positions in word
	for(size_t j = 0; j < inputSize + 1; j++)
	{
		std::string wildCardString = input;
		wildCardString.insert(j, "?");

		std::string testString;

		cout << "Anagrams for wildcard word " << wildCardString << ": " << endl;

		for (it=dictionary.begin(); it!=dictionary.end();)
		{
			std::set<std::string>::iterator current = it;

			if((*current).size() == inputSize+1)
			{
				testString = *current;
				std::sort(testString.begin(), testString.end());
				if(wildcmp(wildCardString.c_str(), testString.c_str()))
				{
					cout << (*current) << endl;
					listToBeReturned.insert(*current);	
					dictionary.erase(it++);
				}
				++it;
			}
			else if((*current).size() < inputSize+1)
			{
				dictionary.erase(it++);
			}
			else
				++it;
			
		}
	}

	return listToBeReturned;
}

static void show_usage(std::string name)
{
	std::cerr << "Usage: " << name << " <option(s)> "
		<< "Options:\n"
		<< "\t-h,--help\t\tShow this help message\n"
		<< "\t-b,--base\t\tBase input word\n"
		<< "\t-r,--result \tSpecify the destination path for results\n"
		<< "\t-d,--database \tSpecify the input database\n"
		<< "\tExample: App.exe -b \"ala\" -r \"output.txt\" -d \"dictSmallLittle.txt\""
		<< std::endl;
}

//// int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nShowCmd)
int main(int argc, char ** argv)
{
	using namespace std;

	if (argc < 4) 
	{
		show_usage(argv[0]);
		return 1;
	}

	string input;
	string output;
	string dictFile;

	/// From: http://www.cplusplus.com/articles/DEN36Up4/
	std::vector <std::string> sources;
	std::string destination;
	for (int i = 1; i < argc; ++i) 
		{
		std::string arg = argv[i];
		if ((arg == "-h") || (arg == "--help"))
		{
			show_usage(argv[0]);
			return 0;
		} 
		else if ((arg == "-b") || (arg == "--base")) 
		{
			if (i + 1 < argc) 
			{ // Make sure we aren't at the end of argv!
				input = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
			} 
			else 
			{ // Uh-oh, there was no argument to the destination option.
				std::cerr << "--base option requires one argument." << std::endl;
				return 1;
			}  
		} 
		else if ((arg == "-r") || (arg == "--results")) 
		{
			if (i + 1 < argc) 
			{ // Make sure we aren't at the end of argv!
				output = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
			} 
			else 
			{ // Uh-oh, there was no argument to the destination option.
				std::cerr << "--base option requires one argument." << std::endl;
				return 1;
			}  
		} 
		else if ((arg == "-d") || (arg == "--database")) 
		{
			if (i + 1 < argc) 
			{ // Make sure we aren't at the end of argv!
				dictFile = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
			} 
			else 
			{ // Uh-oh, there was no argument to the destination option.
				std::cerr << "--base option requires one argument." << std::endl;
				return 1;
			}  
		} 
		else 
		{
			sources.push_back(argv[i]);
		}
	}

	/// LOG TIME - just for debug
	SYSTEMTIME t;
	GetSystemTime(&t); // or GetLocalTime(&t)
	printf("The system time is: %02d:%02d:%02d.%03d\n", 
		t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);

	/// load database
	ifstream infile(dictFile);	

	/// set of all words
	set<string> allWords;

	/// put all database into  set
	string line;
	while (getline(infile, line))
	{
		allWords.insert(line);
	}

	/// get all possible n+1 anagrams from base word
	set<string> listOfAnagrams = getParentsAnagrams(input, allWords);

	list<set<string>> listOfAllAnagrams;
	list<set<string>> tempListOfAllAnagrams;
	
	tempListOfAllAnagrams.push_back(listOfAnagrams);

	/// get all n+1 length anagrams for all possible combination
	/// until next anagrams exits
	while(tempListOfAllAnagrams.size() > 0)
	{
		if(listOfAnagrams.size() > 0)
			listOfAllAnagrams.push_back(listOfAnagrams);

		std::set<std::string>::iterator it;
		for (it=listOfAnagrams.begin(); it!=listOfAnagrams.end(); ++it)
		{
			tempListOfAllAnagrams.push_back(getParentsAnagrams(*it, allWords));
		}

		listOfAnagrams = tempListOfAllAnagrams.front();
		tempListOfAllAnagrams.pop_front();

	}

	/// Log time
	GetSystemTime(&t); 
	printf("The system time is: %02d:%02d:%02d.%03d\n", 
		t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);

	/// save results
	ofstream myfile (output);	

	if (myfile.is_open())
	{

		myfile << input << endl;

		for (list<set<string>>::iterator  it=listOfAllAnagrams.begin(); it != listOfAllAnagrams.end(); ++it)
		{
			set<string> tempList = *it;
			set<string>::iterator itSubSet;

			for (itSubSet=tempList.begin(); itSubSet != tempList.end(); ++itSubSet)
			{
				myfile << *itSubSet << " ";
			}
			myfile << endl;
		}
	
		myfile.close();
	}	

	return 0;
}


