#include "terminal.h"
#include <time.h>

//will ask for the transaction data and store it in terminal data.
//Transaction date is 10 characters string in the format DD / MM / YYYY, e.g 25 / 06 / 2022.
//If the transaction date is NULL, less than 10 characters or wrong format will return WRONG_DATE error, else return OK.
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	char ans;
	printf("Do you want to enter transaction date \nor retrieve from system current date? [enter: e/system: s]: ");
	scanf("%c", ans);
	if (ans == 's' || ans == 'S') {
		printf("Retreiving transaction date from system...\n");
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		int year = tm.tm_year + 1900;
		int month = tm.tm_mon + 1;
		int day = tm.tm_mday;
		termData->transactionDate[0] = day / 10 + '0';
		termData->transactionDate[1] = day % 10 + '0';
		termData->transactionDate[2] = '/';
		termData->transactionDate[3] = month / 10 + '0';
		termData->transactionDate[4] = month % 10 + '0';
		termData->transactionDate[5] = '/';
		termData->transactionDate[6] = '0' + year / 1000;
		termData->transactionDate[7] = '0' + (year % 1000) / 100;
		termData->transactionDate[8] = '0' + (year % 100) / 10;
		termData->transactionDate[9] = '0' + year % 10;
		termData->transactionDate[10] = '\0';
		printf("Retrieved system date : %s\n", termData->transactionDate);
		return OK;
	}
	else {
		char d[12];
		printf("Enter transaction date [DD/MM/YYYY]: ");
		scanf("%s", d);
		if (!d)return WRONG_DATE;
		if (d[11] != '\0' || d[9] == '\0' || d[2] != '/' || d[5] != '/')return WRONG_DATE;
		for (int i = 0; i < 11; i++) {
			if (i == 2 || i == 5)if (d[i] != '/')return WRONG_DATE;
			if (d[i] - '0' < 0 || d[i] - '0' > 9)return WRONG_DATE;
		}
		int days = (d[0] - '0') * 10 + d[1]-'0';
		if (days < 0 || days>31)return WRONG_DATE;
		termData->transactionDate[0] = days / 10 + '0';
		termData->transactionDate[1] = days % 10 + '0';
		int months = (d[3] - '0') * 10 + d[4] - '0';
		if (months < 1 || months>12)return WRONG_DATE;
		termData->transactionDate[3] = months / 10 + '0';
		termData->transactionDate[4] = months % 10 + '0';
		
		int year = (d[6] - '0') * 1000 + (d[7] - '0') * 100 + (d[8] - '0') * 10 + d[9] - '0';
		if (year < 0)return WRONG_DATE;
		termData->transactionDate[6] = d[6];
		termData->transactionDate[7] = d[7];
		termData->transactionDate[8] = d[8];
		termData->transactionDate[9] = d[9];
		termData->transactionDate[10] = '\0';
		printf("Date read: %s\n", termData->transactionDate);
		return OK;
	}
}
//This function compares the card expiry date with the transaction date.
//If the card expiration date is before the transaction date will return EXPIRED_CARD, else return OK.
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
	for (int i = 0, j = 3; i < 6; i++, j++) {
		//if any is smaller then expiration date passed
		if (cardData.cardExpirationDate[i] < termData.transactionDate[j])return EXPIRED_CARD;
	}
	return OK;
}


//This function checks if the PAN is a Luhn number or not.
//If PAN is not a Luhn number will return INVALID_CARD, else return OK.
EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
	char cardPan[20];
	uint8_t size = 20;
	//copy cardpan into cardPan
	for (int i = 0; i < size; i++) {
		cardPan[i] = cardData->primaryAccountNumber[i];
	}
	int checkdigit;
	int startindex;
	//get checkdigit and starindex
	for (int i = size - 1; i >= 0; i--)if (cardPan[i] != '\0') {
		checkdigit = cardPan[i] - '0';
		startindex = i - 1;
		break;
	}
	//replace with new digits
	for (int i = startindex; i >= 0; i -= 2) {
		if (cardPan[i] == '\0')continue;
		int dig = cardPan[i] - '0';
		dig *= 2;
		if (dig < 10)cardPan[i] = '0' + dig;
		else cardPan[i] = '0' + dig - 9;
	}
	uint8_t sum = 0;
	//calculate digit sum
	for (int i = 0; i < size; i++) {
		if (cardPan[i] == '\0')continue;
		sum += cardPan[i] - '0';
	}
	//check if valid
	if (sum % 10 == checkdigit)return OK;
	else return INVALID_CARD;
}


//This function asks for the transaction amountand saves it into terminal data.
//If the transaction amount is less than or equal to 0 will return INVALID_AMOUNT, else return OK.
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{

	return EN_terminalError_t();
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	return EN_terminalError_t();
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
	return EN_terminalError_t();
}

