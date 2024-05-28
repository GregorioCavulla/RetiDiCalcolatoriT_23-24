/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "RPC_xFile.h"

bool_t
xdr_Prenotazione (XDR *xdrs, Prenotazione *objp)
{
	register int32_t *buf;

	int i;
	 if (!xdr_vector (xdrs, (char *)objp->targa, 8,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	 if (!xdr_vector (xdrs, (char *)objp->patente, 6,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	 if (!xdr_vector (xdrs, (char *)objp->tipo, 7,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	 if (!xdr_vector (xdrs, (char *)objp->folder_img, 13,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Tipo (XDR *xdrs, Tipo *objp)
{
	register int32_t *buf;

	int i;
	 if (!xdr_vector (xdrs, (char *)objp->tipo, 7,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Input (XDR *xdrs, Input *objp)
{
	register int32_t *buf;

	int i;
	 if (!xdr_vector (xdrs, (char *)objp->targa, 8,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	 if (!xdr_vector (xdrs, (char *)objp->nuova_patente, 6,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Output (XDR *xdrs, Output *objp)
{
	register int32_t *buf;

	int i;
	 if (!xdr_int (xdrs, &objp->check))
		 return FALSE;
	 if (!xdr_vector (xdrs, (char *)objp->prenotazioni, MAX_RIS,
		sizeof (Prenotazione), (xdrproc_t) xdr_Prenotazione))
		 return FALSE;
	return TRUE;
}
