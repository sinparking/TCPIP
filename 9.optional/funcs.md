int getsockopt(int sock, int level, int optname, void *optval, socklen_t *optlen)
int setsockopt(int sock, int level, int optname, void *optval, socklen_t optlen)

SOL_SOCKET->SO_TYPE Э������
SOL_SOCKET->SO_SNDBUF/SO_RCVBUF ���ͽ��մ���
SOL_SOCKET->SO_REUSEADDR    �Թ�timewait����˿�
IPPROTO_TCP->TCP_NODELAY    ����NAGLE�㷨������Ϊ1����
