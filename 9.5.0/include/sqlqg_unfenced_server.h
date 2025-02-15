/******************************************************************************
**
** Source File Name: sqlqg_unfenced_server.h
**
** (C) COPYRIGHT International Business Machines Corp. 1987, 2006
** All Rights Reserved
** Licensed Materials - Property of IBM
**
** US Government Users Restricted Rights - Use, duplication or
** disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
**
** Function = Include File defining:
**              Query Gateway classes and routines for
**              unfenced-side server objects
**
** Operating System: All
**
*******************************************************************************/
#ifndef SQL_H_SQLQG_UNFENCED_SERVER
#define SQL_H_SQLQG_UNFENCED_SERVER

#include "sqlqg_server.h"

// Status of a connection.
#define SQLQG_CONNECTION_IDLE        0x0000 // initial state, no flags are set
#define SQLQG_CONNECTION_CONNECTED   0x0001 // connection has been established
#define SQLQG_CONNECTION_XASTARTED   0x0002 // transaction has been xaStarted
#define SQLQG_CONNECTION_UPDATED     0x0004 // IUD has been done on this
                                            // connection
#define SQLQG_CONNECTION_XASUSPENDED 0x0008 // transaction has been xaSuspended
#define SQLQG_CONNECTION_XAOPENED    0x0010 // transaction has been xaOpened
#define SQLQG_CONNECTION_XAPREPARED  0x0020 // transaction has been xaPrepared

// xa_start intent
#define SQLQG_READONLY 0
#define SQLQG_UPDATE   1

// Forward declarations.
class Server_Info;
class Server_Attributes;
class User_Info;
class Nickname_Info;
class Remote_Function_Info;

class UnfencedWrapper;
class UnfencedRemote_User;
class UnfencedNickname;
class Remote_Function;

class sqlqg_memPool;

struct Default_Remote_Function;
struct sqlmon_dbase_remote;
struct sqlri_ufob;
struct sqlqg_Fmp_Info;
struct sqlqg_memMgrsInfo;

////////////////////////////////////////////////////////////////////////////////
// class UnfencedServer
////////////////////////////////////////////////////////////////////////////////
class UnfencedServer : public Server {
public:

  // Constructors
  UnfencedServer (sqluint8*        a_server_name,
                  server_kind      a_server_kind,
                  UnfencedWrapper* a_wrapper,
                  sqlint32*        a_rc);

  // whether remote server init at create server time is needed.
  virtual sqlint32 create_server_remote_init (void)
  {
    return 0;
  };

  // return FALSE for basic class.  Supported server should
  // override this method
  virtual bool is_2pc_supported (void) const;

  // Remote server registration hook.  Invoked as a result
  // of CREATE SERVER MAPPING DDL statement.  It should do
  // any registration time checks, and create a packed descriptor
  // from the ddl information.
  virtual sqlint32 register_server (Server_Info*  a_server_info,
                                    Server_Info** a_delta_info);

  // Verifies information specified in an ALTER SERVER DDL statement.
  virtual sqlint32 verify_alter_server_info (Server_Info*  a_server_info,
                                             Server_Info** a_delta_info);

  //Plan a function invocation
  virtual sqlint32 plan_remote_function_invoke (char*     a_signature,
                                                char*     a_chema,
                                                void**    a_exec_desc,
                                                sqlint32* a_exec_desc_len);

  // Remote user management.
  sqlint32 find_current_remote_user (UnfencedRemote_User** a_user,
                                     bool*                 a_new_catalog_info,
                                     Sqlqg_OpType          a_op=sqlqg_Read);

  Sqlqg_Return_Code find_current_tc_originator (UnfencedRemote_User** a_user,
                                                bool*                 a_new_catalog_info,
                                                Sqlqg_OpType          a_op=sqlqg_Read);

  // Find an existing nickname
  sqlint32 find_nickname (sqluint8*          a_schema_name,
                          sqluint8*          a_nickname_name,
                          UnfencedNickname** a_nickname,
                          Sqlqg_OpType       a_op=sqlqg_Read,
                          unsigned short     a_do_create=TRUE);

  sqlint32 get_status (void);
  
  void set_status (sqlint32 a_status);

  void set_status_bit (sqlint32 a_status);

  void remove_status_bit (sqlint32 a_status);

  sqlmon_dbase_remote* get_dbmon_block (void);

  // Methods to maintain the agent's server linked list.
  UnfencedServer* get_prev_server (void);
  void set_prev_server (UnfencedServer* a_server);

