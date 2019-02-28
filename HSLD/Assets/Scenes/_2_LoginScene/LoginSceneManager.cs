using System.Collections;
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
        StartCoroutine(LoginSceneAnimation());
    }

    public void UI_SignInButton()
    {
        // 로그인 버튼 처리입니다.
        typeBuffer = 1;
        
        IDBuffer = GameObject.Find("InputField_ID").transform.Find("Text").gameObject.GetComponent<Text>().text;
        //PWStringBuffer = GameObject.Find("InputField_PW").transform.Find("Text").gameObject.GetComponent<Text>().text;
        //PWBuffer = Int32.Parse(PWStringBuffer);


        //아이디 글자 수 검사
        if(IDBuffer.Length > 20 || IDBuffer.Length < 3)
        {
            Debug.Log("아이디 길이가 20 이상입니다. : " + IDBuffer.Length /*+ "PW : " + PWBuffer*/);
            GameObject.Find("Text_ID_State").gameObject.GetComponent<Text>().text = "아이디는 20자 미만, 3자 이상이여야 합니다.";
            return;
        }

        bool isInputtedIdIsRightFormat = false;

        // 아이디 무결성 검사.
        for (int i = 0; i < IDBuffer.Length; ++i)
        {
            if ('0' <= IDBuffer[i] && '9' >= IDBuffer[i]) continue;
            else if (IDBuffer[i] >= 'A' && IDBuffer[i] <= 'Z') continue;
            else if (IDBuffer[i] >= 'a' && IDBuffer[i] <= 'z') continue;

            //if (i == IDBuffer.Length -1 )

            Debug.Log("뭔데 " + IDBuffer[i] /*+ "PW : " + PWBuffer*/);


            isInputtedIdIsRightFormat = true;
            break;
        }

        if(isInputtedIdIsRightFormat)
        {
            GameObject.Find("Canvas_2D").transform.Find("Text_ID_State").gameObject.GetComponent<Text>().text = "아이디는 영어와 숫자로만 이루어져야합니다.";
            return;
        }
        
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
        GameObject.Find("Canvas_2D").transform.Find("SignUp_UI").gameObject.SetActive(InIsActive);

        //GameObject.Find("OnOff_UI").gameObject.SetActive(true);
    }

    public void UI_SetNewNicknameButton()
    {
        if (GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().isOnNetwork)
        {
            string nickNameBuffer = GameObject.Find("InputField_NickName").transform.Find("Text").gameObject.GetComponent<Text>().text;

            //GameObject.Find("OnOff_UI").transform.Find("SignUp_UI").transform.Find("Canvas").transform.Find("NickName_InputField").transform.Find("Text").gameObject.GetComponent<Text>().text;

            // 글자수 제한, 3 이상, 10 이하일 떄만 트루
            if (nickNameBuffer.Length < 3 || nickNameBuffer.Length > 10) {
                GameObject.Find("SignUp_UI").transform.Find("Text_State").gameObject.GetComponent<Text>().text = "닉네임은 3글자에서 10글자로 정해주세요.";
                return;
            }

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
        GameObject gameCore = GameObject.Find("GameCores").gameObject;

        gameCore.transform.Find("SoundManager").GetComponent<SoundManager>().SFXPlay(GameObject.Find("GameCores").transform.Find("SoundManager").GetComponent<SoundManager>().clips[0], 1.0f);

        gameCore.transform.Find("NetworkManager").GetComponent<NetworkManager>().ID = IDBuffer;
        //GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().PW = PWBuffer;

        // 아이템 비트 하드 코딩.
        int tempItemBit = gameCore.transform.Find("NetworkManager").GetComponent<NetworkManager>().itemBit;
        EditMesh tempEditMesh = gameCore.transform.Find("ClientBase_Space").transform.Find("Sphere_Core").GetComponent<EditMesh>();

        if ((tempItemBit & (1 << 0)) == (1 << 0))
        {
            tempEditMesh.ClientBase_SetActivationOfMaterial(true);
        }

        if ((tempItemBit & (1 << 1)) == (1 << 1))
        {
            tempEditMesh.ClientBase_SetActivationOfObject(true);
        }

        if ((tempItemBit & (1 << 2)) == (1 << 2))
        {
            tempEditMesh.ClientBase_SetActivationOfMovingObject(true);
        }

        // Type값에 따라 로그인에 성공했습니다 또는 회원가입에 성공했습니다 UI를 띄우고 나중에 코루틴으로 해당 UI날리기 --> NUll 참조 에러 날 가능성 있으니 해당사항 체크 필요
        // 메인 UI로 넘어갑니다~~
        gameCore.transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.MainUI_SCENE, true);
    }

    IEnumerator LoginSceneAnimation()
    {
        GameObject Canvas2D = GameObject.Find("Canvas_2D").gameObject;

        GameObject Image_Black = Canvas2D.transform.Find("Image_Black").gameObject;

        int loopCount = 0;
        float sizeCount = 0.0f;

        while(loopCount < 25)
        {
            sizeCount += 0.04f;
            Image_Black.transform.localScale = new Vector3(1.0f, sizeCount, 1.0f);

            ++loopCount;
            yield return new WaitForFixedUpdate();
        }

        //yield return new WaitForSeconds(0.2f);

        GameObject Image_Logo = Canvas2D.transform.Find("Image_Logo").gameObject;

        loopCount = 0;
        sizeCount = 0.0f;

        while (loopCount < 20)
        {
            sizeCount += 0.05f;
            Image_Logo.transform.localScale = new Vector3(sizeCount, sizeCount, 1.0f);

            ++loopCount;
            yield return new WaitForFixedUpdate();
        }

        yield return new WaitForSeconds(0.2f);

        Canvas2D.transform.Find("InputField_ID").gameObject.SetActive(true);
        Canvas2D.transform.Find("SignIn_Button").gameObject.SetActive(true);

        if (GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().parsingNotifyNum == 0)
        {
             Canvas2D.transform.Find("Text_Account").gameObject.SetActive(true);
             Canvas2D.transform.Find("Text_AccountFixed").gameObject.SetActive(true);
        }
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
            // 구글 로그인 유사 시스템 적용 후. 해당 로그 발생하지 않음.
            Debug.Log("동일한 아이디가 존재하지 않습니다.");
        }
        else if (failReason == 2)
        {
            // 구글 로그인 유사 시스템 적용 후, 해당 로그 발생하지 않음.
            Debug.Log("해당 아이디는 다른 비밀번호를 사용합니다.");
        }
        else if (failReason == 3)
        {
            Debug.Log("이미 로그인 중인 계정입니다.");
            GameObject.Find("Text_ID_State").transform.Find("Text").gameObject.GetComponent<Text>().text = "이미 로그인 중인 계정입니다.";
        }
        else if (failReason == 4)
        {
            // 구글 로그인 유사 시스템 적용 후, 해당 로그 발생하지 않음.
            Debug.Log("이미 존재하는 아이디로 회원가입이 불가능합니다.");
        }
    }
    
    #endregion
}
