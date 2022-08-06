#ifndef _APP_C_
#define _APP_C_
#include "app.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <random>
#include <time.h>


void appStart(void)
{
	printf("-=Reading AccountsDB...\n");
	readAccountDB();
	printf("-=Succesfully loaded AccountsDB=-\n\n");
	ST_cardData_t cardData;
	ST_terminalData_t termData;
	ST_transaction_t transData;
	while (getCardHolderName(&cardData) == WRONG_NAME) {
		printf("\n\t--===WRONG_NAME===--\n");
		printf("Re-enter:\n");
	}
	while (getCardExpiryDate(&cardData) == WRONG_EXP_DATE) {
		printf("\n\t--===WRONG_EXP_DATE===--\n");
		printf("Re-enter:\n");
	}
	while (getCardPAN(&cardData) == WRONG_PAN) {
		printf("\n\t--===WRONG_PAN===--\n");
		printf("Re-enter:\n");
	}
	while (setMaxAmount(&termData) == INVALID_AMOUNT) {
		printf("\n\t--===INVALID AMOUNT===--\n");
		printf("Re-enter:\n");
	}
	while (getTransactionDate(&termData) == WRONG_DATE) {
		printf("\n\t--===WRONG_DATE===--\n");
		printf("Re-enter:\n");
	}
	if (isCardExpired(cardData, termData) == EXPIRED_CARD) {
		printf("\n\n\t--===EXPIRED_CARD===--\n");
		return;
	}
	while (getTransactionAmount(&termData) == INVALID_AMOUNT) {
		printf("\n\t--===INVALID_AMOUNT===--\n");
		printf("Re-enter:\n");
	}
	if (isBelowMaxAmount(&termData) == EXCEED_MAX_AMOUNT){
		printf("\n\t--===DECLINED EXCEED_MAX_AMOUNT===--\n");
		return;
	}
	transData.cardHolderData = cardData;
	transData.terminalData = termData;
	EN_transState_t transSt = recieveTransactionData(&transData);
	if (transSt == DECLINED_STOLEN_CARD) {
		printf("\n\t--===DECLINED STOLEN CARD/INVALID ACCOUNT===--\n");
	}
	else if (transSt== DECLINED_INSUFFECIENT_FUND) {
		printf("\n\t--===DECLINED_INSUFFECIENT_FUND===--\n");
	}
	else if (transSt == APPROVED) {
		printf("\n\t-=APPROVED\n");
		printf("\t-=UPDATING ACCOUNTS DB...\n");
		updateAccountDB();
		printf("\t-=SAVING TRANSACTION...\n");
	}
	
}

//Was used to generate random balance and PAN in AccountsDB.txt
//To run this function correctly you need to:
//-comment out srand line in GenerateLuhn() first
void fill() {
	FILE* ptr;
	ptr = fopen("AccountsDB.txt", "w");
	time_t t1;
	srand((unsigned)time(&t1));
	for (int i = 0; i < 255; i++) {
		float amount = (float)(rand() % (int)1e6) / (float)(1e3 / 100);
		fprintf(ptr, "%.0f %s\n", amount, GenerateLuhn());
	}
}
#endif
