#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cassert>
#include <iostream>



// Define the size of the shared memory segment
struct __attribute__((__packed__)) DummyStruct{
  char m_char;
  int  m_int;
  int* ptr;
};
constexpr size_t SHM_SIZE = sizeof(DummyStruct);

int main() {

  int fd = shm_open("/my_shared_memory", O_CREAT | O_RDWR, 0666);
  ftruncate(fd, SHM_SIZE);

  void *shared_memory =
      mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (shared_memory == MAP_FAILED) {
      perror("mmap");
      return 1;
  }

  DummyStruct *q = static_cast<DummyStruct *>(shared_memory);
  q->m_char = 'a';
  q->m_int = 12;
  q->ptr = &(q->m_int);
  std::cout
    << "Producer sent data, " << q->ptr
    << std::endl;

  // Clean up and exit
  munmap(shared_memory, SHM_SIZE);
  close(fd);


  return 0;
}
