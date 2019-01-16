using System.Collections;
using System.Collections.Generic;
using System;

using UnityEngine;
using UnityEngine.UI;

public partial class MainUISceneManager : MonoBehaviour
{
    public bool isDrawSettingUI;
    public bool isDrawCreditUI;
    public bool isDrawVipUI;

    public string inputtedVipCode;

    // 0 level
    private void StartForSetting()
    {
        isDrawSettingUI = false;
        isDrawCreditUI = false;
        isDrawVipUI = false;
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

    // 1 level
    public void UI_DrawVipUI(bool InIsDraw)
    {
        if (InIsDraw)
        {
            if (isDrawVipUI)
                return;

            GameObject.Find("Setting_UI").transform.Find("VIP_OnOff").gameObject.SetActive(true);
            isDrawVipUI = true;
        }
        else
        {
            if (!isDrawVipUI)
                return;

            GameObject.Find("Setting_UI").transform.Find("VIP_OnOff").gameObject.SetActive(false);
            isDrawVipUI = false;
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

    // 2 level
    /*
        UI_SendVipCode

        확인 버튼이 눌렀을 때 실행되며, 인풋필드에서 텍스트를 긁어 네트워크매니저에게 "서버한테 이 텍스트 보내고 VIP Code 맞는 지 물어봐!"를 수행합니다.
     */
    public void UI_SendVipCode()
    {
        inputtedVipCode = GameObject.Find("Setting_UI").transform.Find("VIP_OnOff").transform.Find("Image").transform.Find("InputField").transform.Find("Text").
                gameObject.GetComponent<Text>().text;

        networkObject.SendData(PROTOCOL.DEMAND_VIP_CODE);
    }

    // 3 level

    /*
    NetworkManager_RecvVipResult

    보낸 VIPCode가 맞는지 틀린지의 결과가 전송됩니다. 이에 따라 적합한 UI를 On합니다.
    */
    public void NetworkManager_RecvVipResult(bool isSuccessed, bool isAlreadyVip = false)
    {
        if (isSuccessed)
        {
            GameObject.Find("Setting_UI").transform.Find("VIP_OnOff").transform.Find("Yes_onoff").gameObject.SetActive(true);
        }
        else
        {
            GameObject.Find("Setting_UI").transform.Find("VIP_OnOff").transform.Find("No_onoff").gameObject.SetActive(true);
            if(isAlreadyVip)
            {
                GameObject.Find("Setting_UI").transform.Find("VIP_OnOff").transform.Find("No_onoff").transform.Find("Text_Output").GetComponent<Text>().text = "이미 VIP Code 받은 거 다암ㅋ";
            }
            else
            {
                GameObject.Find("Setting_UI").transform.Find("VIP_OnOff").transform.Find("No_onoff").transform.Find("Text_Output").GetComponent<Text>().text = "미안한데..VIP Code 틀렸어요...";
            }
        }
    }

    /*
        UI_OffYesOrNoOnOffUI

        인자가 True 일 경우 Yes_onoff를 off, False일 경우 No_onoff를 Off하는 함수입니다.
    */
    public void UI_OffYesOrNoOnOffUI(bool isYesOnOffUI)
    {
        if (isYesOnOffUI)
        {
            GameObject.Find("Setting_UI").transform.Find("VIP_OnOff").transform.Find("Yes_onoff").gameObject.SetActive(false);
        }
        else
        {
            GameObject.Find("Setting_UI").transform.Find("VIP_OnOff").transform.Find("No_onoff").gameObject.SetActive(false);
        }
    }
}
