/* RPC_Server.c */

#include "RPC_xFile.h"
#include <dirent.h>
#include <fcntl.h>
#include <rpc/rpc.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// DEFINIZIONE PRIMA OPERAZIONE

<T> *operazione_1_svc(<parametro>, struct svc_req *rp)
{
  static<T> result = -1; // default in caso di errore
  // codice operazione

  return (&result);
}

// DEFINIZIONE SECONDA OPERAZIONE

<T> *operazione_2_svc(<parametro>, struct svc_req *rp)
{
  static<T> result;
  // codice operazione

  return (&result);
}