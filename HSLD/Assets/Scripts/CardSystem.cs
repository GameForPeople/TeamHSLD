using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public enum CARDSET
{
    EVENTCARD,
    TERRAIN_DEFFENSE,
    TERRAIN_NORMAL
}
//public enum ZOOMSTATE
//{
//    IN,
//    OUT
//}

public class CardSystem : MonoBehaviour
{
    private bool isTriggerDone = true;
    private float time_;
    public float timeConstValue;
    [HideInInspector] public GameObject pickedCard; // YSK : private -> public

    public GameObject[] cardSet = new GameObject[5];

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

        if (GameObject.FindWithTag("GameManager").GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_ROLLINGDICE))
            return;

        // 카드를 picking 해야할때를 제외하고는 줌인 / 아웃
        //if (cardSet[0].GetComponent<CardData>().data.currentZoom.Equals(ZOOMSTATE.OUT))
        //{
        //pick
        if (GameObject.FindWithTag("GameManager").GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_PICKINGCARD) || GameObject.FindWithTag("GameManager").GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_PICKINGLOC))
        {
            //init - 지형설치 카드도 초기화할것.
            AllMeshController.myPlanet.GetComponent<AllMeshController>().CleanPickContainer();
            //여기에다가
            CardPosInit();

            pickedCard = card;

            //카드 갯수가 0개있으면, 뽑을수 없음.
            if (pickedCard.GetComponent<CardData>().data.currentCnt < 1)
            {
                pickedCard = null;
                return;
            }

            pickedCard.transform.localPosition = new Vector3(pickedCard.transform.localPosition.x, -290, 0);
            pickedCard.GetComponent<Image>().color = new Color(1, 1, 1, 1.0f);

            Debug.Log(pickedCard.name + "카드 뽑힘");

            if (SoundManager.instance_ != null)
                SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[7], 1.0f);

            //flow 변경
            GameObject.FindWithTag("GameManager").GetComponent<FlowSystem>().FlowChange(FLOW.TO_PICKINGCARD);
        }
        ////줌아웃
        //else
        //    StartCoroutine(ZoomIn());
        //}

        ////줌인
        //else
        //{
        //    StartCoroutine(ZoomOut());
        //}
    }

    ////카드 축소
    //IEnumerator ZoomOut()
    //{

    //    isTriggerDone = false;
    //    for (int i = 0; i < cardSet.Length; i++)
    //    {
    //        time_ = 0;
    //        while (true)
    //        {
    //            time_ += Time.deltaTime;
    //            cardSet[i].GetComponent<RectTransform>().localPosition = Vector2.Lerp(cardSet[i].GetComponent<CardData>().data.zoomInSatusCardVec, cardSet[i].GetComponent<CardData>().data.zoomOutSatusCardVec, (time_ * timeConstValue));
    //            yield return new WaitForEndOfFrame();
    //            if (time_ * timeConstValue > 1f )
    //                break;
    //        }
    //        yield return new WaitForSeconds(0.01f);
    //    }

    //    for(int i =0; i<cardSet.Length;i++)
    //    {
    //        cardSet[i].GetComponent<CardData>().data.currentZoom = ZOOMSTATE.OUT;
    //    }
    //    isTriggerDone = true;
    //}

    ////카드 확대
    //IEnumerator ZoomIn()
    //{
    //    isTriggerDone = false;
    //    for (int i = 0; i < cardSet.Length; i++)
    //    {
    //        time_ = 0;
    //        while (true)
    //        {
    //            time_ += Time.deltaTime;
    //            cardSet[i].GetComponent<RectTransform>().localPosition = Vector2.Lerp(cardSet[i].GetComponent<CardData>().data.zoomOutSatusCardVec, cardSet[i].GetComponent<CardData>().data.zoomInSatusCardVec, (time_ * timeConstValue));
    //            yield return new WaitForEndOfFrame();
    //            if (time_ * timeConstValue > 1f)
    //                break;
    //        }
    //        yield return new WaitForSeconds(0.01f);
    //    }
    //    for (int i = 0; i < cardSet.Length; i++)
    //    {
    //        cardSet[i].GetComponent<CardData>().data.currentZoom = ZOOMSTATE.IN;
    //    }
    //    isTriggerDone = true;
    //}

}
