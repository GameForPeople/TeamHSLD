using System.Collections;
using System.Collections.Generic;

using System;

using UnityEngine;
using UnityEngine.UI;

public class LobbySceneManager : MonoBehaviour {

    IEnumerator CoroutineInstance_Network;
    IEnumerator CoroutineInstance_CountTime;

    public bool isRecvTrue = false;

    private bool isOnRandomMatchingNetwork = false;  // 큐돌릴때 바로 돌리는게 아니고 3초정도 대기하는데, 이거 3초 지낫는지 안지났는지 여부 체크.

    private int minuteCount;
    private int secondCount;

    void Start()
    {
        isRecvTrue = false;

        minuteCount = 0;
        secondCount = 0;
        // GameObject.Find("OnOff_UI").transform.Find("OnOff_All").gameObject.SetActive(false);

        // 해당 씬 들어가자마자 바로 Random Matching 하도록 변경.
        ClickRandomMatching();
    }

    public void ClickRandomMatching()
    {
        //대기 UI 출력
        //GameObject.Find("OnOff_UI").transform.Find("OnOff_All").gameObject.SetActive(true);

        CoroutineInstance_Network = WaitMatchingCoroutine();
        CoroutineInstance_CountTime = CountTimeCoroutine();

        StartCoroutine(CoroutineInstance_CountTime);
        StartCoroutine(CoroutineInstance_Network);
    }

    public void ClickExitMatching()
    {
        if(isOnRandomMatchingNetwork)
            StartCoroutine("ExitMatchingCoroutine");
        else
            SuccessExitMatching();
    }

    public IEnumerator ExitMatchingCoroutine()
    {
        // 해당 코루틴 종료 위치에 대한 숙고 필요.
        StopCoroutine(CoroutineInstance_Network);

        // ---
        // 네트워크 순서를 무너뜨리지 않는 상태에서, SendExitMatching을 전송해야함.
        while (GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().networkSyncLock == true)
        {
            yield return new WaitForSeconds(0.03f); // True이면 이거 건들면 안 됌 감히 씬매니저주제에 디질라공.
        }
        // ---

        GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().SendData((int)PROTOCOL.DEMAND_EXIT_RANDOM);
    }

    public void SuccessExitMatching()
    {
        StopCoroutine(CoroutineInstance_CountTime);
        //StopCoroutine(CoroutineInstance_Network); -> 이건 종료시켜주지 않아도 되는지에 대한 여부 판단 필요 ( 동기화는 항상 되는가? )

        GameObject.Find("OnOff_UI").transform.Find("OnOff_All").gameObject.SetActive(false);
        GameObject.Find("GameCores").transform.Find("SceneControlManager").
        GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.MainUI_SCENE, true);
    }

    public IEnumerator CountTimeCoroutine()
    {
        while(true)
        {
            secondCount++;

            if(secondCount >= 60)
            {
                secondCount = 0;
                minuteCount++;
            }

            GameObject.Find("OnOff_UI").transform.Find("OnOff_All").transform.Find("WaitTimeTextUI").GetComponent<Text>().text 
                = minuteCount.ToString() + ":" + secondCount.ToString();

            yield return new WaitForSeconds(1.0f); // 고냥 있어보일러고...
        }
    }

    public IEnumerator WaitMatchingCoroutine()
    {
        int a;
        yield return new WaitForSeconds(3.0f); // 고냥 있어보일러고...

        isOnRandomMatchingNetwork = true;

        //서버 네트워크 처리 - 방만들어 주세요 -> 항상 가능! 방 인덱스 서버에서 받아야지 관리하기 좋을 듯.
        GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().SendData((int)PROTOCOL.DEMAND_RANDOM_MATCH);

        while (isRecvTrue == false)
        {
            yield return new WaitForSeconds(0.5f);
        }

        if (GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().isHost)
        {
            while (true)
            {
                GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().SendData((int)PROTOCOL.DEMAND_GUEST_JOIN);
                yield return new WaitForSeconds(0.5f);
            }
        }
    }

    public void ChangeRoomScene()
    {
        StopCoroutine(CoroutineInstance_Network);
        StopCoroutine(CoroutineInstance_CountTime);

        GameObject.Find("OnOff_UI").transform.Find("OnOff_All").gameObject.SetActive(false);
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.ROOM_SCENE, true);
    }

    public void FailJoinRoom(int InFailReason)
    {
        if(InFailReason == 1)
        {
            // 해당 번호의 방이 없을 떄,
        }
        else if (InFailReason == 2)
        {
            // 해당 번호의 방이 들어갈 수 없을 때,
        }
        else if (InFailReason == 3)
        {
            // Random일때, 들어갈 수 있는 방이 없을 때
        }
    }

}
