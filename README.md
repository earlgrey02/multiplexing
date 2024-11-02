## I/O Multiplexing with Kqueue

> **Kqueue를 통해 구현해보는 I/O 멀티플렉싱(I/O Multiplexing)**

이 레포지토리는 BSD(Berkeley Software Distribution) 계열 운영체제의 `epoll()`인 `kqueue()`를 통해 I/O 멀티플렉싱을 구현한 예제이다.<br/>

## Description

### I/O Multiplexing

I/O 멀티플렉싱이란 하나의 채널로 여러 파일 디스크럽터(File Descriptor)를 관리하는 기법이다.<br/>
해당 기법을 소켓 프로그래밍에 활용하면 하나의 스레드로 여러 요청을 처리하는 고성능 네트워크 애플리케이션을 구현할 수 있다.

I/O 멀티플렉싱을 구현하는 방법으로는 `select()`, `poll()`, `epoll()` 등의 시스템 콜(System call)이 있다.<br/>
이 방법들 중에서 `select()`와 `poll()`은 파일 디스크럽터들의 상태를 커널(Kernel)이 아닌 유저 스페이스(User Space)에서 관리한다.<br/>
그래서 커널과 유저 스페이스 간 통신이 많아져 성능적인 부분에서 단점을 가지는데, 이에 비해 `epoll()`은 커널에서 직접 파일 디스크럽터들의 상태를 관리해 오버헤드가 없으므로 일반적으로 `epoll()`을 사용해 I/O 멀티플렉싱을 구현한다.

### Event Loop

이벤트 루프(Event Loop)는 I/O 멀티플렉싱을 리액터 패턴(Reactor Pattern)으로 구현한 동시성 처리 모델 중 하나로, 반복문을 돌면서 발생한 이벤트들을 확인하고 그에 맞는 처리를 하는 역할을 한다.<br/>
일반적으로 이벤트 루프는 비동기 및 논블로킹(Non-blocking) I/O를 구현하는데 사용되는데, 이는 이벤트 루프가 입출력을 기다리지 않고 입출력이 준비된 채널에 대해서만 입출력을 수행하기에 가능한 것이다.
이러한 입출력의 준비 상태 등은 발생한 이벤트들을 통해 판단하게 되는데, 이 이벤트들을 관리하는 방법이 앞서 설명한 `epoll()` 등의 이벤트 관리 시스템 콜이다.

이벤트 루프를 활용한 대표적인 예시로 Node.js의 구성 요소 중 하나인 [libuv](https://github.com/libuv/libuv)나 Spring WebFlux에서 WAS(Web Application Server)로 사용하는 [Netty](https://github.com/netty/netty)가 있는데, 이들은 모두 `epoll()`이나 `kqueue()` 등의 시스템 콜을 사용해 이벤트 루프를 구현한다.<br/>
이 중 Netty는 직접 `epoll()` 등의 시스템 콜을 사용하는 것은 아니고 이를 추상화한 Java NIO(New I/O)의 `Selector`를 사용한다.

## How to use

I/O 멀티플렉싱을 구현한 코드는 `multiplexing.cpp`의 `EventLoop`이다.<br/>
이에 대한 테스트로 `EventLoop`를 사용해 하나의 스레드로 여러 클라이언트와 소켓 통신을 수행하는 `server.cpp`와 여러 스레드로 서버에 반복적으로 요청을 보내는 `client.cpp`를 사용할 수 있다.
