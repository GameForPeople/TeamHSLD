using System.Collections;
using System.Collections.Generic;

using System;

using UnityEngine;
using UnityEngine.UI;

/*
    해당 씐에서는 굳이, 게임 오브젝트를 멤버 변수로 갖지 않습니다. (빠른 속도를 필요로 하지 않기 때문이며, 자동 로그인 기능이 추후 추가될 예정입니다) 
 */

public class LoginSceneManager : MonoBehaviour {
    
    public string IDBuffer;
    string PWStringBuffer;
    public int PWBuffer;

    public int typeBuffer;

    public int failReason;

    // Use this for initialization
    //   void Start () {
    //   }

    // Update is called once per frame
    //    void Update () {
    //	}

    public void ClickSignInButton()
    {
        // 로그인 버튼 처리입니다.

        typeBuffer = 1;

        IDBuffer = GameObject.Find("ID_InputField").transform.Find("Text").gameObject.GetComponent<Text>().text;
        PWStringBuffer = GameObject.Find("PW_InputField").transform.Find("Text").gameObject.GetComponent<Text>().text;

        PWBuffer = Int32.Parse(PWStringBuffer);

        Debug.Log("로그인을 시도합니다. ID : " + IDBuffer + "PW : " + PWBuffer);


        if (GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().isOnNetwork)
        {
            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().SendData((int)PROTOCOL.DEMAND_LOGIN);
        }
        else
        {
            // ForOnlyClientTest
            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().ID = "TEST_Account";
            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().PW = 1234;

            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().money = 7777777;
            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().winCount = 0;
            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().loseCount = 0;

            PermitLoginProcess();
        }
    }

    public void ClickSignUpButton()
    {
        // 회원 가입 버튼 처리입니다.

        typeBuffer = 2;

        IDBuffer = GameObject.Find("ID_InputField").transform.Find("Text").gameObject.GetComponent<Text>().text;
        PWStringBuffer = GameObject.Find("PW_InputField").transform.Find("Text").gameObject.GetComponent<Text>().text;

        PWBuffer = Int32.Parse(PWStringBuffer);

        Debug.Log("회원가입을 시도합니다. ID : " + IDBuffer + "PW : " + PWBuffer);

        if (GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().isOnNetwork)
        {
            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().SendData((int)PROTOCOL.DEMAND_LOGIN);
        }
        else
        {
            // ForOnlyClientTest
            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().ID = "TEST_Account";
            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().PW = 1234;

            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().money = 7777777;
            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().winCount = 0;
            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().loseCount = 0;

            PermitLoginProcess();
        }
    }

    public void PermitLoginProcess()
    {
        GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().ID = IDBuffer;
        GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().PW = PWBuffer;

        // Type값에 따라 로그인에 성공했습니다 또는 회원가입에 성공했습니다 UI를 띄우고 나중에 코루틴으로 해당 UI날리기 --> NUll 참조 에러 날 가능성 있으니 해당사항 체크 필요

        // 메인 UI로 넘어갑니다~~
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.MainUI_SCENE);
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

}
