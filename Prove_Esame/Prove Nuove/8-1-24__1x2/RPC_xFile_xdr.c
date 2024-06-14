/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "RPC_xFile.h"

bool_t
xdr_Name (XDR *xdrs, Name *objp)
{
	register int32_t *buf;

	int i;
	 if (!xdr_vector (xdrs, (char *)objp->name, 256,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Input (XDR *xdrs, Input *objp)
{
	register int32_t *buf;

	 if (!xdr_Name (xdrs, &objp->dirName))
		 return FALSE;
	 if (!xdr_char (xdrs, &objp->carattere))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->occorrenze))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Output (XDR *xdrs, Output *objp)
{
	register int32_t *buf;

	int i;
	 if (!xdr_vector (xdrs, (char *)objp->files, 7,
		sizeof (Name), (xdrproc_t) xdr_Name))
		 return FALSE;
	return TRUE;
}
