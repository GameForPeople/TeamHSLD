using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InGameSceneManager : MonoBehaviour {

    public int          network_recvProtocolFlag;

    // 지형 타입입니다.
    public int          network_terrainType;

    // 바뀌어지는 테라리안의 개수입니다.
    public int          network_changeTerrainCount;

    // 바뀌어진 테라리언들의 인덱스가 들어있는 컨테이너 입니다.
    public int[]        network_terrainIndex = new int[12];

    // 사용된 이벤트카드의 타입(인덱스) 입니다.
    public int          network_eventCardType;

    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
