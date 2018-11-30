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

    public void EventCardInstate()
    {
        pickedCard = eventCardInfoSet[Random.Range(0, 7)];
        Debug.Log(pickedCard);
        eventCard.SetActive(true);

        cardName.text = pickedCard.cardName;
        cardImg.sprite = pickedCard.img;

        //선택된 카드 서버로 보내기
        //pickedCard.data.cardIndex
    }

    //사용 할 경우
    public void UseCard()
    {
        eventCard.SetActive(false);
        gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.TO_PICKEVENTCARD);
    }

    //사용 안 할 경우
    public void DiscardCard()
    {
        eventCard.SetActive(false);
        gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.TO_PICKEVENTCARD);
    }
}
