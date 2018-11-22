using System.Collections;
using System.Collections.Generic;

using System;

using UnityEngine;
using UnityEngine.UI;

public class MainUISceneManager : MonoBehaviour
{
    //string friendName_1 = "";
    //string friendName_2 = "";
    //string friendName_3 = "";
    //string friendName_4 = "";

    string[] stateConstString = new string[3];

    public NetworkManager networkObject;

    bool isDrawFriendUI = false;

    public int invitedFriendIndex;

    int friendNum;

    const string voidFriendNickName = "비밀이야..사실몰라";
    public string makeFriendIDBuffer;

    GameObject FriendUICanvas;
    GameObject[] StateTextUI = new GameObject[4];
    GameObject[] NickNameTextUI = new GameObject[4];
    GameObject[] InviteButtonUI = new GameObject[4];

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

        //  친구 UI Set
        FriendUICanvas = GameObject.Find("Friend_UI").transform.Find("OnOFF").transform.Find("Canvas").gameObject;

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

        stateConstString[0] = "미접속";
        stateConstString[1] = "로비";
        stateConstString[2] = "게임중";

        isDrawFriendUI = false;
    }

    public void ClickInGameWithServerButton()
    {
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.LOBBY_SCENE);
    }

    public void ClickInGameOnlyClientButton()
    {
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.INGAME_SCENE);
    }

    public void ClickFriendUIButton()
    {
        if (isDrawFriendUI)
            OffFriendUI();
        else
            networkObject.SendData((int)PROTOCOL.DEMAND_FRIEND_INFO);
    }

    public void ClickMakeFriendButton()
    {
        if (friendNum >= 4)
            return;

        makeFriendIDBuffer =
        FriendUICanvas.transform.Find("INVITE_InputField").transform.Find("Text").gameObject.GetComponent<Text>().text;

        bool bBuffer = false;

        for (int i = 0; i < friendNum; ++i)
        {
            if (String.Compare(makeFriendIDBuffer, networkObject.friendNickNameCont[i]) == 0)
            {
                bBuffer = true;
                break;
            }
        }

        if (bBuffer) return;

        networkObject.SendData((int)PROTOCOL.DEMAND_MAKE_FRIEND);
    }

    public void ClickInviteButton(int InIndex)
    {
        invitedFriendIndex = InIndex;

        // 일단 오늘까지 너무 힘드니까, 지금 안되니까 주석으로 꺼놓자.
        networkObject.SendData((int)PROTOCOL.DEMAND_FRIEND_INVITE);
    }

    // In Game Cores.
    //public void ClickAnswerFriendInvite(int InTrueFalse)
    //{
    //    answerFriendInviteValue = InTrueFalse;
    //
    //    networkObject.SendData((int)PROTOCOL.ANSWER_FRIEND_INVITE);
    //}

    public void OffFriendUI()
    {
        for (int i = 0; i < 4; ++i)
        {
            InviteButtonUI[i].SetActive(false);
        }

        GameObject.Find("Friend_UI").transform.Find("OnOFF").gameObject.SetActive(false);

        isDrawFriendUI = false;
    }

    public void OnFriendUI_NetworkManager(int InFriendNum)
    {
        friendNum = InFriendNum;

        GameObject.Find("Friend_UI").transform.Find("OnOFF").gameObject.SetActive(true);

        for (int i = 0; i < friendNum; ++i)
        {
            StateTextUI[i].GetComponent<Text>().text = stateConstString[networkObject.friendState[i]].ToString();

            if (networkObject.friendState[i] > 0)
            {
                NickNameTextUI[i].GetComponent<Text>().text = networkObject.friendNickNameCont[i].ToString();
            }
            else
            {
                NickNameTextUI[i].GetComponent<Text>().text = voidFriendNickName.ToString();
            }

            if (networkObject.friendState[i] == 1)
            {
                InviteButtonUI[i].SetActive(true);
            }
        }

        isDrawFriendUI = true;
    }

    public void OnFriendWaitUI_NetworkManager()
    {
        // 상대방의 의사를 물어보는 중입니다. 7초 코루틴 실행 필요함.

    }

    public void OnFriendBadStateUI_NetworkManager(int InCase)
    {
        if(InCase == 1)
        {
            // 친구가 게임 초대를 받을 수 없는 상태입니다.
        }
        else if (InCase == 2)
        {
            // 친구가 게임 초대를 받을 수 없는 상태입니다.
        }
    }

}
