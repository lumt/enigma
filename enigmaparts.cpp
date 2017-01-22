#include <iostream>
#include <fstream>
#include <cstdlib>
#include "errors.h"
#include "enigmaparts.h"

using namespace std;

/* PLUGBOARD */

/* Plugboard class constructor*/
Plugboard::Plugboard(char* file) {

	// Initially set all values in configuration array to VOID
	for (int i = 0; i < ALPHABET_SIZE; i++)
		config[i] = VOID;

	// Set initial starting array size
	config_size = 0;

	// Start reading file
	ifstream in(file);

	// Buffer for reading the file
	int integer;

	in >> integer;
	while (in) {
		
		// Check for range out of index
		if (integer < 0 || integer > 25) {
			cerr << "Invalid Index in plugboard file " << file << endl;
			exit(INVALID_INDEX);
		}

		config[config_size] = integer;
		config_size++;

		in >> integer;
	}

	// Check for odd number and out of bound parameters
	if (config_size % 2 == 1 || config_size > ALPHABET_SIZE) {
		cerr << "Incorrect number of parameters in plugboard file ";
		cerr << file << endl;
		exit(INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS);
	}

	// Check if configuration appears twice in plugboard
	for (int i = 0; i < config_size; i++) {
		for (int j = 0; j < config_size; j++) {
			if (j != i)
				if (config[i] == config[j]) {
					cerr << "Impossible plugboard configuration" << endl;
					exit(IMPOSSIBLE_PLUGBOARD_CONFIGURATION); 
				}
		}
	}

	in.close();
}

/* operation to encrypt plugboard */
void Plugboard::encrypt(int &buffer) {

	if (config_size == 0)
		return;

	if (buffer == config[config_size -1]) {
		buffer = config[config_size -2];
	} else {
		for (int i = 0; i < config_size - 1; i++) {
			if (buffer == config[i]) {
				if (i % 2 == 0) {
					buffer = config[i+1];
					break;
				} else {
					buffer = config[i-1];
					break;
				}
			}		      
		}
	}
}

/* END PLUGBOARD */

/* REFLECTOR */

/* Reflector class constructor */
Reflector::Reflector(char* file) {

	// Initially set all values in array to VOID
	for (int i = 0; i < ALPHABET_SIZE; i++)
		config[i] = VOID;

	config_size = 0;

	// Start reading file
	ifstream in(file);
	
	// buffer to read values
	int integer;

	// Read file and store into array
	in >> integer;
	while(in) {

		if (integer < 0 || integer > 25) {
			 cerr << "Invalid Index" << endl;
			 exit(INVALID_INDEX);
		}

		config[config_size] = integer;
		config_size++;

		in >> integer;
	}

	// Error if odd number of reflector configuration
	if (config_size % 2 == 1) {
		cerr << "Incorrect (odd) number of parameters in reflector file ";
		cerr << file << endl;
		exit(INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS);
	}

	// Error if number of configuration is not 26
	if (config_size != ALPHABET_SIZE) {
		cerr << "Insufficient number of mappings in reflector file: ";
		cerr << file << endl;
		exit(INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS);
	}

	// Check for duplicates within configuration
	for (int i = 0; i < config_size; i++) {
		for (int j = 0; j < config_size; j++) {
			if (j != i)
				if (config[i] == config[j]) {
					cerr << "Invalid reflector config." << endl;
					exit(INVALID_REFLECTOR_MAPPING); 
				}
			}
	}

	in.close(); 
}

/* Method to encrypt signal */
void Reflector::encrypt(int &buffer) {

	if (buffer == config[config_size -1]) {
		buffer = config[config_size - 2];
	} else {
		for (int i = 0; i < config_size - 1; i++) {
			if (buffer == config[i]) {
				if (i % 2 == 0) {
					buffer = config[i+1];
					break;
				} else {
					buffer = config[i-1];
					break;
				}
			}
		}
	}
}
/* END REFLECTOR */


/* ROTOR */

