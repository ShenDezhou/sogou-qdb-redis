/*-*- c++ -*-
 *
 * rl_server.h
 * author : KDr2
 *
 */

#ifndef _RL_SERVER_H_INCLUDED
#define _RL_SERVER_H_INCLUDED

#include <vector>
#include <string>

#include <ev.h>
// #include <leveldb/db.h>
// #include <leveldb/options.h>
//#define _FILE_OFFSET_BITS 64
//#include "offdb/QuickdbAdapter.h"
#include "core/dezhoukv.h"
class DezhouKVAdapter {
public:
  DezhouKVAdapter() {
    init();
  }
  inline int _get(const char* key, std::string &value) {
    return get(key, value);
  }
  inline int _set(const char* key, std::string value) {
    return set(key,value);
  } 
};

class RLServer{

public:
    int db_num;
    std::string db_path;
    std::string hostaddr;
    int port;
    int fd;
    int clients_num;
    struct ev_loop* loop;
    ev_io connection_watcher;
    // leveldb::Options *options;
    // leveldb::ReadOptions read_options;
    // leveldb::WriteOptions write_options;
    // leveldb::DB **db;
    //QuickdbAdapter **db;
    DezhouKVAdapter* *db;
    RLServer(const char *db_path, const char *hostaddr, int port, int dbn=0);
    ~RLServer();
    void start();
    static void on_connection(struct ev_loop *loop, ev_io *watcher, int revents);
};


#endif
