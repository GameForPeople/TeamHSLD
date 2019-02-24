using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class TutorialCardSet : MonoBehaviour
{
    private int totalCard = 17;
    public Text displayTotalCnt;
    static public bool isSelect = false;

    public void AddCard(GameObject obj)
    {
        if (obj.GetComponent<CardData>().data.maximumCnt == 15 && obj.GetComponent<CardData>().data.currentCnt > 8)
            return;

        if (obj.GetComponent<CardData>().data.maximumCnt == 10 && obj.GetComponent<CardData>().data.currentCnt > 3)
            return;

        SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[3], 1.0f);

        obj.GetComponent<CardData>().data.currentCnt += 1;

        //Debug.Log(obj.GetComponent<CardData>().data.maximumCnt + " : " + obj.GetComponent<CardData>().data.currentCnt);

        if (obj.GetComponent<CardData>().data.maximumCnt == 15 && obj.GetComponent<CardData>().data.currentCnt == 8)
        {
            if (TutorialManager.index == 1)
                gameObject.GetComponent<TutorialManager>().PointMove(new Vector3(267,187,0));
            else if (TutorialManager.index == 2)
                gameObject.GetComponent<TutorialManager>().PointMove(new Vector3(461,187,0));
            else if (TutorialManager.index == 3)
                gameObject.GetComponent<TutorialManager>().PointMove(new Vector3(62, -64, 0));
        }

        else if (obj.GetComponent<CardData>().data.maximumCnt == 10 && obj.GetComponent<CardData>().data.currentCnt == 3)
        {
            if (TutorialManager.index == 4)
                gameObject.GetComponent<TutorialManager>().PointMove(new Vector3(267, -64, 0));
        }

        totalCard += 1;
        obj.GetComponentInChildren<Text>().text = obj.GetComponent<CardData>().data.currentCnt.ToString();
        displayTotalCnt.text = totalCard.ToString() + " / 30";
    }

    public void DelCard(GameObject obj)
    {
        if (obj.GetComponent<CardData>().data.currentCnt == obj.GetComponent<CardData>().data.minimumCnt)
            return;

        SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[3], 1.0f);

        obj.GetComponent<CardData>().data.currentCnt -= 1;
        totalCard -= 1;
        obj.GetComponentInChildren<Text>().text = obj.GetComponent<CardData>().data.currentCnt.ToString();
        displayTotalCnt.text = totalCard.ToString() + " / 30";
    }

    public void Select()
    {
        if (totalCard < 20)
            return;

        isSelect = true;
        gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.READY_SETCARD);
    }
}
