#include <iostream>
#include <sys/types.h>
#include <sys/shm.h>

int main() { 
    key_t KeyVal = 2345;
    
    // 첫 번째 인수 : 공유 메모리는 커널에서 관리하기 때문에 여러 프로세스가 공유메모리를 가질 수 있고, 커널이 이를 관리하게 된다. 각 공유메모리마다 키 값을 부여해서 접근 할 수 있도록 하는 메커니즘.
    // stl의 map container의 작동방식과 비슷?
    // 두 번째 인수 : 공유 메모리 사이즈
    // 세 번째 인수 : 공유 메모리의 접근 방식과 권한. (IPC_CREAT - 공유메모리 생성, 이미 존재한다면 해당 공유메모리의 식별자를 리턴, IPC_EXCL - IPC_CREATE와 함께 사용, key값을 가지는 공유메모리가 존재
    // 한다면 에러를 확인할 수 있다.)
    // 공유메모리는 일반 파일과 마찬가지로 권한을 가질 수 있다.
    int shmID = shmget(KeyVal, 1024, 0666 | IPC_CREAT);
    if(shmID == -1) {
        std::cout << "Failed To Create Shared Memory!\n";
        return -1;
    }
    
    // 공유 메모리를 조작할 때 사용하는 함수. (삭제 및 정보 가져오기 등)
    // 첫 번째 인수 : 메모리 식별자
    // 두 번째 인수 : 메모리 제어값(IPC_SET - 공유메모리의 권한 변경을 위해서 사용된다, IPC_RMID - 공유메모리를 삭제한다. 바로 삭제되는 건 아니고, 해당 메모리에 접근하고 있는 프로세스의 수가 0이 될 때
    // 삭제된다.)
    // 세 번째 인수 : 가져온 메모리의 정보를 저장하는 구조체.
/*    if(shmctl(shmID, IPC_RMID, NULL) == -1) {
        std::cout << "Failed To Remove Shared Memory!\n";
        return -1;
    }*/

    // ipcs -m 명령어를 통해 커널이 관리하는 IPC(-m -> 공유메모리)를 확인 할 수 있다.
    return 0;
}