개발자: wonjaeyeon
작성 날짜: 2023/11/13

수정 및 작성한 파일들에 대한 설명:

1. defs.h

    파일 설명: defs.h 안에 int set_proc_priority(int pid, int priority), int get_proc_priority(int pid), int cps(void) 함수를 선언하여 새로운 시스템 콜 set_proc_priority, get_proc_priority, cps를 사용할 수 있도록 했습니다. 

	set_proc_priority: 특정 PID를 가진 프로세스의 우선순위를 설정합니다.

	get_proc_priority: 특정 PID를 가진 프로세스의 현재 우선순위를 조회합니다.

	cps: 시스템의 모든 프로세스 상태를 보여주는 함수입니다.

이러한 추가 기능을 통해 xv6 운영체제의 프로세스 관리 및 상태 확인이 가능하게 됩니다.
    

2. user.h

    파일 설명: user.h 안에 xv6 운영체제의 시스템 콜을 정의하고 있으며, 특히 set_proc_priority, get_proc_priority, 그리고 cps라는 세 가지 새로운 시스템 콜이 추가되었습니다.

	set_proc_priority(int pid, int priority): 이 시스템 호출은 특정 프로세스의 우선순위를 설정하는 기능을 합니다. 사용자는 프로세스 식별자(PID)와 새로운 우선순위 값을 제공하여 해당 프로세스의 스케줄링 우선순위를 조정할 수 있습니다.

	get_proc_priority(int pid): 이 함수는 지정된 PID를 가진 프로세스의 현재 우선순위를 반환합니다. 이를 통해 프로세스의 스케줄링 상태를 모니터링하고 관리하는 데 도움이 됩니다.

	cps(void): 현재 실행 중인 모든 프로세스의 상태를 출력하는 시스템 호출입니다. 이 기능은 시스템의 프로세스 관리 및 스케줄링 상태를 실시간으로 파악하는 데 유용합니다.
    

3. proc.h

    파일 설명: proc.h 파일에서 struct proc 구조체에 두 개의 새로운 필드인 priority와 run_count가 추가된 것입니다. 이러한 변경은 프로세스 관리 및 스케줄링에 대한 보다 세밀한 제어를 가능하게 합니다:

	int priority : 이 필드는 프로세스의 우선순위를 저장합니다. 우선순위는 프로세스 스케줄링에 있어 중요한 요소로 작용하며, 낮은 값이 높은 우선순위를 나타냅니다. 이를 통해 시스템은 중요한 작업에 더 높은 실행 우선순위를 부여할 수 있습니다.

	int run_count : run_count 필드는 특정 프로세스가 얼마나 자주 실행되었는지를 추적합니다. 이 값은 프로세스가 CPU에 할당될 때마다 증가하며, 이를 통해 프로세스의 활동 빈도와 실행 패턴을 파악할 수 있습니다. 특히, 기아(Starvation) 현상을 겪고 있는 프로세스를 식별하는 데 유용하게 사용됩니다.

	이러한 추가된 필드들은 xv6 운영체제의 프로세스 스케줄링 메커니즘을 테스트 및 모니터링 하는 데 중요한 역할을 합니다. 우선순위 필드를 통해 프로세스 간의 실행 우선순위를 조정할 수 있으며, run_count를 통해 프로세스의 실행 빈도와 기아 상태를 모니터링할 수 있습니다.

4. proc.c

    파일 설명: proc.c 안에 static struct proc *allocproc(void), void userinit(void), int fork(void), void scheduler(void)를 수정하여 priority와 run_count의 값을 초기화 및 사용하여 프로세스 스케줄러를 모니터링 할 수 있도록 변경했습니다.

	static struct proc *allocproc(void) : p->priority = 5;, p->run_count = 0;와 같이 프로세스를 할당할 때 priority와 run_count 값 설정

	void userinit(void) : p->priority = 5; p->priority = 5;, p->run_count = 0;와 같이 프로세스를 할당할 때 priority와 run_count 값 초기화

	int fork(void) : np->priority = curproc->priority;와 같이 부모 프로세스에서 자식 프로세스가 생성될 때 부모 프로세스의 priority 값을 복사

	void scheduler(void) : 1. 기존에 있던 Round-Robin 방식 2-1. 1차적으로 제작한 선점형 Priority 스케줄링 방식 2-2. Starvation 확인을 위해 run_count 로직을 추가한 Priority 스케줄링 방식 3. Starvation 문제를 해결한 Priority 스케줄링 방식

5. syscall.h

    파일 설명: syscall.h 안에 "#define SYS_set_proc_priority 23, #define SYS_get_proc_priority 24, #define SYS_cps 25"와 같이 시스템 콜 고유 번호를 추가했습니다. SYS_set_proc_priority는 set_proc_priority, SYS_get_proc_priority는 get_proc_priority, SYS_cps는 cps 시스템 콜에 대한 고유 번호로, 각각 23,24,25를 가집니다. 이는 set_proc_priority, get_proc_priority, cps 함수를 사용자 프로그램에서 호출할 때 사용되는 식별자입니다. 사용자 프로그램이 set_proc_priority, get_proc_priority, cps 시스템 콜을 요청할 때, 내부적으로는 각각 23,24,25라는 번호를 사용하여 해당 시스템 콜을 커널에 요청하게 됩니다.


6. syscall.c

    파일 설명: syscall.c 안에 sys_set_proc_priority, sys_get_proc_priority, sys_cps라는 함수의 프로토타입을 선언했습니다. 또한 각 함수는 syschall.h에서 정의한 상수 (시스템 호출 번호)를 사용하여 해당 함수를 참조합니다. 따라서 사용자 프로그램이 원하는 시스템 호출을 요청하면 해당 함수가 호출됩니다.