  UnfencedServer* get_next_server (void);
  void set_next_server (UnfencedServer* a_server);

  UnfencedServer* find_forward (sqluint8* a_server_name); // can be used to
                                                          // search forward to
                                                          // find a specific
                                                          // server by name.

  // Sets UOW status for DBMON and update time changed (if needed)
  static sqlint32 set_dbmon_UOW_status (int a_status);

  // Returns the current application status
  static sqluint32 get_dbmon_UOW_status (void);

  // Resets cache validation state for each cacheable object
  // associated with this server (the server object itself, its
  // wrapper, its remote user(s), any nicknames).  Should be called at
  // the end of a unit of work.
  sqlint32 reset_cached_objects (void);

  // Flush server and its subordinate user and nickname objects from
  // the cache if they could have been affected by DDL performed on
  // behalf of this transaction.
  sqlint32 flush_cached_objects (void);

  // Handy method to check if an option is recognized by DB2 and
  // should be ignored by the wrapper.
  virtual sqluint32 is_reserved_server_option (sqluint8* a_op_name);

  // Returns true if an option name is a reserved function option
  virtual bool is_reserved_function_option (const char* a_function_option) const;

  sqlri_ufob* Get_Fenced_ufobP (void);
  void Set_Fenced_ufobP (sqlri_ufob *ufobP);

  sqlqg_Fmp_Info* get_fmp_info (void);
  void set_fmp_info (sqlqg_Fmp_Info *a_fmpInfo);

  sqlri_ufob* Get_Trusted_ufobP (void);
  void Set_Trusted_ufobP (sqlri_ufob *ufobP);

  sqlri_ufob* get_sec_fenced_ufobP (void) const;
  void set_sec_fenced_ufobP (sqlri_ufob *a_ufobP);

  sqlqg_Fmp_Info* get_sec_fmp_info (void) const;
  void set_sec_fmp_info (sqlqg_Fmp_Info *a_secFmpInfo);

  sqlint32 drop_remote_user (Remote_User* user,
                             Sqlqg_DeleteType a_flag = SQLQG_DELETE_ALL);

  void set_rmid (sqluint32 a_rmid); // Functions for setting and
  sqluint32 get_rmid (void) const;  // retrieving the RM ID

  // Add a temporary remote function mapping based on a Default_Remote_Function object
  Sqlqg_Return_Code insert_remote_function (const Default_Remote_Function* a_remote_funtion);

  // Add a temporary remote function mapping based on a Remote_Function_Info
  Sqlqg_Return_Code insert_remote_function (Remote_Function_Info* a_remote_function);

  // Given the name and signature of a local function, returns
  // information about the remote function it is mapped to.
  Sqlqg_Return_Code get_remote_function(sqluint8* a_function_name_and_signature,
                                        Remote_Function** remote_function);

  // Return true if a server supports F2PC in fenced mode.
  // Refer fenced_2pc_supported member variable in Server_Attributes and child classes.
  virtual bool is_fenced_f2pc_supported (void) const;

  // Determine if wrapper uses CLOB API to handle XML data.
  virtual bool use_xml_clob_interface (void) const;

  //*********************************************
  // Methods to return option values
  //*********************************************
  sqluint8  get_f2pc_option (void);

  sqluint8  get_update_enforced_option (void);

  sqluint32 get_con_unref_threshold (void);

  sqlint32  get_max_async_requests_per_query (void);

  sqluint8  get_stored_procedures_option(void) const ;

  sqluint8  get_fenced_stored_procedures_option(void) const ;

  sqluint8  get_multi_resultsets_option(void) const;

  sqluint8  get_fastpath_option(void) const;

  sqluint8  get_old_name_gen_option(void) const;
 
  bool      is_fastpath_enabled(void);

  friend class UnfencedWrapper;

  /**
    \internal
    \brief Return a set of attributes for the server

    \param a_attrs [out]
       Pointer to allocated attributes.  UnfencedServer object retains control of the object.

    \return 0 or DB2/WSII error code
  */
  virtual sqlint32 get_server_attributes (Server_Attributes** a_attrs);

protected:

  // Destructor.  This is not public to prevent people from calling
  // the destructor directly, rather than invoking wrapper->drop_server().
  virtual ~UnfencedServer (void);

  /**
    \internal
    \brief Allocate a Server_Attributes object

    \return 0 or DB2/WSII error code
  */
  virtual sqlint32 create_my_server_attributes (void);

