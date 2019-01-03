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

        //  친구 UI Set
        FriendUICanvas = GameObject.Find("Friend_UI").transform.Find("OnOFF").transform.Find("Canvas").gameObject;
        FriendUIDynamicCanvas = GameObject.Find("Friend_UI").transform.Find("OnOFF").transform.Find("Canvas_Dynamic").gameObject;

        StateTextUI[0] = FriendUICanvas.transform.Find("State1_Text").gameObject;
        StateTextUI[1] = FriendUICanvas.transform.Find("State2_Text").gameObject;
        StateTextUI[2] = FriendUICanvas.transform.Find("State3_Text").gameObject;
        StateTextUI[3] = FriendUICanvas.transform.Find("State4_Text").gameObject;

        NickNameTextUI[0] = FriendUICanvas.transform.Find("Name1_Text").gameObject;
        NickNameTextUI[1] = FriendUICanvas.transform.Find("Name2_Text").gameObject;
        NickNameTextUI[2] = FriendUICanvas.transform.Find("Name3_Text").gameObject;
        NickNameTextUI[3] = FriendUICanvas.transform.Find("Name4_Text").gameObject;

        InviteButtonUI[0] = FriendUICanvas.transform.Find("INVITE1_BUTTON").gameObject;
        InviteButtonUI[1] = FriendUICanvas.transform.Find("INVITE2_BUTTON").gameObject;
        InviteButtonUI[2] = FriendUICanvas.transform.Find("INVITE3_BUTTON").gameObject;
        InviteButtonUI[3] = FriendUICanvas.transform.Find("INVITE4_BUTTON").gameObject;

        // 친구 UI Off
        for (int i = 0; i < 4; ++i)
        {
            InviteButtonUI[i].SetActive(false);
        }

        GameObject.Find("Friend_UI").transform.Find("OnOFF").gameObject.SetActive(false);

        stateConstString[0] = "ERROR";
        stateConstString[1] = "미접속";
        stateConstString[2] = "로비";
        stateConstString[3] = "게임중";


        //
        isDrawFriendUI = false;
        isDrawSettingUI = false;
        isDrawShopUI = false;
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
