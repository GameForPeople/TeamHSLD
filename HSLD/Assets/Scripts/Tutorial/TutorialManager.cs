using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

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

    INGAME_ENEMYTURN_ROLLINGDICE,   //상대방이 주사위를 굴림
    INGAME_ENEMYTURN_TURNEND,       //상대방 턴 종료

    INGAME_DICEFOCUS,           //나의턴, 주사위 강조
    INGAME_ROLLINGDICE_2,       //주사위를 굴림 
    INGAME_SELECTTERRAINLOC_2,  //지형설치
    INGAME_TURNEND_2,             //턴을 종료하고, 상대턴으로 변경

    INGAME_ENEMYTURN_ROLLINGDICE_2, //상대방이 주사위를 굴림
    INGAME_ENEMYTURN_TURNEND_2, //상대방 턴 종료

    INGAME_BEFOREATTACK,        //공격전, 주사위굴리기

    INGAME_ATTACK,              //공격/방어하기

    ENDGAME_CONDITIONTOWIN      //승리조건
}

public class TutorialManager : MonoBehaviour
{
    public TUTORIAL currentTutorial;

    public GameObject displayTextObj;
    public GameObject[] pointObj = new GameObject[6];

    private Material domMaterial;

    static public int index = 0;

    static public bool isCheck = false;

