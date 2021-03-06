/**
  * (C) 2007-2010 Taobao Inc.
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License version 2 as
  * published by the Free Software Foundation.
  *
  * Version: $Id$
  *
  * Authors:
  *   zhidong <xielun.szd@taobao.com>
  *     - some work details if you want
  */

#include "ob_root_rpc_stub.h"
#include "ob_root_ms_provider.h"
#include "ob_chunk_server_manager.h"
#include "ob_root_sql_proxy.h"

using namespace oceanbase::common;
using namespace oceanbase::rootserver;

ObRootSQLProxy::ObRootSQLProxy(const ObChunkServerManager & server_manager, ObRootRpcStub & rpc_stub)
    :ms_provider_(server_manager), rpc_stub_(rpc_stub)
{
}

ObRootSQLProxy::~ObRootSQLProxy()
{
}

int ObRootSQLProxy::query(const int64_t retry_times, const int64_t timeout, const common::ObString & sql)
{
  int ret = OB_SUCCESS;
  ObServer server;
  int64_t count = 0;
  do
  {
    ret = ms_provider_.get_ms(server);
    if (ret != OB_SUCCESS)
    {
      TBSYS_LOG(WARN, "get ms failed:ret[%d]", ret);
    }
    else
    {
      ret = rpc_stub_.execute_sql(server, sql, timeout);
      if (ret != OB_SUCCESS)
      {
        TBSYS_LOG(WARN, "execute sql failed:sql[%.*s], retry[%ld], ret[%d]",
            sql.length(), sql.ptr(), count, ret);
      }
    }
    ++count;
  } while ((ret != OB_SUCCESS) && (count <= retry_times));
  return ret;
}

