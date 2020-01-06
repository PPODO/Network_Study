#include <iostream>
#include <unistd.h>
#include <signal.h>

void print_signal(int signal) {
    std::cout << "HIHI! : " << signal << std::endl;
}

int main() {
    int i = 0;
    std::cout << "Process ID : " << getpid() << std::endl;

    // 특정 신호에 대한 처리 방식을 결정하는 함수.
    // 첫 번째 인수 : 신호 타입(SIGHUP -> 터미널을 잃어버렸을 때(프로세스가 실행되어 있는 상태에서 콘솔창을 닫음), SIGABRT -> 프로그램이 비정상적으로 종료되었을 때
    //                      SIGINT -> Ctrl + c 키를 입력할 때 발생, SIGIO -> 비동기적인 입력이 발생할 때, SIGKILL -> 프로세스를 강제로 종료할 때 
    //                      SIGSEGV -> 다른 프로세스의 메모리를 침범했을 때, SIGSTOP -> 프로세스를 일시중지할때, SIGCONT -> 일시정지한 프로세스를 다시 움직이게 할 때)
    // 두 번째 인수 : 해당 신호가 발생했을때의 처리 방식
    // 시그널을 받은 프로세스는 종료, 무시, 함수 호출 중 한가지의 작업을 진행한다.
    //signal(SIGINT, SIG_IGN);
    signal(SIGINT, print_signal);

    while(true) {
        std::cout << i << std::endl;
        i++;
        sleep(1);
    }
    return 0;
}