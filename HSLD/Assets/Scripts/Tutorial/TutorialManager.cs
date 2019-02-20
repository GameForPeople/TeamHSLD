using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum TUTORIAL
{
    READY_SELECTORDERCARD,      //선 후공 카드 선택
    READY_SELECTCARD,           //덱 구성

    INGAME_ROLLINGDICE,         //주사위 굴리기
    INGAME_SELECTTERRAINCARD,   //지형 카드 선택
    INGAME_SELECTFLAG,          //거점 정하기
    INGAME_SELECTTERRAINLOC,    //지형 설치하기

    INGAME_ATTACK,              //공격/방어하기
    INGAME_GETEVENTCARD,        //이벤트카드 얻기
    INGAME_USEEVENTCARD,        //이벤트카드 사용하기

    ENDGAME_CONDITIONTOWIN      //승리조건
}

public class TutorialManager : MonoBehaviour
{
    public TUTORIAL currentTutorial;
    public int index;

    public void DoingTutorial(TUTORIAL tutorial)
    {
        index = 0;
        switch (tutorial)
        {
            case TUTORIAL.READY_SELECTORDERCARD:
                break;
            case TUTORIAL.READY_SELECTCARD:
                break;
            case TUTORIAL.INGAME_ROLLINGDICE:
                break;
            case TUTORIAL.INGAME_SELECTTERRAINCARD:
                break;
            case TUTORIAL.INGAME_SELECTFLAG:
                break;
            case TUTORIAL.INGAME_SELECTTERRAINLOC:
                break;
            case TUTORIAL.INGAME_ATTACK:
                break;
            case TUTORIAL.INGAME_GETEVENTCARD:
                break;
            case TUTORIAL.INGAME_USEEVENTCARD:
                break;
            case TUTORIAL.ENDGAME_CONDITIONTOWIN:
                break;
        }
    }

    private void Update()
    {
        if(Input.anyKeyDown)
        {
            Debug.Log("111");
        }
    }
}
