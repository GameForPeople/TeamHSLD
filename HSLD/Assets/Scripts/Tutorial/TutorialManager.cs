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
    public void pointOff()
    {
        pointObj3.SetActive(false);
        index = 0;
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

    }
}
