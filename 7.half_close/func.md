shutdown(sock,option)
SHUT_RD：断开输入流。套接字无法接收数据（即使缓冲区收到数据也会被清除），无法调用输入相关函数。

SHUT_WD：断开输出流。套接字无法发送数据，但如果输出缓冲区中还有未传输的数据，则将传递到目标主机。

SHUT_RDWR：同时断开I/O流。相当于分两次调用shutdown()，其中一次以SHUT_RD为参数，另一次以SHUT_WR为参数。
