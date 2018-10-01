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
        if (obj.GetComponent<CardData>().data.Cnt > (maximumCnt - 1) || totalCard > 29)
            return;

        obj.GetComponent<CardData>().data.Cnt += 1;
        totalCard += 1;
        obj.GetComponentInChildren<Text>().text = obj.GetComponent<CardData>().data.Cnt.ToString();
        displayTotalCnt.text = totalCard.ToString();
    }

    public void DelCard(GameObject obj)
    {
        if (obj.GetComponent<CardData>().data.Cnt == minimumCnt)
            return;

        obj.GetComponent<CardData>().data.Cnt -= 1;
        totalCard -= 1;
        obj.GetComponentInChildren<Text>().text = obj.GetComponent<CardData>().data.Cnt.ToString();
        displayTotalCnt.text = totalCard.ToString();
    }

    public void Select()
    {
        if (totalCard < 20)
            return;

        gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.READY_SETCARD);
    }
}
