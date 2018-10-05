using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


public class TitleSceneManager : MonoBehaviour
{

    public void ClickOnlyClientTestButton()
    {
        //GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.LOGIN_SCENE);
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.MainUI_SCENE);
    }

    public void ClickNetworkTestButton()
    {
        GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().StartNetworkFunction();

        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.LOGIN_SCENE, true);
    }

    // 클라이언트 분들이, 서버 테스트를 병행하고 싶으실 때, 로컬 IP를 적어주시면 됩니다.
    public void ClickChangeIPButton()
    {
        string IPv4Buffer = GameObject.Find("IP_InputField").transform.Find("Text").gameObject.GetComponent<Text>().text;
        //Debug.Log(IPv4Buffer);
        GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().iP_ADDRESS = IPv4Buffer;
    }

    // 실제 상용화 시, 생성자에서 해당 함수 호출이 필요합니다.
    public void PasingServerIPButton()
    {
        GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().ParsingServerIP();
    }

    // 로딩 UI 테스트를 위한 함수입니다.
    // Not Network!
    public void ClickLoadUITestButton()
    {
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.LOGIN_SCENE , true);
    }
}
