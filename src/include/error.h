#ifndef ERROR_H
#define ERROR_H

#include "util.h"

extern int tokPos;

void newline();
void error(int, string, ...);
void impossible(string, ...);
void reset(string filename);
int anyErrors();

#endif