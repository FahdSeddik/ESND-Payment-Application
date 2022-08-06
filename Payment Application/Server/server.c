#ifndef _SERVER_C_
#define _SERVER_C_
#include "server.h"
#include <stdio.h>
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