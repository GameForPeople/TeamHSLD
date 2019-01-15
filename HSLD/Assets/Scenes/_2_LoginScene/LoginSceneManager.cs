﻿using System.Collections;
using System.Collections.Generic;

using System;

using UnityEngine;
using UnityEngine.UI;
using System.Text;

/*
    해당 씐에서는 굳이, 게임 오브젝트를 멤버 변수로 갖지 않습니다. (빠른 속도를 필요로 하지 않기 때문이며, 자동 로그인 기능이 추후 추가될 예정입니다) 
 */

public class LoginSceneManager : MonoBehaviour {
    
    public string IDBuffer;

    public int typeBuffer;  // 1이면 로그인, 2이면 회원가입.

    public int failReason;

    #region [ Release Func ]

    void Start()
    {
        if (GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().isOnNetwork == true)
        {
            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().StartNetworkFunction();
        }
    }

    public void UI_SignInButton()
    {
        // 로그인 버튼 처리입니다.
        typeBuffer = 1;
        
        IDBuffer = GameObject.Find("InputField_ID").transform.Find("Text").gameObject.GetComponent<Text>().text;
        //PWStringBuffer = GameObject.Find("InputField_PW").transform.Find("Text").gameObject.GetComponent<Text>().text;
        //PWBuffer = Int32.Parse(PWStringBuffer);

        Debug.Log("로그인을 시도합니다. ID : " + IDBuffer /*+ "PW : " + PWBuffer*/);

        if (GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().isOnNetwork)
        {
            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().SendData(PROTOCOL.DEMAND_LOGIN);
        }
        else
        {
            // ForOnlyClientTest
            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().ID = "TEST_해솔리드";
           // GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().PW = 1234;

            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().money = 20190101;
            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().winCount = 0;
            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().loseCount = 0;

            PermitLoginProcess();
        }
    }

    public void UI_OnOff_NewNicknameUI(bool InIsActive = true)
    {
        //GameObject.Find("SignUp_UI").transform.Find("OnOff_All").gameObject.SetActive(true);
        GameObject.Find("OnOff_UI").transform.Find("SignUp_UI").gameObject.SetActive(InIsActive);

        //GameObject.Find("OnOff_UI").gameObject.SetActive(true);
    }

    public void UI_SetNewNicknameButton()
    {
        if (GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().isOnNetwork)
        {
            string nickNameBuffer = GameObject.Find("OnOff_UI").transform.Find("SignUp_UI").transform.Find("Canvas").transform.Find("InputField_NickName").transform.Find("Text").gameObject.GetComponent<Text>().text;

            //GameObject.Find("OnOff_UI").transform.Find("SignUp_UI").transform.Find("Canvas").transform.Find("NickName_InputField").transform.Find("Text").gameObject.GetComponent<Text>().text;

            // 글자수 제한, 4 이상, 10 이하일 떄만 트루
            if (nickNameBuffer.Length < 4) { Debug.Log("4글자 미만은 닉네임 안돼요"); return; }
            if (nickNameBuffer.Length > 10) { Debug.Log("10글자 이상은 닉네임 안돼요"); return; }

            // 한글 닉네임 지원에 따른 주석처리...!
            //bool nickNameTest = false;
            //for (int i = 0; i < nickNameBuffer.Length; ++i)
            //{
            //    if (nickNameBuffer[i] >= 'A' && nickNameBuffer[i] <= 'Z')
            //    { }
            //    else if (nickNameBuffer[i] >= 'a' && nickNameBuffer[i] <= 'z')
            //    { }
            //    else
            //    {
            //       
            //       // nickNameTest = true;
            //    }
            //}
            //if (nickNameTest) return;

            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().nickName = nickNameBuffer;
            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().money = 0;

            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().SendData(PROTOCOL.CHANGE_NICKNAME);
        }
        else
        {
            Debug.Log("Network Error");
        }
    }

    // From Network Recv -> if (recvType == (int)PROTOCOL.PERMIT_LOGIN)
    public void PermitLoginProcess()
    {
        GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().ID = IDBuffer;
        //GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().PW = PWBuffer;

        // Type값에 따라 로그인에 성공했습니다 또는 회원가입에 성공했습니다 UI를 띄우고 나중에 코루틴으로 해당 UI날리기 --> NUll 참조 에러 날 가능성 있으니 해당사항 체크 필요

        // 메인 UI로 넘어갑니다~~
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.MainUI_SCENE, true);
    }

    #endregion

    #region [ Old Func ]

    public void UI_SignUpButton()
    {
        // 회원 가입 버튼 처리입니다.

        typeBuffer = 2;

        IDBuffer = GameObject.Find("InputField_ID").transform.Find("Text").gameObject.GetComponent<Text>().text;
       // PWStringBuffer = GameObject.Find("InputField_PW").transform.Find("Text").gameObject.GetComponent<Text>().text;

       // PWBuffer = Int32.Parse(PWStringBuffer);

        //Debug.Log("회원가입을 시도합니다. ID : " + IDBuffer + "PW : " + PWBuffer);

        if (GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().isOnNetwork)
        {
            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().SendData(PROTOCOL.DEMAND_LOGIN);
        }
        else
        {
            // ForOnlyClientTest
            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().ID = "TEST_해솔리드";
            //GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().PW = 1234;

            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().money = 20190102;
            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().winCount = 0;
            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().loseCount = 0;

            PermitLoginProcess();
        }
    }

    public void FailLoginProcess()
    {
        // 나중에는 관련 UI를 띄어주고, 코루틴으로 해당 UI날리기 --> 다만 이 작업 중에, 씐 날릴 경우, 코루틴이 None에 접근하는 문제점이 발생할 수 있음!! 이거 어떻게 예외 처리해?? 몰랗ㅎㅎㅎ
        if(failReason == 1)
        {
            Debug.Log("동일한 아이디가 존재하지 않습니다.");
        }
        else if (failReason == 2)
        {
            Debug.Log("해당 아이디는 다른 비밀번호를 사용합니다.");
        }
        else if (failReason == 3)
        {
            Debug.Log("이미 로그인 중인 계정입니다.");
        }
        else if (failReason == 4)
        {
            Debug.Log("이미 존재하는 아이디로 회원가입이 불가능합니다.");
        }
    }
    
    #endregion
}
