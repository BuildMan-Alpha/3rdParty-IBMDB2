/*
 * Start of xa.h header
 *
 * Define a symbol to prevent multiple inclusions of this header file
 */

#ifndef XA_H
#define XA_H

#include "sqlsystm.h"

/*
 * Transaction branch identification: XID and NULLXID:
 */
#define XIDDATASIZE   128             /* size in bytes */
#define MAXGTRIDSIZE  64              /* maximum size in bytes of gtrid */
#define MAXBQUALSIZE  64              /* maximum size in bytes of bqual */

// Internal XID version used by DB2
struct xid_t {
  sqlint32 formatID;                  /* format identifier */
  sqlint32 gtrid_length;              /* value from 1 through 64 */
  sqlint32 bqual_length;              /* value from 1 through 64 */
  char data[XIDDATASIZE];
  };
typedef struct xid_t XID;

//
// Standard XID ... as specified by the XA Open Group
//
struct xid_t_std {
  long     formatID;                  /* format identifier */
  long     gtrid_length;              /* value from 1 through 64 */
  long     bqual_length;              /* value from 1 through 64 */
  char data[XIDDATASIZE];
  };
typedef struct xid_t_std XID_STD;

/*
 * A value of -1 in formatID means that the XID is null.
*/
extern XID NULLXID;                   /* null transaction branch value */
/*
 * Declarations of routines by which RMs call TMs:
 */
#if (defined(__STDC__) || defined(__cplusplus) || defined(DB2NT))
#ifdef __cplusplus
extern "C" {
#endif
 #ifndef DB2NT
  extern int SQL_API_FN ax_reg(int, XID *, sqlint32);
  extern int SQL_API_FN ax_unreg(int, sqlint32);
 #else           /* cdecl linkage convention on NT */
  extern int __cdecl ax_reg(int, XID *, sqlint32);
  extern int __cdecl ax_unreg(int, sqlint32);
 #endif
#ifdef __cplusplus
}
#endif
#else /* ifndef __STDC__ */
  extern int ax_reg();
  extern int ax_unreg();
#endif /* ifndef __STDC__ */
/*
 * XA Switch Data Structure
 */
#define RMNAMESZ 32                   /* length of resource manager name,
                                    including the null terminator */
#define MAXINFOSIZE 256               /* maximum size in bytes of xa_info strings */

//
// Following is the 32bit version of the xa_switch_t.
//
struct xa_switch_t {
  char name[RMNAMESZ];                /* name of resource manager */
  sqlint32 flags;                     /* resource manager specific options */
  sqlint32 version;                   /* must be 0 */
#if (defined(__STDC__) || defined(__cplusplus) || defined (DB2NT)) /*@d34606pab*/
 #ifndef DB2NT
  int (PSQL_API_FN xa_open_entry) (char *, int, sqlint32);/* xa_open function pointer */
  int (PSQL_API_FN xa_close_entry) (char *, int, sqlint32);/* xa_close function pointer*/
  int (PSQL_API_FN xa_start_entry) (XID *, int, sqlint32);/* xa_start function pointer */
  int (PSQL_API_FN xa_end_entry) (XID *, int, sqlint32);/* xa_end function pointer */
  int (PSQL_API_FN xa_rollback_entry) (XID *, int, sqlint32);/* xa_rollback function pointer */
  int (PSQL_API_FN xa_prepare_entry) (XID *, int, sqlint32);/* xa_prepare function pointer */
  int (PSQL_API_FN xa_commit_entry) (XID *, int, sqlint32);/* xa_commit function pointer */
  int (PSQL_API_FN xa_recover_entry) (XID *, sqlint32, int, sqlint32);
                                       /* xa_recover function pointer*/
  int (PSQL_API_FN xa_forget_entry) (XID *, int, sqlint32);/* xa_forget function pointer */
  int (PSQL_API_FN xa_complete_entry) (int *, int *, int, sqlint32);
                                       /* xa_complete function pointer */
 #else           /* CDECL linkage convention on NT */
  int (__cdecl * xa_open_entry) (char *, int, sqlint32);/* xa_open function pointer */
  int (__cdecl * xa_close_entry) (char *, int, sqlint32);/* xa_close function pointer*/
  int (__cdecl * xa_start_entry) (XID *, int, sqlint32);/* xa_start function pointer */
  int (__cdecl * xa_end_entry) (XID *, int, sqlint32);/* xa_end function pointer */
  int (__cdecl * xa_rollback_entry) (XID *, int, sqlint32);/* xa_rollback function pointer */
  int (__cdecl * xa_prepare_entry) (XID *, int, sqlint32);/* xa_prepare function pointer */
  int (__cdecl * xa_commit_entry) (XID *, int, sqlint32);/* xa_commit function pointer */
  int (__cdecl * xa_recover_entry) (XID *, sqlint32, int, sqlint32);
                                       /* xa_recover function pointer*/
  int (__cdecl * xa_forget_entry) (XID *, int, sqlint32);/* xa_forget function pointer */
  int (__cdecl * xa_complete_entry) (int *, int *, int, sqlint32);
                                       /* xa_complete function pointer */

