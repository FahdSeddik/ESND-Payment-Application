#include "server.h"

EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	return EN_transState_t();
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
	return EN_serverError_t();
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
	return EN_serverError_t();
}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	return EN_serverError_t();
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData)
{
	return EN_serverError_t();
}
