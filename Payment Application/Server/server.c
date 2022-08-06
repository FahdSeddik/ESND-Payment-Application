#ifndef _SERVER_C_
#define _SERVER_C_
#include "server.h"
#include <stdio.h>

int index = -1;
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

void updateAccountDB(void)
{
	FILE* ptr;
	ptr = fopen("./AccountsDB.txt", "w");
	if (!ptr){
		printf("'AccountsDB.txt' File Corrupted/Not Found.\n");
		return;
	}
	for (int i = 0; i < 255; i++) {
		if (AccountDB[i].primaryAccountNumber[0] == '\0')break;
		fprintf(ptr, "%f %s\n", AccountDB[i].balance, AccountDB[i].primaryAccountNumber);
	}
}

//This function will take all transaction dataand validate its data.
//It checks the account detailsand amount availability.
//If the account does not exist return DECLINED_STOLEN_CARD, if the amount is not available will return DECLINED_INSUFFECIENT_FUND, if a transaction can't be saved will return INTERNAL_SERVER_ERROR and will not save the transaction, else returns APPROVED.
//It will update the database with the new balance.
EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	if (isValidAccount(&transData->cardHolderData)==ACCOUNT_NOT_FOUND) {
		transData->transState = DECLINED_STOLEN_CARD;
		if (saveTransaction(transData) == SAVING_FAILED)return INTERNAL_SERVER_ERROR;
		return DECLINED_STOLEN_CARD;
	}
	if (isAmountAvailable(&transData->terminalData)==LOW_BALANCE) {
		transData->transState = DECLINED_INSUFFECIENT_FUND;
		if(saveTransaction(transData) == SAVING_FAILED)return INTERNAL_SERVER_ERROR;
		return DECLINED_INSUFFECIENT_FUND;
	}
	transData->transState = APPROVED;
	AccountDB[index].balance -= transData->terminalData.transAmount;
	if(saveTransaction(transData)==SAVING_FAILED)return INTERNAL_SERVER_ERROR;
	return APPROVED;
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
	uint8_t* PAN =cardData->primaryAccountNumber;
	//linear search through AccountsDB
	bool found = false;
	
	for (int i = 0; i < 255; i++) {
		//if not empty(no more records)
		if (AccountDB[i].primaryAccountNumber[0] != '\0') {
			bool equal = true;
			for (int j = 0; j < 20; j++) {
				if (AccountDB[i].primaryAccountNumber[j] != PAN[j]) {
					equal = false;
					break;
				}
			}
			if (equal) {
				found = true;
				index = i;
				break;
			}
		}
		else break;
	}
	if (!found)return ACCOUNT_NOT_FOUND;
	return OK_serverError;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
	if (AccountDB[index].balance > termData->transAmount)return LOW_BALANCE;
	return OK_serverError;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	FILE* ptr;
	ptr = fopen("./INFO.txt", "r");
	if (!ptr)return SAVING_FAILED;
	int transnum;
	fscanf(ptr, "%d", &transnum);
	fclose(ptr);
	ptr = fopen("./INFO.txt", "w");
	fprintf(ptr, "%d", transnum+1);
	fclose(ptr);
	transData->transactionSequenceNumber = transnum+1;
	ptr = fopen("./TransactionsDB.txt", "a");
	if (!ptr) return SAVING_FAILED;
	//-=Transaction=-
	//	-Card Holder Name:
	//	-PAN:
	//	-Expiry Date:
	//	-Amount:
	//	-Max Amount:
	//	-Transaction State:
	//	-Transaction Sequence Number:
	fprintf(ptr, "-=Transaction=-\n");
	fprintf(ptr, "\tCard Holder Name: %s\n", transData->cardHolderData.cardHolderName);
	fprintf(ptr, "\t-PAN: %s\n", transData->cardHolderData.primaryAccountNumber);
	fprintf(ptr, "\t-Expiry Date: %s\n", transData->cardHolderData.cardExpirationDate);
	fprintf(ptr, "\t-Amount: %f\n", transData->terminalData.transAmount);
	fprintf(ptr, "\t-Max Amount: %f\n", transData->terminalData.maxTransAmount);
	fprintf(ptr, "\t-Transaction State: ");
	if (transData->transState == APPROVED)fprintf(ptr, "APPROVED\n");
	else if (transData->transState == DECLINED_INSUFFECIENT_FUND)fprintf(ptr, "DECLINED_INSUFFECIENT_FUND\n");
	else if (transData->transState == DECLINED_STOLEN_CARD)fprintf(ptr, "DECLINED_STOLEN_CARD\n");
	fprintf(ptr, "\t-Transaction Sequence Number: %d\n\n", transData->transactionSequenceNumber);
	fclose(ptr);
	return OK_serverError;
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData)
{
	return OK_serverError;
}
#endif