/* Constructor for Rotor class */
Rotor::Rotor (int rotor_number, int _start_pos, char* rot_file, char* pos_file) 
{
	if (_start_pos == VOID) {
		cerr << "No starting position for rotor " << rotor_number;
		cerr << " in rotor position file: " << pos_file << endl;
		exit(NO_ROTOR_STARTING_POSITION);
	}

	// Start reading
	ifstream in(rot_file);

	// Set all values in config array to VOID
	for (int i = 0; i < ALPHABET_SIZE * 2; i++)
		config[i] = VOID;

	int integer;
	config_size = 0;

	in >> integer;
	while(in) {
			
		// Checks index range
		if (integer < 0 || integer > 25) {
			cerr << "Invalid Index" << endl;
			exit(INVALID_INDEX);
		}

		config[config_size] = integer;
		config_size++;
		in >> integer;
	}

	// Check number of inputs
	if (config_size < ALPHABET_SIZE) {
		cerr << "Not all inputs mapped in rotor file: " << rot_file << endl;
		exit(INVALID_ROTOR_MAPPING);
	}

	// Check duplicates
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		for (int j = 0; j < ALPHABET_SIZE; j++) {
			if (i != j) {
				if (config[i] == config[j]) {
					cerr << "Invalid mapping of input " << j;
					cerr << " to output " << config[i];
					cerr << " (output " << config[i];
					cerr << " is already mapped to from input " << i;
					cerr << ") " << endl;
					exit(INVALID_ROTOR_MAPPING);
				}
			}
		}
	}
	in.close();


	// Set relative starting position
	int idx = 0;
	for (int i = _start_pos; i < ALPHABET_SIZE; i++) {
		rel_config[idx] = config[i];
		idx++;
	}

	for (int i = idx; i < ALPHABET_SIZE; i++)
		rel_config[i] = config[i - idx];

	// Decrement relative positions by starting position
	// to obtain actual encryption
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (rel_config[i] - _start_pos < 0)
			rel_config[i] = ALPHABET_SIZE + (rel_config[i] - _start_pos);
		else
			rel_config[i] -= _start_pos;
	}

	// Initialize number of notches
	num_notches = 0;

	// Read and set notches from given rotor file
	for (int i = ALPHABET_SIZE; i < ALPHABET_SIZE * 2; i++) {
		if (config[i] != VOID) {
			notches[i - ALPHABET_SIZE] = config[i];
			num_notches++;
		} else {
			break;
		}
	}

	// Similarly, turn all position of notches into indices
	for (int i = 0; i < num_notches; i++) {
		notches[i] = config[notches[i]];
		if (notches[i] - _start_pos < 0)
			notches[i] = ALPHABET_SIZE + (notches[i] - _start_pos);
		else
			notches[i] -= _start_pos;
	}

	// Initialize pointer to other rotor as NULL
	leftrotor = NULL;
}


/* Method to rotate the rotor by one step */
void Rotor::rotate() {

	// Rotate relative configuration by swapping elements within array
	int temp;
	for (int i = 0; i < 25; i++) {
		temp = rel_config[i];
		rel_config[i] = rel_config[i+1];
		rel_config[i+1] = temp;
	}

	// Decrement all elements in configuration by 1
	// to reflect actual change
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if ( rel_config[i] - 1 < 0)
			rel_config[i] = ALPHABET_SIZE + (rel_config[i] - 1);
		else
			rel_config[i] -= 1;
	}

	// Implement same idea with notches to track them
	for (int i = 0; i < num_notches; i++) {
		if ( notches[i] - 1 < 0)
			notches[i] = ALPHABET_SIZE + (notches[i] - 1);
		else
			notches[i] -= 1;
	}

	// Check if top position is a notch
	if (is_notch(rel_config[0])) {
		leftrotor->rotate();
	}
}

/* Method to check if current position is a notch */
bool Rotor::is_notch(int position) {
	
	// checks if left rotor exists
	if (!leftrotor)
		return false;
	
	// Is a notch when indices line up to the top position
	for (int i = 0; i < num_notches; i++) {
		if (position == notches[i])
			return true;
	}

	// fails test, so not a notch
	return false;
}

/* Method to encrypt incoming buffer */
void Rotor::encrypt_in(int &buffer) {
	// Encrypt from index to element value
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (buffer == i) {
			buffer = rel_config[i];
			break;
		}
	}
}

/* Method to encrypt outgoing buffer */
void Rotor::encrypt_out(int &buffer) {
	// Encrypt from element value back to index
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (buffer == rel_config[i]) {
			buffer = i;
			break;
		}
	}
}

/* Method to set left rotor */
void Rotor::set_left_rotor(Rotor *rotor_ptr) {
	leftrotor = rotor_ptr;
}
/* END ROTOR */

