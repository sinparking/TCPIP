int getsockopt(int sock, int level, int optname, void *optval, socklen_t *optlen)
int setsockopt(int sock, int level, int optname, void *optval, socklen_t optlen)

SOL_SOCKET->SO_TYPE 协议类型
SOL_SOCKET->SO_SNDBUF/SO_RCVBUF 发送接收窗口
SOL_SOCKET->SO_REUSEADDR    略过timewait分配端口
IPPROTO_TCP->TCP_NODELAY    禁用NAGLE算法，设置为1禁用