  /**
    \internal
    \brief Update server attributes with information from the catalog

    \param a_server_info [in]
       Server_Info object with catalog updates.

   \return 0 or DB2/WSII error code
  */
  sqlint32 set_catalog_server_attributes (Server_Info* a_server_info);


  ////////////////
  // Data.
  ////////////////
  sqlint32 status;                      // status of connection.
                                        // Values are given by SQLQGCON_
                                        // #defines above.
                                        // It is used as a short cut to
                                        // determine whether xaStart
                                        // has to be issued to the data
                                        // sources.  It is also used to
                                        // track whether any update has
                                        // been done using this connection.

  // Pointer to dbmon block (hung off the ACB) for this server.
  // It contains the agent level DBMON counts.
  struct sqlmon_dbase_remote *dbmon_block;

  // Maintains the agent's linked list of servers.
  UnfencedServer* prev;
  UnfencedServer* next;

  sqlri_ufob *Fenced_ufobP;

  sqlqg_Fmp_Info *m_fmpInfo;

  sqlri_ufob *Trusted_ufobP;

  // Ufob, Fmp Info for security request associated with this server
  sqlri_ufob     *m_secFencedUfobP;
  sqlqg_Fmp_Info *m_secFmpInfo;

  // Set to TRUE only when the first call of get_remote_function
  // happens and the remote function mappings have been set up
  // (remote function definitions initialized)
  bool               m_remote_functions_ready;

  sqlqg_memPool     *m_mem_pool;         // Memory allocation manager
                                         // used to manage function and type definitions
  sqlqg_memMgrsInfo *m_mem_managers;     // Memory managers info for this server
                                         // used to manage function and type defintiions
  Hash_Table         m_remote_functions; // Hash table of entries that describe how
                                         // local functions map to remote functions.
  Server_Attributes* m_attributes;       //*< \brief Server characteristics


  //*******************************************************************
  // Server options for both relational and generic
  //*******************************************************************
  sqluint8  f2pc_option;                      // DB2_TWO_PHASE_COMMIT (Y|N|D)
  sqluint8  update_enforced_option;           // DB2_UPDATE_ENFORCED (Y|N)
  sqluint32 m_con_unref_threshold;            // Connection count for
                                              // auto-disconnection
  sqlint32  m_max_async_requests_per_query;   // DB2_MAX_ASYNC_REQUESTS_PER_QUERY
  sqluint8  m_stored_procedures_option;       //STORED_PROCEDURES
  sqluint8  m_fenced_stored_procedures_option;//FENCED_STORED_PROCEDURES
  sqluint8  m_multi_resultsets_option;        //MULTI_RESULTSETS
  sqluint8  m_fastpath_option;                //FASTPATH (Y|N)
  sqluint8  m_old_name_gen_option;            //OLD_NAME_GEN (Y|N)
  sqluint32 m_rmid;
  sqluint8  m_xml_clob_interface_option;      // DB2_XML_CLOB_INTERFACE(Y|N)

  ////////////////
  // Methods.
  ////////////////

  // Initializes the UnfencedServer object.
  virtual sqlint32 initialize_server (Server_Info* a_cat_info,
                                      sqlint32     a_init_subclass=TRUE);

  // Verifies information specified on CREATE SERVER statement.
  virtual sqlint32 verify_register_server_info (Server_Info*  a_server_info,
                                                Server_Info** a_delta_info);

  // verify_my_register_info() allows a server subclass instance to
  // verify any server info specific to that server at registration time.
  // It is called from verify_register_server_info() method after
  // verifying options applicable to all servers (currently none).
  virtual sqlint32 verify_my_register_server_info (Server_Info*  a_server_info,
                                                   Server_Info** a_delta_info);

  // verify_my_alter_info() allows a server subclass instance to
  // verify any server info specific to that server at ALTER SERVER time.
  virtual sqlint32 verify_my_alter_server_info (Server_Info*  a_server_info,
                                                Server_Info** a_delta_info);

  // verify_function_mappings verifies the set of function mappings
  // specified on an ALTER SERVER statement.
  sqlint32 verify_function_mapping_info (Server_Info*  a_server_info,
                                         Server_Info** a_delta_info);

  // verify_my_function_mapping_info() allows a server subclass instance to
  // verify a function mapping specific to that server at ALTER SERVER time.
  virtual sqlint32 verify_my_function_mapping_info (
                                         Remote_Function_Info* a_map_info);

