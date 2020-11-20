#include <fcntl.h>
#include <microhttpd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

static enum MHD_Result
handler(void *ctx,
    struct MHD_Connection *conn,
    const char *url,
    const char *method,
    const char *version,
    const char *upload_data,
    size_t *upload_data_size,
    void **con_ctx) {
  (void)url;
  (void)method;
  (void)version;
  (void)upload_data;
  (void)upload_data_size;
  (void)con_ctx;

  int fd = open(ctx, O_RDONLY);
  if (fd < 0) {
    return MHD_NO;
  }
  struct stat s;
  if (fstat(fd, &s)) {
    return MHD_NO;
  }
  struct MHD_Response *resp = MHD_create_response_from_fd(s.st_size, fd);
  MHD_add_response_header(resp, MHD_HTTP_HEADER_CONTENT_TYPE, "application/x-ns-proxy-autoconfig");
  enum MHD_Result res = MHD_queue_response(conn, MHD_HTTP_OK, resp);
  MHD_destroy_response(resp);
  return res;
}

int
main(int argc, const char **argv) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s <proxy.pac> [port]\n", argv[0]);
    exit(1);
  }
  const char *file = argv[1];
  int fd = open(file, O_RDONLY);
  if (fd < 0) {
    perror(file);
    exit(2);
  }
  int port = (argc >= 3) ? atoi(argv[2]) : 8888;
  struct MHD_Daemon *d = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION, port,
      NULL, NULL,
      &handler, (void*)file,
      MHD_OPTION_END);
  if (!d) {
    fprintf(stderr, "ERROR\n");
    exit(3);
  }
  while (1) {
    pause();
  }
  MHD_stop_daemon(d);
  return 0;
}
      
