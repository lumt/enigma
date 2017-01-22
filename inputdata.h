#ifndef INPUTDATA_H
#define INPUTDATA_H

/* function to check plugboard.pb, reflector.rf and rotor.rot file */
void check_file(char* file);

/* function to check rotor rotor.pos file */
void check_pos_file(int *rotor_pos, int rotor_count, char *file);

#endif
