#ifndef _DSKY_H
#define _DSKY_H

// Write DSKY standard layout.
void dskySetup();

// Get noun, verb, parm from DSKY.
// noun:
// verb:
// parm:
// allowNV: 0 ignor nouns and verbs, 1 allow nouns, verbs and params.
int16_t dskyGetNounVerbParm(int16_t *noun, int16_t *verb, int16_t *parm, int16_t allowNV);

// Process input from DSKY.
void dskyProcess();
  
#endif // _DSKY_H
