using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CardData : MonoBehaviour
{
    public CardInfo data;

    private void Start()
    {
        InitSO();
    }

    private void InitSO()
    {
        if(data.currentCard.Equals(CARDSET.TERRAIN_DEFFENSE))
            data.currentCnt = 1;
        if (data.currentCard.Equals(CARDSET.TERRAIN_NORMAL))
            data.currentCnt = 5;

        //data.currentZoom = ZOOMSTATE.IN;
    }
}
