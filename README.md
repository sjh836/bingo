# C소켓 기반 빙고 네트워크 게임

## 개발환경
* OS : CentOS 7
* gcc : gcc (GCC) 4.8.5 20150623 (Red Hat 4.8.5-11)

## 어플리케이션 프로토콜
서버와 클라이언트는 매 턴마다 turn[]이라는 배열을 주고 받으며 통신하고 동기화한다. turn[]은 클라이언트는 수신만 할 뿐이며, 모든 연산은 서버에서 이루어져서 클라이언트에게 보내준다.

* turn[0] = 플레이어 숫자선택
* turn[1] = 클라이언트 빙고 수
* turn[2] = 서버 빙고 수
* turn[3] = 게임종료여부(0=진행중, 1=클라이언트 승리, 2=서버 승리, 3=무승부)

## 과제 제약사항
* 1대1서버 혹은 순차서버(다중서버X)
* 리눅스 기반

### 즐겜
1. 게임시작
![게임시작](http://img1.daumcdn.net/thumb/R1920x0/?fname=http%3A%2F%2Fcfile8.uf.tistory.com%2Fimage%2F2418845058F75E5B2CBAE1)
2. 클라이언트 승리
![게임시작](http://img1.daumcdn.net/thumb/R1920x0/?fname=http%3A%2F%2Fcfile30.uf.tistory.com%2Fimage%2F261D895058F75E5F2011D4)
3. 서버 승리
![게임시작](http://img1.daumcdn.net/thumb/R1920x0/?fname=http%3A%2F%2Fcfile7.uf.tistory.com%2Fimage%2F2310575058F75E5D0539EC)
4. 무승부
![게임시작](http://img1.daumcdn.net/thumb/R1920x0/?fname=http%3A%2F%2Fcfile6.uf.tistory.com%2Fimage%2F2333545058F75E5C296CF7)