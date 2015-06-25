/***********      .---.         .-"-.      *******************\
* -------- *     /   ._.       / � ` \     * ---------------- *
* Author's *     \_  (__\      \_�v�_/     * humus@rogers.com *
*   note   *     //   \\       //   \\     * ICQ #47010716    *
* -------- *    ((     ))     ((     ))    * ---------------- *
*          ****--""---""-------""---""--****                  ********\
* This file is a part of the work done by Humus. You are free to use  *
* the code in any way you like, modified, unmodified or copy'n'pasted *
* into your own work. However, I expect you to respect these points:  *
*  @ If you use this file and its contents unmodified, or use a major *
*    part of this file, please credit the author and leave this note. *
*  @ For use in anything commercial, please request my approval.      *
*  @ Share your work and ideas too as much as you can.                *
\*********************************************************************/

#include "Tokenizer.h"
#include <stdio.h>

bool isWhiteSpace(const char ch){
	return (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n');
}

bool isNumeric(const char ch){
	return (ch >= '0' && ch <= '9');
}

bool isAlphabetical(const char ch){
	return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_');
}

bool isNewLine(const char ch){
	return (ch == '\r' || ch == '\n');
}


Tokenizer::Tokenizer(unsigned int nBuffers){
	str = NULL;
	
	buffers.setCount(nBuffers);
	memset(buffers.getArray(), 0, nBuffers * sizeof(TokBuffer));

	reset();
}

Tokenizer::~Tokenizer(){
	for (unsigned int i = 0; i < buffers.getCount(); i++){
		if (buffers[i].buffer) delete buffers[i].buffer;
	}	
	if (str != NULL) delete str;
}

void Tokenizer::setString(const char *string){
	if (str != NULL) delete str;

	length = strlen(string);

	str = new char[(length + 1) * sizeof(char)];
	strcpy(str, string);

	reset();
}

bool Tokenizer::setFile(const char *fileName){
	FILE *file;

	if (str != NULL) delete str;

	if ((file = fopen(fileName, "rb")) != NULL){
		fseek(file, 0, SEEK_END);
		length = ftell(file);
		fseek(file, 0, SEEK_SET);

		str = new char[(length + 1) * sizeof(char)];
		fread(str, length, 1, file);
		str[length] = '\0';

		fclose(file);
		
		reset();
		return true;
	}
	return false;
}

void Tokenizer::reset(){
	end = 0;
}

bool Tokenizer::goToNext(BOOLFUNC isAlpha){
	start = end;

	while (start < length && isWhiteSpace(str[start])) start++;
	end = start + 1;
	
	if (start < length){
		if (isNumeric(str[start])){
			while (isNumeric(str[end]) || str[end] == '.') end++;
		} else if (isAlpha(str[start])){
			while (isAlpha(str[end]) || isNumeric(str[end])) end++;
		}
		return true;
	}
	return false;
}

bool Tokenizer::goToNextLine(){
	if (end < length){
		start = end;

		while (end < length && !isNewLine(str[end])) end++;

		if (isNewLine(str[end + 1]) && str[end] != str[end + 1]) end += 2; else end++;
		return true;
	}
	return false;
}


char *Tokenizer::next(BOOLFUNC isAlpha){
	if (goToNext(isAlpha)){
		unsigned int size = end - start;
		char *buffer = getBuffer(size + 1);
		strncpy(buffer, str + start, size);
		buffer[size] = '\0';
		return buffer;
	}
	return NULL;
}

char *Tokenizer::nextAfterToken(const char *token, BOOLFUNC isAlpha){
	/*while ((str = next()) != NULL){
		if (stricmp(str, token) == 0){
			return next();
		}
	}*/

	while (goToNext(isAlpha)){
		if (strncmp(str + start, token, end - start) == 0){
			return next();
		}
	}

	return NULL;
}

char *Tokenizer::nextLine(){
	if (goToNextLine()){
		unsigned int size = end - start;
		char *buffer = getBuffer(size + 1);
		strncpy(buffer, str + start, size);
		buffer[size] = '\0';
		return buffer;
	}
	return NULL;
}
/*
char *Tokenizer::getAllRight(){
	if (start < length){
		strcpy(buffer, str + start);
		return true;
	}
	return false;
}

*/

char *Tokenizer::getBuffer(unsigned int size){
	currentBuffer++;
	if (currentBuffer >= buffers.getCount()) currentBuffer = 0;

	if (size > buffers[currentBuffer].bufferSize){
		delete buffers[currentBuffer].buffer;
		buffers[currentBuffer].buffer = new char[buffers[currentBuffer].bufferSize = size];
	}

	return buffers[currentBuffer].buffer;
}