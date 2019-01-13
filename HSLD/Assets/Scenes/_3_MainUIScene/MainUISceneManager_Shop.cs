using System.Collections;
using System.Collections.Generic;
using System;

using UnityEngine;
using UnityEngine.UI;

public partial class MainUISceneManager : MonoBehaviour
{
    public bool isDrawShopUI;

    public int itemIndex;

    private void StartForShop()
    {
        isDrawShopUI = false;
    }

    public void UI_DrawShopUI(bool InIsDraw)
    {
        if (InIsDraw)
        {
            if (isDrawShopUI)
                return;

            GameObject.Find("Shop_UI").transform.Find("OnOff").gameObject.SetActive(true);
            isDrawShopUI = true;
        }
        else
        {
            if (!isDrawShopUI)
                return;

            GameObject.Find("Shop_UI").transform.Find("OnOff").gameObject.SetActive(false);
            isDrawShopUI = false;
        }
    }

    /*
        UI_BuyItem(int InIndex)

        플레이어가 각 Buy에 해당하는 버튼을 눌렀을 때, 
        각 버튼마다 할당되어 있는 인덱스(아이템 인덱스)를 서버로 요청합니다.
    */
    public void UI_BuyItem(int InIndex)
    {
        itemIndex = InIndex;
        networkObject.SendData((int)PROTOCOL.DEMAND_MAKE_FRIEND);
    }

    /*
        NetworkManager_AnswerBuy()
      
        NetworkManager_Recv에서 호출하는 함수로,
        인자로 넘어온 int에 따라 다른 일을 하게 됩니다.
    */
    public void NetworkManager_AnswerBuy(int InType) //...AnswerBuy 문법 상태가..
    {
        /*
         InType == -1 성공, 0 돈없음, 1 이미 구매한 아이템, 2 그런 아이템 안팔아요
         */
        GameObject.Find("Shop_UI").transform.Find("OnOff").transform.Find("OnOff").gameObject.SetActive(true);

        if (InType == -1)
        {
            GameObject.Find("Shop_UI").transform.Find("OnOff").transform.Find("OnOff").transform.Find("Text").
                gameObject.GetComponent<Text>().text = "구매 성공!";
        }
        else if (InType == 0)
        {
            GameObject.Find("Shop_UI").transform.Find("OnOff").transform.Find("OnOff").transform.Find("Text").
                gameObject.GetComponent<Text>().text = "살 돈 없는데?";
        }
        else if (InType == 1)
        {
            GameObject.Find("Shop_UI").transform.Find("OnOff").transform.Find("OnOff").transform.Find("Text").
                gameObject.GetComponent<Text>().text = "이미 산건데?";
        }
        else if (InType == 2)
        {
            GameObject.Find("Shop_UI").transform.Find("OnOff").transform.Find("OnOff").transform.Find("Text").
                gameObject.GetComponent<Text>().text = "몰라 에러남";
        }
    }

    /*
        UI_OffNotifyUI()

        구매관련 팝업 UI, 버튼 누르면 꺼지게하는 함수입니다.
    */
    public void UI_OffNotifyUI()
    {
        GameObject.Find("Shop_UI").transform.Find("OnOff").transform.Find("OnOff").gameObject.SetActive(false);
    }
}
