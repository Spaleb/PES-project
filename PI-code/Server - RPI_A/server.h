#ifndef SERVER_H
#define SERVER_H

#include <string>

void initWemos();
void startPi();
void handleWemos();

void sendToPi(const std::string& msg);

extern int sock;

#endif
