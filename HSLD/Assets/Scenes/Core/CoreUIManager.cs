using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.UI;

public class CoreUIManager : MonoBehaviour
{

    public int answerFriendInviteValue = 0; // ClickAnswerFriendInvite UI에서 Ok를 누르면 1, No를 누르면 0
    public int answerFriendMakeValue = 0; // ClickAnswerFriendInvite UI에서 Ok를 누르면 1, No를 누르면 0

    public NetworkManager networkObject;
    GameObject MakeFriendUI;
    GameObject InviteFriendUI;
    GameObject DetailUserDataUI;
    GameObject ResultMakeFriendUI;

    // Use this for initialization
    void Start()
    {
        networkObject = GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>();
        MakeFriendUI = GameObject.Find("GameCores").transform.Find("Dynamic_Canvas").transform.Find("MakeFriendUI").gameObject;
        InviteFriendUI = GameObject.Find("GameCores").transform.Find("Dynamic_Canvas").transform.Find("InviteFriendUI").gameObject;
        DetailUserDataUI = GameObject.Find("GameCores").transform.Find("UserDataUI").transform.Find("Canvas_Dynamic").transform.Find("DetailUserDataUI").gameObject;
        ResultMakeFriendUI = GameObject.Find("GameCores").transform.Find("Dynamic_Canvas").transform.Find("ResultMakeFriendUI").gameObject;
    }

    public void ClickAnswerFriendInvite(int InTrueFalse)
    {
        answerFriendInviteValue = InTrueFalse;

        InviteFriendUI.SetActive(false);

        networkObject.SendData((int)PROTOCOL.ANSWER_FRIEND_INVITE);
    }

    public void ClickMakeFriendInvite(int InTrueFalse)
    {
        answerFriendMakeValue = InTrueFalse;

        MakeFriendUI.SetActive(false);

        networkObject.SendData((int)PROTOCOL.ANSWER_MAKE_FRIEND);
    }

    public void OnUI_DEMAND_FRIEND_UDP(string InString)
    {
        MakeFriendUI.transform.Find("NameString").GetComponent<Text>().text = InString;
        MakeFriendUI.SetActive(true);
    }

    public void OnUI_INVITE_FRIEND_UDP()
    {
        InviteFriendUI.SetActive(true);
    }

    public void OnUI_CHECK_ANSWER_MAKE_FRIEND(int InValue)
    {
        if (InValue == 0)
        {
            GameObject.Find("GameCores").transform.Find("Dynamic_Canvas").
                transform.Find("CHECK_ANSWER_MAKE_FRIEND_FALSE_UI").gameObject.SetActive(true);
        }
        else if (InValue == 1)
        {
            //-- 갱신 필요합니다.
            if (
                GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().nowScene
                == SCENE_NAME.MainUI_SCENE)
            {
                if (GameObject.Find("MainUISceneManager").GetComponent<MainUISceneManager>().isDrawFriendUI == true)
                {
                    GameObject.Find("MainUISceneManager").GetComponent<MainUISceneManager>().UI_FriendUIButton();
                    GameObject.Find("MainUISceneManager").GetComponent<MainUISceneManager>().UI_FriendUIButton();
                }
            }
            //--
            GameObject.Find("GameCores").transform.Find("Dynamic_Canvas").
            transform.Find("CHECK_ANSWER_MAKE_FRIEND_TRUE_UI").gameObject.SetActive(true);
        }
    }

    public void OffUI_CHECK_ANSWER_MAKE_FRIEND(int InUIIndex)
    {
        //Click
        if (InUIIndex == 0)
        {
            GameObject.Find("GameCores").transform.Find("Dynamic_Canvas").
                transform.Find("CHECK_ANSWER_MAKE_FRIEND_FALSE_UI").gameObject.SetActive(false);
        }
        else if (InUIIndex == 1)
        {
            GameObject.Find("GameCores").transform.Find("Dynamic_Canvas").
            transform.Find("CHECK_ANSWER_MAKE_FRIEND_TRUE_UI").gameObject.SetActive(false);
        }
    }

    public void OnOffDetailUserDataUI(bool InBValue)
    {
        if (InBValue == true)
        {
            DetailUserDataUI.transform.Find("WinText").GetComponent<Text>().text = networkObject.winCount.ToString();
            DetailUserDataUI.transform.Find("LoseText").GetComponent<Text>().text = networkObject.loseCount.ToString();

            DetailUserDataUI.SetActive(true);
        }
        else if (InBValue == false)
        {
            DetailUserDataUI.SetActive(false);
        }
    }

    public void OnOffResultMakeFriendUI(bool InBValue)
    {
        if (InBValue == true)
        {
            ResultMakeFriendUI.SetActive(true);
        }
        else if (InBValue == false)
        {
            ResultMakeFriendUI.SetActive(false);
        }
    }
}
