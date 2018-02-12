ulimit -c unlimited
./redis-leveldb -d -D127.0.0.1:9111 -H127.0.0.1 -P8823 1>std 2>err
