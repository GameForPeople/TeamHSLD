using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum CARDSET
{
    EVENTCARD,
    TERRAIN_DEFFENSE,
    TERRAIN_NORMAL
}
public enum ZOOMSTATE
{
    IN,
    OUT
}

public class CardSystem : MonoBehaviour
{
    private bool isTriggerDone = true;
    private float time_;
    public float timeConstValue;
    [HideInInspector] public GameObject pickedCard; // YSK : private -> public

    public GameObject[] cardSet = new GameObject[8];

    //줌 인 / 아웃 판별
    public void PickingCard(GameObject card)
    {
        if (!isTriggerDone)
            return;

        //if (GameObject.FindWithTag("GameManager").GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_ROLLINGDICE))
        //    return;

        // 카드를 picking 해야할때를 제외하고는 줌인 / 아웃
        if (cardSet[0].GetComponent<CardData>().data.currentZoom.Equals(ZOOMSTATE.OUT))
        {
            //pick
            if (GameObject.FindWithTag("GameManager").GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_PICKINGCARD))
            {
                pickedCard = card;
                Debug.Log(pickedCard.name + "카드 뽑힘");

                if (SoundManager.instance_ != null)
                    SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[7], 1.0f);
                //flow 변경
                GameObject.FindWithTag("GameManager").GetComponent<FlowSystem>().FlowChange(FLOW.TO_PICKINGCARD);
            }
            //줌아웃
            else
                StartCoroutine(ZoomIn());
        }

        //줌인
        else
        {
            StartCoroutine(ZoomOut());
        }
    }

    //카드 축소
    IEnumerator ZoomOut()
    {

        isTriggerDone = false;
        for (int i = 0; i < cardSet.Length; i++)
        {
            time_ = 0;
            while (true)
            {
                time_ += Time.deltaTime;
                cardSet[i].GetComponent<RectTransform>().localPosition = Vector2.Lerp(cardSet[i].GetComponent<CardData>().data.zoomInSatusCardVec, cardSet[i].GetComponent<CardData>().data.zoomOutSatusCardVec, (time_ * timeConstValue));
                yield return new WaitForEndOfFrame();
                if (time_ * timeConstValue > 1f )
                    break;
            }
            yield return new WaitForSeconds(0.01f);
        }

        for(int i =0; i<cardSet.Length;i++)
        {
            cardSet[i].GetComponent<CardData>().data.currentZoom = ZOOMSTATE.OUT;
        }
        isTriggerDone = true;
    }

    //카드 확대
    IEnumerator ZoomIn()
    {
        isTriggerDone = false;
        for (int i = 0; i < cardSet.Length; i++)
        {
            time_ = 0;
            while (true)
            {
                time_ += Time.deltaTime;
                cardSet[i].GetComponent<RectTransform>().localPosition = Vector2.Lerp(cardSet[i].GetComponent<CardData>().data.zoomOutSatusCardVec, cardSet[i].GetComponent<CardData>().data.zoomInSatusCardVec, (time_ * timeConstValue));
                yield return new WaitForEndOfFrame();
                if (time_ * timeConstValue > 1f)
                    break;
            }
            yield return new WaitForSeconds(0.01f);
        }
        for (int i = 0; i < cardSet.Length; i++)
        {
            cardSet[i].GetComponent<CardData>().data.currentZoom = ZOOMSTATE.IN;
        }
        isTriggerDone = true;
    }

}
