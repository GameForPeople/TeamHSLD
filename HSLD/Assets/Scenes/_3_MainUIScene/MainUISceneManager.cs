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

    public bool isDrawFriendUI = false; // in coreUiManager Ref

    bool isDrawFriendInviteWait = false;

    public int invitedFriendIndex;

    int friendNum;
    public int friendWaitTimeCount = 7;

    const string voidFriendNickName = "_ERROR_NOT_FIND_NickName_";
    public string makeFriendIDBuffer;

    GameObject FriendUICanvas;
    GameObject FriendUIDynamicCanvas;

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
        GameObject.Find("GameCores").transform.Find("UserDataUI").gameObject.SetActive(false);
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
        {
            // 내 친구수가 4명 이상.
            OnUI_CHECK_DEMAND_MAKE_FRIEND(4);
            return;
        }

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

        if (bBuffer)
        {
            // 이미 친구인 놈.
            OnUI_CHECK_DEMAND_MAKE_FRIEND(3);
            return;
        }

        if (String.Compare(makeFriendIDBuffer, networkObject.ID) == 0)
        {
            // 내 아이디잖아
            OnUI_CHECK_DEMAND_MAKE_FRIEND(5);
            return;
        }

        if (String.Compare(makeFriendIDBuffer, networkObject.nickName) == 0)
        {
            // 내 닉네임이잖아.
            OnUI_CHECK_DEMAND_MAKE_FRIEND(5);
            return;
        }

        networkObject.SendData((int)PROTOCOL.DEMAND_MAKE_FRIEND);
    }

    public void ClickInviteButton(int InIndex)
    {
        invitedFriendIndex = InIndex;
        networkObject.enemyId = networkObject.friendNickNameCont[InIndex];
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
        friendWaitTimeCount = 7;

        OnFriendInviteWaitUI();
        StartCoroutine(WaitFriendJoinCoroutine());
    }

    public IEnumerator WaitFriendJoinCoroutine()
    {
        while (friendWaitTimeCount > 0)
        {
            FriendUIDynamicCanvas.transform.Find("OnFriendWaitUI").
                transform.Find("WaitTime").GetComponent<Text>().text = friendWaitTimeCount.ToString();

            networkObject.SendData((int)PROTOCOL.DEMAND_FRIEND_JOIN);
            yield return new WaitForSeconds(1.0f);
            friendWaitTimeCount--;
            // UI Set
        }

        OffFriendInviteWaitUI();
        networkObject.SendData((int)PROTOCOL.DELAY_FRIEND_INVITE);
    }

    public void OnFriendInviteWaitUI()
    {
        if(!isDrawFriendInviteWait)
        {
            FriendUIDynamicCanvas.transform.Find("OnFriendWaitUI").gameObject.SetActive(true);
            isDrawFriendInviteWait = true;
        }
    }

    public void OffFriendInviteWaitUI()
    {
        if (isDrawFriendInviteWait)
        {
            FriendUIDynamicCanvas.transform.Find("OnFriendWaitUI").gameObject.SetActive(false);
            isDrawFriendInviteWait = false;
        }
    }

    public void OnFriendBadStateUI_NetworkManager(int InCase)
    {
        if (InCase == 1)
        {
            // 친구가 게임 초대를 받을 수 없는 상태입니다.
        }
        else if (InCase == 2)
        {
            // 친구가 게임 초대를 받을 수 없는 상태입니다.
        }
    }

    public void OnUI_CHECK_DEMAND_MAKE_FRIEND(int InFailReason)
    {
        // by Network Send or 

        if (InFailReason == -1)
        {
            // 정상적으로 친구에세 데이터 송신
            FriendUIDynamicCanvas.transform.Find("CHECK_DEMAND_MAKE_FRIEND_TRUE").gameObject.SetActive(true);
        }
        else if (InFailReason == 0)
        {
            // 상대방이 미로그인
            FriendUIDynamicCanvas.transform.Find("CHECK_DEMAND_MAKE_FRIEND_FALSE_0").gameObject.SetActive(true);
        }
        else if (InFailReason == 1)
        {
            // 상대방이 맥스 왕인싸
            FriendUIDynamicCanvas.transform.Find("CHECK_DEMAND_MAKE_FRIEND_FALSE_1").gameObject.SetActive(true);
        }
        else if (InFailReason == 2)
        {
            // 상대방이 받을 수 없는 상태.
            FriendUIDynamicCanvas.transform.Find("CHECK_DEMAND_MAKE_FRIEND_FALSE_2").gameObject.SetActive(true);
        }
        else if (InFailReason == 3)
        {
            // 이미 상대방이 친구인 경우
            FriendUIDynamicCanvas.transform.Find("CHECK_DEMAND_MAKE_FRIEND_FALSE_3").gameObject.SetActive(true);
        }
        else if (InFailReason == 4)
        {
            // 님 인싸여서 더이상 친구 추가 못함 ㅋ
            FriendUIDynamicCanvas.transform.Find("CHECK_DEMAND_MAKE_FRIEND_FALSE_4").gameObject.SetActive(true);
        }
        //else if (InFailReason == 5) // 스스로 친추 하려고 할 때.
    }

    public void OffUI_CHECK_DEMAND_MAKE_FRIEND(int InUIIndex)
    {
        //Click

        if (InUIIndex == -1)
        {
            // 정상적으로 친구에세 데이터 송신
            FriendUIDynamicCanvas.transform.Find("CHECK_DEMAND_MAKE_FRIEND_TRUE").gameObject.SetActive(false);
        }
        else if (InUIIndex == 0)
        {
            // 상대방이 미로그인
            FriendUIDynamicCanvas.transform.Find("CHECK_DEMAND_MAKE_FRIEND_FALSE_0").gameObject.SetActive(false);
        }
        else if (InUIIndex == 1)
        {
            // 상대방이 맥스 왕인싸
            FriendUIDynamicCanvas.transform.Find("CHECK_DEMAND_MAKE_FRIEND_FALSE_1").gameObject.SetActive(false);
        }
        else if (InUIIndex == 2)
        {
            // 상대방이 받을 수 없는 상태.
            FriendUIDynamicCanvas.transform.Find("CHECK_DEMAND_MAKE_FRIEND_FALSE_2").gameObject.SetActive(false);
        }
        else if (InUIIndex == 3)
        {
            // 이미 상대방이 친구인 경우
            FriendUIDynamicCanvas.transform.Find("CHECK_DEMAND_MAKE_FRIEND_FALSE_3").gameObject.SetActive(false);
        }
        else if (InUIIndex == 4)
        {
            // 님 인싸여서 더이상 친구 추가 못함 ㅋ
            FriendUIDynamicCanvas.transform.Find("CHECK_DEMAND_MAKE_FRIEND_FALSE_4").gameObject.SetActive(false);
        }
    }

}
