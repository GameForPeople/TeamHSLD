using System.Collections;
using System.Collections.Generic;

using System;

using UnityEngine;
using UnityEngine.UI;

public class MainUISceneManager : MonoBehaviour {

    public NetworkManager networkObject;

    // Use this for initialization
    void Start()
    {
        networkObject = GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>();

        // GameObject.Find("PW_InTEXT").GetComponent<Text>().text = networkObject.GetComponent<NetworkManager>().PW.ToString();
        //GameObject.Find("ID_InTEXT").GetComponent<Text>().text = networkObject.ID;
        //GameObject.Find("Win_InTEXT").GetComponent<Text>().text = networkObject.winCount.ToString();
        //GameObject.Find("Lose_InTEXT").GetComponent<Text>().text = networkObject.loseCount.ToString();
        //GameObject.Find("Money_InTEXT").GetComponent<Text>().text = networkObject.money.ToString();


        //
        GameObject.Find("GameCores").transform.Find("UserDataUI").gameObject.SetActive(true);
        GameObject.Find("GameCores").transform.Find("UserDataUI").transform.Find("Canvas_Dynamic").transform.Find("Money_Text").gameObject.GetComponent<Text>().text
            = networkObject.money.ToString();

        GameObject.Find("GameCores").transform.Find("UserDataUI").transform.Find("Canvas").transform.Find("NickName_Text").gameObject.GetComponent<Text>().text
            = networkObject.nickName.ToString();
    }

    public void ClickInGameWithServerButton()
    {
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.LOBBY_SCENE);
    }

    public void ClickInGameOnlyClientButton()
    {
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.INGAME_SCENE);
    }
}
