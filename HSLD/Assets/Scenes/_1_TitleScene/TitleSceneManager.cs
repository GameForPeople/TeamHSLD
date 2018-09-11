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

        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.LOGIN_SCENE);
    }

    public void ClickChangeIPButton()
    {
        string IPv4Buffer = GameObject.Find("IP_InputField").transform.Find("Text").gameObject.GetComponent<Text>().text;
        //Debug.Log(IPv4Buffer);
        GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().iP_ADDRESS = IPv4Buffer;
    }

    public void PasingServerIPButton()
    {
        GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().ParsingServerIP();
    }
}
