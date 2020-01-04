#include <iostream>
#include <sys/types.h>
#include <sys/shm.h>

int main() {
    key_t KeyVal = 2345;

    // 이미 생성된 메모리 식별자를 가져옴. 이렇게 써도 되는지는 모르겠음.
    int shmID = shmget(KeyVal, 0, IPC_CREAT);
    if(shmID == -1) {
        std::cout << "Get Shared Memory Failure!\n";
        return -1; 
    }

    // 공유 메모리에 접근하기 위해서 사용하는 함수.
    // 첫 번째 인수 : 공유 메모리 식별자
    // 두 번째 인수 : 공유 메모리의 주소, NULL을 넣게 되면 운영체제가 공유메모리 영역에서 사용할 수 있는 지역을 찾아서 반환해준다. 아마 식별자를 모를 때를 제외한 나머지 상황에서는 전부 NULL을 넣겠지?
    // 세 번째 인수 : 공유 메모리의 접근하려는 프로세스의 권한.
    auto shmAddress = shmat(shmID, NULL, SHM_RDONLY);
    if(shmAddress == (int*)-1) {
        std::cout << "Attach Shared Memory Failure!\n";
        return -1;
    }
    // 공유 메모리를 프로세스에서 떼어낼 때 사용하는 함수.
    // 삭제와는 다름. 해당 프로세스를 공유 메모리와 '분리'시킬 뿐임.
    // 첫 번째 인수 : 공유 메모리의 주소
    shmdt(shmAddress);
    return 0;
}