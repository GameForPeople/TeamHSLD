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
        data.Cnt = 1;
        data.currentZoom = ZOOMSTATE.IN;
    }
}
