using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ScreenManager : MonoBehaviour {

    void Awake()
    {
        Screen.sleepTimeout = SleepTimeout.NeverSleep;  // 화면이 꺼지지 않도록 설정
        Screen.SetResolution(1280, 720, true);          // 해상도 설정 1280 - 720 , 전체화면 동일
    }

}
