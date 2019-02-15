using System.Collections;
using System.Collections.Generic;

using System;

using UnityEngine;
using UnityEngine.UI;

public partial class MainUISceneManager : MonoBehaviour
{
    public NetworkManager networkObject;
    public SoundManager soundManager;

    #region [ Release Func ]
    void Start()
    {
        // -----
        GameObject.Find("GameCores").transform.Find("ClientBaseManager").GetComponent<ClientBaseManager>().OnOff_ClientBaseSpace(true);
        // -----

        soundManager = GameObject.Find("GameCores").transform.Find("SoundManager").GetComponent<SoundManager>();

        networkObject = GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>();

        //
        StartForOthers();
        StartForFriend();
        StartForSetting();
        StartForShop();
        StartForCharacter();
    }

    public void UI_InGameWithServerButton(bool inIsClassicMode)
    {
        networkObject.isClassicMode = inIsClassicMode;

        soundManager.SFXPlay(soundManager.clips[0], 1.0f);
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.LOBBY_SCENE);
    }
    #endregion

    #region [ Client Test Func ]
    public void UI_InGameOnlyClientButton(bool inIsClassicMode)
    {
        soundManager.SFXPlay(soundManager.clips[0], 1.0f);
        //GameObject.Find("GameCores").transform.Find("UserDataUI").gameObject.SetActive(false);

        if(inIsClassicMode)
            GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.INGAME_SCENE);
        else
            GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.INGAME_CASUAL_SCENE);
    }
    #endregion

}
