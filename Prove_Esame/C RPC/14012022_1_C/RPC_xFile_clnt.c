/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "RPC_xFile.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

int *
elimina_occorrenze_numeriche_1(Name *argp, CLIENT *clnt)
{
	static int clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, ELIMINA_OCCORRENZE_NUMERICHE,
		(xdrproc_t) xdr_Name, (caddr_t) argp,
		(xdrproc_t) xdr_int, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

Output *
lista_file_direttorio_1(Name *argp, CLIENT *clnt)
{
	static Output clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, LISTA_FILE_DIRETTORIO,
		(xdrproc_t) xdr_Name, (caddr_t) argp,
		(xdrproc_t) xdr_Output, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}
