# 과제2

## 1번 
CTetris는 Tetris 클래스를 상속받아 구현한다.

- Tetris.h와 Tetris.cpp는 수정하지 않는다.
- main 함수는 그대로 실행돼야 한다.
- 빈 칸은 0, 벽은 1로 표현돼야 한다.
- main 함수의 setOfColorBlockArrays 배열에서 블록 1은 0이 아닌 부분을 10으로 표현한다. 즉, 블록은 * 10을 한 값을 저장한다.
- 각 블록은 drawScreen 함수에 의해 서로 다른 특수 문자로 표현된다. 
  
## 2번

- CTetris.h는 수정하지 않는다.
- CTetris::init()를 구현할 것
  - 이 함수 안에서 Tetris::init() 호출하는 코드는 수정하지 말 것
  - Tetris::init() 함수는 setOfBlockArrays 배열이 0과 1 이외의 원소를 갖는 경우에 0 아닌 원소들을 자동으로 1로 변환하는 기능을 가지고 있음
- CTetris::accept()
  - 이 함수 안에서 Tetris::accept() 호출하는 코드는 수정하지 말 것
  - 이 함수 안에서 필요시 다음 함수 호출을 사용할 수 있음
    - oCScreen = deleteFullLines(oCScreen, currCBlk, top, wallDepth);
    - deleteFullLines() 함수는 Tetris.cpp 에서 제공하는 non-member 함수로서 extern 선언되어 외부 파일에서 호출할 수 있음
    - deleteFullLines() 함수는 첫번째 인자가 0과 1 이외의 원소를 갖는 행렬일지라도 정상적으로 삭제 기능을 수행할 수 있음
