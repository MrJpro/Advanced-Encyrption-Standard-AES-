/*
Josue E. Sandoval
3/29/2022
AES attempt 2
*/


#include"AES2.h"
#include <iostream>
#include<string>



using namespace std;

int main() {


	//AES2 aes("DataFile.txt");
	//AES2 aes("DataFile2.txt");
	AES2 aes("encryptedText.txt");


	//adds initial input
	/*
	aes.InputInsert(0x32);
	aes.InputInsert(0x43);
	aes.InputInsert(0xf6);
	aes.InputInsert(0xa8);
	aes.InputInsert(0x88);
	aes.InputInsert(0x5a);
	aes.InputInsert(0x30);
	aes.InputInsert(0x8d);
	aes.InputInsert(0x31);
	aes.InputInsert(0x31);
	aes.InputInsert(0x98);
	aes.InputInsert(0xa2);
	aes.InputInsert(0xe0);
	aes.InputInsert(0x37);
	aes.InputInsert(0x07);
	aes.InputInsert(0x34);
	*/

	//adds the output
	/*
	aes.InputInsert(0x39);
	aes.InputInsert(0x25);
	aes.InputInsert(0x84);
	aes.InputInsert(0x1d);
	aes.InputInsert(0x02);
	aes.InputInsert(0xdc);
	aes.InputInsert(0x09);
	aes.InputInsert(0xfb);
	aes.InputInsert(0xdc);
	aes.InputInsert(0x11);
	aes.InputInsert(0x85);
	aes.InputInsert(0x97);
	aes.InputInsert(0x19);
	aes.InputInsert(0x6a);
	aes.InputInsert(0x0b);
	aes.InputInsert(0x32);
	*/




	//adds the key
	aes.InputKey(0x2b);
	aes.InputKey(0x7e);
	aes.InputKey(0x15);
	aes.InputKey(0x16);
	aes.InputKey(0x28);
	aes.InputKey(0xae);
	aes.InputKey(0xd2);
	aes.InputKey(0xa6);
	aes.InputKey(0xab);
	aes.InputKey(0xf7);
	aes.InputKey(0x15);
	aes.InputKey(0x88);
	aes.InputKey(0x09);
	aes.InputKey(0xcf);
	aes.InputKey(0x4f);
	aes.InputKey(0x3c);
	
	aes.ReadFile();
	//aes.encrypt();
	//aes.GenerateInputBlocks();
	//aes.encrypt2();
	//aes.DisplayInputBlocks();
	aes.decrypt();
	//aes.decrypt2();

	//
	//aes.encrypt();
	
	//aes.DisplayOutput();


	
	



	return 0;
}