    IEnumerator READY_SELECTCARD_Cor()
    {
        SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[16], 1.0f);
        pointObj[0].SetActive(false);
        pointObj[2].SetActive(true);
        pointObj[2].transform.localPosition = new Vector3(464,-163,0);
        pointObj[2].transform.localScale = new Vector3(2.71f, 2.71f, 2.71f);
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
        pointObj[2].SetActive(false);
        pointObj[0].SetActive(true);
        pointObj[0].transform.localPosition = new Vector3(263,95,0);
        pointObj[0].transform.localScale = new Vector3(1.58f, 1.58f, 1.58f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "위쪽의 3종류의 카드는 일반 지형 카드로,";
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(10, 335, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(261, 335, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 550;
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(266, 339, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        pointObj[0].transform.localPosition = new Vector3(263, 95, 0);
        pointObj[0].transform.localScale = new Vector3(1.58f, 1.58f, 1.58f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "각각 속성이 있으며, 공격이 가능합니다.";
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(10, 335, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(261, 335, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 550;
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(266, 339, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        pointObj[0].transform.localPosition = new Vector3(170,-149,0);
        pointObj[0].transform.localScale = new Vector3(1.08f, 1.08f, 1.08f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "아래쪽의 2종류의 카드는 방어 지형 카드로,";
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(10, 92, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(261, 92, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 550;
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(266, 88, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        pointObj[0].transform.localPosition = new Vector3(170, -149, 0);
        pointObj[0].transform.localScale = new Vector3(1.08f, 1.08f, 1.08f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "상대방의 공격을 방어할 수 있습니다.";
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(10, 92, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(261, 92, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 550;
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(266, 88, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        pointObj[0].SetActive(false);
        pointObj[1].SetActive(true);
        pointObj[1].transform.localPosition = new Vector3(-333,-37,0);
        pointObj[1].transform.localScale = new Vector3(3.25f, 3.25f, 3.25f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "승리 조건은 게임 승리에 중요한 역할을 합니다.";
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-665, 225, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(12, 225, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 225, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 3.5f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(4.0f);
        pointObj[1].transform.localPosition = new Vector3(-333, -37, 0);
        pointObj[1].transform.localScale = new Vector3(3.25f, 3.25f, 3.25f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "게임에 유리하게 덱을 구성해보세요!";
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-665, 225, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(12, 225, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 225, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 3f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(3.5f);
        pointObj[1].SetActive(false);
        yield return new WaitForSeconds(1f);
        PointMove(new Vector3(62, 187, 0));
    }

    public void PointMove(Vector3 pos)
    {
        for (int i = 0; i < 5; i++)
            gameObject.GetComponent<TutorialFlowSystem>().cardSetCanvas.transform.GetChild(i).transform.GetChild(0).GetComponent<Image>().raycastTarget = false;

        gameObject.GetComponent<TutorialFlowSystem>().cardSetCanvas.transform.GetChild(index).transform.GetChild(0).GetComponent<Image>().raycastTarget = true;
        pointObj[2].transform.localScale = new Vector3(1, 1, 1);
        pointObj[2].SetActive(true);
        pointObj[2].transform.localPosition = pos;
        //Debug.Log(index);
        index += 1;
    }

    public void PointArrowMove(Vector3 pos)
    {
        pointObj[3].SetActive(true);
        pointObj[3].transform.localPosition = pos;
        index += 1;
    }

    public void pointOff()
    {

        for (int i = 0; i<pointObj.Length;i++)
            pointObj[i].SetActive(false);
        index = 0;
    }

    IEnumerator INGAME_ROLLINGDICE_Cor()
    {
        SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[16], 1.0f);
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
        yield return new WaitForSeconds(2.5f);
        pointObj[2].SetActive(true);
        pointObj[2].transform.localPosition = new Vector3(484, -194, 0);
        pointObj[2].transform.localScale = new Vector3(3.84f, 3.84f, 3.84f);
        SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[17], 0.7f);
        index = 5;
    }

    IEnumerator INGAME_SELECTTERRAINCARD_Cor()
    {
        SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[16], 1.0f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-635, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "그 다음, 나의 덱에서 설치하고 싶은 지형을 고르세요.";
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        pointObj[2].SetActive(true);
        pointObj[2].transform.localPosition = new Vector3(-193, -304, 0);
        pointObj[2].transform.localScale = new Vector3(1.23f, 1.23f, 1.23f);
        SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[17], 0.7f);
        index = 6;
    }

    IEnumerator INGAME_SELECTFLAG_Cor()
    {
        SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[16], 1.0f);
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
        SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[16], 1.0f);
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
        SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[16], 1.0f);
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
        yield return new WaitForSeconds(8);
        SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[16], 1.0f);
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
        DoingTutorial(TUTORIAL.INGAME_ENEMYTURN_ROLLINGDICE);
    }

    IEnumerator INGAME_ENEMYTURN_TURNEND_Cor()
    {
        yield return new WaitForSeconds(3);
        TerrainChange(14);
        TerrainChange(65);
        TerrainChange(15);
        TerrainChange(19);
        TerrainChange(316);
        TerrainChange(319);
        TerrainChange(300);
        TerrainChange(71);
        TerrainChange(69);
        TerrainChange(280);
        TerrainChange(59);
        TerrainChange(302);
        yield return new WaitForSeconds(2);
        SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[16], 1.0f);
        pointObj[4].SetActive(true);
        pointObj[4].transform.localPosition = new Vector3(-374, -2, 0);
        pointObj[4].transform.localScale = new Vector3(1.23f, 1.23f, 1.23f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-570, 110, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(-315.5f, 110, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(-311, 110, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 546;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "상대방이 거점을 지정했습니다.";
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-570, 110, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(-315.5f, 110, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(-311, 110, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 546;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "이제 다른 곳으로 땅을 넓혀볼까요?";
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        pointOff();
        gameObject.GetComponent<TutorialFlowSystem>().FlowChange(FLOW.ENEMYTURN_PICKINGEVENTCARDLOC);
        yield return new WaitForSeconds(3.0f);
        DoingTutorial(TUTORIAL.INGAME_DICEFOCUS);
    }

    public void TerrainChange(int name)
    {
        for(int i =0; i< GameObject.FindWithTag("Planet").transform.childCount;i++)
        {
            if(GameObject.FindWithTag("Planet").transform.GetChild(i).GetComponent<MeshController>().MeshNumber.Equals(name))
            {
                GameObject.FindWithTag("Planet").transform.GetChild(i).GetComponent<MeshController>().terrainstate = Terrain.BARREN;
                GameObject.FindWithTag("Planet").transform.GetChild(i).GetComponent<MeshController>().isFixed = true;

                domMaterial = Resources.Load<Material>("M_BarrenEnemy");
                GameObject.FindWithTag("Planet").transform.GetChild(i).GetComponent<MeshRenderer>().material = domMaterial;
                GameObject.FindWithTag("Planet").transform.GetChild(i).GetComponent<MeshController>().currentIdentify = Identify.ENEMY;
                GameObject.FindWithTag("Planet").transform.GetChild(i).GetComponent<MeshController>().isMine = false;
                return;
            }
        }
    }
    
    IEnumerator INGAME_ENEMYTURN_TURNEND_2_Cor()
    {
        TerrainChange(180);
        TerrainChange(312);
        TerrainChange(210);
        TerrainChange(133);
        TerrainChange(257);
        TerrainChange(233);
        TerrainChange(111);
        TerrainChange(39);
        pointObj[5].SetActive(true);
        pointObj[5].transform.localPosition = new Vector3(-73, 102, 0);
        pointObj[5].transform.localScale = new Vector3(1, 1, 1);
        yield return new WaitForSeconds(3f);
        pointOff();
        DoingTutorial(TUTORIAL.INGAME_BEFOREATTACK);
    }

    IEnumerator INGAME_ENEMYTURN_ROLLINGDICE_2_Cor()
    {
        yield return new WaitForSeconds(8f);
        GameObject.Find("DiceManager").GetComponent<DiceObject>().DiceSystem_Roll(3, 5);
        yield return new WaitForSeconds(3f);
        gameObject.GetComponent<TutorialFlowSystem>().FlowChange(FLOW.ENEMYTURN_ROLLINGDICE);
        DoingTutorial(TUTORIAL.INGAME_ENEMYTURN_TURNEND_2);
    }

    IEnumerator INGAME_BEFOREATTACK_Cor()
    {
        SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[16], 1.0f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-635, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "두 번째 턴이 종료되었습니다.";
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-635, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "내 땅과 상대 땅이 맞닿게 되었는데요,";
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-635, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "이제 공격을 할 수 있습니다.";
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-635, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "그럼, 상대방을 공격해 볼까요?";
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        gameObject.GetComponent<TutorialFlowSystem>().FlowChange(FLOW.ENEMYTURN_PICKINGEVENTCARDLOC);
    }

    IEnumerator INGAME_ATTACK_Cor()
    {
        SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[16], 1.0f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-635, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "상대방의 땅과 우위 상성 관계에 있는 지형을";
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-635, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "상대방의 땅 위에 곂쳐서";
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-635, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "설치하면 공격 할 수 있습니다.";
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        index = 31;
        PointArrowMove(new Vector3(-15, 87, 0));

    }

    IEnumerator ENDGAME_CONDITIONTOWIN_Cor()
    {
        SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[16], 1.0f);
        yield return new WaitForSeconds(3f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-635, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "지금까지 게임 플레이에 대해 알아보았습니다!";
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(2.5f);
        displayTextObj.GetComponent<TutorialDisplayTxt>().startPos = new Vector3(-635, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().endPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().displayPos = new Vector3(0, 230, 0);
        displayTextObj.GetComponent<TutorialDisplayTxt>().sizeX = 1420;
        displayTextObj.GetComponent<TutorialDisplayTxt>().text = "이것으로 튜토리얼을 마칩니다.";
        displayTextObj.GetComponent<TutorialDisplayTxt>().delayTime = 2f;
        displayTextObj.SetActive(true);
        yield return new WaitForSeconds(4.5f);
        SceneManager.LoadScene("MainUIScene");
    }

    public void DoingTutorial(TUTORIAL tutorial)
    {
        switch (tutorial)
        {
            case TUTORIAL.READY_SELECTORDERCARD:
                SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[16], 1.0f);
                currentTutorial = TUTORIAL.READY_SELECTORDERCARD;
                pointObj[0].SetActive(true);
                pointObj[0].transform.localPosition = new Vector3(0, 0, 0);
                pointObj[0].transform.localScale = new Vector3(1.63f, 1.63f, 1.63f);
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
                
                StartCoroutine(INGAME_TURNEND_Cor());
                displayTextObj.GetComponent<TutorialDisplayTxt>().inputColor = new Color32(0, 0, 0, 170);
                currentTutorial = TUTORIAL.INGAME_TURNEND;
                break;
            case TUTORIAL.INGAME_ENEMYTURN_ROLLINGDICE:
                
                GameObject.Find("DiceManager").GetComponent<DiceObject>().DiceSystem_Roll(6, 6);
                displayTextObj.GetComponent<TutorialDisplayTxt>().inputColor = new Color32(0, 0, 0, 170);
                currentTutorial = TUTORIAL.INGAME_ENEMYTURN_ROLLINGDICE;
                gameObject.GetComponent<TutorialFlowSystem>().FlowChange(FLOW.ENEMYTURN_ROLLINGDICE);
                DoingTutorial(TUTORIAL.INGAME_ENEMYTURN_TURNEND);
                break;
            case TUTORIAL.INGAME_ENEMYTURN_TURNEND:
                
                displayTextObj.GetComponent<TutorialDisplayTxt>().inputColor = new Color32(0, 0, 0, 170);
                StartCoroutine(INGAME_ENEMYTURN_TURNEND_Cor());
                currentTutorial = TUTORIAL.INGAME_ENEMYTURN_TURNEND;
                break;
            case TUTORIAL.INGAME_DICEFOCUS:
                
                pointObj[2].SetActive(true);
                pointObj[2].transform.localPosition = new Vector3(474, -190, 0);
                pointObj[2].transform.localScale = new Vector3(4.04f, 4.04f, 4.04f);
                currentTutorial = TUTORIAL.INGAME_DICEFOCUS;
                break;
            case TUTORIAL.INGAME_ROLLINGDICE_2:
                
                pointOff();
                pointObj[2].SetActive(true);
                pointObj[2].transform.localPosition = new Vector3(2, -312, 0);
                pointObj[2].transform.localScale = new Vector3(1.18f, 1.18f, 1.18f);
                currentTutorial = TUTORIAL.INGAME_ROLLINGDICE_2;
                break;
            case TUTORIAL.INGAME_SELECTTERRAINLOC_2:
                
                index = 22;
                PointArrowMove(new Vector3(-72, 89, 0));
                currentTutorial = TUTORIAL.INGAME_SELECTTERRAINLOC_2;
                break;
            case TUTORIAL.INGAME_TURNEND_2:
                
                pointObj[2].SetActive(true);
                pointObj[2].transform.localPosition = new Vector3(526, -193, 0);
                pointObj[2].transform.localScale = new Vector3(2.34f, 2.34f, 2.34f);
                currentTutorial = TUTORIAL.INGAME_TURNEND_2;
                break;
            case TUTORIAL.INGAME_ENEMYTURN_ROLLINGDICE_2:
                
                currentTutorial = TUTORIAL.INGAME_ENEMYTURN_ROLLINGDICE_2;
                StartCoroutine(INGAME_ENEMYTURN_ROLLINGDICE_2_Cor());
                break;
            case TUTORIAL.INGAME_ENEMYTURN_TURNEND_2:
                
                currentTutorial = TUTORIAL.INGAME_ENEMYTURN_TURNEND_2;
                StartCoroutine(INGAME_ENEMYTURN_TURNEND_2_Cor());
                break;
            case TUTORIAL.INGAME_BEFOREATTACK:
                currentTutorial = TUTORIAL.INGAME_BEFOREATTACK;
                
                displayTextObj.GetComponent<TutorialDisplayTxt>().inputColor = new Color32(0, 0, 0, 170);
                StartCoroutine(INGAME_BEFOREATTACK_Cor());
                break;
            case TUTORIAL.INGAME_ATTACK:
                
                currentTutorial = TUTORIAL.INGAME_ATTACK;
                displayTextObj.GetComponent<TutorialDisplayTxt>().inputColor = new Color32(0, 0, 0, 170);
                StartCoroutine(INGAME_ATTACK_Cor());
                break;
            case TUTORIAL.ENDGAME_CONDITIONTOWIN:
                
                currentTutorial = TUTORIAL.ENDGAME_CONDITIONTOWIN;
                displayTextObj.GetComponent<TutorialDisplayTxt>().inputColor = new Color32(0, 0, 0, 170);
                StartCoroutine(ENDGAME_CONDITIONTOWIN_Cor());
                break;
        }
    }
}