 #endif

#else /* ifndef __STDC__ */
  int ( PSQL_API_FN xa_open_entry)();              /* xa_open function pointer */
  int ( PSQL_API_FN xa_close_entry)();             /* xa_close function pointer */
  int ( PSQL_API_FN xa_start_entry)();             /* xa_start function pointer */
  int ( PSQL_API_FN xa_end_entry)();               /* xa_end function pointer */
  int ( PSQL_API_FN xa_rollback_entry)();          /* xa_rollback function pointer */
  int ( PSQL_API_FN xa_prepare_entry)();           /* xa_prepare function pointer */
  int ( PSQL_API_FN xa_commit_entry)();            /* xa_commit function pointer */
  int ( PSQL_API_FN xa_recover_entry)();           /* xa_recover function pointer */
  int ( PSQL_API_FN xa_forget_entry)();            /* xa_forget function pointer */
  int ( PSQL_API_FN xa_complete_entry)();          /* xa_complete function pointer */
#endif /* ifndef __STDC__ */
  };

//
// Following is the X/Open Group defined version of the xa_switch_t.
//
struct xa_switch_t_std {

  char name[RMNAMESZ];                /* name of resource manager */
  long     flags;                     /* resource manager specific options */
  long     version;                   /* must be 0 */
#if (defined(__STDC__) | defined(__cplusplus) | defined(DB2OS2) | defined (DB2NT)) /*@d34606pab*/
 #ifndef DB2NT
  int (PSQL_API_FN xa_open_entry) (char *, int, long);/* xa_open function pointer */
  int (PSQL_API_FN xa_close_entry) (char *, int, long);/* xa_close function pointer*/
  int (PSQL_API_FN xa_start_entry) (XID_STD *, int, long);/* xa_start function pointer */
  int (PSQL_API_FN xa_end_entry) (XID_STD *, int, long);/* xa_end function pointer */
  int (PSQL_API_FN xa_rollback_entry) (XID_STD *, int, long);/* xa_rollback function pointer */
  int (PSQL_API_FN xa_prepare_entry) (XID_STD *, int, long);/* xa_prepare function pointer */
  int (PSQL_API_FN xa_commit_entry) (XID_STD *, int, long);/* xa_commit function pointer */
  int (PSQL_API_FN xa_recover_entry) (XID_STD *, long, int, long);
                                       /* xa_recover function pointer*/
  int (PSQL_API_FN xa_forget_entry) (XID_STD *, int, long);/* xa_forget function pointer */
  int (PSQL_API_FN xa_complete_entry) (int *, int *, int, long);
                                       /* xa_complete function pointer */
 #else           /* CDECL linkage convention on NT */
  int (__cdecl * xa_open_entry) (char *, int, long);/* xa_open function pointer */
  int (__cdecl * xa_close_entry) (char *, int, long);/* xa_close function pointer*/
  int (__cdecl * xa_start_entry) (XID_STD *, int, long);/* xa_start function pointer */
  int (__cdecl * xa_end_entry) (XID_STD *, int, long);/* xa_end function pointer */
  int (__cdecl * xa_rollback_entry) (XID_STD *, int, long);/* xa_rollback function pointer */
  int (__cdecl * xa_prepare_entry) (XID_STD *, int, long);/* xa_prepare function pointer */
  int (__cdecl * xa_commit_entry) (XID_STD *, int, long);/* xa_commit function pointer */
  int (__cdecl * xa_recover_entry) (XID_STD *, long, int, long);
                                       /* xa_recover function pointer*/
  int (__cdecl * xa_forget_entry) (XID_STD *, int, long);/* xa_forget function pointer */
  int (__cdecl * xa_complete_entry) (int *, int *, int, long);
                                       /* xa_complete function pointer */
 #endif
#else /* ifndef __STDC__ */
  int ( PSQL_API_FN xa_open_entry)();              /* xa_open function pointer */
  int ( PSQL_API_FN xa_close_entry)();             /* xa_close function pointer */
  int ( PSQL_API_FN xa_start_entry)();             /* xa_start function pointer */
  int ( PSQL_API_FN xa_end_entry)();               /* xa_end function pointer */
  int ( PSQL_API_FN xa_rollback_entry)();          /* xa_rollback function pointer */
  int ( PSQL_API_FN xa_prepare_entry)();           /* xa_prepare function pointer */
  int ( PSQL_API_FN xa_commit_entry)();            /* xa_commit function pointer */
  int ( PSQL_API_FN xa_recover_entry)();           /* xa_recover function pointer */
  int ( PSQL_API_FN xa_forget_entry)();            /* xa_forget function pointer */
  int ( PSQL_API_FN xa_complete_entry)();          /* xa_complete function pointer */
#endif /* ifndef __STDC__ */
  };

