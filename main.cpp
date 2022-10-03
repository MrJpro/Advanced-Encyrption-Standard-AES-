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




	
	aes.SetKey(); // works properly
	
    //aes.encrypt();
	//aes.encrypt2();
	
	aes.decrypt();
	//aes.decrypt2();

	//aes.DisplayOutput();


	
	//example string of 16-byte hexadecmial string
   // 2b 7e 15 16 28 ae d2 a6 ab f7 15 88 09 cf 4f 3c



	return 0;
}