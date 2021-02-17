#include "ipc.h"
#include "tock.h"

int ipc_discover(const char* pkg_name) {
  int len = strlen(pkg_name);
  allow_ro_return_t prev = allow_readonly(IPC_DRIVER_NUM, 0, pkg_name, len);
  syscall_return_t res   = command2(IPC_DRIVER_NUM, 1, 0, 0);
  prev = allow_readonly(IPC_DRIVER_NUM, 0, prev.ptr, prev.size);
  if (res.type != TOCK_SYSCALL_SUCCESS_U32) {
    return tock_error_to_rcode(res.data[0]);
  } else {
    return res.data[0];
  }
}

int ipc_register_svc(subscribe_cb callback, void *ud) {
  return subscribe(IPC_DRIVER_NUM, 0, callback, ud);
}

int ipc_register_client_cb(int svc_id, subscribe_cb callback, void *ud) {
  if (svc_id <= 0) {
    return -1;
  }
  return subscribe(IPC_DRIVER_NUM, svc_id, callback, ud);
}

int ipc_notify_svc(int pid) {
  syscall_return_t res = command2(IPC_DRIVER_NUM, 2, pid, 0);
  if (res.type != TOCK_SYSCALL_SUCCESS) {
    return tock_error_to_rcode(res.data[0]);
  } else {
    return TOCK_SUCCESS;
  }
}

int ipc_notify_client(int pid) {
  return command(IPC_DRIVER_NUM, 3, pid, 0);
}

allow_rw_return_t ipc_share(int pid, void* base, int len) {
  return allow_readwrite(IPC_DRIVER_NUM, pid, base, len);
}