7. usys.S

    파일 설명: usys.S 파일에 SYSCALL(set_proc_priority), SYSCALL(get_proc_priority), SYSCALL(cps)를 추가함으로써, 사용자 프로그램은 함수를 호출하여 해당 시스템 콜을 실행할 수 있게 되었습니다. SYSCALL 함수는 내부적으로 T_SYSCALL 인터럽트를 발생시켜 실제 시스템 콜 처리를 커널에 위임합니다.

8. sysproc.c

    파일 설명: sys_set_proc_priority(void), sys_get_proc_priority(void), sys_cps(void)를 추가했다. 해당 함수들은 사용자 프로그램에 시스템 콜을 호출할 때 실행됩니다. 이 함수의 목적은 사용자 프로그램에서 제공된 인자를 검증하고, 실제 해당 함수를 호출하는 것입니다.해당 함수는 사용자 프로그램에서 제공된 인자를 검증하고, 해당 인자를 사용하여 해당 함수를 호출하는 역할을 합니다.

9. forknexec.c

    파일 설명: 이 파일은 forknexec 시스템 콜을 테스트하는 사용자 프로그램입니다. "echo" 명령어와 "NEWPROC_CALL" 문자열을 인자로 사용하여 forknexec를 호출하고, 그 결과를 출력합니다.

10. getprocpriority.c

    파일 설명: 이 파일은 get_proc_priority 시스템 호출을 테스트하는 사용자 프로그램입니다. 사용자가 제공한 PID(process identifier)를 입력 받아, 해당 프로세스의 우선순위를 조회하고 출력합니다. 명령어 사용 방식은 getprocpriority pid이며, 입력된 PID에 해당하는 프로세스의 우선순위를 화면에 표시합니다. 유효하지 않은 PID가 입력되거나 프로세스의 우선순위를 가져오는 데 실패할 경우 오류 메시지를 출력합니다.

11. setprocpriority.c

    파일 설명: 이 파일은 set_proc_priority 시스템 호출을 사용하는 사용자 프로그램입니다. 이 프로그램은 사용자로부터 프로세스 ID(PID)와 새로 설정할 우선순위 값을 입력받아, 지정된 프로세스의 우선순위를 변경합니다. 사용 방법은 nice pid priority이며, 여기서 pid는 프로세스 식별자, priority는 1부터 10 사이의 새로운 우선순위입니다. 유효하지 않은 우선순위 값(1-10 범위를 벗어난 값)이 입력되면 오류 메시지를 출력하고 프로그램이 종료됩니다. 정상적인 입력이 제공되면, 해당 PID의 프로세스 우선순위를 설정하고, 설정된 PID와 우선순위를 화면에 출력합니다. 이 프로그램을 통해 사용자는 시스템의 프로세스 스케줄링을 직접 조정할 수 있게 됩니다.

12. ptest.c

    파일 설명: 이 파일은 프로세스 생성 및 CPU 시간 소모를 시뮬레이션하는 사용자 프로그램입니다. 이 프로그램은 사용자가 ptest&;의 횟수를 통해 해당 수만큼의 자식 프로세스들을 생성합니다. 각 자식 프로세스는 랜덤한 범위의 계산 작업을 수행하여 CPU 시간을 소모합니다. 생성된 모든 프로세스들은 priority 값이 5입니다.

13. ptest2.c

    파일 설명: 이 파일은 프로세스 생성과 랜덤한 우선순위 설정을 결합하여 시뮬레이션하는 사용자 프로그램입니다. 이 프로그램은 사용자가 ptest2&;의 횟수를 통해 해당 수만큼의 자식 프로세스들을 생성합니다. 각 자식 프로세스에 랜덤하게 설정된 우선순위를 부여합니다.

14. ps.c

    파일 설명: 이 파일은 xv6 운영체제에서 실행 중인 모든 프로세스의 상태를 보여주는 사용자 프로그램입니다. 이 프로그램은 운영체제의 프로세스 테이블을 순회하며, 각 프로세스의 이름, PID, 상태, 우선순위, 그리고 실행 횟수(run_count)를 출력합니다.


15. Makefile 수정

    변경 내용 1 : 새로운 프로그램인 ptest, ptest2, ps, getprocpriority, setprocpriority라는 새로운 사용자 프로그램들을 xv6의 사용자 프로그램 목록에 추가했습니다.
	
    추가된 코드: "_ptest", "_ptest2", "_ps", "_getprocpriority", "_setprocpriority" 추가

	UPROGS=\
	    ... [already implemented in XV6]
	    _ptest\
    	    _ptest2\
    	    _ps\
    	    _getprocpriority\
    	    _setprocpriority\

    이렇게 수정함으로써, xv6를 빌드할 때 ptest, ptest2, ps, getprocpriority, setprocpriority 프로그램들도 함께 컴파일되고, 파일 시스템 이미지에 포함됩니다.


    변경 내용 2 : 'EXTRA' 변수에 'ptest.c', 'ptest2.c', 'ps.c', 'getprocpriority.c', 'setprocpriority.c'를 포함했습니다.

    추가된 코드 : 'ptest.c', 'ptest2.c', 'ps.c', 'getprocpriority.c', 'setprocpriority.c' 추가

	EXTRA=\
    	    ... [already implemented in XV6]
    	    ptest.c\
    	    ptest2.c\
    	    ps.c\
    	    getprocpriority.c\
    	    setprocpriority.c\


    이 수정을 통해, ptest, ptest2, ps, getprocpriority, setprocpriority 소스 파일들이 배포용 디렉토리에 추가되었습니다. 
