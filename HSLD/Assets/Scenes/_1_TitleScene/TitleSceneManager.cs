using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

using System.Text;

public class TitleSceneManager : MonoBehaviour
{
    #region [ Release Func ]

    GameObject waitParsingUI;
    public int parsingServerCommand;

    void Start()
    {
        waitParsingUI = GameObject.Find("Canvas").transform.Find("Image_WaitParsingUI").gameObject;

        parsingServerCommand = -1;

        StartCoroutine(HandleTitleSceneLogic());
        GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().ParsingServerIP();
    }

    /*
     * 1. 검은 화면 투명 - 불투명하게 하기
     * 2. ControlNextLogic 함수 호출.
     */
    private IEnumerator HandleTitleSceneLogic()
    {
        GameObject titleBlackUI = GameObject.Find("Overlay_Canvas").transform.Find("Image_Black").gameObject;

        // 검은 화면 투명하게 하기
        float blackUIAlphaValue = 1.0f;
        int loopCount = 0;

        while(loopCount < 20)
        {
            blackUIAlphaValue -= 0.05f;
            titleBlackUI.GetComponent<Image>().color = new Color(0.0f, 0.0f, 0.0f, blackUIAlphaValue);
            ++loopCount;

            yield return new WaitForSeconds(0.05f);
        }

        // 검은 화면 안보이는 상황에서 1초 대기
        titleBlackUI.GetComponent<Image>().color = new Color(0.0f, 0.0f, 0.0f, 0.0f);
        yield return new WaitForSeconds(1.0f);

        // 검은 화면 불투명하게 하기
        blackUIAlphaValue = 0.0f;
        loopCount = 0;

        while (loopCount < 20)
        {
            blackUIAlphaValue += 0.05f;
            titleBlackUI.GetComponent<Image>().color = new Color(0.0f, 0.0f, 0.0f, blackUIAlphaValue);
            ++loopCount;
            yield return new WaitForSeconds(0.05f);
        }

        // 아직 파싱 server IP 상태일 경우, 대기해줌.
        while (parsingServerCommand == -1)
        {
            yield return new WaitForSeconds(0.1f);
        }

        // 다음 로직 제어.
        ControlNextLogic();
    }

    private void ControlNextLogic()
    {
        // 현재 테스트 모드이면 테스트 관련 기능 버튼을 키고, 
        if (parsingServerCommand == 0)
        {
            GameObject.Find("Overlay_Canvas").gameObject.SetActive(false);
            StartCoroutine(DrawParsingUI());
        }
        // 정상 라이브 모드이면 바로 로그인 씐으로 이동해주고
        else if (parsingServerCommand == 1)
        {
            UI_StartWithServer();
        }
        // 서버 닫힌 상태면 관련 UI출력해주고 종료.
        else if (parsingServerCommand == 2)
        {
            DrawExitUI();
            GameObject.Find("GameCores").transform.Find("AppQuitManager").GetComponent<AppQuitManager>().Quit(false, 0, 5);
        }
    }

    private IEnumerator DrawParsingUI()
    {
        UI_OnOff_WaitParsingUI(true);
        yield return new WaitForSeconds(0.1f);
        UI_OnOff_WaitParsingUI(false);
    }

    private void DrawExitUI()
    {
        GameObject.Find("Overlay_Canvas").transform.Find("OnOff").gameObject.SetActive(true);
    }

    // 실제 상용화 시, 생성자에서 해당 함수 호출이 필요합니다.
    public void UI_StartWithServer()
    {
        //GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().StartNetworkFunction();
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().isOnNetwork = true;
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.LOGIN_SCENE);
    }

    public void UI_OnOff_WaitParsingUI(bool InIsActive)
    {
        waitParsingUI.SetActive(InIsActive);
    }

    public void NetworkManager_DrawIPAndState(string inIpString, string inNotifyString)
    {
        waitParsingUI.transform.Find("Text_IP").GetComponent<Text>().text = inIpString;
        waitParsingUI.transform.Find("Text_Notify").GetComponent<Text>().text = inNotifyString;
    }

    #endregion

    #region [ Client Test Function ]

    public void UI_StartWithLocal()
    {
        GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().iP_ADDRESS = "127.0.0.1";
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().isOnNetwork = true;
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.LOGIN_SCENE);
    }

    public void UI_StartOnlyClient()
    {
        //GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.LOGIN_SCENE);
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.MainUI_SCENE);
    }

    // 클라이언트 분들이, 서버 테스트를 병행하고 싶으실 때, 로컬 IP를 적어주시면 됩니다.
    public void UI_ChangeIPButton()
    {
        string IPv4Buffer = GameObject.Find("INPUTFIELD_IP").transform.Find("Text").gameObject.GetComponent<Text>().text;
        //Debug.Log(IPv4Buffer);
        GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().iP_ADDRESS = IPv4Buffer;
    }

    #endregion

    #region [ Debug Func ]

    // 로딩 UI 테스트를 위한 함수입니다.
    // Not Network!
    public void UI_TestLoadUI()
    {
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.LOGIN_SCENE, true, LOAD_UI_TYPE.SKIING_PENGUIN, 3.5f);
    }

    // Test Function 1128 in Dice
    public void UI_GoToMainUIScene()
    {
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.MainUI_SCENE);
    }

    // Test Function 1128 in Dice
    public void UI_DiceTest()
    {
        int value = Random.Range(1, 7);

        Debug.Log("주사위 값은 :" + value);
        GameObject.Find("_DiceManager").GetComponent<DiceObject>().DiceSystem_Roll(value, value);
    }

    // Test For Byte Size
    public void UI_LOGTEST()
    {
        string testString = "ABC안녕";
        byte[] stringToByteBuffer = Encoding.Default.GetBytes(testString);
        //Debug.Log("스트링의 내용 : " + testString + " 스트링의 크기 : " + testString.Length);
        //Debug.Log("contents의 내용 : " + stringToByteBuffer.ToString() + " 바이트의 크기 : " + stringToByteBuffer.Length);
    }

    #endregion
}
