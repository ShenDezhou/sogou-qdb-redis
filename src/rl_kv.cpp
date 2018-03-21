/*-*- c++ -*-
 *
 * rl_set.cpp
 * author : KDr2
 *
 */

#include <iostream>
#include <sstream>
#include <algorithm>
#include <functional>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>

#include <gmp.h>

#include "rl.h"
#include "rl_util.h"
#include "rl_server.h"
#include "rl_connection.h"
#include "rl_request.h"

void RLRequest::rl_incr(){

    if(args.size()!=1){
        connection->write_error("ERR wrong number of arguments for 'incr' command");
        return;
    }

    std::string &keyname = args[0];
    std::string out;
    void* value = reinterpret_cast<void*>(const_cast<char*>(out.data()));
    size_t value_size = 0;
    //struct timeval tv={5,0};
    int ret;
    // leveldb::Status status;

    // status = connection->server->db[connection->db_index]->Get(keyname.c_str(), out);
    
    //ret = get(keyname.c_str(), out);
    ret = connection->server->db[connection->db_index]->_get(keyname.c_str(), out);

    char *str_oldv=NULL;
    if(ret == -1){
        str_oldv = strdup("0");
    }else if(ret == 0){
        str_oldv=(char*)malloc(out.size()+1);
        memcpy(str_oldv,out.data(),out.size());
        str_oldv[out.size()]=0;
    }else{
        connection->write_error("INCR ERROR 1");
        return;
    }

    mpz_t old_v;
    mpz_init(old_v);
    mpz_set_str(old_v,str_oldv,10);
    free(str_oldv);
    mpz_add_ui(old_v,old_v,1);
    char *str_newv=mpz_get_str(NULL,10,old_v);
    mpz_clear(old_v);

    // status = connection->server->db[connection->db_index]->put(connection->server->write_options,
    //          args[0], leveldb::Slice(str_newv, strlen(str_newv)));
    //ret = set(keyname.c_str(), str_newv);
    ret = connection->server->db[connection->db_index]->_set(keyname.c_str(), str_newv); 

    if(!ret) {
        connection->write_error("INCR ERROR 2");
    } else {
        connection->write_integer(str_newv, strlen(str_newv));
    }
    free(str_newv);
}


void RLRequest::rl_incrby(){

    if(args.size()!=2){
        connection->write_error("ERR wrong number of arguments for 'incrby' command");
        return;
    }

    std::string &keyname = args[0];
    std::string out;
    void* value = reinterpret_cast<void*>(const_cast<char*>(out.data()));
    size_t value_size = 0;
    struct timeval tv={5,0};
    int ret;
    // leveldb::Status status;
    // status = connection->server->db[connection->db_index]->Get(connection->server->read_options,
    //          args[0], &out);
    //ret = get(keyname.c_str(), out);
    ret = connection->server->db[connection->db_index]->_get(keyname.c_str(), out);

    mpz_t delta;
    mpz_init(delta);
    mpz_set_str(delta,args[1].c_str(),10);

    char *str_oldv=NULL;
    if(ret == -1){
        str_oldv = strdup("0");
    }else if(ret == 0){
        str_oldv=(char*)malloc(out.size()+1);
        memcpy(str_oldv,out.data(),out.size());
        str_oldv[out.size()]=0;
    }else{
        connection->write_error("INCRBY ERROR 1");
        return;
    }

    mpz_t old_v;
    mpz_init(old_v);
    mpz_set_str(old_v,str_oldv,10);
    free(str_oldv);
    mpz_add(old_v,old_v,delta);
    char *str_newv=mpz_get_str(NULL,10,old_v);
    mpz_clear(delta);
    mpz_clear(old_v);

    // status = connection->server->db[connection->db_index]->Put(connection->server->write_options,
             // args[0], leveldb::Slice(str_newv, strlen(str_newv)));
    //ret = set(keyname.c_str(), str_newv);
    ret = connection->server->db[connection->db_index]->_set(keyname.c_str(), str_newv); 


    if(!ret) {
        connection->write_error("INCRBY ERROR 2");
    }else{
        connection->write_integer(str_newv, strlen(str_newv));
    }
    free(str_newv);
}


