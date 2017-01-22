#ifndef ENIGMAPARTS_H
#define ENIGMAPARTS_H

/* Define constants used */
#define VOID            -1  // Constant to represent meaningless value
#define ALPHABET_SIZE   26  // Constant to represent size of alphabet


/* PLUGBOARD */
class Plugboard {
  int config[ALPHABET_SIZE];
  int config_size;
  
public:
  Plugboard (char* file);

  // Method to encrypt incoming signal
  void encrypt(int &buffer);
};
/* END PLUGBOARD */


/* REFLECTOR */
class Reflector {
  int config[ALPHABET_SIZE*2];
  int config_size;

public:
  Reflector (char* file);

  // Method to encrypt incoming signal
  void encrypt(int &buffer);
};
/* END REFLECTOR */

/* ROTOR */
class Rotor {
  int config[ALPHABET_SIZE*2];
  int config_size;

  int rel_config[ALPHABET_SIZE];

  int notches[ALPHABET_SIZE];
  int num_notches;
  
  Rotor *leftrotor;

  // Method to check if rotated, if notch is in top position
  bool is_notch(int position);

 public:
  // Constructor
  Rotor(int rotor_number, int _start_pos, char* rot_file, char* pos_file);
  void print_config();

  // Method to rotate the rotor and change internal configuration
  void rotate();

  // Method to encrypt incoming and output signal
  void encrypt_in(int &buffer);
  void encrypt_out(int &buffer);

  // Method to set left rotor
  void set_left_rotor(Rotor *rotor_ptr);
};
/* END ROTOR */

#endif
