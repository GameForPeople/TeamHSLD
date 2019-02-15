using System.Collections;
using System.Collections.Generic;

using UnityEngine;

using UnityEngine.UI;

public class RoomSceneManager : MonoBehaviour
{
    NetworkManager networkObject;

    bool isGameReady = true;
    int startCount = 5;

    //IEnumerator CharacterCoroutine;
    //bool isOnChangeCharacter = true;

    IEnumerator WaitGameCoroutine;

    #region [ Release Func ]
    void Start()
    {
        // 유저 데이터 필요 없어! [DEV_77]
        //GameObject.Find("GameCores").transform.Find("UserDataUI").gameObject.SetActive(false);

        networkObject = GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>();

        // 방 인덱스 등록
        //GameObject.Find("RoomIndex_TEXT").GetComponent<Text>().text = networkObject.GetComponent<NetworkManager>().roomIndex.ToString();

        // 자기 자신의 아이디 노출

        // DEV_85
        //GameObject.Find("PlayerID_TEXT").GetComponent<Text>().text = networkObject.GetComponent<NetworkManager>().nickName;

        // 나중에 계급이나 랭크, 캐릭터 이미지 같은것도 추가되면 여기서 해야처리해야할 듯

        // 방장이면 서버로부터 계속 Guest들어왔는지 여부 확인하도록함
        //if (networkObject.GetComponent<NetworkManager>().isHost)
        //{
        //    StartCoroutine("HostRoomCoroutine");
        //}
        //else
        //{

        // DEV_85

        // 상대방 닉네임 노출

        //GameObject.Find("EnemyID_TEXT").GetComponent<Text>().text = networkObject.GetComponent<NetworkManager>().enemyId;
        //Debug.Log("적 닉네임 한번 찍어봅시다. " + networkObject.GetComponent<NetworkManager>().enemyId);

        //DEV_66
        //GameObject.Find("NotifyText_TEXT").GetComponent<Text>().text = "캐릭터를 선택하세요.";

        // DEV_85

        //GameObject.Find("NotifyText_TEXT").GetComponent<Text>().text = "게임을 시작합니다.";
       // WaitGameCoroutine = WaitGame();
       // StartCoroutine(WaitGameCoroutine);
      
        if(networkObject.isClassicMode)
            GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.INGAME_SCENE);
        else
            GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.INGAME_CASUAL_SCENE);
        //}
    }

    IEnumerator WaitGame()
    {
        while (startCount > 0)
        {
            GameObject.Find("TIMER_TEXT").GetComponent<Text>().text = startCount.ToString();

            yield return new WaitForSeconds(1.0f);
            --startCount;
        }

        // UseDataUI Off
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.INGAME_SCENE);
    }

    #endregion

    #region [ DEV_66에서 제외된 함수들 ]

    // 게임 카운트 15부터 0까지 세도록
    IEnumerator StartCharacter()
    {
        SetCharacterIndex(1);   // 1번으로 미리 설정해놓음.

        while (startCount > 0)
        {
            GameObject.Find("TIMER_TEXT").GetComponent<Text>().text = startCount.ToString();

            yield return new WaitForSeconds(1.0f);
            networkObject.SendData(PROTOCOL.DEMAND_ENEMY_CHARACTER);
            --startCount;
        }

        GameObject.Find("Character_1_Button").SetActive(false);
        GameObject.Find("Character_2_Button").SetActive(false);

        GameObject.Find("NotifyText_TEXT").GetComponent<Text>().text = "게임을 시작합니다.";
        //isOnChangeCharacter = false;
        startCount = 3;
        yield return new WaitForSeconds(0.5f);

        while (startCount > 0)
        {
            GameObject.Find("TIMER_TEXT").GetComponent<Text>().text = startCount.ToString();

            yield return new WaitForSeconds(1.0f);
            networkObject.SendData(PROTOCOL.DEMAND_ENEMY_CHARACTER);
            --startCount;
        }

        yield return new WaitForSeconds(1.0f);

        // UseDataUI Off
        // DEV_77
        //GameObject.Find("GameCores").transform.Find("UserDataUI").gameObject.SetActive(false);
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.INGAME_SCENE);
    }

    public void SetCharacterIndex(int InCharacterIndex)
    {
       // if (isOnChangeCharacter)
       // {
            if (InCharacterIndex == 1)
                GameObject.Find("Player_Character").GetComponent<Text>().text = "캐릭터 1";
            else if (InCharacterIndex == 2)
                GameObject.Find("Player_Character").GetComponent<Text>().text = "캐릭터 2";

            networkObject.playerCharacterIndex = InCharacterIndex;
       // }
    }

    public void SetEnemyCharacter_Network()
    {
        if (networkObject.enemyCharacterIndex == 1)
            GameObject.Find("Enemy_Character").GetComponent<Text>().text = "캐릭터 1";
        else if (networkObject.enemyCharacterIndex == 2)
            GameObject.Find("Enemy_Character").GetComponent<Text>().text = "캐릭터 2";
    }

    #endregion

    #region [ Old Func ]

    //IEnumerator HostRoomCoroutine()
    //{
    //    while (isGameReady)
    //    {
    //        networkObject.GetComponent<NetworkManager>().SendData((int)PROTOCOL.DEMAND_ROOMHOST);
    //
    //        yield return new WaitForSeconds(1.0f);
    //    }
    //}

    // host만 받는 함수입니다. 게스트가 조인한 경우!
    //public void GuestJoinRoom()
    //{
    //    isGameReady = false;
    //
    //    GameObject.Find("EnemyID_TEXT").GetComponent<Text>().text = networkObject.GetComponent<NetworkManager>().enemyId;
    //    StartCoroutine("StartCount");
    //}

    #endregion
}
