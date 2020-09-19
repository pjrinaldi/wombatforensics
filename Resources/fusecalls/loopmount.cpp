int loopdev_setup_device(const char * file, uint64_t offset, const char * device) {
  int file_fd = open(file, O_RDWR);
  int device_fd = -1; 

  struct loop_info64 info;

  if(file_fd < 0) {
    fprintf(stderr, "Failed to open backing file (%s).\n", file);
    goto error;
  }

  if((device_fd = open(device, O_RDWR)) < 0) {
    fprintf(stderr, "Failed to open device (%s).\n", device);
    goto error;
  }

  if(ioctl(device_fd, LOOP_SET_FD, file_fd) < 0) {
    fprintf(stderr, "Failed to set fd.\n");
    goto error;
  }

  close(file_fd);
  file_fd = -1; 

  memset(&info, 0, sizeof(struct loop_info64)); /* Is this necessary? */
  info.lo_offset = offset;
  /* info.lo_sizelimit = 0 => max avilable */
  /* info.lo_encrypt_type = 0 => none */

  if(ioctl(device_fd, LOOP_SET_STATUS64, &info)) {
    fprintf(stderr, "Failed to set info.\n");
    goto error;
  }

  close(device_fd);
  device_fd = -1; 

  return 0;

  error:
    if(file_fd >= 0) {
      close(file_fd);
    }   
    if(device_fd >= 0) {
      ioctl(device_fd, LOOP_CLR_FD, 0); 
      close(device_fd);
    }   
    return 1;
}
