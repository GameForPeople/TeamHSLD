using System.Collections;
using System.Collections.Generic;

using System;

using UnityEngine;
using UnityEngine.UI;

public class LobbySceneManager : MonoBehaviour {

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
