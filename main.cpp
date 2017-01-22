#include <iostream>
#include <fstream>
#include <ctype.h>
#include <cstdlib>
#include "errors.h"
#include "inputdata.h"
#include "enigmaparts.h"

using namespace std;

int main(int argc, char* argv[])
{
	// Check number of command line arguments
	if (argc < 3) {
		cerr << "usage: enigma plugboard-file reflector-file ";
		cerr << "(<rotor-file>* rotor-positions)?" << endl;
		return INSUFFICIENT_NUMBER_OF_PARAMETERS;
	}
	
	/* PLUGBOARD */
	check_file(argv[1]);
	Plugboard *plugboard = new Plugboard(argv[1]);
	/* END PLUGBOARD */


	/* REFLECTOR */
	check_file(argv[2]);
	Reflector *reflector = new Reflector(argv[2]);
	/* END REFLECTOR */

	
	/* ROTOR */
	int rotor_count = argc - 4;

	Rotor **rotors = NULL;

	// if rotors exists
	// load neccessary files and initialize rotor object
	if (rotor_count > 0) {

		// allocate memory to an array of pointers to rotors
		rotors = new Rotor* [rotor_count];

		int rotor_pos[rotor_count];
		check_pos_file(rotor_pos, rotor_count, argv[argc-1]);
		
		for (int i = 0; i < rotor_count; i++) {

			check_file(argv[i+3]);

			// allocate memory for each rotor and initialize
			rotors[i] = new Rotor(i, rotor_pos[i], argv[i+3], argv[argc-1]);
		}

		// configure rotor conenctions
		for (int i = 1; i < rotor_count; i++) {
			rotors[i]->set_left_rotor(rotors[i-1]);
		}
	}
	/* END ROTOR */


	/* ENCRYPTION */
	char input;
	int output;

	cin >> ws;
	while (cin >> input) {

		// check for proper input
		if (!isalpha(input) || !isupper(input)) {
			cerr << input << " is not a valid input character (input ";
			cerr << "characters must be upper case letters A-Z)!" << endl;
			return INVALID_INPUT_CHARACTER;
		} else {

			// convert output to 0-based index integer
			output = (int)(input) - 'A';

			// encrypt through proper route
			plugboard->encrypt(output);

			if (rotor_count > 0) {
				rotors[rotor_count - 1]->rotate();
				for (int i = rotor_count - 1; i >= 0; i--) 
					rotors[i]->encrypt_in(output);
			}

			reflector->encrypt(output);

			if (rotor_count > 0)
				for (int i = 0; i < rotor_count; i++)
					rotors[i]->encrypt_out(output);

			plugboard->encrypt(output);

			// convert output back to character
			cout << (char)(output + 'A');
		}
	}
	/* END ENCRYPTION */

	// delete memory that was allocated
	delete plugboard;
	delete reflector;
	delete[] rotors;

	return NO_ERROR;
}
