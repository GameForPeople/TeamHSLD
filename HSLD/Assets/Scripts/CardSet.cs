using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class CardSet : MonoBehaviour
{
    private int totalCard = 17;
    public Text displayTotalCnt;

    public void AddCard(GameObject obj)
    {
        if (obj.GetComponent<CardData>().data.currentCnt > (obj.GetComponent<CardData>().data.maximumCnt - 1) || totalCard > 29)
            return;

        obj.GetComponent<CardData>().data.currentCnt += 1;
        totalCard += 1;
        obj.GetComponentInChildren<Text>().text = obj.GetComponent<CardData>().data.currentCnt.ToString();
        displayTotalCnt.text = totalCard.ToString();
    }

    public void DelCard(GameObject obj)
    {
        if (obj.GetComponent<CardData>().data.currentCnt == obj.GetComponent<CardData>().data.minumumCnt)
            return;

        obj.GetComponent<CardData>().data.currentCnt -= 1;
        totalCard -= 1;
        obj.GetComponentInChildren<Text>().text = obj.GetComponent<CardData>().data.currentCnt.ToString();
        displayTotalCnt.text = totalCard.ToString();
    }

    public void Select()
    {
        if (totalCard < 20)
            return;

        gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.READY_SETCARD);
    }
}
