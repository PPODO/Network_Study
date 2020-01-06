#include <iostream>
#include <sys/ipc.h>
#include <sys/sem.h>

const key_t SEMAPHORE_KEY_VALUE = 1063;

int main() {
    // 세마포어 또한 커널에서 관리하기 때문에 ~~~get함수 사용
    // 첫 번째 인수 : 세마포어의 키 값
    // 두 번째 인수 : 한 집합에 여러개의 세마포어가 존재할 수 있음. 그 세마포어의 수를 결정하는 부분.
    // 세 번재 인수 : 생성 방식 결정과 접근 관리
    int Semaphore = semget(SEMAPHORE_KEY_VALUE, 1, IPC_CREAT | 0666);
    if(Semaphore == -1) {
        std::cout << "Failed To Create Semaphore\n";
        return -1;
    }

    // 세마포어를 조작하는 함수. 세마포어를 초기화 할때에도 사용한다.
    // 첫 번째 인수 : 세마포어의 값
    // 두 번째 인수 : 집합에서의 세마포어 위치
    // 세 번째 인수 : 명렁어
    // 네 번째 인수 : 들어갈 값 
    // struct union semun{
	//              int val;    // SETVAL의 인수
    //              struct semid_ds *buf;   // IPC_STAT, IPC_SET의 버퍼
    //              unsigned short int *array;   // GETALL, SETALL 사용시의 변수 배열
    //              struct seminfo *__buf;   // IPC_INFO의 버퍼
    // }
    if(semctl(Semaphore, 0, SETVAL, 1) == -1) {
        std::cout << "Failed To Initialize Semaphore\n";
        return -1;
    }

    // 세마포어를 제어하기 위한 구조체
    sembuf buffer;
    buffer.sem_num = 0; // 집합에서의 세마포어의 위치
    buffer.sem_op = -1;  // 잠금을 얻을지 돌려줄지에 대한 값(-1 -> 얻음, 1 -> 돌려줌)
    buffer.sem_flg = SEM_UNDO; // 작동방식(IPC_NOWAIT -> 세마포어를 사용할 수 없을 때 대기하지 않고 바로 리턴함., SEM_UNDO -> 임계영역에 들어간 상태에서 프로세스를 종료시키면
                               //커널이 자동적으로 세마포어를 회수할 수 있도록 함.)
    if(semop(Semaphore, &buffer, 1) == -1) {
        std::cout << "Failure!\n";
        return -1;
    }

    if(semctl(Semaphore, 0, IPC_RMID) == -1) {
        std::cout << "Failed To Remove Semaphore!\n";
        return -1;
    }
    return 0;
}