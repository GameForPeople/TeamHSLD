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

    GameObject ResultMakeFriendUI;
    GameObject AnnouncementUI;

    // Use this for initialization
    void Start()
    {
        networkObject = GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>();
        MakeFriendUI = GameObject.Find("GameCores").transform.Find("Dynamic_Canvas").transform.Find("MakeFriendUI").gameObject;
        InviteFriendUI = GameObject.Find("GameCores").transform.Find("Dynamic_Canvas").transform.Find("InviteFriendUI").gameObject;
        ResultMakeFriendUI = GameObject.Find("GameCores").transform.Find("Dynamic_Canvas").transform.Find("ResultMakeFriendUI").gameObject;
        AnnouncementUI = GameObject.Find("GameCores").transform.Find("AnnouncementUI").gameObject;
    }

    public void NetworkManager_DrawAnnouncementUI(string InString)
    {
        StartCoroutine(AnnouncementUICoroutine(InString));
    }

    IEnumerator AnnouncementUICoroutine(string InString)
    {
        //------------------------------------
        AnnouncementUI.SetActive(true);
        AnnouncementUI.transform.Find("Text").GetComponent<Text>().text = " ";
        yield return new WaitForSeconds(0.5f);
        //------------------------------------


        //------------------------------------
        AnnouncementUI.transform.Find("Text").GetComponent<Text>().text
            = "안녕하세요. HSLD 운영자입니다 :)";
        yield return new WaitForSeconds(2.0f);
        AnnouncementUI.transform.Find("Text").GetComponent<Text>().text = " ";
        yield return new WaitForSeconds(0.5f);
        //------------------------------------


        //------------------------------------
        AnnouncementUI.transform.Find("Text").GetComponent<Text>().text
            = "긴급 공지 사항이 있어 안내드립니다.";
        yield return new WaitForSeconds(2.0f);
        AnnouncementUI.transform.Find("Text").GetComponent<Text>().text = " ";
        yield return new WaitForSeconds(0.5f);
        //------------------------------------


        //------------------------------------
        AnnouncementUI.transform.Find("Text").GetComponent<Text>().text = InString;
        yield return new WaitForSeconds(3.0f);
        AnnouncementUI.transform.Find("Text").GetComponent<Text>().text = " ";
        yield return new WaitForSeconds(0.5f);
        //------------------------------------

        //------------------------------------
        AnnouncementUI.transform.Find("Text").GetComponent<Text>().text =
            "항상 저희 게임을 사랑해주셔서 감사합니다. ";
        yield return new WaitForSeconds(2.0f);
        AnnouncementUI.transform.Find("Text").GetComponent<Text>().text = " ";
        yield return new WaitForSeconds(0.5f);
        AnnouncementUI.SetActive(false);
        //------------------------------------
    }

    /*
        ClickAnswerFriendInvite

        다른 친구가, 게임 초대를 요청하면 UDP 메세지를 받고, UI가 출력되는데,
        이때, 동의 시 true, 아닐 시 false로 셋 후, 전송
    */
    public void ClickAnswerFriendInvite(int InTrueFalse)
    {
        answerFriendInviteValue = InTrueFalse;

        InviteFriendUI.SetActive(false);

        networkObject.SendData(PROTOCOL.ANSWER_FRIEND_INVITE);
    }

    public void ClickMakeFriendInvite(int InTrueFalse)
    {
        answerFriendMakeValue = InTrueFalse;

        MakeFriendUI.SetActive(false);

        networkObject.SendData(PROTOCOL.ANSWER_MAKE_FRIEND);
    }

    public void OnUI_DEMAND_FRIEND_UDP(string InString)
    {
        MakeFriendUI.transform.Find("NameString").GetComponent<Text>().text = InString;
        MakeFriendUI.SetActive(true);
    }

    /*
        OnUI_INVITE_FRIEND_UDP()

        UDP로 부터, "친구와 같이하기"의 패킷을 받을 때 호출되는 함수.
    */
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
