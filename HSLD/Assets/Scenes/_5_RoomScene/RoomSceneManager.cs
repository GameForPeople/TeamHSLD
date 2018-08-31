using System.Collections;
using System.Collections.Generic;

using UnityEngine;

using UnityEngine.UI;

public class RoomSceneManager : MonoBehaviour {
    GameObject networkObject;

    bool isGameReady = true;
    int startCount = 5;

    void Start()
    {
        networkObject = GameObject.Find("GameCores").transform.Find("NetworkManager").gameObject;

        // 방 인덱스 등록
        GameObject.Find("RoomIndex_TEXT").GetComponent<Text>().text = networkObject.GetComponent<NetworkManager>().roomIndex.ToString();

        // 자기 자신의 아이디 노출
        GameObject.Find("PlayerID_TEXT").GetComponent< Text >().text = networkObject.GetComponent<NetworkManager>().ID;

        // 나중에 계급이나 랭크, 캐릭터 이미지 같은것도 추가되면 여기서 해야처리해야할 듯

        // 방장이면 서버로부터 계속 Guest들어왔는지 여부 확인하도록함
        if (networkObject.GetComponent<NetworkManager>().isHost)
        {
            StartCoroutine("HostRoomCoroutine");
        }
        else
        {
            GameObject.Find("EnemyID_TEXT").GetComponent<Text>().text = networkObject.GetComponent<NetworkManager>().enemyId;
            StartCoroutine("StartCount");
        }
    }

    IEnumerator HostRoomCoroutine()
    {
        while (isGameReady)
        {
            networkObject.GetComponent<NetworkManager>().SendData((int)PROTOCOL.DEMAND_ROOMHOST);

            yield return new WaitForSeconds(1.0f);
        }
    }

    // host만 받는 함수입니다. 게스트가 조인한 경우!
    public void GuestJoinRoom()
    {
        isGameReady = false;

        GameObject.Find("EnemyID_TEXT").GetComponent<Text>().text = networkObject.GetComponent<NetworkManager>().enemyId;
        StartCoroutine("StartCount");
    }

    // 게임 카운트 5부터 0까지 세도록
    IEnumerator StartCount()
    {
        while(startCount > 0)
        {
            GameObject.Find("TIMER_TEXT").GetComponent<Text>().text = startCount.ToString();

            yield return new WaitForSeconds(1.0f);
            --startCount;
        }

        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.INGAME_SCENE);
    }
}