void RLRequest::rl_get(){

    if(args.size()!=1){
        connection->write_error("ERR wrong number of arguments for 'get' command");
        return;
    }

    std::string &keyname = args[0];
    std::string out;
    void* value = reinterpret_cast<void*>(const_cast<char*>(out.data()));
    size_t value_size = 0;
    struct timeval tv={5,0};
    int ret;
    // leveldb::Status status;
    // status = connection->server->db[connection->db_index]->Get(connection->server->read_options,
    //          args[0], &out);
    //ret = get(keyname.c_str(), out);
    ret = connection->server->db[connection->db_index]->_get(keyname.c_str(), out);

    if(ret==-1) {
        connection->write_nil();
    }else if(ret==0) {
        connection->write_bulk(out);
    } else {
        connection->write_error("GET ERROR 1");
    }
}


void RLRequest::rl_set(){

    if(args.size()!=2){
        connection->write_error("ERR wrong number of arguments for 'set' command");
        return;
    }
    std::string &keyname = args[0];
    std::string &valuename = args[1];
    struct timeval tv={5,0};
    int ret;
    
    // leveldb::Status status;
    // status = connection->server->db[connection->db_index]->Put(connection->server->write_options,
    //          args[0], args[1]);
    //ret = set(keyname.c_str(), valuename);
    ret = connection->server->db[connection->db_index]->_set(keyname.c_str(), valuename);

    if(!ret) {
        connection->write_error("SET ERROR 1");
    } else {
        connection->write_status("OK");
    }
}

void RLRequest::rl_del(){

    if(args.size()!=1){
        connection->write_error("ERR wrong number of arguments for 'del' command");
        return;
    }
    std::string &keyname = args[0];
    std::string out;
    void* value = reinterpret_cast<void*>(const_cast<char*>(out.data()));
    size_t value_size = 0;
    struct timeval tv={5,0};
    int ret;
    // std::string out;
    // leveldb::Status status;

    // status = connection->server->db[connection->db_index]->Get(connection->server->read_options,
    //          args[0], &out);
    //ret = get(keyname.c_str(), out);
    ret = connection->server->db[connection->db_index]->_get(keyname.c_str(), out);

    if(ret==-1) {
        connection->write_integer("0", 1);
    } else if(ret==0) {
        //ret = connection->server->db[connection->db_index]->del((void*)keyname.c_str(), keyname.size(), 0, &tv);
        if(!ret){
            connection->write_error("DELETE ERROR");
        }else{
            connection->write_integer("1", 1);
        }
    }
}


void RLRequest::rl_mget(){

    if(args.size()<1){
        connection->write_error("ERR wrong number of arguments for 'mget' command");
        return;
    }

    std::string &keyname = args[0];
    std::string out;
    void* value = reinterpret_cast<void*>(const_cast<char*>(out.data()));
    size_t value_size = 0;
    struct timeval tv={5,0};
    int ret;

    // connection->write_mbulk_header(args.size());

    std::vector<std::string>::iterator it=args.begin();

    for(;it!=args.end();it++){
        // status = connection->server->db[connection->db_index]->Get(connection->server->read_options,
        //          *it, &out);
        //ret = get(it->c_str(), out);
        ret = connection->server->db[connection->db_index]->_get(it->c_str(), out);

        if(ret != 0) {
            connection->write_nil();
        } else {
            connection->write_bulk(out);
        }
    }
}


void RLRequest::rl_mset(){

    if(args.size()<2 || args.size()%2!=0){
        connection->write_error("ERR wrong number of arguments for 'mset' command");
        return;
    }

    // leveldb::WriteBatch write_batch;
    struct timeval tv={5,0};
    int ret;
    // for(uint32_t i=0;i<args.size();i+=2){
    //     write_batch.Put(args[i], args[i+1]);
    // }
    // std::vector<std::string>::iterator it=args.begin();

    for(uint32_t i=0;i<args.size();i+=2){
        std::string &keyname = args[i];
        std::string &valuename = args[i+1];
        // status = connection->server->db[connection->db_index]->Get(connection->server->read_options,
        //          *it, &out);
        //ret = set(keyname.c_str(), valuename);
        ret = connection->server->db[connection->db_index]->_set(keyname.c_str(), valuename);
    }

    // leveldb::Status status = connection->server->db[connection->db_index]->Write(
    //     connection->server->write_options, &write_batch);
    if(ret != 0) {
        connection->write_error("MSET ERROR 1");
    } else {
        connection->write_status("OK");
    }
}
