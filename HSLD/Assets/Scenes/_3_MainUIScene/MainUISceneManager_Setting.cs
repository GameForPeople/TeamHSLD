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

            soundManager.SFXPlay(soundManager.clips[2], 1.0f);

            GameObject.Find("Setting_UI").transform.Find("OnOff").gameObject.SetActive(true);
            isDrawSettingUI = true;
        }
        else
        {
            if (!isDrawSettingUI)
                return;

            soundManager.SFXPlay(soundManager.clips[3], 1.0f);

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

            soundManager.SFXPlay(soundManager.clips[2], 1.0f);

            GameObject.Find("Setting_UI").transform.Find("VIP_OnOff").gameObject.SetActive(true);
            isDrawVipUI = true;
        }
        else
        {
            if (!isDrawVipUI)
                return;

            soundManager.SFXPlay(soundManager.clips[3], 1.0f);

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

            soundManager.SFXPlay(soundManager.clips[2], 1.0f);

            GameObject.Find("Setting_UI").transform.Find("Credit_OnOff").gameObject.SetActive(true);
            isDrawCreditUI = true;
        }
        else
        {
            if (!isDrawCreditUI)
                return;

            soundManager.SFXPlay(soundManager.clips[3], 1.0f);

            GameObject.Find("Setting_UI").transform.Find("Credit_OnOff").gameObject.SetActive(false);
            isDrawCreditUI = false;
        }
    }

    public void UI_OpenFaceBook()
    {
        soundManager.SFXPlay(soundManager.clips[2], 1.0f);
        Application.OpenURL("https://www.facebook.com/bridge1000/");
    }

    public void UI_OpenGitHub()
    {
        soundManager.SFXPlay(soundManager.clips[2], 1.0f);
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
        soundManager.SFXPlay(soundManager.clips[2], 1.0f);

        if (isSuccessed)
        {
            PlayParticle(1);
            soundManager.SFXPlay(soundManager.clips[16], 1.0f);

            GameObject.Find("Setting_UI").transform.Find("VIP_OnOff").transform.Find("Yes_onoff").gameObject.SetActive(true);
        }
        else
        {
            PlayParticle(2);
            soundManager.SFXPlay(soundManager.clips[16], 1.0f);

            GameObject.Find("Setting_UI").transform.Find("VIP_OnOff").transform.Find("No_onoff").gameObject.SetActive(true);
            if(isAlreadyVip)
            {
                GameObject.Find("Setting_UI").transform.Find("VIP_OnOff").transform.Find("No_onoff").transform.Find("Text_Output").GetComponent<Text>().text = "이미 VIP Code를 받은 계정입니다.";
            }
            else
            {
                GameObject.Find("Setting_UI").transform.Find("VIP_OnOff").transform.Find("No_onoff").transform.Find("Text_Output").GetComponent<Text>().text = "Vip Code를 다시 확인해주세요.";
            }
        }
    }

    /*
        UI_OffYesOrNoOnOffUI

        인자가 True 일 경우 Yes_onoff를 off, False일 경우 No_onoff를 Off하는 함수입니다.
    */
    public void UI_OffYesOrNoOnOffUI(bool isYesOnOffUI)
    {
        soundManager.SFXPlay(soundManager.clips[3], 1.0f);

        if (isYesOnOffUI)
        {
            GameObject.Find("Setting_UI").transform.Find("VIP_OnOff").transform.Find("Yes_onoff").gameObject.SetActive(false);
        }
        else
        {
            GameObject.Find("Setting_UI").transform.Find("VIP_OnOff").transform.Find("No_onoff").gameObject.SetActive(false);
        }
    }

    /*
     UI_ExitGame()

        설정 UI에서, 게임종료 버튼이 눌렸을 때, 게임을 고냥 종료해주는 함수.
     */
    public void UI_ExitGame()
    {
        soundManager.SFXPlay(soundManager.clips[0], 1.0f);

#if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false; //play모드를 false로.
                                                         //#elif UNITY_WEBPLAYER
                                                         //        Application.OpenURL("http://google.com"); //구글웹으로 전환
#else
                        Application.Quit(); //어플리케이션 종료
#endif
    }
}
