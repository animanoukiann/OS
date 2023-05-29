#include <iostream>
#include <queue>
#include <vector>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <fcntl.h>

class Task {
public:
    int arg_1, arg_2, id, result;
    Task(int arg_1, int arg_2, int id) {
        this->arg_1 = arg_1;
        this->arg_2 = arg_2;
        this->id = id;
    }

    void execute_task() {
        switch (this->id) {
            case 0:
                std::cout << "add";
                result = this->arg_1 + this->arg_2;
                break;
            case 1:
                std::cout << "sub";
                result = this->arg_1 - this->arg_2;
                break;
            case 2:
                std::cout << "mul";
                result = this->arg_1 * this->arg_2;
                break;
            case 3:
                std::cout << "div";
                if (this->arg_2 == 0) {
                    exit(EXIT_FAILURE);
                }
                result = this->arg_1 / this->arg_2;
                break;
            default:
                break;
        }
    }
};

class ThreadPool {
private:
    std::queue<Task*> taskQueue;
    std::vector<pthread_t> threads;
    pthread_mutex_t lock;
    pthread_cond_t hasFunction;

public:
    ThreadPool(int threadsCount) {
        if (pthread_mutex_init(&lock, nullptr) < 0) {
            perror("Couldn't init mutex");
        }

        if (pthread_cond_init(&hasFunction, nullptr) < 0) {
            perror("Couldn't init cond_var");
        }

        threads.resize(threadsCount);
        for (int i = 0; i < threadsCount; i++) {
            pthread_create(&threads[i], nullptr, execute, this);
        }
    }

    ~ThreadPool() {
        pthread_cond_destroy(&hasFunction);
        pthread_mutex_destroy(&lock);
        for (int i = 0; i < (int)threads.size(); i++) {
            pthread_kill(threads[i], SIGKILL);
        }
    }

    void addTask(Task* task) {
        pthread_mutex_lock(&lock);
        taskQueue.push(task);
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&hasFunction);
    }

    static void* execute(void* arg) {
        ThreadPool* threadPool = (ThreadPool*)arg;
        while (true) {
            pthread_mutex_lock(&threadPool->lock);
            while (threadPool->taskQueue.empty()) {
                pthread_cond_wait(&threadPool->hasFunction, &threadPool->lock);
            }
            Task* task = threadPool->taskQueue.front();
            threadPool->taskQueue.pop();
            pthread_mutex_unlock(&threadPool->lock);
            task->execute_task();
        }
    }
};

int main() {
    ThreadPool threadPool(5);
      const char* SHARED_MEMORY_NAME = "shared_memory";
    const int SHARED_MEMORY_SIZE = 4096;

    sem_t* request_sem = sem_open("request_sem", O_CREAT, 0644, 0);
    sem_t* result_sem = sem_open("result_sem", O_CREAT, 0644, 0);

    if (request_sem == SEM_FAILED || result_sem == SEM_FAILED) {
        std::cerr << "Could not create semaphore\n";
        exit(EXIT_FAILURE);
    }

    int fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        std::cerr << "File open failed\n";
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, SHARED_MEMORY_SIZE) == -1) {
        std::cerr << "Ftruncate failed\n";
        exit(EXIT_FAILURE);
    }

    Task* sharedTask = (Task*)mmap(NULL, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (sharedTask == MAP_FAILED) {
        std::cerr << "Mmap failed";
        exit(EXIT_FAILURE);
    }

    while (true) {
        sem_wait(request_sem);
        threadPool.addTask(sharedTask);
        sem_post(result_sem);
    }

    munmap(sharedTask, SHARED_MEMORY_SIZE);
    close(fd);
    shm_unlink(SHARED_MEMORY_NAME);

    sem_close(request_sem);
    sem_close(result_sem);
    sem_unlink("request_sem");
    sem_unlink("result_sem");

    return 0;
}
