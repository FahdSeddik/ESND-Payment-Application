#ifndef _CARD_C_
#define _CARD_C_
#include "card.h"
#include <random>
#include <time.h>
#include <stdio.h>
int getNeededLuhnDigit(uint8_t* cardPan) {
	int startindex=17;
	int sum = 0;
	char cp[21];
	for (int i = 0; i <= startindex; i++) {
		cp[i] = cardPan[i];
	}
	cp[startindex + 1] = '\0';
	for (int i = startindex; i >= 0; i -= 2) {
		int digit = (cp[i] - '0') * 2;
		if (digit < 10)cp[i] = (char)(digit + '0');
		else cp[i] = (char)(digit - 9 + '0');
	}
	for (int i = 0; i <= startindex; i++) {
		sum += cp[i] - '0';
	}
	return sum % 10;
}

uint8_t* GenerateLuhn() {
	time_t t1;
	srand((unsigned)time(&t1));
	uint8_t* cardpan= new uint8_t[21];
	cardpan[0] = (char)('0' + rand() % 9 + 1);
	for (int i = 1; i <= 17; i++) {
		cardpan[i] = (char)('0' + rand() % 10);
	}
	cardpan[18] = '\0';
	int valid = getNeededLuhnDigit(cardpan);
	cardpan[18] = (char)('0' + valid);
	cardpan[19] = '\0';
	return cardpan;
}

//Ask for cardholder's name and store it into card data
//Card holder name is 24 characters string max and 20 min
//if cardholder name is null, less than 20 cahracters or more than 24 will return
// WRONG_NAME error, else return OK
EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	
	char name[100];
	for (int i = 0; i < 26; i++)name[i] = '\0';
	printf("Enter card holder name [20-24 characters]: ");
	scanf("%s", name);
	if (!name)return WRONG_NAME;
	if (name[18] == '\0' || name[24] != '\0')return WRONG_NAME;
	for(int i=0;i<25;i++){
		cardData->cardHolderName[i] = name[i];
	}
	printf("Read name: %s\n", cardData->cardHolderName);
	return (EN_cardError_t)OK_cardError;
}
//will ask for the card expiry dateand store it in card data.
//Card expiry date is 5 characters string in the format "MM/YY", e.g "05/25".
//If the card expiry date is NULL, less or more than 5 characters, or has the wrong format will return WRONG_EXP_DATE error, else return OK.
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	char cardexp[100];
	for (int i = 0; i < 7; i++)cardexp[i] = '\0';
	uint32_t month, year;
	printf("Enter expiry date [MM/YY]: ");
	scanf("%s", cardexp);
	if (!cardexp)return WRONG_EXP_DATE;
	if (cardexp[6] != '\0' || cardexp[3]=='\0')return WRONG_EXP_DATE;
	month = (cardexp[0] - '0') * 10 + (cardexp[1] - '0');
	year = (cardexp[3] - '0') * 10 + (cardexp[4] - '0');
	if (month < 1 || month>12 || cardexp[2]!='/' || (year > 99 || year < 0))return WRONG_EXP_DATE;
	for (int i = 0; i < 6; i++) {
		cardData->cardExpirationDate[i] = cardexp[i];
	}
	printf("Read expiry date: %s\n", cardData->cardExpirationDate);
	return (EN_cardError_t)OK_cardError;
}
//This function will ask for the card's Primary Account Number and store it in card data.
//PAN is 20 characters alphanumeric only string 19 character max, and 16 character min.
//If the PAN is NULL, less than 16 or more than 19 characters, will return WRONG_PAN error, else return OK.
EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	char ans;
	printf("Do you want to generate Luhn number? [y/n]");
	scanf("%c", &ans);
	if (ans == 'y' || ans == 'Y') {
		uint8_t *cardpan = GenerateLuhn();
		printf("Generated card number: %s\n", cardpan);

	}
	else {
		char cardpan[21];
		printf("Enter PAN [16-19 characters]:");
		scanf("%s", cardpan);
		if (!cardpan)return WRONG_PAN;
		if (cardpan[19] != '\0' || cardpan[14] == '\0')return WRONG_PAN;
		for (int i = 0; i < 20; i++) {
			cardData->primaryAccountNumber[i] = cardpan[i];
		}
		printf("Read PAN: %s\n", cardData->primaryAccountNumber);
	}
	return OK_cardError;
}
#endif