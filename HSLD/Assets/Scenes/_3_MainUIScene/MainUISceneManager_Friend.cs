using System.Collections;
using System.Collections.Generic;

using System;

using UnityEngine;
using UnityEngine.UI;

public partial class MainUISceneManager : MonoBehaviour
{
    public bool isDrawFriendUI = false; // in coreUiManager Ref
    bool isDrawFriendInviteWait = false;

    public int invitedFriendIndex;

    int friendNum;
    public int friendWaitTimeCount = 7;

    const string voidFriendNickName = "_NOT_DEFINE_ID_";
    public string makeFriendIDBuffer;

    GameObject FriendUIFixedCanvas;
    GameObject FriendUIDynamicCanvas;

    string[] stateConstString = new string[4];

    GameObject[] SlotUI = new GameObject[4];

    GameObject[] StateTextUI = new GameObject[4];
    GameObject[] NickNameTextUI = new GameObject[4];
    GameObject[] InviteButtonUI = new GameObject[4];
    GameObject[] ConceptImageUI = new GameObject[4];

    private void StartForFriend()
    {
        //  친구 UI Set
        FriendUIFixedCanvas = GameObject.Find("Friend_UI").transform.Find("OnOff").transform.Find("Canvas_Fixed").gameObject;
        FriendUIDynamicCanvas = GameObject.Find("Friend_UI").transform.Find("OnOff").transform.Find("Canvas_Dynamic").gameObject;

        SlotUI[0] = FriendUIFixedCanvas.transform.Find("Slot_0").gameObject;
        SlotUI[1] = FriendUIFixedCanvas.transform.Find("Slot_1").gameObject;
        SlotUI[2] = FriendUIFixedCanvas.transform.Find("Slot_2").gameObject;
        SlotUI[3] = FriendUIFixedCanvas.transform.Find("Slot_3").gameObject;

        // 게임 오브젝트 로드.
        for (int i = 0; i < 4; ++i)
        {
            StateTextUI[i] = SlotUI[i].transform.Find("Text_State").gameObject;
            NickNameTextUI[i] = SlotUI[i].transform.Find("Text_Name").gameObject;
            InviteButtonUI[i] = SlotUI[i].transform.Find("Button_Invite").gameObject;
            ConceptImageUI[i] = SlotUI[i].transform.Find("Image_Concept").gameObject;
        }

        // 친구 UI Off
        for (int i = 0; i < 4; ++i)
        {
            InviteButtonUI[i].SetActive(false);
        }

        // Friend UI 관련 설정
        isDrawFriendUI = false;
        GameObject.Find("Friend_UI").transform.Find("OnOff").gameObject.SetActive(false);

        stateConstString[0] = "ERROR";
        stateConstString[1] = "미접속";
        stateConstString[2] = "로비";
        stateConstString[3] = "게임중";
    }

    #region [ Release Func ]

    /*
        UI_FriendUIButton()

        친구 UI를 키거나, 끌 때 사용하는 함수입니다.
        다른 UI들과 다르게, 켜질 때, 네트워크 함수가 호출되어야합니다. (친구 정보 써치)
    */
    public void UI_FriendUIButton()
    {
        if (isDrawFriendUI)
            OffFriendUI();
        else
            networkObject.SendData((int)PROTOCOL.DEMAND_FRIEND_INFO);
    }

    /*
        OffFriendUI()

        UI_FriendUIButton()에서 친구 UI를 끌때 사용하는 함수입니다.
    */
    private void OffFriendUI()
    {
        for (int i = 0; i < 4; ++i)
        {
            InviteButtonUI[i].SetActive(false);
        }

        GameObject.Find("Friend_UI").transform.Find("OnOff").gameObject.SetActive(false);

        isDrawFriendUI = false;
    }

