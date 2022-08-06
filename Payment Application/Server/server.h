#ifndef _SERVER_H_
#define _SERVER_H_


#include "../Terminal/terminal.h"

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef struct ST_accountsDB_t
{
	float balance;
	uint8_t primaryAccountNumber[20];
}ST_accountsDB_t;

typedef enum EN_serverError_t
{
	OK_serverError, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND, LOW_BALANCE
}EN_serverError_t;

typedef enum EN_transState_t
{
	APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, INTERNAL_SERVER_ERROR
}EN_transState_t;

typedef struct ST_transaction_t
{
	ST_cardData_t cardHolderData;
	ST_terminalData_t terminalData;
	EN_transState_t transState;
	uint32_t transactionSequenceNumber;
}ST_transaction_t;

EN_transState_t recieveTransactionData(ST_transaction_t*);
EN_serverError_t isValidAccount(ST_cardData_t*);
EN_serverError_t isAmountAvailable(ST_terminalData_t*);
EN_serverError_t saveTransaction(ST_transaction_t*);
EN_serverError_t getTransaction(uint32_t, ST_transaction_t*);


#endif
