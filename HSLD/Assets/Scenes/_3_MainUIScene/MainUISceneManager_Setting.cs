using System.Collections;
using System.Collections.Generic;
using System;

using UnityEngine;
using UnityEngine.UI;

public partial class MainUISceneManager : MonoBehaviour {
    public bool isDrawSettingUI;
    public bool isDrawCreditUI;

    private void StartForSetting()
    {
        isDrawSettingUI = false;
        isDrawCreditUI = false;
    }

    public void UI_DrawSettingUI(bool InIsDraw)
    {
        if (InIsDraw)
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

    public void UI_DrawCreditUI(bool InIsDraw)
    {
        if (InIsDraw)
        {
            if (isDrawCreditUI)
                return;

            GameObject.Find("Setting_UI").transform.Find("Credit_OnOff").gameObject.SetActive(true);
            isDrawCreditUI = true;
        }
        else
        {
            if (!isDrawCreditUI)
                return;

            GameObject.Find("Setting_UI").transform.Find("Credit_OnOff").gameObject.SetActive(false);
            isDrawCreditUI = false;
        }
    }



    public void UI_OpenFaceBook()
    {
        Application.OpenURL("https://www.facebook.com/bridge1000/");
    }

    public void UI_OpenGitHub()
    {
        Application.OpenURL("https://github.com/GameForPeople/TeamHSLD");
    }

}