/*
 *Flag definitions for the RM switch
 */
#define TMNOFLAGS    0x00000000L       /* no resource manager features selected */
#define TMREGISTER   0x00000001L       /* resource manager dynamically registers */
#define TMNOMIGRATE  0x00000002L       /* resource manager does not support
                                       association migration */
#define TMUSEASYNC   0x00000004L       /* resource manager supports
                                       asynchronous operations */
/*
 * Flag definitions for xa_and ax_ routines
 */
/* use TMNOFLAGS, defined above, when not specifying other flags */
#define TMASYNC      0x80000000L        /* perform routine asynchronously */
#define TMONEPHASE   0x40000000L        /* caller is using on-phase commit
                                        optimisation */
#define TMFAIL       0x20000000L        /* dissociates caller and marks
                                        transaction branch rollback-only */
#define TMNOWAIT     0x10000000L        /* return if blocking condition exists */
#define TMRESUME     0x08000000L        /* caller is resuming association
                                        with suspended transaction branch */
#define TMSUCCESS    0x04000000L        /* dissociate caller from transaction
                                        branch*/
#define TMSUSPEND    0x02000000L        /* caller is suspending, not ending,
                                        association */
#define TMSTARTRSCAN 0x01000000L        /* start a recovery scan */
#define TMENDRSCAN   0x00800000L        /* end a recovery scan */
#define TMMULTIPLE   0x00400000L        /* wait for any asynchronous operation */
#define TMJOIN       0x00200000L        /* caller is joining existing transaction
                                        branch */
#define TMMIGRATE    0x00100000L        /* caller intends to perfrom migration */
/*
 * ax_() return codes (transaction manager reports to resource manager)
 */
#define TM_JOIN      2                  /* caller is joining existing transaction
                                        branch */
#define TM_RESUME    1                  /* caller is resuming association with
                                        suspended transaction branch */
#define TM_OK        0                  /* normal execution */
#define TMER_TMERR   -1                 /* an error occured in the
                                        transaction manager */
#define TMER_INVAL   -2                 /* invalid arguments were given */
#define TMER_PROTO   -3                 /* routine invoked in an improper context */
/*
 * xa_() return codes (resource manager reports to transaction manager)
 */
#define XA_RBBASE      100              /* The inclusive lower bound of the
                                        rollback codes */
#define XA_RBROLLBACK  XA_RBBASE        /* The rollback was caused by an
                                        unspecified reason */
#define XA_RBCOMMFAIL  XA_RBBASE+1      /* The rollback was caused by a
                                        communication failure */
#define XA_RBDEADLOCK  XA_RBBASE+2      /* A deadlock was detected */
#define XA_RBINTEGRITY XA_RBBASE+3      /* A condition that violates the integrity
                                        of the resources was detected */
#define XA_RBOTHER     XA_RBBASE+4      /* The resource manager rolled back the
                                        transaction branch for a reason not on
                                        this list */
#define XA_RBPROTO     XA_RBBASE+5      /* A protocol error occurred in the
                                        resource manager */
#define XA_RBTIMEOUT   XA_RBBASE+6      /* A transaction branch took too long */
#define XA_RBTRANSIENT XA_RBBASE+7      /* May retry the transaction branch */
#define XA_RBEND       XA_RBTRANSIENT   /* The inclusive upper bound of the
                                        rollback codes */

#define XA_NOMIGRATE  9                 /* resumption must occur where
                                        suspension occured */
#define XA_HEURHAZ    8                 /* the transaction branch may have
                                        been heuristically completed */
#define XA_HEURCOM    7                 /* the transaction branch has been
                                        heuristically committed */
#define XA_HEURRB     6                 /* the transaction branch has been
                                        heuristically rolled back */
#define XA_HEURMIX    5                 /* the transaction branch has been
                                        heuristically committed and rolled
                                        back */
#define XA_RETRY      4                 /* routine returned with no effect and
                                        may be re-issued */
#define XA_RDONLY     3                 /* the transaction branch was read-
                                         only and has been committed */
#define XA_OK         0                 /* normal execution */
#define XAER_ASYNC    -2                /* asynchronous operation already outstanding */
#define XAER_RMERR    -3                /* a resource manager error occured in
                                        the transaction branch */
#define XAER_NOTA     -4                /* the XID is not valid */
#define XAER_INVAL    -5                /* invalid arguments were given */
#define XAER_PROTO    -6                /* routine invoked in an improper context */
#define XAER_RMFAIL   -7                /* resource manager unavailable */
#define XAER_DUPID    -8                /* the XID already exits */
#define XAER_OUTSIDE  -9                /* resource manager doing work outside */
                                        /* global transaction */

#endif /* ifndef XA_H */
/*
 * End of xa.h header
 */
