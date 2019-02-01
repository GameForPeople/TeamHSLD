using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public partial class InGameSceneManager : MonoBehaviour {

    // 사용된 이벤트카드의 타입(인덱스) 입니다.
    public int network_eventCardType;

    // Use this for initialization
    private void StartForEvent () {
        network_eventCardType = -1;
    }

    /*
     * SendEventcardIndex
     * 
     *  - 뽑은 이벤트 카드의 인덱스를 인자로 받아, 서버로 전송(대기)합니다.
     */
    public void SendEventcardIndex(int InEventcardIndex)
    {
        network_eventCardType = InEventcardIndex;
        network_sendProtocol = (int)PROTOCOL.NOTIFY_EVENTCARD_INDEX;
    }

    /*
    * RecvEventcardIndex
    * 
    *  - 상대편 클라이언트가 전송한 이벤트 카드 인덱스를 인자로 받아, 처리해주는 함수를 호출합니다.
    */
    public void NetworkManager_RecvEventcardIndex(int InEventcardIndex)
    {
        network_eventCardType = InEventcardIndex;

        //-- 성현이형 받은 이벤트 카드의 인덱스를 처리해주는 함수가 여기! 필요합니다.

        //--

        gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.ENEMYTURN_PICKEVENTCARD);
    }
}
