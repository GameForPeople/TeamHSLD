using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System;


public partial class InGameSceneManager : MonoBehaviour {

    //public enum EVENT_TYPE : int
    //{
    //    지형_파괴   = 101,
    //    소유권_전환 = 102,
    //    내_지형_속성_변경 = 201,
    //    상대_지형_속성_변경 = 202,
    //    특수_카드_방어 = 301,
    //    주사위_두배 = 401
    //}

    // 사용된 이벤트카드의 타입(인덱스) 입니다.
    public int network_sendEventCardType;
    public int network_recvEventCardType;

    // event card type 관련 함수를 받았을 때, 함수 인터페이스를 일치하기 위해, 의미없이 전달할 때 사용하는 변수들 입니다.
    // 오직 인자 전달용입니다.
    /* readonly */ private int tempTerrainIndex;
    /* readonly */ private int tempCardIndex;
    /* readonly */ private bool tempIsDefense;

    // Use this for initialization
    private void StartForEvent () {
        network_sendEventCardType = -1;
        network_recvEventCardType = -1;

        tempTerrainIndex = -1;
        tempCardIndex = -1;
        tempIsDefense = true;
    }

    /*
     * SendEventcardIndex
     * 
     *  - 뽑은 이벤트 카드의 인덱스를 인자로 받아, 서버로 전송(대기)합니다.
     */
    public void SendEventcardIndex(int InEventcardIndex)
    {
        network_sendEventCardType = InEventcardIndex;
        network_sendProtocol = (int)PROTOCOL.NOTIFY_EVENTCARD_INDEX;
    }

    public bool NetworkManager_IsUsedDefenceCard;
    public int[] NetworkManager_TerrainIndex;
    public int NetworkManager_CardIndex;
    /*
     * SendEventBuffer
     * 
     *  - 해당 카드에 대한, 추가적인 정보를 전송할 때 사용합니다.
     */
    public void SendEventBuffer(bool InIsUsedDefenceCard, int[] indexRefA, int indexRefB)
    {
        switch (network_sendEventCardType)
        {
            case 101:
                // 선택한 상대 지형
                NetworkManager_TerrainIndex = indexRefA;
                break;

            case 111:
                // 선택한 상대 지형
                NetworkManager_TerrainIndex = indexRefA;
                break;

            case 201:
                // 내 지형 인덱스 및 변경하는 속성
                NetworkManager_TerrainIndex = indexRefA;
                NetworkManager_CardIndex = indexRefB;
                break;

            case 202:
                // 상대 지형 인덱스 및 변경하는 속성
                NetworkManager_TerrainIndex = indexRefA;
                NetworkManager_CardIndex = indexRefB;
                break;

            case 301:
                // 사용할 건지, 안할건지 여부 전송.
                NetworkManager_IsUsedDefenceCard = InIsUsedDefenceCard;
                break;

            case 401:
                // 추가적인 정보 필요 없음.
                break;
        }
        network_sendProtocol = (int)PROTOCOL.NOTIFY_EVENT_BUFFER;
    }

    /*
    * RecvEventcardIndex
    * 
    *  - 상대편 클라이언트가 전송한 이벤트 카드 인덱스를 확인합니다.
    */
    public void NetworkManager_RecvEventcardIndex(int InEventcardIndex)
    {
        network_recvEventCardType = InEventcardIndex;

        //-- 받은 이벤트 카드의 인덱스가 무엇인지 확인하는 함수입니다.

        //액티브가 On됫다가 2초후에 Off됨.
        GameObject.FindWithTag("GameManager").GetComponent<EventCardManager>().InstateEnemyEventCard(network_recvEventCardType);
        
        //--
        gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.ENEMYTURN_PICKEVENTCARD);
    }


    //서버에서 전달받은거 변경.
    //public void MeshInfoUpdate(int eventIndex, int terrainIndex, int cardIndex, bool isDefense)

    public void NetworkManager_RecvEventBuffer(int inNotifyEventCase)
    {
        switch (inNotifyEventCase)
        {
            case 101:   //지형_파괴 
            case 111:   //소유권_전환
                GameObject.FindWithTag("GameManager").GetComponent<EventCardManager>().MeshInfoUpdate(
                    inNotifyEventCase,
                    BitConverter.ToInt32(networkManager.NewDataRecvBuffer, 8),
                    tempCardIndex,
                    tempIsDefense
                );
                break;

            case 201:   //내_지형_속성_변경
            case 202:   //상대_지형_속성_변경
                GameObject.FindWithTag("GameManager").GetComponent<EventCardManager>().MeshInfoUpdate(
                    inNotifyEventCase,
                    BitConverter.ToInt32(networkManager.NewDataRecvBuffer, 8),
                    BitConverter.ToInt32(networkManager.NewDataRecvBuffer, 12),
                    tempIsDefense
                );
                break;

            //특수_카드_방어
            case 301:
                GameObject.FindWithTag("GameManager").GetComponent<EventCardManager>().MeshInfoUpdate(
                    inNotifyEventCase,
                    tempTerrainIndex,
                    tempCardIndex,
                    BitConverter.ToBoolean(networkManager.NewDataRecvBuffer, 8)
                );
                break;

            //주사위_두배
            case 401:
                // 불리지 않음.
                break;
        }
    }
}
