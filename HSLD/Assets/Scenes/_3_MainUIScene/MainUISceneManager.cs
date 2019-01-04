using System.Collections;
using System.Collections.Generic;

using System;

using UnityEngine;
using UnityEngine.UI;

public partial class MainUISceneManager : MonoBehaviour
{
    public NetworkManager networkObject;

    #region [ Release Func ]
    void Start()
    {
        // -----
        GameObject.Find("GameCores").transform.Find("ClientBaseManager").GetComponent<ClientBaseManager>().OnOff_ClientBaseSpace(true);
        // -----


        networkObject = GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>();

        //
        StartForOthers();
        StartForFriend();
        StartForSetting();
        StartForShop();
    }

    public void UI_InGameWithServerButton()
    {
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.LOBBY_SCENE);
    }
    #endregion

    #region [ Client Test Func ]
    public void UI_InGameOnlyClientButton()
    {
        GameObject.Find("GameCores").transform.Find("UserDataUI").gameObject.SetActive(false);
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.INGAME_SCENE);
    }
    #endregion

}
