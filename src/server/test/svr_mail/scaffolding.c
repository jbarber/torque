#include "license_pbs.h" /* See here for the software license */
#include <stdlib.h>
#include <stdio.h> /* fprintf */

#include "server.h" /* server mail_info*/

struct server server;
int LOGLEVEL = 0;

int enqueue_threadpool_request(void *(*func)(void *),void *arg)
  {
  fprintf(stderr, "The call to enqueue_threadpool_request to be mocked!!\n");
  exit(1);
  }

void svr_format_job(FILE *fh, mail_info *mi, const char *fmt)
  {
  fprintf(stderr, "The call to svr_format_job to be mocked!!\n");
  exit(1);
  }

int get_svr_attr_l(int index, long *l)
  {
  return(0);
  }
  
int get_svr_attr_str(int index, char **str)
  {
  return(0);
  }
