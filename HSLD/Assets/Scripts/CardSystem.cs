using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum CARDSET
{
    EVENTCARD,
    TERRAIN
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
    private GameObject pickedCard;

    public GameObject[] cardSet = new GameObject[8];

    //나중에 SO로 데이터 저장
    private Vector2[] zoomInSatusCardVec = new Vector2[8];
    private Vector2[] zoomOutSatusCardVec = new Vector2[8];

    private void Init()
    {
        zoomInSatusCardVec[0] = new Vector2(-350, -294);
        zoomInSatusCardVec[1] = new Vector2(-311, -294);
        zoomInSatusCardVec[2] = new Vector2(-261, -294);
        zoomInSatusCardVec[3] = new Vector2(-219, -294);
        zoomInSatusCardVec[4] = new Vector2(-162, -294);
        zoomInSatusCardVec[5] = new Vector2(118, -294);
        zoomInSatusCardVec[6] = new Vector2(152, -294);
        zoomInSatusCardVec[7] = new Vector2(194, -294);

        zoomOutSatusCardVec[0] = new Vector2(-317, -294);
        zoomOutSatusCardVec[1] = new Vector2(-234, -294);
        zoomOutSatusCardVec[2] = new Vector2(-151, -294);
        zoomOutSatusCardVec[3] = new Vector2(-68, -294);
        zoomOutSatusCardVec[4] = new Vector2(15, -294);
        zoomOutSatusCardVec[5] = new Vector2(144, -294);
        zoomOutSatusCardVec[6] = new Vector2(226, -294);
        zoomOutSatusCardVec[7] = new Vector2(309, -294);
    }

    private void Start()
    {
        Init();
    }

    //줌 인 / 아웃 판별
    public void PickingCard(GameObject card)
    {
        if (!isTriggerDone)
            return;

        //if (GameObject.FindWithTag("GameManager").GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_ROLLINGDICE))
        //    return;

        // 카드를 picking 해야할때를 제외하고는 줌인 / 아웃
        if (cardSet[0].GetComponent<CardInfo>().currentZoom.Equals(ZOOMSTATE.OUT))
        {
            //pick
            if (GameObject.FindWithTag("GameManager").GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_PICKINGCARD))
            {
                pickedCard = card;
                Debug.Log(pickedCard.name + "카드 뽑힘");

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
        for (int i = 0; i < zoomOutSatusCardVec.Length; i++)
        {
            time_ = 0;
            while (true)
            {
                time_ += Time.deltaTime;
                cardSet[i].GetComponent<RectTransform>().localPosition = Vector2.Lerp(zoomInSatusCardVec[i], zoomOutSatusCardVec[i], (time_ * timeConstValue));
                yield return new WaitForEndOfFrame();
                if (time_ * timeConstValue > 1f )
                    break;
            }
            yield return new WaitForSeconds(0.01f);
        }

        for(int i =0; i<cardSet.Length;i++)
        {
            cardSet[i].GetComponent<CardInfo>().currentZoom = ZOOMSTATE.OUT;
        }
        isTriggerDone = true;
    }

    //카드 확대
    IEnumerator ZoomIn()
    {
        isTriggerDone = false;
        for (int i = 0; i < zoomOutSatusCardVec.Length; i++)
        {
            time_ = 0;
            while (true)
            {
                time_ += Time.deltaTime;
                cardSet[i].GetComponent<RectTransform>().localPosition = Vector2.Lerp(zoomOutSatusCardVec[i], zoomInSatusCardVec[i], (time_ * timeConstValue));
                yield return new WaitForEndOfFrame();
                if (time_ * timeConstValue > 1f)
                    break;
            }
            yield return new WaitForSeconds(0.01f);
        }
        for (int i = 0; i < cardSet.Length; i++)
        {
            cardSet[i].GetComponent<CardInfo>().currentZoom = ZOOMSTATE.IN;
        }
        isTriggerDone = true;
    }

}
