using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class CardSet : MonoBehaviour
{
    private int totalCard = 17;
    public Text displayTotalCnt;
    static public bool isSelect = false;

    public void AddCard(GameObject obj)
    {
        if (obj.GetComponent<CardData>().data.currentCnt > (obj.GetComponent<CardData>().data.maximumCnt - 1) || totalCard > 29)
            return;

        SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[4], 1.0f);

        obj.GetComponent<CardData>().data.currentCnt += 1;
        totalCard += 1;
        obj.GetComponentInChildren<Text>().text = obj.GetComponent<CardData>().data.currentCnt.ToString();
        displayTotalCnt.text = totalCard.ToString();
    }

    public void DelCard(GameObject obj)
    {
        if (obj.GetComponent<CardData>().data.currentCnt == obj.GetComponent<CardData>().data.minumumCnt)
            return;

        SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[4], 1.0f);

        obj.GetComponent<CardData>().data.currentCnt -= 1;
        totalCard -= 1;
        obj.GetComponentInChildren<Text>().text = obj.GetComponent<CardData>().data.currentCnt.ToString();
        displayTotalCnt.text = totalCard.ToString();
    }

    public void Select()
    {
        if (totalCard < 20)
            return;

        isSelect = true;
        gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.READY_SETCARD);
    }
}
