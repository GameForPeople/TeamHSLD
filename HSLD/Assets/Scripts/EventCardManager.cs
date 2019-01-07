using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class EventCardManager : MonoBehaviour
{
    public CardInfo[] eventCardInfoSet;
    private CardInfo pickedCard;

    public GameObject eventCard;
    public Text cardName;
    public Image cardImg;
    private int selectedIndex;

    public void EventCardInstate()
    {
        pickedCard = eventCardInfoSet[Random.Range(0, 7)];
        eventCard.SetActive(true);

        cardName.text = pickedCard.cardName;
        cardImg.sprite = pickedCard.img;
        selectedIndex = pickedCard.cardIndex;

        //선택된 카드 서버로 보내기
        //pickedCard.data.cardIndex
    }

    //사용 할 경우
    public void UseCard()
    {
        eventCard.SetActive(false);
        gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.TO_PICKEVENTCARD);

        switch(selectedIndex)
        {
            case 101:
                break;
            case 111:
                break;
            case 201:
                break;
            case 202:
                break;
            case 301:
                TurnSystem.enemyEventCardDefense = true;
                break;
            case 401:
                DiceSystem.isDiceDouble = true;
                break;
        }
    }
}
