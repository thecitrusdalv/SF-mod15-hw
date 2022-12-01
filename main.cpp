#include <iostream>
#include "chat.h"

int main ()
{
	Chat chat;
	for (int i = 0; i < 400; i++) {
		char temp[LOGINLENGTH];
		for (int j = 0; j < LOGINLENGTH; j++) {
			temp[j] = char(rand() % 26 + 'a');
		}
		chat.reg(temp, (char*)"1234", 4);
	}
	chat.show();
	return 0;
}
