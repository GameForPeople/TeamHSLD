using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class CardSet : MonoBehaviour
{
    private int totalCard = 5;
    public Text displayTotalCnt;
    public int maximumCnt;
    public int minimumCnt = 1;

    public void AddCard(GameObject obj)
    {
        if (obj.GetComponent<CardInfo>().Cnt > (maximumCnt - 1) || totalCard > 29)
            return;

        obj.GetComponent<CardInfo>().Cnt += 1;
        totalCard += 1;
        obj.GetComponentInChildren<Text>().text = obj.GetComponent<CardInfo>().Cnt.ToString();
        displayTotalCnt.text = totalCard.ToString();
    }

    public void DelCard(GameObject obj)
    {
        if (obj.GetComponent<CardInfo>().Cnt == minimumCnt)
            return;

        obj.GetComponent<CardInfo>().Cnt -= 1;
        totalCard -= 1;
        obj.GetComponentInChildren<Text>().text = obj.GetComponent<CardInfo>().Cnt.ToString();
        displayTotalCnt.text = totalCard.ToString();
    }

    public void Select()
    {
        if (totalCard < 20)
            return;

        gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.READY_SETCARD);
    }
}
