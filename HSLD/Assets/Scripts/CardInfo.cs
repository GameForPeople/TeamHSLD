using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CardInfo : ScriptableObject
{
    public CARDSET currentCard;
    public ZOOMSTATE currentZoom;

    public Vector2 zoomInSatusCardVec;
    public Vector2 zoomOutSatusCardVec;

    public int cardIndex;
    public string cardName;

    public int currentCnt;
    public int maximumCnt;
    public int minumumCnt;
}
    