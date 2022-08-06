#ifndef _SERVER_C_
#define _SERVER_C_
#include "server.h"
#include <stdio.h>


ST_accountsDB_t AccountDB[255];

void readAccountDB(void) {
	FILE* ptr;
	ptr = fopen("./AccountsDB.txt", "r");
	if (!ptr) {
		printf("'AccountsDB.txt' File Corrupted/Not Found.\n");
		return;
	}
	uint8_t pan[20];
	float amount;
	int i = 0;
	while (fscanf(ptr, "%f %s", &amount, pan)) {
		if (i > 0 && AccountDB[i - 1].balance == amount && AccountDB[i - 1].primaryAccountNumber[6] == pan[6])break;
		AccountDB[i].balance = amount;
		for (int j = 0; j < 20; j++) {
			AccountDB[i].primaryAccountNumber[j] = pan[j];
		}
		i++;
	}
	fclose(ptr);
}

//This function will take all transaction dataand validate its data.
//It checks the account detailsand amount availability.
//If the account does not exist return DECLINED_STOLEN_CARD, if the amount is not available will return DECLINED_INSUFFECIENT_FUND, if a transaction can't be saved will return INTERNAL_SERVER_ERROR and will not save the transaction, else returns APPROVED.
//It will update the database with the new balance.
EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	return APPROVED;
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
	return OK_serverError;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
	return OK_serverError;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	return OK_serverError;
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData)
{
	return OK_serverError;
}
#endif