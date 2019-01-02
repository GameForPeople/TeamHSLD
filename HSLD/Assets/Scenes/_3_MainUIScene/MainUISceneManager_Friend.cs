using System.Collections;
using System.Collections.Generic;

using System;

using UnityEngine;
using UnityEngine.UI;

public partial class MainUISceneManager : MonoBehaviour
{
    #region [ Release Func ]

    public void UI_FriendUIButton()
    {
        if (isDrawFriendUI)
            OffFriendUI();
        else
            networkObject.SendData((int)PROTOCOL.DEMAND_FRIEND_INFO);
    }

    private void OffFriendUI()
    {
        for (int i = 0; i < 4; ++i)
        {
            InviteButtonUI[i].SetActive(false);
        }

        GameObject.Find("Friend_UI").transform.Find("OnOFF").gameObject.SetActive(false);

        isDrawFriendUI = false;
    }

    public void UI_MakeFriendButton()
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

    public void UI_InviteButton(int InIndex)
    {
        invitedFriendIndex = InIndex;
        networkObject.enemyId = networkObject.friendNickNameCont[InIndex];
        networkObject.SendData((int)PROTOCOL.DEMAND_FRIEND_INVITE);
    }

    #endregion

    //public void ClickAnswerFriendInvite(int InTrueFalse)
    //{
    //    answerFriendInviteValue = InTrueFalse;
    //
    //    networkObject.SendData((int)PROTOCOL.ANSWER_FRIEND_INVITE);
    //}

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

            if (networkObject.friendState[i] == 2)
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