  // set_my_default_remote_function_mappings establishes the built-in remote
  // function mappings for this wrapper
  virtual sqlint32 set_my_default_remote_function_mappings(void);

  // Establish the remote function mappings for this wrapper
  virtual Sqlqg_Return_Code set_catalog_remote_function_mappings(Server_Info *server_info);

  // Sets up remote function mappings
  virtual Sqlqg_Return_Code set_remote_function_mappings(void);

  // Remove a remote function mapping from the internal table
  Sqlqg_Return_Code drop_remote_function(Remote_Function* remote_function);

  // Remove all remote function mappings from the internal table
  void drop_all_remote_functions(void);

  //Remove all local types
  virtual void drop_all_local_types(void);

};

// Inline methods

inline sqlint32
UnfencedServer::get_status (void) 
{
  return status;
}

inline void
UnfencedServer::set_status (sqlint32 a_status) 
{
  status = a_status;
}

inline void
UnfencedServer::set_status_bit (sqlint32 a_status) 
{
  status |= a_status;
}

inline void
UnfencedServer::remove_status_bit (sqlint32 a_status)
{
  status &= ~a_status;
}

inline sqlmon_dbase_remote*
UnfencedServer::get_dbmon_block (void)
{
  return dbmon_block;
}

inline UnfencedServer*
UnfencedServer::get_prev_server (void)
{
  return prev;
}

inline void
UnfencedServer::set_prev_server (UnfencedServer* a_server)
{
  prev = a_server;
}

inline UnfencedServer*
UnfencedServer::get_next_server (void)
{
  return next;
}

inline void
UnfencedServer::set_next_server (UnfencedServer* a_server)
{
  next = a_server;
}

inline sqlri_ufob*
UnfencedServer::Get_Fenced_ufobP (void)
{
  return Fenced_ufobP;
}

inline void
UnfencedServer::Set_Fenced_ufobP (sqlri_ufob *ufobP)
{
  Fenced_ufobP = ufobP;
}

inline sqlqg_Fmp_Info*
UnfencedServer::get_fmp_info (void)
{
  return m_fmpInfo;
}

inline void
UnfencedServer::set_fmp_info (sqlqg_Fmp_Info *a_fmpInfo)
{
  m_fmpInfo = a_fmpInfo;
}

inline void
UnfencedServer::Set_Trusted_ufobP (sqlri_ufob *ufobP)
{
  Trusted_ufobP = ufobP;
}

inline sqlri_ufob*
UnfencedServer::get_sec_fenced_ufobP (void) const
{
  return m_secFencedUfobP;
}

inline void
UnfencedServer::set_sec_fenced_ufobP (sqlri_ufob *a_ufobP)
{
  m_secFencedUfobP = a_ufobP;
}

inline sqlqg_Fmp_Info*
UnfencedServer::get_sec_fmp_info (void) const
{
  return m_secFmpInfo;
}

inline sqlri_ufob*
UnfencedServer::Get_Trusted_ufobP (void)
{
  return Trusted_ufobP;
}

inline void
UnfencedServer::set_sec_fmp_info (sqlqg_Fmp_Info *a_secFmpInfo)
{
  m_secFmpInfo = a_secFmpInfo;
}

inline sqluint8
UnfencedServer::get_f2pc_option (void)
{
  return f2pc_option;
}

inline sqluint8
UnfencedServer::get_update_enforced_option (void)
{
  return update_enforced_option;
}

inline sqluint32
UnfencedServer::get_con_unref_threshold (void)
{
  return m_con_unref_threshold;
}

inline void
UnfencedServer::set_rmid (sqluint32 a_rmid)
{
  m_rmid = a_rmid;
}

inline sqluint32
UnfencedServer::get_rmid(void) const
{
  return m_rmid;
}

inline sqlint32
UnfencedServer::get_max_async_requests_per_query (void)
{
  return m_max_async_requests_per_query;
}

inline sqluint8
UnfencedServer::get_stored_procedures_option (void) const
{
  return m_stored_procedures_option;
}

inline sqluint8
UnfencedServer::get_fenced_stored_procedures_option (void) const
{
  return m_fenced_stored_procedures_option;
}

inline sqluint8
UnfencedServer::get_multi_resultsets_option (void) const
{
  return m_multi_resultsets_option;
}

inline sqluint8
UnfencedServer::get_fastpath_option (void) const
{
  return m_fastpath_option;
}

inline sqluint8
UnfencedServer::get_old_name_gen_option (void) const
{
  return m_old_name_gen_option;
}
#endif
