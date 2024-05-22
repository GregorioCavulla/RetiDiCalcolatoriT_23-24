/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _RPC_XFILE_H_RPCGEN
#define _RPC_XFILE_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct Name {
	char name[256];
};
typedef struct Name Name;

struct Output {
	int numeroDirettori;
	Name files[10];
};
typedef struct Output Output;

#define FILEPROG 0x20000020
#define FILEVERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define ELIMINA_OCCORRENZE_NUMERICI 1
extern  int * elimina_occorrenze_numerici_1(Name *, CLIENT *);
extern  int * elimina_occorrenze_numerici_1_svc(Name *, struct svc_req *);
#define LISTA_SOTTODIRETTORI 2
extern  Output * lista_sottodirettori_1(Name *, CLIENT *);
extern  Output * lista_sottodirettori_1_svc(Name *, struct svc_req *);
extern int fileprog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define ELIMINA_OCCORRENZE_NUMERICI 1
extern  int * elimina_occorrenze_numerici_1();
extern  int * elimina_occorrenze_numerici_1_svc();
#define LISTA_SOTTODIRETTORI 2
extern  Output * lista_sottodirettori_1();
extern  Output * lista_sottodirettori_1_svc();
extern int fileprog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_Name (XDR *, Name*);
extern  bool_t xdr_Output (XDR *, Output*);

#else /* K&R C */
extern bool_t xdr_Name ();
extern bool_t xdr_Output ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_RPC_XFILE_H_RPCGEN */
