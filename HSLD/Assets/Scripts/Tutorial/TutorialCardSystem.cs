using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

//public enum ZOOMSTATE
//{
//    IN,
//    OUT
//}

public class TutorialCardSystem : MonoBehaviour
{
    private bool isTriggerDone = true;
    private float time_;
    public float timeConstValue;
    [HideInInspector] public GameObject pickedCard; // YSK : private -> public

    public GameObject[] cardSet = new GameObject[5];

    private TutorialFlowSystem flowSystem;

    private void Start()
    {
        flowSystem = GameObject.FindWithTag("GameManager").GetComponent<TutorialFlowSystem>();
    }

    public void CardCntUpdate()
    {
        for (int i = 0; i < cardSet.Length; i++)
            cardSet[i].GetComponentInChildren<Text>().text = cardSet[i].GetComponent<CardData>().data.currentCnt.ToString();
    }

    public void CardPosInit()
    {
        for (int i = 0; i < cardSet.Length; i++)
        {
            cardSet[i].transform.localPosition = new Vector3(cardSet[i].transform.localPosition.x, -330, 0);
            cardSet[i].GetComponent<Image>().color = new Color(1, 1, 1, 0.3f);
        }
    }

    //카드선택
    public void PickingCard(GameObject card)
    {
        if (!isTriggerDone)
            return;

        if(card.GetComponent<CardData>().data.cardIndex != 1 && TutorialManager.index != 6)
            return;

        if (card.GetComponent<CardData>().data.cardIndex == 1 && TutorialManager.index == 6)
        {
            gameObject.GetComponent<TutorialManager>().pointOff();
            gameObject.GetComponent<TutorialManager>().DoingTutorial(TUTORIAL.INGAME_SELECTFLAG);
        }
            

        if (flowSystem.currentFlow.Equals(FLOW.TO_ROLLINGDICE))
            return;

        if (flowSystem.currentFlow.Equals(FLOW.TO_PICKINGCARD) || flowSystem.currentFlow.Equals(FLOW.TO_PICKINGLOC))
        {

            //init - 지형설치 카드도 초기화할것.
            TutorialAllMeshController.myPlanet.GetComponent<TutorialAllMeshController>().CleanPickContainer();
            pickedCard = card;
            //여기에다가
            CardPosInit();
            if (GameObject.Find("GameCores") != null)
            {
                GameObject.FindWithTag("GameManager").GetComponent<InGameSceneManager>().SendTerrainType(card.GetComponent<CardData>().data.cardIndex);
            }
            pickedCard.transform.localPosition = new Vector3(pickedCard.transform.localPosition.x, -290, 0);
            pickedCard.GetComponent<Image>().color = new Color(1, 1, 1, 1.0f);

            //카드 갯수가 0개있으면, 뽑을수 없음.
            if (card.GetComponent<CardData>().data.currentCnt < 1)
            {
                card = null;
                return;
            }
            SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[10], 1.0f);

            //flow 변경
            flowSystem.FlowChange(FLOW.TO_PICKINGCARD);
        }

    }
    

}
