#ifndef RPC_RPC_SERVER_H
#define RPC_RPC_SERVER_H

#include <google/protobuf/service.h>
#include "base/mutex.h"
#include "net/tcp_server.h"
#include "rpc_codec.h"

class RpcServer : private NonCopyable {
 public:
  explicit RpcServer(EventLoop* loop, const InetAddress& listenAddr);
  void RegisterService(google::protobuf::Service *service);
  void set_thread_num(int num) { server_.set_thread_num(num); }
  void Start() { server_.Start(); }
 private:
  // 为了传参数的类
  struct RpcInfo {
    google::protobuf::Message *response;
    int64_t id;
  };
  void OnConnection(TcpConnectionPtr conn);
  void OnMessage(TcpConnectionPtr conn, Buffer *buf);
  void OnRpcMessgae(TcpConnectionPtr conn, RpcMessagePtr message);
  void OnCallBackDone(TcpConnectionPtr conn, RpcInfo *info);
  RpcCodec codec_;
  TcpServer server_;
  Mutex mutex_;
  std::unordered_map<std::string, google::protobuf::Service *> services_; // GUARDED_BY(mutex_) 
};

#endif