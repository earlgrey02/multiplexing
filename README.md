## I/O Multiplexing with Kqueue

> **Kqueue를 통해 구현해보는 I/O Multiplexing**

## Description

### I/O Multiplexing

I/O 멀티플렉싱(I/O Multiplexing)이란 하나의 프로세스로 여러 파일 디스크럽터(File Descriptor)를 관리하는 기법이다.
서버 및 클라이언트 구조에서 설명하자면, 하나의 스레드로 여러 요청을 처리할 수 있도록 하는 기법이라는 것이다.<br/>
해당 기법을 Linux에서 구현하는 방법으로는 `select()`, `poll()`, `epoll()` 등이 있다.

이 방법들 중에서 `select()`와 `poll()`은 파일 디스크럽터들의 상태를 커널(Kernel)이 아닌 유저 스페이스(User Space)에서 관리한다.
그래서 커널과 유저 스페이스 간 통신이 많아져 비효율적이다.<br/>
이에 비해 `epoll()`은  커널에서 파일 디스크럽터들의 상태를 관리해 오버헤드가 없다는 이점을 가진다.

그래서 해당 예제에서는 `epoll()`을 통해 I/O 멀티플렉싱을 구현한다. 
그러나 본인의 운영체제가 MacOS 관계로, BSD 운영체제의 `epoll()`인 `kqueue()`를 사용한다.

### Java NIO

I/O 멀티플렉싱을 활용한 대표적인 예시로 Java NIO(New I/O)가 있다.
Java NIO의 `Selector`가 `epoll()`과 `kqueue()`를 통해 I/O 멀티플렉싱을 구현하며, 이를 통해 단일 스레드로 여러 소켓 통신을 처리할 수 있다.<br/>
대표적으로 Spring WebFlux에서 사용되는 Netty는 `Selector` 기반 이벤트 루프를 통해 비동기 및 논블로킹(Non-blocking) I/O를 구현한다.
