using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

using System.Text;

public class TitleSceneManager : MonoBehaviour
{
    #region [ Release Func ]

    GameObject waitParsingUI;

    void Start()
    {
        waitParsingUI = GameObject.Find("Canvas").transform.Find("Image_WaitParsingUI").gameObject;
        GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().ParsingServerIP();
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
        GameObject.Find("DiceManager").GetComponent<DiceObject>().DiceSystem_Roll(6, 6);
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
