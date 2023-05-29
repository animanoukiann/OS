#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>

class Function {
public:
    int id;
    int arg_1;
    int arg_2;
    int result;

    Function(int id, int arg_1, int arg_2): id(id), arg_1(arg_1), arg_2(arg_2), result(0){}
};

int main()
{
    const char* name = "/rpc_shm";
    int fd = shm_open(name, O_RDWR, 0);
    if (fd == -1)
    {
        std::cerr << "File open failed\n";
        exit(errno);
    }
    int shm_size = sizeof(Function);
    Function* adr = (Function*)mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (adr == MAP_FAILED)
    {
        std::cerr << "Mmap failed\n";
        exit(errno);
    }

    const char* sem1_name = "/request_sem";
    const char* sem2_name = "/result_sem";
    sem_t* sem1 = sem_open(sem1_name, 0);
    sem_t* sem2 = sem_open(sem2_name, 0);
    if (sem1 == SEM_FAILED || sem2 == SEM_FAILED)
    {
        std::cerr << "Semaphore open failed\n";
        exit(errno);
    }

    int functionid;
    int number1, number2;

    while (true)
    {
        std::cout << "Choose the function id:\nadd-0\nsub-1\nmul-2\ndiv-3\n";
        std::cin >> functionid;
        std::cout << "Enter the arguments\n";
        std::cin >> number1 >> number2;

        Function func(functionid, number1, number2);
        *adr = func;

        sem_post(sem1);
        sem_wait(sem2);

        std::cout << "The result is: " << adr->result << std::endl;
        sleep(1);
    }

    munmap(adr, shm_size);
    close(fd);

    return 0;
}
