﻿using System.Collections;
using System.Collections.Generic;

using System;

using UnityEngine;
using UnityEngine.UI;

public class LobbySceneManager : MonoBehaviour {

    IEnumerator CoroutineInstance_Network;
    IEnumerator CoroutineInstance_CountTime;

    public bool isRecvTrue = false;

    private int minuteCount;
    private int secondCount; 

    void Start()
    {
        isRecvTrue = false;

        minuteCount = 0;
        secondCount = 0;
        GameObject.Find("OnOff_UI").transform.Find("OnOff_All").gameObject.SetActive(false);
    }

    public void ClickRandomMatching()
    {
        //대기 UI 출력
        GameObject.Find("OnOff_UI").transform.Find("OnOff_All").gameObject.SetActive(true);

        CoroutineInstance_Network = WaitMatchingCoroutine();
        CoroutineInstance_CountTime = CountTimeCoroutine();

        StartCoroutine(CoroutineInstance_CountTime);
        StartCoroutine(CoroutineInstance_Network);
    }

    public void ClickExitMatching()
    {
        //StopCoroutine(CoroutineInstance_Network);
        //StopCoroutine(CoroutineInstance_CountTime);
        //
        //// ---
        //// 네트워크 순서를 무너뜨리지 않는 상태에서, SendExitMatching을 전송해야함.
        //// ---
        //
        //GameObject.Find("OnOff_UI").transform.Find("OnOff_All").gameObject.SetActive(false);
        //GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().
        //    ChangeScene(SCENE_NAME.MainUI_SCENE);
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
        yield return new WaitForSeconds(2.0f); // 고냥 있어보일러고...

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
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.ROOM_SCENE);
    }




    //old Function


    public void ClickMakeRoom()
    {
        //서버 네트워크 처리 - 방만들어 주세요 -> 항상 가능! 방 인덱스 서버에서 받아야지 관리하기 좋을 듯.
        GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().SendData((int)PROTOCOL.DEMAND_MAKEROOM);
    }

    public void ClickJoinRoom()
    {
        string roomIndexBuffer = GameObject.Find("RoomIndex_InputField").transform.Find("Text").gameObject.GetComponent<Text>().text;

        // 서버에서 해당 인덱스에 대해, Join 여부를 확인하고, 가능할 시만 Join 처리.. 조인이 안될경우 UI출력 및, 해당 처리
        GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().roomIndex = Int32.Parse(roomIndexBuffer);
        GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().SendData((int)PROTOCOL.DEMAND_JOINROOM);
    }

    public void ClickJoinRandomRoom()
    {
        // 서버에서 고냥 입장 가능한 방 찾아줌... --> 매칭 알고리즘은 기획의도에 따라 서버에서 구현 여부 판단
        // 없을 경우, 해당 UI출력 필요
        GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().roomIndex = -1;
        GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().SendData((int)PROTOCOL.DEMAND_JOINROOM);
    }

    public void PermitMakeRoom()
    {
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.ROOM_SCENE);
    }

    public void PermitJoinRoom()
    {
        GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.ROOM_SCENE);
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
