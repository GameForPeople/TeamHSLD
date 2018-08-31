using System.Collections;
using System.Collections.Generic;

using System;

using UnityEngine;
using UnityEngine.UI;

public class MainUISceneManager : MonoBehaviour {

    public GameObject networkObject;

	// Use this for initialization
	void Start () {
        networkObject = GameObject.Find("GameCores").transform.Find("NetworkManager").gameObject;

        GameObject.Find("ID_InTEXT").GetComponent<Text>().text = networkObject.GetComponent<NetworkManager>().ID;

        GameObject.Find("PW_InTEXT").GetComponent<Text>().text = networkObject.GetComponent<NetworkManager>().PW.ToString();
        GameObject.Find("Win_InTEXT").GetComponent<Text>().text = networkObject.GetComponent<NetworkManager>().winCount.ToString();
        GameObject.Find("Lose_InTEXT").GetComponent<Text>().text = networkObject.GetComponent<NetworkManager>().loseCount.ToString();
        GameObject.Find("Money_InTEXT").GetComponent<Text>().text = networkObject.GetComponent<NetworkManager>().money.ToString();
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
