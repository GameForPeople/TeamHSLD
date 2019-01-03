using System.Collections;
using System.Collections.Generic;
using System;

using UnityEngine;
using UnityEngine.UI;

public partial class MainUISceneManager : MonoBehaviour
{
    public bool isDrawShopUI;

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
}
