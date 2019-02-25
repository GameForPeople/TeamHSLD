using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public enum TUTORIAL
{
    READY_SELECTORDERCARD,      //선 후공 카드 선택
    READY_SELECTCARD,           //덱 구성

    INGAME_ROLLINGDICE,         //주사위 굴리기
    INGAME_SELECTTERRAINCARD,   //지형 카드 선택
    INGAME_SELECTFLAG,          //거점 정하기
    INGAME_SELECTTERRAINLOC,    //지형 설치하기

    INGAME_SELECTLOCDONE,       //지형설치완료
    INGAME_TURNEND,             //턴을 종료하고, 상대턴으로 변경

    INGAME_ATTACK,              //공격/방어하기
    INGAME_GETEVENTCARD,        //이벤트카드 얻기
    INGAME_USEEVENTCARD,        //이벤트카드 사용하기

    ENDGAME_CONDITIONTOWIN      //승리조건
}

public class TutorialManager : MonoBehaviour
{
    public TUTORIAL currentTutorial;

    public GameObject displayTextObj;
    public GameObject pointObj1;
    public GameObject pointObj2;
    public GameObject pointObj3;
    public GameObject pointObj4;

    static public int index = 0;

    static public bool isCheck = false;

    IEnumerator READY_SELECTCARD_Cor()
    {
        pointObj1.SetActive(false);
        pointObj3.SetActive(true);
        pointObj3.transform.localPosition = new Vector3(464,-163,0);
        pointObj3.transform.localScale = new Vector3(2.71f, 2.71f, 2.71f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "1분 동안 최대 30장 이내에서";
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-665, 225, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(12, 225, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 225, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 3f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(3.5f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "덱을 자유롭게 구성할 수 있습니다.";
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-665, 225, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(12, 225, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 225, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 3f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(3.5f);
        pointObj3.SetActive(false);
        pointObj1.SetActive(true);
        pointObj1.transform.localPosition = new Vector3(263,95,0);
        pointObj1.transform.localScale = new Vector3(1.58f, 1.58f, 1.58f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "위쪽의 3종류의 카드는 일반 지형 카드로,";
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(10, 335, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(261, 335, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 550;
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(266, 339, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        pointObj1.transform.localPosition = new Vector3(263, 95, 0);
        pointObj1.transform.localScale = new Vector3(1.58f, 1.58f, 1.58f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "각각 속성이 있으며, 공격이 가능합니다.";
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(10, 335, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(261, 335, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 550;
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(266, 339, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        pointObj1.transform.localPosition = new Vector3(170,-149,0);
        pointObj1.transform.localScale = new Vector3(1.08f, 1.08f, 1.08f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "아래쪽의 2종류의 카드는 방어 지형 카드로,";
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(10, 92, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(261, 92, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 550;
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(266, 88, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        pointObj1.transform.localPosition = new Vector3(170, -149, 0);
        pointObj1.transform.localScale = new Vector3(1.08f, 1.08f, 1.08f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "상대방의 공격을 방어할 수 있습니다.";
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(10, 92, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(261, 92, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 550;
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(266, 88, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        pointObj1.SetActive(false);
        pointObj2.SetActive(true);
        pointObj2.transform.localPosition = new Vector3(-333,-37,0);
        pointObj2.transform.localScale = new Vector3(3.25f, 3.25f, 3.25f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "승리 조건은 게임 승리에 중요한 역할을 합니다.";
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-665, 225, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(12, 225, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 225, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 3.5f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(4.0f);
        pointObj2.transform.localPosition = new Vector3(-333, -37, 0);
        pointObj2.transform.localScale = new Vector3(3.25f, 3.25f, 3.25f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "게임에 유리하게 덱을 구성해보세요!";
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-665, 225, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(12, 225, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 225, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 3f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(3.5f);
        pointObj2.SetActive(false);
        yield return new WaitForSeconds(1f);
        PointMove(new Vector3(62, 187, 0));
    }

    public void PointMove(Vector3 pos)
    {
        for (int i = 0; i < 5; i++)
            gameObject.GetComponent<TutorialFlowSystem>().cardSetCanvas.transform.GetChild(i).transform.GetChild(0).GetComponent<Image>().raycastTarget = false;

        gameObject.GetComponent<TutorialFlowSystem>().cardSetCanvas.transform.GetChild(index).transform.GetChild(0).GetComponent<Image>().raycastTarget = true;
        pointObj3.transform.localScale = new Vector3(1, 1, 1);
        pointObj3.SetActive(true);
        pointObj3.transform.localPosition = pos;
        //Debug.Log(index);
        index += 1;
    }

    public void PointArrowMove(Vector3 pos)
    {
        pointObj4.SetActive(true);
        pointObj4.transform.localPosition = pos;
        index += 1;
    }

    public void pointOff()
    {
        pointObj1.SetActive(false);
        pointObj2.SetActive(false);
        pointObj3.SetActive(false);
        pointObj4.SetActive(false);
        index = 0;
    }

    IEnumerator INGAME_ROLLINGDICE_Cor()
    {
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-635, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "게임이 시작되었습니다!";
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-635, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "제일 먼저 주사위를 굴려줍니다.";
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-635, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "게이지가 꽉 찰수록 높은 숫자가 나올 확률이 높아집니다.";
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(1);
        pointObj3.SetActive(true);
        pointObj3.transform.localPosition = new Vector3(484, -194, 0);
        pointObj3.transform.localScale = new Vector3(3.84f, 3.84f, 3.84f);
        yield return new WaitForSeconds(1.5f);
        index = 5;
    }

    IEnumerator INGAME_SELECTTERRAINCARD_Cor()
    {
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-635, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "그 다음, 나의 덱에서 설치하고 싶은 지형을 고르세요.";
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        pointObj3.SetActive(true);
        pointObj3.transform.localPosition = new Vector3(-193, -304, 0);
        pointObj3.transform.localScale = new Vector3(1.23f, 1.23f, 1.23f);
        yield return new WaitForSeconds(2.5f);
        index = 6;
    }

    IEnumerator INGAME_SELECTFLAG_Cor()
    {
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-635, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "첫 턴에서는 가장 먼저 '거점'을 점령해야 합니다.";
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-635, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "거점은 총 4곳에 위치해 있습니다.";
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-635, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "그럼, 가장 가까운 거점에서 시작해볼까요 ?";
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        
        index = 7;
    }

    IEnumerator INGAME_SELECTTERRAINLOC_Cor()
    {
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-635, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "거점을 정했다면, 거점 주위에 지형을 생성시켜서";
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-635, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "거점을 완벽하게 점령해보세요.";
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);

        index = 8;
        PointArrowMove(new Vector3(0, 0, 0));
    }

    IEnumerator INGAME_SELECTLOCDONE_Cor()
    {
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-635, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "거점을 완벽하게 점령했습니다!";
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-635, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "거점을 완벽하게 점령한 다음부터";
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-635, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "다른 곳도 점령할 수 있습니다.";
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
    }

    IEnumerator INGAME_TURNEND_Cor()
    {
        yield return new WaitForSeconds(2.5f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-635, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "상대 턴일 때에는 상대방의 주사위 숫자와";
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-635, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "지형 설치를 볼 수 있습니다.";
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
    }

    public void DoingTutorial(TUTORIAL tutorial)
    {
        switch (tutorial)
        {
            case TUTORIAL.READY_SELECTORDERCARD:
                currentTutorial = TUTORIAL.READY_SELECTORDERCARD;
                pointObj1.SetActive(true);
                pointObj1.transform.localPosition = new Vector3(0, 0, 0);
                pointObj1.transform.localScale = new Vector3(1.63f, 1.63f, 1.63f);
                displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-635, -20, 0);
                displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(0, -20, 0);
                displayTextObj.GetComponent<TutorialDisplayTxt>().inputColor = new Color32(58, 58, 58, 255);
                displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, -20, 0);
                displayTextObj.GetComponent<TutorialDisplayTxt>().text = "순서를 결정하기 위해 카드를 선택하세요.";
                displayTextObj.SetActive(true);
                break;
            case TUTORIAL.READY_SELECTCARD:
                currentTutorial = TUTORIAL.READY_SELECTCARD;
                displayTextObj.GetComponent<TutorialDisplayTxt>().inputColor = new Color32(0, 0, 0, 170);
                StartCoroutine(READY_SELECTCARD_Cor());
                break;
            case TUTORIAL.INGAME_ROLLINGDICE:
                currentTutorial = TUTORIAL.INGAME_ROLLINGDICE;
                displayTextObj.GetComponent<TutorialDisplayTxt>().inputColor = new Color32(0, 0, 0, 170);
                StartCoroutine(INGAME_ROLLINGDICE_Cor());
                break;
            case TUTORIAL.INGAME_SELECTTERRAINCARD:
                displayTextObj.GetComponent<TutorialDisplayTxt>().inputColor = new Color32(0, 0, 0, 170);
                StartCoroutine(INGAME_SELECTTERRAINCARD_Cor());
                currentTutorial = TUTORIAL.INGAME_SELECTTERRAINCARD;
                break;
            case TUTORIAL.INGAME_SELECTFLAG:
                StartCoroutine(INGAME_SELECTFLAG_Cor());
                displayTextObj.GetComponent<TutorialDisplayTxt>().inputColor = new Color32(0, 0, 0, 170);
                currentTutorial = TUTORIAL.INGAME_SELECTFLAG;
                break;
            case TUTORIAL.INGAME_SELECTTERRAINLOC:
                StartCoroutine(INGAME_SELECTTERRAINLOC_Cor());
                displayTextObj.GetComponent<TutorialDisplayTxt>().inputColor = new Color32(0, 0, 0, 170);
                currentTutorial = TUTORIAL.INGAME_SELECTTERRAINLOC;
                break;
            case TUTORIAL.INGAME_SELECTLOCDONE:
                StartCoroutine(INGAME_SELECTLOCDONE_Cor());
                displayTextObj.GetComponent<TutorialDisplayTxt>().inputColor = new Color32(0, 0, 0, 170);
                currentTutorial = TUTORIAL.INGAME_SELECTLOCDONE;
                break;
            case TUTORIAL.INGAME_TURNEND:
                displayTextObj.GetComponent<TutorialDisplayTxt>().inputColor = new Color32(0, 0, 0, 170);
                currentTutorial = TUTORIAL.INGAME_TURNEND;
                break;
            case TUTORIAL.INGAME_ATTACK:
                currentTutorial = TUTORIAL.INGAME_ATTACK;
                break;
            case TUTORIAL.INGAME_GETEVENTCARD:
                currentTutorial = TUTORIAL.INGAME_GETEVENTCARD;
                break;
            case TUTORIAL.INGAME_USEEVENTCARD:
                currentTutorial = TUTORIAL.INGAME_USEEVENTCARD;
                break;
            case TUTORIAL.ENDGAME_CONDITIONTOWIN:
                currentTutorial = TUTORIAL.ENDGAME_CONDITIONTOWIN;
                break;
        }
    }
}
