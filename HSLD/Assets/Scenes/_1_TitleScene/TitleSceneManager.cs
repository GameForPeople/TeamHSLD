using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


public class TitleSceneManager : MonoBehaviour
{
    void Start()
    {
        GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().ParsingServerIP();
    }

    public void UI_StartWithServer()
    {
        //GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().StartNetworkFunction();
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().isOnNetwork = true;
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.LOGIN_SCENE);
    }

    // 실제 상용화 시, 생성자에서 해당 함수 호출이 필요합니다.
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

    // 로딩 UI 테스트를 위한 함수입니다.
    // Not Network!
    public void UI_TestLoadUI()
    {
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.LOGIN_SCENE , true);
    }
}
