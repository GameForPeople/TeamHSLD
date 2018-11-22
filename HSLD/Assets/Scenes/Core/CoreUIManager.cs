using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.UI;

public class CoreUIManager : MonoBehaviour {

    public int answerFriendInviteValue = 0; // ClickAnswerFriendInvite UI에서 Ok를 누르면 1, No를 누르면 0
    public int answerFriendMakeValue = 0; // ClickAnswerFriendInvite UI에서 Ok를 누르면 1, No를 누르면 0

    public NetworkManager networkObject;

    // Use this for initialization
    void Start () {
        networkObject = GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>();
    }

    // Update is called once per frame
    void Update () {
		
	}

    public void ClickAnswerFriendInvite(int InTrueFalse)
    {
        answerFriendInviteValue = InTrueFalse;
    
        networkObject.SendData((int)PROTOCOL.ANSWER_FRIEND_INVITE);
    }

    public void ClickMakeFriendInvite(int InTrueFalse)
    {
        answerFriendMakeValue = InTrueFalse;

        networkObject.SendData((int)PROTOCOL.ANSWER_FRIEND_INVITE);
    }
}
