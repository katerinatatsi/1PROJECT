#ifndef SHARED_VAMANA_HPP
#define SHARED_VAMANA_HPP

#include <semaphore.h>

struct VamanaSharedMemory {
    float filtered_vamana_recall;
    float stitched_vamana_recall;
    bool filtered_complete;
    bool stitched_complete;
    sem_t filtered_sem;
    sem_t stitched_sem;
    sem_t results_mutex;
};

// Function declarations
int create_vamana_shared_memory(key_t key);
VamanaSharedMemory* attach_vamana_shared_memory(int shmid);
void detach_vamana_shared_memory(VamanaSharedMemory* shm);
void destroy_vamana_shared_memory(int shmid);
void init_vamana_shared_memory(VamanaSharedMemory* shm);

#endif
