using System.Collections;
using System.Collections.Generic;
using System;

using UnityEngine;
using UnityEngine.UI;

public partial class MainUISceneManager : MonoBehaviour {
    public bool isDrawSettingUI;

    public void UI_DrawSettingUI(bool InIsDraw)
    {
        if(InIsDraw)
        {
            if (isDrawSettingUI)
                return;

            GameObject.Find("Setting_UI").transform.Find("OnOff").gameObject.SetActive(true);
            isDrawSettingUI = true;
        }
        else
        {
            if (!isDrawSettingUI)
                return;

            GameObject.Find("Setting_UI").transform.Find("OnOff").gameObject.SetActive(false);
            isDrawSettingUI = false;
        }
    }
}
