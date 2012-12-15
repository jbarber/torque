/*
 * tmsock_recov.c - This file contains the functions to record a job's
 * demux socket numbers, nodeid, and last task id
 */

#include <pbs_config.h>   /* the master config generated by configure */

#include <sys/param.h>
#include "pbs_ifl.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "server_limits.h"
#include "list_link.h"
#include "attribute.h"
#include "pbs_job.h"
#include "log.h"
#include "../lib/Liblog/pbs_log.h"
#include "../lib/Liblog/log_event.h"
#include "../lib/Libifl/lib_ifl.h"
#include "svrfunc.h"


u_long addclient(char *name);


/*
 * save_tmsock() - Saves the tm sockets of a job to disk
 *
 */

int save_tmsock(

  job    *pjob,          /* pointer to job structure */
  int     fds,           /* I */
  char   *buffer,        /* O */
  size_t *buf_remaining, /* M */
  size_t  buf_size)      /* I */

  {
  if ((pjob->ji_stdout > 0) && (pjob->ji_stdout < 1024))
    {
    /* We don't have real port numbers (yet), so don't bother */

    return(PBSE_NONE);
    }

  DBPRT(("saving extra job info stdout=%d stderr=%d taskid=%u nodeid=%u\n",
    pjob->ji_stdout,
    pjob->ji_stderr,
    pjob->ji_taskid,
    pjob->ji_nodeid));

  /* FIXME: need error checking here */

  save_struct((char *)&pjob->ji_stdout, sizeof(int), fds, buffer, buf_remaining, buf_size);
  save_struct((char *)&pjob->ji_stderr, sizeof(int), fds, buffer, buf_remaining, buf_size);
  save_struct((char *)&pjob->ji_taskid, sizeof(tm_task_id), fds, buffer, buf_remaining, buf_size);
  save_struct((char *)&pjob->ji_nodeid, sizeof(tm_node_id), fds, buffer, buf_remaining, buf_size);

  return(PBSE_NONE);
  }  /* END save_tmsock() */




/*
 * recov_tmsock() - Recovers the tm sockets of a job
 *
 */

int recov_tmsock(

  int fds,
  job *pjob) /* I */   /* pathname to job save file */

  {
  const char *id = "recov_tmsock";

  static int sizeofint = sizeof(int);

  if (read_ac_socket(fds, (char *)&pjob->ji_stdout, sizeofint) != sizeofint)
    {
    log_err(errno, id, (char *)"read");

    return(1);
    }

  if (read_ac_socket(fds, (char *)&pjob->ji_stderr, sizeofint) != sizeofint)
    {
    log_err(errno, id, (char *)"read");

    return(1);
    }

  if (read_ac_socket(fds, (char *)&pjob->ji_taskid, sizeof(tm_task_id)) != sizeof(tm_task_id))
    {
    log_err(errno, id, (char *)"read");

    return(1);
    }

  if (read_ac_socket(fds, (char *)&pjob->ji_nodeid, sizeof(tm_node_id)) != sizeof(tm_node_id))
    {
    log_err(errno, id, (char *)"read");

    return(1);
    }

  DBPRT(("recovered extra job info stdout=%d stderr=%d taskid=%u nodeid=%u\n",

         pjob->ji_stdout,
         pjob->ji_stderr,
         pjob->ji_taskid,
         pjob->ji_nodeid));

  return(0);
  }  /* END recov_tmsock() */

