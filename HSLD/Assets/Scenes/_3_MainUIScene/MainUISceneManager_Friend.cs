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
    int[] oldFriendActiveCharacterIndex = new int[4];

    string[] stateConstString = new string[4];

    GameObject[] SlotUI = new GameObject[4];

    GameObject[] StateTextUI = new GameObject[4];
    GameObject[] NickNameTextUI = new GameObject[4];
    GameObject[] InviteButtonUI = new GameObject[4];
    GameObject[] ConceptImageUI = new GameObject[4];

    IEnumerator waitFriendJoinCoroutine;

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

        oldFriendActiveCharacterIndex[0] = -1;
        oldFriendActiveCharacterIndex[1] = -1;
        oldFriendActiveCharacterIndex[2] = -1;
        oldFriendActiveCharacterIndex[3] = -1;
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
            networkObject.SendData(PROTOCOL.DEMAND_FRIEND_INFO);
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
            SlotUI[i].SetActive(false);
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

        networkObject.SendData(PROTOCOL.DEMAND_MAKE_FRIEND);
    }


    /*
        UI_InviteButton()

        내 친구 목록의 인덱스를 전송하여, 
        클라이언트 단계에서 먼저 가능 여부 확인하고, 가능시에만 서버로 요청.
    */
    public void UI_InviteButton(int InIndex)
    {
        // 보낼때 몇번 인덱스를 보낼 껀지 위하여 사용함.
        invitedFriendIndex = InIndex;

        // 보내려고 쓰는 게 아니고, 클라단에서 필요해서 해당 내용 저장.
        networkObject.enemyId = networkObject.friendNickNameCont[InIndex];

        networkObject.SendData(PROTOCOL.DEMAND_FRIEND_INVITE);
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
            SlotUI[i].SetActive(true);

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

        for( int i = 0; i < friendNum; ++i)
        {
            if(oldFriendActiveCharacterIndex[i] == -1)
            {
                SlotUI[i].transform.Find("Image_Character_Set").transform.Find("Image_" + networkObject.friendActiveCharacterIndex[i].ToString()).gameObject.SetActive(true);
            }
            else if(oldFriendActiveCharacterIndex[i] == networkObject.friendActiveCharacterIndex[i])
            {
                continue;
            }
            else
            {
                SlotUI[i].transform.Find("Image_Character_Set").transform.Find("Image_" + oldFriendActiveCharacterIndex[i].ToString()).gameObject.SetActive(false);
                SlotUI[i].transform.Find("Image_Character_Set").transform.Find("Image_" + networkObject.friendActiveCharacterIndex[i].ToString()).gameObject.SetActive(true);
            }

            oldFriendActiveCharacterIndex[i] = networkObject.friendActiveCharacterIndex[i];
        }

        isDrawFriendUI = true;
    }

    /*
        OnUI_CHECK_DEMAND_MAKE_FRIEND(int InFailReason)

        친구 만들기 버튼 클릭 시, 친구 만들기 실패할 경우, 호출되는 함수입니다.
        failReson을 인자로 받으며 각 인자에 따라 다른 UI 및 문구를 출력해야 합니다.
    */
    public void OnUI_CHECK_DEMAND_MAKE_FRIEND(int InFailReason)
    {
        // -1 : 좋았어!
        // 0 : 상대방이 미로그인
        // 1 : 상대방의 친구 수가 이미 4명 이상
        // 2 : 상대방이 받을 수 없는 상태...? 이게 뭐지? 왜 못받아?
        // 3 : 해당 닉네임이 없는 경우
        // 4 : 님 인싸여서 더이상 친구 추가 못함 ㅋ
        // 5 : 이미 친구인 경우
        // 6 : 니 아이디잖아

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

    /*
        OnFriendWaitUI_NetworkManager
     
        - NOTIFY_FRIEND_INVITE (Network - Recv) 에서, 1일 때, (친구와 같이하기에서 상대방을 기다리는 중일 때,)
           UI를 출력하기 위해 호출하는 함수.
     */
    public void OnFriendWaitUI_NetworkManager()
    {
        // 상대방의 의사를 물어보는 중입니다. 7초 코루틴 실행 필요함.
        friendWaitTimeCount = 7;

        OnFriendInviteWaitUI();
        waitFriendJoinCoroutine = WaitFriendJoinCoroutine();
        StartCoroutine(waitFriendJoinCoroutine);
    }

    /*
       OnFriendInviteWaitUI

       바로 위 OnFriendWaitUI_NetworkManager()에서 호출하며, WaitUI를 켜는 함수입니다. (굳이 함수일 필요가 있을까?)
    */
    private void OnFriendInviteWaitUI()
    {
        if (!isDrawFriendInviteWait)
        {
            FriendUIDynamicCanvas.transform.Find("OnFriendWaitUI").gameObject.SetActive(true);
            isDrawFriendInviteWait = true;
        }
    }

    /*
        WaitFriendJoinCoroutine

        바로 위에 위! OnFriendWaitUI_NetworkManager()에서 호출하며, WaitUI를 갱신하며, 
        친구가 들어왔는지 여부를(네트워크 통신)을 수행하는 코루틴 함수 입니다.

        7초 내에 연락이 안올경우, 마지막으로 Delay_Friend_Invite를 전송합니다.
    */
    public IEnumerator WaitFriendJoinCoroutine()
    {
        while (friendWaitTimeCount > 0)
        {
            FriendUIDynamicCanvas.transform.Find("OnFriendWaitUI").
                transform.Find("WaitTime").GetComponent<Text>().text = friendWaitTimeCount.ToString();

            networkObject.SendData(PROTOCOL.DEMAND_FRIEND_JOIN);
            yield return new WaitForSeconds(1.0f);
            friendWaitTimeCount--;
            // UI Set
        }

        OffFriendInviteWaitUI();
        networkObject.SendData(PROTOCOL.DELAY_FRIEND_INVITE);
    }

    /*
        OffFriendWaitUI_NetworkManager
        
        친구를 초대한 호스트가, 대기 UI와 함께 대기중인 상황에서, 친구가 거절을 누른 것이 명확한 경우!
        대기 UI를 꺼주고, 네트워크 기능 호출과, 대기 시간을 갱신하는 코루틴을 종료하는 함수.

        HOSTCHECK_FRIEND_INVITE (networkManager-> Recv)에서 호출됨.
    */
    public void OffFriendWaitUI_NetworkManager()
    {
        // 코루틴 동기화 문제가 발생하지 않지 않을 까요?
        StopCoroutine(waitFriendJoinCoroutine);

        // 필요 없을 수 있습니다.
        friendWaitTimeCount = -10;

        OffFriendInviteWaitUI();
    }


    /*
       OffFriendInviteWaitUI

       바로 위 OffFriendWaitUI_NetworkManager에서 호출하며, WaitUI를 끄는 함수입니다.
    */
    private void OffFriendInviteWaitUI()
    {
        if (isDrawFriendInviteWait)
        {
            FriendUIDynamicCanvas.transform.Find("OnFriendWaitUI").gameObject.SetActive(false);
            isDrawFriendInviteWait = false;
        }
    }

    #endregion

    //public void ClickAnswerFriendInvite(int InTrueFalse)
    //{
    //    answerFriendInviteValue = InTrueFalse;
    //
    //    networkObject.SendData((int)PROTOCOL.ANSWER_FRIEND_INVITE);
    //}

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