    /*
        UI_MakeFriendButton()
        
        친구 ID를 입력하고, 친구 신청 버튼을 누를 떄, 
        클라이언트 단계에서 먼저 가능 여부 확인하고, 가능시에만 서버로 요청.
    */
    public void UI_MakeFriendButton()
    {
        if (friendNum >= 4)
        {
            // 내 친구수가 4명 이상이야 안되 저리가.
            OnUI_CHECK_DEMAND_MAKE_FRIEND(4);
            return;
        }

        makeFriendIDBuffer =
        FriendUIFixedCanvas.transform.Find("InputField_Invite").transform.Find("Text").gameObject
        .GetComponent<Text>().text;

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
            // 이미 친구인 애 또 친추하네 이자식 ㅡ
            OnUI_CHECK_DEMAND_MAKE_FRIEND(5);
            return;
        }

        if (String.Compare(makeFriendIDBuffer, networkObject.ID) == 0)
        {
            // 내 아이디잖아
            OnUI_CHECK_DEMAND_MAKE_FRIEND(6);
            return;
        }

        if (String.Compare(makeFriendIDBuffer, networkObject.nickName) == 0)
        {
            // 니 닉네임이잖아 ㅡㅡ
            OnUI_CHECK_DEMAND_MAKE_FRIEND(6);
            return;
        }

        networkObject.SendData((int)PROTOCOL.DEMAND_MAKE_FRIEND);
    }


    /*
        UI_InviteButton()

        내 친구 목록의 인덱스를 전송하여, 
        클라이언트 단계에서 먼저 가능 여부 확인하고, 가능시에만 서버로 요청.
    */
    public void UI_InviteButton(int InIndex)
    {
        invitedFriendIndex = InIndex;

        // 보내려고 쓰는 게 아니고, 클라단에서 필요해서 해당 내용 저장.
        networkObject.enemyId = networkObject.friendNickNameCont[InIndex];

        networkObject.SendData((int)PROTOCOL.DEMAND_FRIEND_INVITE);
    }

    /*
        OnFriendUI_NetworkManager(int InFriendNum)

        네트워크매니저(Recv)에서 호출되는 함수입니다.
        친구 창 UI를 킬 때, 서버에 친구 정보를 요청하고, 해당 정보를 받아서 처리하는 함수에서
        해당 데이터를 다 처리 후, 이제 UI를 켜! 하는 함수입니다.
    */
    public void OnFriendUI_NetworkManager(int InFriendNum)
    {
        friendNum = InFriendNum;

        GameObject.Find("Friend_UI").transform.Find("OnOff").gameObject.SetActive(true);

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

            if (networkObject.friendState[i] == 2)
            {
                InviteButtonUI[i].SetActive(true);
            }
        }

        isDrawFriendUI = true;
    }

    #endregion

    //public void ClickAnswerFriendInvite(int InTrueFalse)
    //{
    //    answerFriendInviteValue = InTrueFalse;
    //
    //    networkObject.SendData((int)PROTOCOL.ANSWER_FRIEND_INVITE);
    //}

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
        if (!isDrawFriendInviteWait)
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
            // 해당 닉네임이 없는 경우
            FriendUIDynamicCanvas.transform.Find("CHECK_DEMAND_MAKE_FRIEND_FALSE_3").gameObject.SetActive(true);
        }
        else if (InFailReason == 4)
        {
            // 님 인싸여서 더이상 친구 추가 못함 ㅋ
            FriendUIDynamicCanvas.transform.Find("CHECK_DEMAND_MAKE_FRIEND_FALSE_4").gameObject.SetActive(true);
        }
        else if (InFailReason == 5)
        {
            // 이미 친구인 경우
            FriendUIDynamicCanvas.transform.Find("CHECK_DEMAND_MAKE_FRIEND_FALSE_5").gameObject.SetActive(true);
        }
        else if (InFailReason == 6)
        {
            // 니 아이디잖아
            FriendUIDynamicCanvas.transform.Find("CHECK_DEMAND_MAKE_FRIEND_FALSE_6").gameObject.SetActive(true);
        }
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
