#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdexcept>
#include "../hpp/shared_vamana.hpp"

void init_vamana_shared_memory(VamanaSharedMemory* shm) {
    shm->filtered_vamana_recall = 0.0;
    shm->stitched_vamana_recall = 0.0;
    shm->filtered_complete = false;
    shm->stitched_complete = false;
    
    sem_init(&shm->filtered_sem, 1, 0);
    sem_init(&shm->stitched_sem, 1, 0);
    sem_init(&shm->results_mutex, 1, 1);
}

int create_vamana_shared_memory(key_t key) {
    int shmid = shmget(key, sizeof(VamanaSharedMemory), IPC_CREAT | 0666);
    if (shmid == -1) {
        throw std::runtime_error("Failed to create shared memory");
    }
    
    VamanaSharedMemory* shm = attach_vamana_shared_memory(shmid);
    init_vamana_shared_memory(shm);
    detach_vamana_shared_memory(shm);
    return shmid;
}

VamanaSharedMemory* attach_vamana_shared_memory(int shmid) {
    void* shm_ptr = shmat(shmid, nullptr, 0);
    if (shm_ptr == (void*)-1) {
        throw std::runtime_error("Failed to attach shared memory");
    }
    return static_cast<VamanaSharedMemory*>(shm_ptr);
}

void detach_vamana_shared_memory(VamanaSharedMemory* shm) {
    if (shmdt(shm) == -1) {
        throw std::runtime_error("Failed to detach shared memory");
    }
}

void destroy_vamana_shared_memory(int shmid) {
    VamanaSharedMemory* shm = attach_vamana_shared_memory(shmid);
    sem_destroy(&shm->filtered_sem);
    sem_destroy(&shm->stitched_sem);
    sem_destroy(&shm->results_mutex);
    
    detach_vamana_shared_memory(shm);
    if (shmctl(shmid, IPC_RMID, nullptr) == -1) {
        throw std::runtime_error("Failed to destroy shared memory");
    }
}