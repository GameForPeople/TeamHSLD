using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CardInfo : ScriptableObject
{
    public CARDSET currentCard;
    //public ZOOMSTATE currentZoom;

    public int cardIndex;
    public string cardName;

    public Sprite img;

    public int currentCnt;
    public int maximumCnt;
    public int minumumCnt;
}
    