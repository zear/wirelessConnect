/*
 * callbacks.h
 *
 *  Created on: Dec 3, 2012
 *      Author: johnny
 */

#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#include "menu.h"

void actionStub(MenuItem *this);

void actOptions();
void actConnect();
void actQuit();

void actProfileBack();
void actProfile();
void actProfiles();

void actOptionsMode();
void actOptionsESSID();
void actOptionsEnc();
void actOptionsPassword();
void actOptionsDHCP();
void actOptionsIP();
void actOptionsNETMASK();
void actOptionsBack();

void actProfiles();
void actProfileSave();
void actProfileLoad(MenuItem *this);

#endif /* CALLBACKS_H_ */
