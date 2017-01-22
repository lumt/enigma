#include <iostream>
#include <fstream>
#include <ctype.h>
#include <cstdlib>
#include <string>
#include "errors.h"
#include "enigmaparts.h"
#include "inputdata.h"

using namespace std;

/* function to check plugboard file */
void check_file(char* file) {

	// Check file for errors and non digit characters
	ifstream check(file);

	if (!check) {
		cerr << "Error opening config file" << endl;
		exit(ERROR_OPENING_CONFIGURATION_FILE);
	}

	// Find extension of file
	string str(file);
	size_t pos = str.find(".");
	string extension = str.substr(pos);

	// Check for non numeric inputs
	char character;
	check >> character;
	check >> ws;
	while (check) {

		if (!isdigit(character)) {

			// Output relevant error with file
			if (!extension.compare(".pb"))
				cerr << "Non-numeric character in plugboard file ";
			else if (!extension.compare(".rf"))
				cerr << "Non-numeric character in reflector file ";
			else if (!extension.compare(".rot"))
				cerr << "Non-numeric character for mapping in rotor file ";

			cerr << file << endl;
			exit(NON_NUMERIC_CHARACTER);
		}

		check >> character;
	}

	check.close();
}


/* function to load rotor position file */
void check_pos_file(int *rotor_pos, int rotor_count, char *file) {
	
	// Check file beforehand for errors and non digit characters
	ifstream check(file);

	if (!check) {
		cerr << "Error opening config file" << endl;
		exit(ERROR_OPENING_CONFIGURATION_FILE);
	}

	char character;

	check >> character;
	check >> ws;
	while (check) {
		if (!isdigit(character)) {
			cerr << "Non-numeric character in rotor positions file ";
			cerr << file << endl;
			exit(NON_NUMERIC_CHARACTER);
		}
		check >> character;
	}

	check.close();

	// Start reading
	ifstream in(file);

	// Set all values in array to -1
	for (int i = 0; i < rotor_count; i++)
		rotor_pos[i] = VOID;

	int integer;
	int idx = 0;

	in >> integer;
	while (in) {
		
		// Check validity of index
		if (integer < 0 || integer > 25) {
			cerr << "Invalid Index" << endl;
			exit(INVALID_INDEX);
		}

		rotor_pos[idx] = integer;
		idx++;
		in >> integer;
	}

	in.close();
}

